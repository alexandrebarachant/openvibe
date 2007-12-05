#include "ovkCAlgorithmProxy.h"
#include "ovkCAlgorithm.h"

using namespace OpenViBE;
using namespace Kernel;

CAlgorithmProxy::CAlgorithmProxy(const IKernelContext& rKernelContext, CAlgorithm& rAlgorithm)
	:TKernelObject < IAlgorithmProxy >(rKernelContext)
	,m_rAlgorithm(rAlgorithm)
{
}

CIdentifier CAlgorithmProxy::getNextInputParameterIdentifier(
	const CIdentifier& rPreviousInputParameterIdentifier) const
{
	return m_rAlgorithm.getNextInputParameterIdentifier(rPreviousInputParameterIdentifier);
}

IParameter* CAlgorithmProxy::getInputParameter(
	const CIdentifier& rInputParameterIdentifier)
{
	return m_rAlgorithm.getInputParameter(rInputParameterIdentifier);
}

CIdentifier CAlgorithmProxy::getNextOutputParameterIdentifier(
	const CIdentifier& rPreviousOutputParameterIdentifier) const
{
	return m_rAlgorithm.getNextOutputParameterIdentifier(rPreviousOutputParameterIdentifier);
}

IParameter* CAlgorithmProxy::getOutputParameter(
	const CIdentifier& rOutputParameterIdentifier)
{
	return m_rAlgorithm.getOutputParameter(rOutputParameterIdentifier);
}

boolean CAlgorithmProxy::isOutputTriggerActive(
	const CIdentifier& rOutputTriggerIdentifier) const
{
	return m_rAlgorithm.isOutputTriggerActive(rOutputTriggerIdentifier);
}

boolean CAlgorithmProxy::activateInputTrigger(
	const CIdentifier& rInputTriggerIdentifier,
	const boolean bTriggerState)
{
	return m_rAlgorithm.activateInputTrigger(rInputTriggerIdentifier, bTriggerState);
}

boolean CAlgorithmProxy::initialize(void)
{
	return m_rAlgorithm.initialize();
}

boolean CAlgorithmProxy::uninitialize(void)
{
	return m_rAlgorithm.uninitialize();
}

boolean CAlgorithmProxy::process(void)
{
	return m_rAlgorithm.process();
}

boolean CAlgorithmProxy::process(
	const CIdentifier& rTriggerIdentifier)
{
	return m_rAlgorithm.process(rTriggerIdentifier);
}
