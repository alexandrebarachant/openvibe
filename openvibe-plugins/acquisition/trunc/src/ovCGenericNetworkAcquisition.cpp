#include "ovCGenericNetworkAcquisition.h"

using namespace OpenViBE;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Acquisition;

boolean CGenericNetworkAcquisitionDesc::getBoxPrototype(IBoxProto& rPrototype) const
{
	rPrototype.addOutput("EEG stream",           OV_TypeId_EEGStream);

	rPrototype.addSetting("EEG server hostname", OV_TypeId_String,  "localhost");
	rPrototype.addSetting("EEG server port",     OV_TypeId_Integer, "9999");

	return true;
}

uint32 CGenericNetworkAcquisitionDesc::getClockFrequency(void) const
{
	return 100; // $$$
}

boolean CGenericNetworkAcquisition::processClock(
	IBoxAlgorithmContext& rBoxAlgorithmContext,
	CMessageClock& rMessageClock)
{
	rBoxAlgorithmContext.markAlgorithmAsReadyToProcess();
	return true; // $$$
}

boolean CGenericNetworkAcquisition::process(IBoxAlgorithmContext& rBoxAlgorithmContext)
{
	IBoxContext& l_rBoxContext=rBoxAlgorithmContext.getBoxContext();

	return true;
}
