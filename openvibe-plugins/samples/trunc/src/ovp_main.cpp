#include "ovp_defines.h"
#include "ovpCScenarioImporterXML.h"
#include "ovpCScenarioExporterXML.h"
#include "ovpCCrashingBox.h"
#include "ovpCBoxAlgorithmProducer.h"
#include "ovpCBoxAlgorithmConsumer.h"
#include "ovpCSinusSignalGenerator.h"
#include "ovpCTimeSignalGenerator.h"
#include "ovpCIdentity.h"
#include "ovpCLog.h"
#include "ovpCTest.h"

// #define _WithTBE_

#if defined _WithTBE_
#include "ovpCTimeBasedEpoching.h"
#endif

static OpenViBEPlugins::Samples::CScenarioImporterXMLDesc* gst_pScenarioImporterXMLDesc=NULL;
static OpenViBEPlugins::Samples::CScenarioExporterXMLDesc* gst_pScenarioExporterXMLDesc=NULL;

static OpenViBEPlugins::Samples::CCrashingBoxDesc* gst_pCrashingBoxDesc=NULL;
static OpenViBEPlugins::Samples::CBoxAlgorithmProducerDesc* gst_pBoxAlgorithmProducerDesc=NULL;
static OpenViBEPlugins::Samples::CBoxAlgorithmConsumerDesc* gst_pBoxAlgorithmConsumerDesc=NULL;
static OpenViBEPlugins::Samples::CSinusSignalGeneratorDesc* gst_pSinusSignalGeneratorDesc=NULL;
static OpenViBEPlugins::Samples::CTimeSignalGeneratorDesc* gst_pTimeSignalGeneratorDesc=NULL;
static OpenViBEPlugins::Samples::CIdentityDesc* gst_pIdentityDesc=NULL;
static OpenViBEPlugins::Samples::CLogDesc* gst_pLogDesc=NULL;
static OpenViBEPlugins::Samples::CTestDesc* gst_pTestDesc=NULL;

#if defined _WithTBE_
static OpenViBEPlugins::Samples::CTimeBasedEpochingDesc* gst_pTimeBasedEpochingDesc=NULL;
#endif

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext)
{
	rPluginModuleContext.getTypeManager().registerStreamType(OpenViBE::CIdentifier(0x1234,0x5678), "producer/consumer", OV_UndefinedIdentifier);

	rPluginModuleContext.getTypeManager().registerEnumerationType(OpenViBE::CIdentifier(0x1234,0x0000), "sample enumeration");
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OpenViBE::CIdentifier(0x1234,0x0000), "enum value 1", 1);
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OpenViBE::CIdentifier(0x1234,0x0000), "enum value 2", 2);
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OpenViBE::CIdentifier(0x1234,0x0000), "enum value 3", 3);
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OpenViBE::CIdentifier(0x1234,0x0000), "enum value 4", 4);

	rPluginModuleContext.getTypeManager().registerBitMaskType(OpenViBE::CIdentifier(0x0000,0x5678), "sample bitmask");
	rPluginModuleContext.getTypeManager().registerBitMaskEntry(OpenViBE::CIdentifier(0x0000,0x5678), "bitmask value 1", 1);
	rPluginModuleContext.getTypeManager().registerBitMaskEntry(OpenViBE::CIdentifier(0x0000,0x5678), "bitmask value 2", 2);
	rPluginModuleContext.getTypeManager().registerBitMaskEntry(OpenViBE::CIdentifier(0x0000,0x5678), "bitmask value 3", 3);
	rPluginModuleContext.getTypeManager().registerBitMaskEntry(OpenViBE::CIdentifier(0x0000,0x5678), "bitmask value 4", 4);

	gst_pScenarioImporterXMLDesc=new OpenViBEPlugins::Samples::CScenarioImporterXMLDesc();
	gst_pScenarioExporterXMLDesc=new OpenViBEPlugins::Samples::CScenarioExporterXMLDesc();

	gst_pCrashingBoxDesc=new OpenViBEPlugins::Samples::CCrashingBoxDesc();
	gst_pBoxAlgorithmProducerDesc=new OpenViBEPlugins::Samples::CBoxAlgorithmProducerDesc();
	gst_pBoxAlgorithmConsumerDesc=new OpenViBEPlugins::Samples::CBoxAlgorithmConsumerDesc();
	gst_pSinusSignalGeneratorDesc=new OpenViBEPlugins::Samples::CSinusSignalGeneratorDesc();
	gst_pTimeSignalGeneratorDesc=new OpenViBEPlugins::Samples::CTimeSignalGeneratorDesc();
	gst_pIdentityDesc=new OpenViBEPlugins::Samples::CIdentityDesc();
	gst_pLogDesc=new OpenViBEPlugins::Samples::CLogDesc();
	gst_pTestDesc=new OpenViBEPlugins::Samples::CTestDesc();

#if defined _WithTBE_
	gst_pTimeBasedEpochingDesc=new OpenViBEPlugins::Samples::CTimeBasedEpochingDesc();
#endif

	return true;
}

OVP_API OpenViBE::boolean onUninitialize(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext)
{
	delete gst_pTestDesc;
	delete gst_pLogDesc;
	delete gst_pIdentityDesc;
	delete gst_pTimeSignalGeneratorDesc;
	delete gst_pSinusSignalGeneratorDesc;
	delete gst_pBoxAlgorithmConsumerDesc;
	delete gst_pBoxAlgorithmProducerDesc;
	delete gst_pCrashingBoxDesc;

	delete gst_pScenarioImporterXMLDesc;
	delete gst_pScenarioExporterXMLDesc;

#if defined _WithTBE_
	delete gst_pTimeBasedEpochingDesc;
#endif

	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext, OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
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
		case 8: rpPluginObjectDescription=gst_pLogDesc; break;
		case 9: rpPluginObjectDescription=gst_pTestDesc; break;

#if defined _WithTBE_
		case 10: rpPluginObjectDescription=gst_pTimeBasedEpochingDesc; break;
#endif

		default: return false;
	}
	return true;
}

}
