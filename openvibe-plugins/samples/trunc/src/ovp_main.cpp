#include "ovp_defines.h"
#include "ovpCScenarioImporterXML.h"
#include "ovpCScenarioExporterXML.h"
#include "ovpCCrashingBox.h"
#include "ovpCBoxAlgorithmProducer.h"
#include "ovpCBoxAlgorithmConsumer.h"
#include "ovpCSinusSignalGenerator.h"
#include "ovpCTimeSignalGenerator.h"
#include "ovpCIdentity.h"

static OpenViBEPlugins::Samples::CScenarioImporterXMLDesc* gst_pScenarioImporterXMLDesc=NULL;
static OpenViBEPlugins::Samples::CScenarioExporterXMLDesc* gst_pScenarioExporterXMLDesc=NULL;

static OpenViBEPlugins::Samples::CCrashingBoxDesc* gst_pCrashingBoxDesc=NULL;
static OpenViBEPlugins::Samples::CBoxAlgorithmProducerDesc* gst_pBoxAlgorithmProducerDesc=NULL;
static OpenViBEPlugins::Samples::CBoxAlgorithmConsumerDesc* gst_pBoxAlgorithmConsumerDesc=NULL;
static OpenViBEPlugins::Samples::CSinusSignalGeneratorDesc* gst_pSinusSignalGeneratorDesc=NULL;
static OpenViBEPlugins::Samples::CTimeSignalGeneratorDesc* gst_pTimeSignalGeneratorDesc=NULL;
static OpenViBEPlugins::Samples::CIdentityDesc* gst_pIdentityDesc=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(void)
{
	gst_pScenarioImporterXMLDesc=new OpenViBEPlugins::Samples::CScenarioImporterXMLDesc();
	gst_pScenarioExporterXMLDesc=new OpenViBEPlugins::Samples::CScenarioExporterXMLDesc();

	gst_pCrashingBoxDesc=new OpenViBEPlugins::Samples::CCrashingBoxDesc();
	gst_pBoxAlgorithmProducerDesc=new OpenViBEPlugins::Samples::CBoxAlgorithmProducerDesc();
	gst_pBoxAlgorithmConsumerDesc=new OpenViBEPlugins::Samples::CBoxAlgorithmConsumerDesc();
	gst_pSinusSignalGeneratorDesc=new OpenViBEPlugins::Samples::CSinusSignalGeneratorDesc();
	gst_pTimeSignalGeneratorDesc=new OpenViBEPlugins::Samples::CTimeSignalGeneratorDesc();
	gst_pIdentityDesc=new OpenViBEPlugins::Samples::CIdentityDesc();

	return true;
}

OVP_API OpenViBE::boolean onUninitialize(void)
{
	delete gst_pIdentityDesc;
	delete gst_pTimeSignalGeneratorDesc;
	delete gst_pSinusSignalGeneratorDesc;
	delete gst_pBoxAlgorithmConsumerDesc;
	delete gst_pBoxAlgorithmProducerDesc;
	delete gst_pCrashingBoxDesc;

	delete gst_pScenarioImporterXMLDesc;
	delete gst_pScenarioExporterXMLDesc;

	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=gst_pScenarioImporterXMLDesc; break;
		case 1: rpPluginObjectDescription=gst_pScenarioExporterXMLDesc; break;

		case 2: rpPluginObjectDescription=gst_pCrashingBoxDesc; break;
		case 3: rpPluginObjectDescription=gst_pBoxAlgorithmProducerDesc; break;
		case 4: rpPluginObjectDescription=gst_pBoxAlgorithmConsumerDesc; break;
		case 5: rpPluginObjectDescription=gst_pSinusSignalGeneratorDesc; break;
		case 6: rpPluginObjectDescription=gst_pTimeSignalGeneratorDesc; break;
		case 7: rpPluginObjectDescription=gst_pIdentityDesc; break;

		default: return false;
	}
	return true;
}

}
