#include "ovIBoxAlgorithm.h"

using namespace OpenViBE;
using namespace OpenViBE::Plugins;

boolean IBoxAlgorithm::processEvent(
	IBoxAlgorithmContext& rBoxAlgorithmContext,
	CMessageEvent& rMessageEvent)
{
	return false;
}

boolean IBoxAlgorithm::processSignal(
	IBoxAlgorithmContext& rBoxAlgorithmContext,
	CMessageSignal& rMessageSignal)
{
	return false;
}

boolean IBoxAlgorithm::processClock(
	IBoxAlgorithmContext& rBoxAlgorithmContext,
	CMessageClock& rMessageClock)
{
	return false;
}

boolean IBoxAlgorithm::processInput(
	IBoxAlgorithmContext& rBoxAlgorithmContext,
	uint32 ui32InputIndex)
{
	return false;
}
