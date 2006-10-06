#include "ovp_defines.h"
#include "ovCBoxAlgorithm.h"
#include "ovCScenarioImporterXML.h"
#include "ovCScenarioExporterXML.h"
#include <iostream>

static OpenViBESamples::CBoxAlgorithmDesc* gst_pBoxAlgorithmDesc=NULL;
static OpenViBESamples::CScenarioImporterXMLDesc* gst_pScenarioImporterXML=NULL;
static OpenViBESamples::CScenarioExporterXMLDesc* gst_pScenarioExporterXML=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(void)
{
	gst_pBoxAlgorithmDesc=new OpenViBESamples::CBoxAlgorithmDesc();
	gst_pScenarioImporterXML=new OpenViBESamples::CScenarioImporterXMLDesc();
	gst_pScenarioExporterXML=new OpenViBESamples::CScenarioExporterXMLDesc();
	return true;
}

OVP_API OpenViBE::boolean onUninitialize(void)
{
	delete gst_pScenarioExporterXML;
	delete gst_pScenarioImporterXML;
	delete gst_pBoxAlgorithmDesc;
	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=gst_pBoxAlgorithmDesc; break;
		case 1: rpPluginObjectDescription=NULL; break;
		case 2: rpPluginObjectDescription=gst_pScenarioImporterXML; break;
		case 3: rpPluginObjectDescription=gst_pScenarioExporterXML; break;
		default: return false;
	}
	return true;
}

}
