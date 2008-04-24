#include "ovp_defines.h"

#include "ovpCXMLStimulationScenarioPlayer.h"
#include "ovpCKeyboardStimulator.h"

#include "box-algorithms/stimulation/ovpCBoxAlgorithmStimulationMultiplexer.h"
#include "box-algorithms/stimulation/ovpCBoxAlgorithmSoundPlayer.h"

OVP_Declare_Begin();
	OVP_Declare_New(OpenViBEPlugins::Stimulation::CXMLStimulationScenarioPlayerDesc);
	OVP_Declare_New(OpenViBEPlugins::Stimulation::CKeyboardStimulatorDesc);

	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmStimulationMultiplexerDesc);
	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmSoundPlayerDesc);
OVP_Declare_End();
