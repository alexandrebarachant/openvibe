#include "ovp_defines.h"

#include "ovpCScenarioImporterXML.h"
#include "ovpCScenarioExporterXML.h"
#include "ovpCScenarioExporterSVG.h"

#include "box-algorithms/ovpCCrashingBox.h"
#include "box-algorithms/ovpCSinusSignalGenerator.h"
#include "box-algorithms/ovpCTimeSignalGenerator.h"
#include "box-algorithms/ovpCIdentity.h"
#include "box-algorithms/ovpCLog.h"
#include "box-algorithms/ovpCTest.h"
#include "box-algorithms/ovpCBoxAlgorithmClock.h"
#include "box-algorithms/ovpCBoxAlgorithmClockStimulator.h"

#include "algorithms/ovpCAlgorithmAddition.h"
#include "box-algorithms/ovpCBoxAlgorithmAdditionTest.h"

#include "box-algorithms/ovpCBoxAlgorithmNothing.h"

#include "box-algorithms/ovpCBoxAlgorithmMeanVariance.h"

#include "box-algorithms/ovpCTestCodecToolkit.h"

OVP_Declare_Begin();
	// rPluginModuleContext.getTypeManager().registerStreamType(OpenViBE::CIdentifier(0x1234,0x5678), "producer/consumer", OV_UndefinedIdentifier);

	// rPluginModuleContext.getTypeManager().registerEnumerationType(OpenViBE::CIdentifier(0x1234,0x0000), "sample enumeration");
	// rPluginModuleContext.getTypeManager().registerEnumerationEntry(OpenViBE::CIdentifier(0x1234,0x0000), "enum value 1", 1);
	// rPluginModuleContext.getTypeManager().registerEnumerationEntry(OpenViBE::CIdentifier(0x1234,0x0000), "enum value 2", 2);
	// rPluginModuleContext.getTypeManager().registerEnumerationEntry(OpenViBE::CIdentifier(0x1234,0x0000), "enum value 3", 3);
	// rPluginModuleContext.getTypeManager().registerEnumerationEntry(OpenViBE::CIdentifier(0x1234,0x0000), "enum value 4", 4);

	// rPluginModuleContext.getTypeManager().registerBitMaskType(OpenViBE::CIdentifier(0x0000,0x5678), "sample bitmask");
	// rPluginModuleContext.getTypeManager().registerBitMaskEntry(OpenViBE::CIdentifier(0x0000,0x5678), "bitmask value 1", 1);
	// rPluginModuleContext.getTypeManager().registerBitMaskEntry(OpenViBE::CIdentifier(0x0000,0x5678), "bitmask value 2", 2);
	// rPluginModuleContext.getTypeManager().registerBitMaskEntry(OpenViBE::CIdentifier(0x0000,0x5678), "bitmask value 3", 4);
	// rPluginModuleContext.getTypeManager().registerBitMaskEntry(OpenViBE::CIdentifier(0x0000,0x5678), "bitmask value 4", 8);

	OVP_Declare_New(OpenViBEPlugins::Samples::CScenarioImporterXMLDesc);
	OVP_Declare_New(OpenViBEPlugins::Samples::CScenarioExporterXMLDesc);
	OVP_Declare_New(OpenViBEPlugins::Samples::CScenarioExporterSVGDesc);

	// OVP_Declare_New(OpenViBEPlugins::Samples::CCrashingBoxDesc);
	// OVP_Declare_New(OpenViBEPlugins::Samples::CBoxAlgorithmProducerDesc);
	// OVP_Declare_New(OpenViBEPlugins::Samples::CBoxAlgorithmConsumerDesc);
	OVP_Declare_New(OpenViBEPlugins::Samples::CSinusSignalGeneratorDesc);
	OVP_Declare_New(OpenViBEPlugins::Samples::CTimeSignalGeneratorDesc);
	OVP_Declare_New(OpenViBEPlugins::Samples::CIdentityDesc);
	// OVP_Declare_New(OpenViBEPlugins::Samples::CLogDesc);
	// OVP_Declare_New(OpenViBEPlugins::Samples::CBoxAlgorithmClockDesc);
	OVP_Declare_New(OpenViBEPlugins::Samples::CBoxAlgorithmClockStimulatorDesc);

	// OVP_Declare_New(OpenViBEPlugins::Samples::CTestDesc);

	// OVP_Declare_New(OpenViBEPlugins::Samples::CBoxAlgorithmAdditionTestDesc);
	// OVP_Declare_New(OpenViBEPlugins::Samples::CAlgorithmAdditionDesc);

	// OVP_Declare_New(OpenViBEPlugins::Samples::CBoxAlgorithmNothingDesc);

	OVP_Declare_New(OpenViBEPlugins::Samples::CBoxAlgorithmMeanVarianceDesc);

	OVP_Declare_New(OpenViBEPlugins::Samples::CTestCodecToolkitDesc);
OVP_Declare_End();
