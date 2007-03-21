#include "ovp_defines.h"
#include "ovpCScenarioImporterXML.h"
#include "ovpCScenarioExporterXML.h"
#include "ovpCBoxAlgorithmProducer.h"
#include "ovpCBoxAlgorithmConsumer.h"
#include "ovpCSinusSignalGenerator.h"
#include "ovpCIdentity.h"

static OpenViBEPlugins::Samples::CScenarioImporterXMLDesc* gst_pScenarioImporterXMLDesc=NULL;
static OpenViBEPlugins::Samples::CScenarioExporterXMLDesc* gst_pScenarioExporterXMLDesc=NULL;

static OpenViBEPlugins::Samples::CBoxAlgorithmProducerDesc* gst_pBoxAlgorithmProducerDesc=NULL;
static OpenViBEPlugins::Samples::CBoxAlgorithmConsumerDesc* gst_pBoxAlgorithmConsumerDesc=NULL;
static OpenViBEPlugins::Samples::CSinusSignalGeneratorDesc* gst_pSinusSignalGeneratorDesc=NULL;
static OpenViBEPlugins::Samples::CIdentityDesc* gst_pIdentityDesc=NULL;


extern "C"
{

OVP_API OpenViBE::boolean onInitialize(void)
{
	gst_pScenarioImporterXMLDesc=new OpenViBEPlugins::Samples::CScenarioImporterXMLDesc();
	gst_pScenarioExporterXMLDesc=new OpenViBEPlugins::Samples::CScenarioExporterXMLDesc();

	gst_pBoxAlgorithmProducerDesc=new OpenViBEPlugins::Samples::CBoxAlgorithmProducerDesc();
	gst_pBoxAlgorithmConsumerDesc=new OpenViBEPlugins::Samples::CBoxAlgorithmConsumerDesc();
	gst_pSinusSignalGeneratorDesc=new OpenViBEPlugins::Samples::CSinusSignalGeneratorDesc();
	gst_pIdentityDesc=new OpenViBEPlugins::Samples::CIdentityDesc();

	return true;
}

OVP_API OpenViBE::boolean onUninitialize(void)
{
	delete gst_pIdentityDesc;
	delete gst_pSinusSignalGeneratorDesc;
	delete gst_pBoxAlgorithmConsumerDesc;
	delete gst_pBoxAlgorithmProducerDesc;

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

		case 2: rpPluginObjectDescription=gst_pBoxAlgorithmProducerDesc; break;
		case 3: rpPluginObjectDescription=gst_pBoxAlgorithmConsumerDesc; break;
		case 4: rpPluginObjectDescription=gst_pSinusSignalGeneratorDesc; break;
		case 5: rpPluginObjectDescription=gst_pIdentityDesc; break;

		default: return false;
	}
	return true;
}

}
