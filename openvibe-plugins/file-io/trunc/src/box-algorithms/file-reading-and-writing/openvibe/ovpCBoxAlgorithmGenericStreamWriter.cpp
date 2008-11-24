#include "ovpCBoxAlgorithmGenericStreamWriter.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::FileIO;

/*
uint64 CBoxAlgorithmGenericStreamWriter::getClockFrequency(void)
{
	return 0; // the box clock frequency
}
*/

boolean CBoxAlgorithmGenericStreamWriter::initialize(void)
{
	// CString l_sSettingValue;
	// getStaticBoxContext().getSettingValue(0, l_sSettingValue);
	// ...

	return true;
}

boolean CBoxAlgorithmGenericStreamWriter::uninitialize(void)
{
	// ...

	return true;
}

/*
boolean CBoxAlgorithmGenericStreamWriter::processEvent(IMessageEvent& rMessageEvent)
{
	// ...

	// getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}
*/

/*
boolean CBoxAlgorithmGenericStreamWriter::processSignal(IMessageSignal& rMessageSignal)
{
	// ...

	// getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}
*/

/*
boolean CBoxAlgorithmGenericStreamWriter::processClock(IMessageClock& rMessageClock)
{
	// ...

	// getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}
*/

/*
boolean CBoxAlgorithmGenericStreamWriter::processInput(uint32 ui32InputIndex)
{
	// ...

	// getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}
*/

boolean CBoxAlgorithmGenericStreamWriter::process(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	// ...

	// l_rStaticBoxContext.getInputCount();
	// l_rStaticBoxContext.getOutputCount();
	// l_rStaticBoxContext.getSettingCount();

	// l_rDynamicBoxContext.getInputChunkCount()
	// l_rDynamicBoxContext.getInputChunk(i, )
	// l_rDynamicBoxContext.getOutputChunk(i, )

	return true;
}
