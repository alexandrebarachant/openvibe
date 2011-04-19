#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include "ovp_defines.h"

#include "box-algorithms/signal-processing/ovpCBoxAlgorithmStreamConcatenation.h"
#include "box-algorithms/signal-processing/ovpCBoxAlgorithmStreamSynchro.h"

#include "box-algorithms/stimulation/ovpCBoxAlgorithmP300SpellerSteadyStateStimulator.h"
#include "box-algorithms/stimulation/ovpCBoxAlgorithmSequenceTransducteur.h"
#include "box-algorithms/stimulation/ovpCBoxAlgorithmP300SpellerStimulatorLess.h"
#include "box-algorithms/stimulation/ovpCBoxAlgorithmStim2Signal.h"
#include "box-algorithms/stimulation/ovpCBoxAlgorithmStim2SignalAndPPort.h"
#include "box-algorithms/stimulation/ovpCBoxAlgorithmStim2Synchro.h"

OVP_Declare_Begin()

	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmStreamConcatenationDesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmStreamSynchroDesc);

	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmP300SpellerSteadyStateStimulatorDesc);
	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmSequenceTransducteurDesc);
	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmP300SpellerStimulatorLessDesc);
	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmStim2SignalDesc);
#if defined OVP_OS_Windows
	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmStim2SignalAndPPortDesc);
#endif
	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmStim2SynchroDesc);

OVP_Declare_End()
