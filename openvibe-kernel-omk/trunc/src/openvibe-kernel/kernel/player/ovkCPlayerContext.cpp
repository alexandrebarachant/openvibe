#include "ovkCPlayerContext.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
#define boolean OpenViBE::boolean

CPlayerContext::CPlayerContext(const IKernelContext& rKernelContext, ::PsSimulatedBox* pSimulatedBox)
	:TKernelObject<IPlayerContext>(rKernelContext)
	,TPluginObjectContext<IPlayerContext>(rKernelContext)
	,m_pSimulatedBox(pSimulatedBox)
{
}

boolean CPlayerContext::sendSignal(
	const CMessageSignal& rMessageSignal)
{
	// TODO
	log() << LogLevel_Debug << "CPlayerContext::sendSignal - Not yet implemented\n";
	return false;
}

boolean CPlayerContext::sendMessage(
	const CMessageEvent& rMessageEvent,
	const CIdentifier& rTargetIdentifier)
{
	// TODO
	log() << LogLevel_Debug << "CPlayerContext::sendMessage - Not yet implemented\n";
	return false;
}

boolean CPlayerContext::sendMessage(
	const CMessageEvent& rMessageEvent,
	const CIdentifier* pTargetIdentifier,
	const uint32 ui32TargetIdentifierCount)
{
	// TODO
	log() << LogLevel_Debug << "CPlayerContext::sendMessage - Not yet implemented\n";
	return false;
}
