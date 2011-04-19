#include "ovpCSynchroBuffer.h"

#include <cstdlib>
#include <cstring>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessing;

#define DEBUG_ENGINE

#ifdef DEBUG_ENGINE
#include <fstream>
#include <iomanip>
extern std::ofstream	theEngineDebug;
#define DebugSynchroBuffer(id, text)	theEngineDebug	<< id << ' ' << text << ' '\
														<< std::setw(4) << int(m_endPtr			- m_beginPtr)			<< ' ' \
														<< std::setw(4) << int(m_endSynchroPtr	- m_beginSynchroPtr)	<< ' ' \
														<< std::setw(4) << int(m_synchroOldPtr	- m_beginSynchroPtr)	<< ' ' \
														<< std::setw(4) << int(m_synchroPtr		- m_beginSynchroPtr)	<< ' ' \
														<< std::setw(4) << int(m_synchroPtr		- m_synchroOldPtr)		<< ' ' \
														<< int(*(m_synchroOldPtr-1))	<<  int(*m_synchroOldPtr)		<< ' ' \
														<< int(*(m_synchroPtr-1))		<<  int(*m_synchroPtr)			<< ' ' \
														<< std::endl
#if 0
#define DebugSynchroBuffer1(id, text)	theEngineDebug	<< id << ' ' << text << ' '\
														<< std::setw(4) << int(m_endPtr			- m_beginPtr)			<< ' ' \
														<< std::endl
#else
#define DebugSynchroBuffer1(id, text)
#endif
#else
#define DebugSynchroBuffer(id, text)
#define DebugSynchroBuffer1(id, text)
#endif

CSynchroBuffer::CSynchroBuffer(const OpenViBE::uint32 groupId)
	: m_groupId(groupId)
	, m_initialized(false)
	, m_inSynchro(false)
	, m_detected(false)
{
}

void CSynchroBuffer::Build(const OpenViBE::uint64 samplingRate, const OpenViBE::uint32 nbChannels, const OpenViBE::uint32 nbSamples, const OpenViBE::uint32 durationBuffer)
{
	m_initialized			= true;
	m_nbChannelSamples		= OpenViBE::uint32(samplingRate*durationBuffer);
	m_nbChannels			= nbChannels;
	m_nbChankSamples		= nbSamples;
	
	setDimensionCount(2);
	setDimensionSize(0, m_nbChannels);
	setDimensionSize(1, m_nbChannelSamples);
	
	m_beginPtr				= getBuffer();
	m_endPtr				= m_beginPtr + 1;
	m_limitPtr				= m_beginPtr + m_nbChannelSamples - m_nbChankSamples;
	
	m_beginSynchroPtr		= m_beginPtr + (m_nbChannels - 1)*m_nbChannelSamples;
	m_endSynchroPtr			= m_beginSynchroPtr + 1;

	m_synchroOldPtr			= m_beginSynchroPtr;
	
	memset(m_beginPtr, 0, size_t(m_nbChannels*m_nbChannelSamples*sizeof(OpenViBE::float64)));
}

void CSynchroBuffer::Push(const OpenViBE::IMatrix& data)
{
	const OpenViBE::float64*	pData = data.getBuffer();

	for(OpenViBE::uint32 iChan=0; iChan < m_nbChannels; iChan++)
		memcpy(m_endPtr + iChan*m_nbChannelSamples, pData + iChan*m_nbChankSamples, m_nbChankSamples*sizeof(OpenViBE::float64));
	
	m_endPtr		+= m_nbChankSamples;
	m_endSynchroPtr	+= m_nbChankSamples;
	
	FindEdge();
}

void CSynchroBuffer::FindEdge()
{
	DebugSynchroBuffer1(m_groupId, "Push     ");
	
	if(m_endPtr >= m_limitPtr)
	{	DebugSynchroBuffer(m_groupId, "Exit     ");
		exit(-10);
	}

	if(m_detected)
	{	DebugSynchroBuffer(m_groupId, "Wait     ");
		return;
	}

	OpenViBE::float64*	pSynchroOld	= m_synchroOldPtr;
	OpenViBE::float64*	pSynchro	= pSynchroOld + 1;
	OpenViBE::boolean	found		= false;
	
	while(pSynchro < m_endSynchroPtr)
	{	if((*pSynchroOld == 0) && (*pSynchro != 0))
		{	found	= true;
			break;
		}

		pSynchroOld++;
		pSynchro++;
	}

	if(found)
	{	if(!m_inSynchro)
		{	Shift(pSynchro - m_beginSynchroPtr);
			
			m_inSynchro		= true;			
			m_synchroOldPtr	= m_beginSynchroPtr;
		}
		else
		{	m_detected		= true;
			m_synchroPtr	= pSynchro;
			DebugSynchroBuffer(m_groupId, "FindEdge ");
	}	}
	else if(!m_inSynchro)
	{	m_endPtr			-= m_nbChankSamples;
		m_endSynchroPtr		-= m_nbChankSamples;
	}
}

void CSynchroBuffer::TagUndetected()
{
	m_detected		= false;
	m_synchroOldPtr	= m_synchroPtr;
}

void CSynchroBuffer::Correct(const OpenViBE::uint32 shift)
{
	OpenViBE::uint32	off2		= m_synchroPtr - m_beginSynchroPtr;
	OpenViBE::uint32	off1		= off2 - shift;
	OpenViBE::uint32	nbSamples	= m_endSynchroPtr - m_synchroPtr;
	
	for(OpenViBE::uint32 iChan=0; iChan < m_nbChannels; iChan++)
		memcpy(m_beginPtr + iChan*m_nbChannelSamples + off1, m_beginPtr + iChan*m_nbChannelSamples + off2, nbSamples*sizeof(OpenViBE::float64));
	
	m_endPtr		-= shift;
	m_endSynchroPtr	-= shift;
	m_synchroPtr	-= shift;
	
	DebugSynchroBuffer(m_groupId, "Correct  ");
}

void CSynchroBuffer::Shift(const OpenViBE::uint32 shift)
{
	OpenViBE::uint32	nbSamples	= (m_endPtr - m_beginPtr) - shift;
	
	for(OpenViBE::uint32 iChan=0; iChan < m_nbChannels; iChan++)
		memcpy(m_beginPtr + iChan*m_nbChannelSamples, m_beginPtr + shift + iChan*m_nbChannelSamples, nbSamples*sizeof(OpenViBE::float64));
	
	m_endPtr		-= shift;
	m_endSynchroPtr	-= shift;
	m_synchroPtr	-= shift;
//	DebugSynchroBuffer(m_groupId, "Shift    ");
}

void CSynchroBuffer::GetResult(OpenViBE::IMatrix& result, const OpenViBE::uint32& offChannels)
{
	OpenViBE::uint32 outputChunkSize = result.getDimensionSize(1);

	for(OpenViBE::uint32 iChan=0; iChan < m_nbChannels; iChan++)
		memcpy(result.getBuffer() + (iChan + offChannels)*outputChunkSize, m_beginPtr + iChan*m_nbChannelSamples, outputChunkSize*sizeof(OpenViBE::float64));

	Shift(outputChunkSize);

	DebugSynchroBuffer(m_groupId, "GetResult");
}
