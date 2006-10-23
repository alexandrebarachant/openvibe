#include "ovkCBoxAlgorithmContext.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

CBoxAlgorithmContext::CBoxAlgorithmContext(PsSimulatedBox& rSimulatedBox, const IBox& rBox)
	:TPluginObjectContext<IBoxAlgorithmContext>()
	,m_oBoxContext(rSimulatedBox, rBox)
	,m_oPlayerContext(rSimulatedBox)
	,m_bReadyToProcess(false)
{
}

IBoxContext& CBoxAlgorithmContext::getBoxContext(void)
{
	return m_oBoxContext;
}

IPlayerContext& CBoxAlgorithmContext::getPlayerContext(void)
{
	return m_oPlayerContext;
}

boolean CBoxAlgorithmContext::markAlgorithmAsReadyToProcess(void)
{
	m_bReadyToProcess=true;
}

boolean CBoxAlgorithmContext::isAlgorithmReadyToProcess(void)
{
	return m_bReadyToProcess;
}
