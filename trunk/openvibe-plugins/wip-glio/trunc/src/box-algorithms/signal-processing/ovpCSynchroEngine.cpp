
#include "ovpCSynchroEngine.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessing;

#define DEBUG_ENGINE

#ifdef DEBUG_ENGINE
#include <fstream>
#include <iomanip>
std::ofstream	theEngineDebug("c:/tmp/CSynchroEngine.txt");
#endif

CSynchroEngine::CSynchroEngine()
	: m_SynchroBuffer1(0)
	, m_SynchroBuffer2(1)
{
}

void CSynchroEngine::Build(const OpenViBE::uint32 group, const OpenViBE::uint64 samplingRate, const OpenViBE::uint32 nbChannels, const OpenViBE::uint32 nbSamples, const OpenViBE::uint32 durationBuffer)
{
	if(group == 0)
		m_SynchroBuffer1.Build(samplingRate, nbChannels, nbSamples, durationBuffer);
	else
		m_SynchroBuffer2.Build(samplingRate, nbChannels, nbSamples, durationBuffer);
}

void CSynchroEngine::Push(const OpenViBE::uint32 group, const OpenViBE::IMatrix& data)
{
	if(group == 0)
		m_SynchroBuffer1.Push(data);
	else
		m_SynchroBuffer2.Push(data);
}

OpenViBE::uint32 CSynchroEngine::NbChunks(const OpenViBE::uint32 outputChunkSize)
{
	OpenViBE::uint32 offset1, nbSamples1, offset2, nbSamples2;
	if(!HasSynchro(offset1, nbSamples1, offset2, nbSamples2))
		return 0;

	if(nbSamples1 > nbSamples2)
		m_SynchroBuffer1.Correct(nbSamples1 - nbSamples2);
	else if(nbSamples2 > nbSamples1)
		m_SynchroBuffer2.Correct(nbSamples2 - nbSamples1);

	HasSynchro(offset1, nbSamples1, offset2, nbSamples2);

	OpenViBE::uint32 nbChunks	= (offset1 <= offset2) ? offset1/outputChunkSize : offset2/outputChunkSize;	

	return nbChunks;
}

void CSynchroEngine::GetResult(OpenViBE::IMatrix& result)
{
	m_SynchroBuffer1.GetResult(result, 0);
	m_SynchroBuffer2.GetResult(result, m_SynchroBuffer1.NbChannels());
}
