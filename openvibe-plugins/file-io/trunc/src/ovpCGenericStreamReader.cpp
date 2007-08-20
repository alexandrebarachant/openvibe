#include "ovpCGenericStreamReader.h"

#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;
using namespace OpenViBE::Kernel;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::FileIO;
using namespace OpenViBEToolkit;
using namespace std;


OpenViBE::boolean CGenericChunk::read(std::ifstream& oFile)
{
	//buffer to read the next data header
	const uint64 l_ui64HeaderSize = sizeof(m_ui32CurrentInput)+sizeof(m_ui64StartTime)+sizeof(m_ui64EndTime)+sizeof(m_ui64ChunkSize);

	uint8 l_pTempBuffer[l_ui64HeaderSize];

	//reads all the information about the next chunk
	oFile.read(reinterpret_cast<char * >(l_pTempBuffer), l_ui64HeaderSize);

	if(oFile.bad() || oFile.fail())
	{
		//if the eof has been reached
		if(oFile.eof())
		{
			m_oPlugin.getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Info <<"Reached the end of the input file.\n";
			return true;
		}
		else
		{
			m_oPlugin.getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Warning <<"Error while reading the input file.\n";
			return false;
		}
	}

	System::Memory::littleEndianToHost(l_pTempBuffer, &m_ui32CurrentInput);
	System::Memory::littleEndianToHost(l_pTempBuffer+sizeof(m_ui32CurrentInput), &m_ui64StartTime);
	System::Memory::littleEndianToHost(l_pTempBuffer+sizeof(m_ui32CurrentInput)+sizeof(m_ui64StartTime), &m_ui64EndTime);
	System::Memory::littleEndianToHost(l_pTempBuffer+sizeof(m_ui32CurrentInput)+sizeof(m_ui64StartTime)+sizeof(m_ui64EndTime), &m_ui64ChunkSize);

	//reads the actual chunk datas
	if(m_pChunkBuffer)
	{
		delete[] m_pChunkBuffer;
	}

	m_pChunkBuffer = new uint8[(size_t)m_ui64ChunkSize];
	oFile.read(reinterpret_cast<char * >(m_pChunkBuffer), (std::streamsize)m_ui64ChunkSize);

	if(oFile.bad() || oFile.fail())
	{
		m_oPlugin.getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Warning <<"Error while reading the input file.\n";
		return false;
	}

	return true;
}


CGenericStreamReader::CGenericStreamReader(void) :
	m_oCurrentChunk(*this),
	m_bError(false)
{

}

void CGenericStreamReader::release(void)
{
	delete this;
}

boolean CGenericStreamReader::initialize(void)
{
	const IBox* l_pBox=getBoxAlgorithmContext()->getStaticBoxContext();

	// Parses box settings to find filename
	l_pBox->getSettingValue(0, m_sFileName);

	if(!m_oFile.is_open())
	{
		m_oFile.open(m_sFileName, ios::binary | ios::in);

		if(m_oFile.bad())
		{
			getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Warning <<"Couldn't open the input file : "<<m_sFileName<<".\n";

			m_bError = true;

			return false;
		}
		else
		{
			m_bError = !m_oCurrentChunk.read(m_oFile);
		}
	}

	return !m_bError;
}

boolean CGenericStreamReader::uninitialize(void)
{
	if(m_oFile.is_open())
	{
		m_oFile.close();
	}

	return true;
}


boolean CGenericStreamReader::processClock(CMessageClock &rMessageClock)
{
	if(m_bError)
	{
		return false;
	}

	m_ui64CurrentTime = rMessageClock.getTime();

	if(m_ui64CurrentTime >= m_oCurrentChunk.m_ui64StartTime)
	{
		getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	}

	return true;
}

boolean CGenericStreamReader::process(void)
{
	while(m_ui64CurrentTime >= m_oCurrentChunk.m_ui64StartTime && !m_oFile.eof() && !m_bError)
	{
		IBoxIO* l_pBoxIO=getBoxAlgorithmContext()->getDynamicBoxContext();

		//sends the data
		l_pBoxIO->appendOutputChunkData(m_oCurrentChunk.m_ui32CurrentInput, 
				m_oCurrentChunk.m_pChunkBuffer, m_oCurrentChunk.m_ui64ChunkSize);

		l_pBoxIO->markOutputAsReadyToSend(m_oCurrentChunk.m_ui32CurrentInput, 
				m_oCurrentChunk.m_ui64StartTime, m_oCurrentChunk.m_ui64EndTime);

		m_bError = !m_oCurrentChunk.read(m_oFile);
	}

	return true;
}
