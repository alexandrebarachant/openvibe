#include "ovkCPlayerContext.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
#define boolean OpenViBE::boolean

CPlayerContext::CPlayerContext(::PsSimulatedBox* pSimulatedBox)
	:TPluginObjectContext<IPlayerContext>()
	,m_pSimulatedBox(pSimulatedBox)
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
