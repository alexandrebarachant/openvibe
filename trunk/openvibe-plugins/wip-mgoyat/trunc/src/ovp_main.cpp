#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include "ovp_defines.h"

#include "box-algorithms/acquisition/ovpCBoxAlgorithmBrutEEGSignalServer.h"
#include "box-algorithms/acquisition/ovpCBoxAlgorithmCVKserver.h"
#include "box-algorithms/acquisition/ovpCBoxAlgorithmExperimentStarter.h"

#include "box-algorithms/classification/ovpCBoxAlgorithmClassifierQualifier.h"

#include "algorithms/signal-processing/ovpCAlgorithmEMA.h"
#include "box-algorithms/signal-processing/ovpCBoxAlgorithmEMA.h"
#include "box-algorithms/signal-processing/ovpCBoxAlgorithmTimeShaker.h"
#include "box-algorithms/signal-processing/ovpCBoxAlgorithmStim2Matrix.h"
#include "box-algorithms/signal-processing/ovpCBoxAlgorithmStim2Signal.h"
#include "box-algorithms/signal-processing/ovpCBoxAlgorithmSignal2Stim.h"
#include "box-algorithms/signal-processing/ovpCBoxAlgorithmRecalleurP300PortParallele.h"
#include "box-algorithms/signal-processing/ovpCBoxAlgorithmTestAhmed.h"

#include "box-algorithms/stimulation/ovpCBoxAlgorithmP300SpellerSteadyStateStimulator.h"
#include "box-algorithms/stimulation/ovpCBoxAlgorithmSequenceTransducteur.h"
#include "box-algorithms/stimulation/ovpCBoxAlgorithmP300SpellerStimulatorLess.h"
#include "box-algorithms/stimulation/ovpCBoxAlgorithmSequenceGenerateur.h"
#include "box-algorithms/stimulation/ovpCBoxAlgorithmP300StimulateurPattern.h"
#include "box-algorithms/stimulation/ovpCBoxAlgorithmP300TargetPattern.h"
#include "box-algorithms/stimulation/ovpCBoxAlgorithmStimulationNumberFilter.h"
#include "box-algorithms/stimulation/ovpCBoxAlgorithmStimulationArtefactFilter.h"

#include "box-algorithms/streaming/ovpCBoxAlgorithmStreamedMatrixMerger.h"

OVP_Declare_Begin()

	OVP_Declare_New(OpenViBEPlugins::Acquisition::CBoxAlgorithmBrutEEGSignalServerDesc)
	OVP_Declare_New(OpenViBEPlugins::Acquisition::CBoxAlgorithmCVKServerDesc)
#if defined OVP_OS_Windows
	OVP_Declare_New(OpenViBEPlugins::Acquisition::CBoxAlgorithmExperimentStarterDesc)
#endif

	OVP_Declare_New(OpenViBEPlugins::Classification::CBoxAlgorithmClassifierQualifierDesc);

	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CAlgorithmEMADesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmEMADesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxTimeMoverDesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmStim2MatrixDesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmStim2SignalDesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmSignal2StimDesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmRecalleurP300Desc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmTestAhmedDesc);

	rPluginModuleContext.getTypeManager().registerEnumerationType (OVP_TypeId_SequenceGenerationType, "Sequence generation type");
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_SequenceGenerationType, "Random Raw + Column",      OVP_TypeId_SequenceRand_RC.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_SequenceGenerationType, "Random Raw then Column",          OVP_TypeId_SequenceRand_R_Rand_C.toUInteger());
	
	rPluginModuleContext.getTypeManager().registerEnumerationType(OV_TypeId_SSComponent, "Stimulation Filter Action");
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OV_TypeId_SSComponent, "No SS stop", OV_TypeId_SSComponent_NONE.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OV_TypeId_SSComponent, "SS stop interTrial", OV_TypeId_SSComponent_INTERCHAR.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OV_TypeId_SSComponent, "SS stop interSegment", OV_TypeId_SSComponent_INTERREPET.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OV_TypeId_SSComponent, "SS stop each inter", OV_TypeId_SSComponent_INTERALL.toUInteger());

	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmP300SpellerSteadyStateStimulatorDesc);
	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmSequenceTransducteurDesc);
	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmP300SpellerStimulatorLessDesc);
	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmSequenceGenerateurDesc);
	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmP300StimulateurPatternDesc);
	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmP300TargetPatternDesc);
	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmNbStimFilterDesc);
	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmStimArtefactFilterDesc);

	OVP_Declare_New(OpenViBEPlugins::Streaming::CBoxAlgorithmStreamedMatrixMergerDesc)

OVP_Declare_End()
