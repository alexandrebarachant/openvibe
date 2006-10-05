#include "ovCStandardAcquisitionBehavior.h"

using namespace OpenViBE;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Acquisition;

uint32 CStandardAcquisitionBehaviorDesc::getClockFrequency(void) const
{
	return 100; // $$$
}

boolean CStandardAcquisitionBehavior::processEvent(
	IBoxBehaviorContext& rBoxBehaviorContext,
	CMessageEvent& rMessageEvent)
{
	return false;
}

boolean CStandardAcquisitionBehavior::processSignal(
	IBoxBehaviorContext& rBoxBehaviorContext,
	CMessageSignal& rMessageSignal)
{
	return false;
}

boolean CStandardAcquisitionBehavior::processClock(
	IBoxBehaviorContext& rBoxBehaviorContext,
	CMessageClock& rMessageClock)
{
	return true;
}

boolean CStandardAcquisitionBehavior::processInput(
	IBoxBehaviorContext& rBoxBehaviorContext,
	uint32 ui32InputIndex)
{
}
