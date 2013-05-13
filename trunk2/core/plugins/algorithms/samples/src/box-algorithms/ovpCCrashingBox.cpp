
#include "ovpCCrashingBox.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Samples;
using namespace std;

OpenViBE::boolean CCrashingBox::initialize(
	OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext)
{
	throw 0;
	return true;
}

OpenViBE::boolean CCrashingBox::uninitialize(
	OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext)
{
	int one=(int)1.0;
	int zero=(int)sin(0.0);
	int division_by_zero=one/zero;
	return division_by_zero?true:false;
}

OpenViBE::boolean CCrashingBox::processInput(
	OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext,
	OpenViBE::uint32 ui32InputIndex)
{
	rBoxAlgorithmContext.markAlgorithmAsReadyToProcess();
	return true;
}

OpenViBE::boolean CCrashingBox::process(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext)
{
	*((int*)NULL)=0;

	return true;
}
