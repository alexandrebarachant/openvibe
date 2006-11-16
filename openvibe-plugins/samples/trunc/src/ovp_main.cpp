#include "ovp_defines.h"
#include "ovCBoxAlgorithmProducer.h"
#include "ovCBoxAlgorithmConsumer.h"
#include "ovCScenarioImporterXML.h"
#include "ovCScenarioExporterXML.h"
#include <iostream>

static OpenViBEPlugins::Samples::CBoxAlgorithmProducerDesc* gst_pBoxAlgorithmProducer=NULL;
static OpenViBEPlugins::Samples::CBoxAlgorithmConsumerDesc* gst_pBoxAlgorithmConsumer=NULL;
static OpenViBEPlugins::Samples::CScenarioImporterXMLDesc* gst_pScenarioImporterXML=NULL;
static OpenViBEPlugins::Samples::CScenarioExporterXMLDesc* gst_pScenarioExporterXML=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(void)
{
	gst_pBoxAlgorithmProducer=new OpenViBEPlugins::Samples::CBoxAlgorithmProducerDesc();
	gst_pBoxAlgorithmConsumer=new OpenViBEPlugins::Samples::CBoxAlgorithmConsumerDesc();
	gst_pScenarioImporterXML=new OpenViBEPlugins::Samples::CScenarioImporterXMLDesc();
	gst_pScenarioExporterXML=new OpenViBEPlugins::Samples::CScenarioExporterXMLDesc();
	return true;
}

OVP_API OpenViBE::boolean onUninitialize(void)
{
	delete gst_pScenarioExporterXML;
	delete gst_pScenarioImporterXML;
	delete gst_pBoxAlgorithmConsumer;
	delete gst_pBoxAlgorithmProducer;
	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=gst_pBoxAlgorithmProducer; break;
		case 1: rpPluginObjectDescription=gst_pBoxAlgorithmConsumer; break;
		case 2: rpPluginObjectDescription=gst_pScenarioImporterXML; break;
		case 3: rpPluginObjectDescription=gst_pScenarioExporterXML; break;
		default: return false;
	}
	return true;
}

}
