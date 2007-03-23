#include "ovkCBoxAlgorithmContext.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
#define boolean OpenViBE::boolean

CBoxAlgorithmContext::CBoxAlgorithmContext(const IKernelContext& rKernelContext, ::PsSimulatedBox* pSimulatedBox, const IBox* pBox)
	:TKernelObject<IBoxAlgorithmContext>(rKernelContext)
	,TPluginObjectContext<IBoxAlgorithmContext>(rKernelContext)
	,m_oStaticBoxContext(rKernelContext, pBox)
	,m_oDynamicBoxContext(rKernelContext, pSimulatedBox)
	,m_oPlayerContext(rKernelContext, pSimulatedBox)
	,m_bReadyToProcess(false)
{
}

IStaticBoxContext* CBoxAlgorithmContext::getStaticBoxContext(void)
{
	return &m_oStaticBoxContext;
}

IDynamicBoxContext* CBoxAlgorithmContext::getDynamicBoxContext(void)
{
	return &m_oDynamicBoxContext;
}

IPlayerContext* CBoxAlgorithmContext::getPlayerContext(void)
{
	return &m_oPlayerContext;
}

boolean CBoxAlgorithmContext::markAlgorithmAsReadyToProcess(void)
{
	m_bReadyToProcess=true;
	return true;
}

boolean CBoxAlgorithmContext::isAlgorithmReadyToProcess(void)
{
	return m_bReadyToProcess;
}
