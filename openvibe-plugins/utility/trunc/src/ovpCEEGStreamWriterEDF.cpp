#include "ovpCEEGStreamWriterEDF.h"

#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Utility;

boolean CEEGStreamWriterEDFDesc::getBoxPrototype(
	IBoxProto& rPrototype) const
{
	rPrototype.addInput("Incoming stream", OV_UndefinedIdentifier);

	rPrototype.addSetting("Filename", OV_TypeId_String, "dumped_stream_edf.txt");

	return true;
}

uint32 CEEGStreamWriterEDFDesc::getClockFrequency(void) const
{
	return 0;
}

boolean CEEGStreamWriterEDF::processInput(
	IBoxAlgorithmContext& rBoxAlgorithmContext,
	uint32 ui32InputIndex)
{
	rBoxAlgorithmContext.markAlgorithmAsReadyToProcess();
	return true;
}

boolean CEEGStreamWriterEDF::process(
	IBoxAlgorithmContext& rBoxAlgorithmContext)
{
	std::cout << "whatever you want" << std::endl;
}
