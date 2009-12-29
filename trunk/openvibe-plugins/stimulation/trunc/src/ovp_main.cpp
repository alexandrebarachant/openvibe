#include "ovp_defines.h"

#include "ovpCXMLStimulationScenarioPlayer.h"
#include "ovpCKeyboardStimulator.h"

#include "box-algorithms/stimulation/ovpCBoxAlgorithmStimulationMultiplexer.h"
#include "box-algorithms/stimulation/ovpCBoxAlgorithmSoundPlayer.h"

#include "box-algorithms/stimulation/ovpCBoxAlgorithmRunCommand.h"

#include "box-algorithms/stimulation/adaptation/ovpCBoxAlgorithmStimulationFilter.h"

#include "ovpCBoxAlgorithmP300SpellerStimulator.h"
#include "ovpCBoxAlgorithmP300IdentifierStimulator.h"

OVP_Declare_Begin();
	rPluginModuleContext.getTypeManager().registerEnumerationType(OVP_TypeId_StimulationFilterAction, "Stimulation Filter Action");
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_StimulationFilterAction, "Select", OVP_TypeId_StimulationFilterAction_Select.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_StimulationFilterAction, "Reject", OVP_TypeId_StimulationFilterAction_Reject.toUInteger());

	OVP_Declare_New(OpenViBEPlugins::Stimulation::CXMLStimulationScenarioPlayerDesc);
	OVP_Declare_New(OpenViBEPlugins::Stimulation::CKeyboardStimulatorDesc);

	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmStimulationMultiplexerDesc);
	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmSoundPlayerDesc);

	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmRunCommandDesc);

	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmStimulationFilterDesc);

	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmP300SpellerStimulatorDesc);
	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmP300IdentifierStimulatorDesc);
OVP_Declare_End();
