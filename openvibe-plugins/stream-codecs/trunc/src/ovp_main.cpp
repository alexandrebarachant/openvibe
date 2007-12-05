
#include "algorithms/decoders/ovpCAcquisitionDecoder.h"
#include "algorithms/decoders/ovpCExperimentInformationDecoder.h"
#include "algorithms/decoders/ovpCFeatureVectorDecoder.h"
#include "algorithms/decoders/ovpCSignalDecoder.h"
#include "algorithms/decoders/ovpCSpectrumDecoder.h"
#include "algorithms/decoders/ovpCStimulationDecoder.h"
#include "algorithms/decoders/ovpCStreamedMatrixDecoder.h"

// #include "algorithms/decoders/ovpCAcquisitionEncoder.h"
#include "algorithms/encoders/ovpCExperimentInformationEncoder.h"
#include "algorithms/encoders/ovpCFeatureVectorEncoder.h"
#include "algorithms/encoders/ovpCSignalEncoder.h"
#include "algorithms/encoders/ovpCSpectrumEncoder.h"
#include "algorithms/encoders/ovpCStimulationEncoder.h"
#include "algorithms/encoders/ovpCStreamedMatrixEncoder.h"

#include "box-algorithms/ovpCDecoderAlgorithmTest.h"
#include "box-algorithms/ovpCEncoderAlgorithmTest.h"

static OpenViBEPlugins::StreamCodecs::CAcquisitionDecoderDesc g_oAcquisitionDecoderDesc;
static OpenViBEPlugins::StreamCodecs::CExperimentInformationDecoderDesc g_oExperimentInformationDecoderDesc;
static OpenViBEPlugins::StreamCodecs::CFeatureVectorDecoderDesc g_oFeatureVectorDecoderDesc;
static OpenViBEPlugins::StreamCodecs::CSignalDecoderDesc g_oSignalDecoderDesc;
static OpenViBEPlugins::StreamCodecs::CSpectrumDecoderDesc g_oSpectrumDecoderDesc;
static OpenViBEPlugins::StreamCodecs::CStimulationDecoderDesc g_oStimulationDecoderDesc;
static OpenViBEPlugins::StreamCodecs::CStreamedMatrixDecoderDesc g_oStreamedMatrixDecoderDesc;

// static OpenViBEPlugins::StreamCodecs::CAcquisitionEncoderDesc g_oAcquisitionEncoderDesc;
static OpenViBEPlugins::StreamCodecs::CExperimentInformationEncoderDesc g_oExperimentInformationEncoderDesc;
static OpenViBEPlugins::StreamCodecs::CFeatureVectorEncoderDesc g_oFeatureVectorEncoderDesc;
static OpenViBEPlugins::StreamCodecs::CSignalEncoderDesc g_oSignalEncoderDesc;
static OpenViBEPlugins::StreamCodecs::CSpectrumEncoderDesc g_oSpectrumEncoderDesc;
static OpenViBEPlugins::StreamCodecs::CStimulationEncoderDesc g_oStimulationEncoderDesc;
static OpenViBEPlugins::StreamCodecs::CStreamedMatrixEncoderDesc g_oStreamedMatrixEncoderDesc;

static OpenViBEPlugins::StreamCodecs::CDecoderAlgorithmTestDesc g_oDecoderAlgorithmTestDesc;
static OpenViBEPlugins::StreamCodecs::CEncoderAlgorithmTestDesc g_oEncoderAlgorithmTestDesc;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext)
{
	return true;
}

OVP_API OpenViBE::boolean onUninitialize(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext)
{
	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext, OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=&g_oAcquisitionDecoderDesc; break;
		case 1: rpPluginObjectDescription=&g_oExperimentInformationDecoderDesc; break;
		case 2: rpPluginObjectDescription=&g_oFeatureVectorDecoderDesc; break;
		case 3: rpPluginObjectDescription=&g_oSignalDecoderDesc; break;
		case 4: rpPluginObjectDescription=&g_oSpectrumDecoderDesc; break;
		case 5: rpPluginObjectDescription=&g_oStimulationDecoderDesc; break;
		case 6: rpPluginObjectDescription=&g_oStreamedMatrixDecoderDesc; break;

		case 7: rpPluginObjectDescription=NULL; break; // rpPluginObjectDescription=&g_oAcquisitionEncoderDesc; break;
		case 8: rpPluginObjectDescription=&g_oExperimentInformationEncoderDesc; break;
		case 9: rpPluginObjectDescription=&g_oFeatureVectorEncoderDesc; break;
		case 10: rpPluginObjectDescription=&g_oSignalEncoderDesc; break;
		case 11: rpPluginObjectDescription=&g_oSpectrumEncoderDesc; break;
		case 12: rpPluginObjectDescription=&g_oStimulationEncoderDesc; break;
		case 13: rpPluginObjectDescription=&g_oStreamedMatrixEncoderDesc; break;

		case 14: rpPluginObjectDescription=&g_oDecoderAlgorithmTestDesc; break;
		case 15: rpPluginObjectDescription=&g_oEncoderAlgorithmTestDesc; break;

		default: return false;
	}
	return true;
}

}
