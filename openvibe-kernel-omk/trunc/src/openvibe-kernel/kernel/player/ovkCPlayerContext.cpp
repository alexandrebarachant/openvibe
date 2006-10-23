#include "ovkCPlayerContext.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;

CPlayerContext::CPlayerContext(PsSimulatedBox& rSimulatedBox)
	:TPluginObjectContext<IPlayerContext>()
	,m_rSimulatedBox(rSimulatedBox)
{
}

boolean CPlayerContext::sendSignal(
	const CMessageSignal& rMessageSignal)
{
	// TODO
	return false;
}

boolean CPlayerContext::sendMessage(
	const CMessageEvent& rMessageEvent,
	const CIdentifier& rTargetIdentifier)
{
	// TODO
	return false;
}

boolean CPlayerContext::sendMessage(
	const CMessageEvent& rMessageEvent,
	const CIdentifier* pTargetIdentifier,
	const uint32 ui32TargetIdentifierCount)
{
	// TODO
	return false;
}
