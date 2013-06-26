
#include <vector>
#include <openvibe/ov_all.h>
#include "ovp_defines.h"

#include "box-algorithms/ovpCBoxAlgorithmCSPSpatialFilterTrainer.h"		// ghent univ
#include "algorithms/ovpCAlgorithmUnivariateStatistics.h"				// gipsa
#include "box-algorithms/ovpCBoxAlgorithmUnivariateStatistics.h"		// gipsa
#include "box-algorithms/ovpCBoxAlgorithmSynchro.h"						// gipsa

#include "box-algorithms/ovpCSteadyStateFrequencyComparison.h"			// inserm
#include "box-algorithms/ovpCEpoching.h"								// inserm
#include "box-algorithms/ovpCBandFrequencyAverage.h"					// inserm

// @BEGIN inserm-gpl
#include "algorithms/ovpCDetectingMinMax.h"
#include "box-algorithms/ovpCDetectingMinMaxBoxAlgorithm.h"

#include "box-algorithms/ovpCWindowingFunctions.h"
#include "box-algorithms/ovpCSpectralAnalysisCospectra.h"
#include "box-algorithms/ovpCFastICA.h"
#include "box-algorithms/ovpCSpectralAnalysis.h"
#include "box-algorithms/ovpCTemporalFilter.h"

#include "algorithms/ovpCApplyTemporalFilter.h"
#include "algorithms/ovpCComputeTemporalFilterCoefficients.h"
#include "box-algorithms/ovpCTemporalFilterBoxAlgorithm.h"

#include "algorithms/ovpCDownsampling.h"
#include "box-algorithms/ovpCDownsamplingBoxAlgorithm.h"

#include "algorithms/ovpCDetectingMinMax.h"
#include "box-algorithms/ovpCDetectingMinMaxBoxAlgorithm.h"
// @END inserm-gpl

OVP_Declare_Begin();

#if defined TARGET_HAS_ThirdPartyITPP
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CBoxAlgorithmCSPSpatialFilterTrainerDesc); // ghent univ
#endif		

	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmSynchroDesc)		// gipsa
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CAlgoUnivariateStatisticDesc);	// gipsa
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxUnivariateStatisticDesc);	// gipsa

// @BEGIN inserm
	rPluginModuleContext.getTypeManager().registerEnumerationType (OVP_TypeId_ComparisonMethod, "Comparison method");
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_ComparisonMethod, "Ratio", OVP_TypeId_ComparisonMethod_Ratio.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_ComparisonMethod, "Substraction", OVP_TypeId_ComparisonMethod_Substraction.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_ComparisonMethod, "Laterality index", OVP_TypeId_ComparisonMethod_LateralityIndex.toUInteger());

	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBandFrequencyAverageDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CEpochingDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CSteadyStateFrequencyComparisonDesc)
// @END inserm
		
// @BEGIN inserm-gpl
	rPluginModuleContext.getTypeManager().registerBitMaskType (OVP_TypeId_SpectralComponent, "Spectral component");
	rPluginModuleContext.getTypeManager().registerBitMaskEntry(OVP_TypeId_SpectralComponent, "Amplitude",      OVP_TypeId_SpectralComponent_Amplitude.toUInteger());
	rPluginModuleContext.getTypeManager().registerBitMaskEntry(OVP_TypeId_SpectralComponent, "Phase",          OVP_TypeId_SpectralComponent_Phase.toUInteger());
	rPluginModuleContext.getTypeManager().registerBitMaskEntry(OVP_TypeId_SpectralComponent, "Real part",      OVP_TypeId_SpectralComponent_RealPart.toUInteger());
	rPluginModuleContext.getTypeManager().registerBitMaskEntry(OVP_TypeId_SpectralComponent, "Imaginary part", OVP_TypeId_SpectralComponent_ImaginaryPart.toUInteger());

	rPluginModuleContext.getTypeManager().registerEnumerationType (OVP_TypeId_FilterMethod, "Filter method");
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_FilterMethod, "Butterworth", OVP_TypeId_FilterMethod_Butterworth.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_FilterMethod, "Chebychev",   OVP_TypeId_FilterMethod_Chebychev.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_FilterMethod, "Yule-Walker", OVP_TypeId_FilterMethod_YuleWalker.toUInteger());

	rPluginModuleContext.getTypeManager().registerEnumerationType (OVP_TypeId_FilterType, "Filter type");
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_FilterType, "Low pass",  OVP_TypeId_FilterType_LowPass.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_FilterType, "Band pass", OVP_TypeId_FilterType_BandPass.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_FilterType, "High pass", OVP_TypeId_FilterType_HighPass.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_FilterType, "Band stop", OVP_TypeId_FilterType_BandStop.toUInteger());

	rPluginModuleContext.getTypeManager().registerEnumerationType (OVP_TypeId_WindowMethod, "Window method");
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_WindowMethod, "Hamming",     OVP_TypeId_WindowMethod_Hamming.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_WindowMethod, "Hanning",     OVP_TypeId_WindowMethod_Hanning.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_WindowMethod, "Hann",        OVP_TypeId_WindowMethod_Hann.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_WindowMethod, "Blackman",    OVP_TypeId_WindowMethod_Blackman.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_WindowMethod, "Triangular",  OVP_TypeId_WindowMethod_Triangular.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_WindowMethod, "Square root", OVP_TypeId_WindowMethod_SquareRoot.toUInteger());

	rPluginModuleContext.getTypeManager().registerEnumerationType (OVP_TypeId_FrequencyCutOffRatio, "Frequency cut off ratio");
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_FrequencyCutOffRatio, "1/4", OVP_TypeId_FrequencyCutOffRatio_14.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_FrequencyCutOffRatio, "1/3", OVP_TypeId_FrequencyCutOffRatio_13.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_FrequencyCutOffRatio, "1/2", OVP_TypeId_FrequencyCutOffRatio_12.toUInteger());

	rPluginModuleContext.getTypeManager().registerEnumerationType (OVP_TypeId_MinMax, "Min/Max");
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_MinMax, "Min", OVP_TypeId_MinMax_Min.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_MinMax, "Max", OVP_TypeId_MinMax_Max.toUInteger());
#if defined TARGET_HAS_ThirdPartyITPP

	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CSpectralAnalysisDesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CSpectralAnalysisCospectraDesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CFastICADesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CTemporalFilterDesc);

	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CWindowingFunctionsDesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CComputeTemporalFilterCoefficientsDesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CTemporalFilterBoxAlgorithmDesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CApplyTemporalFilterDesc);
#endif // TARGET_HAS_ThirdPartyITPP

	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CDownsamplingDesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CDownsamplingBoxAlgorithmDesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CDetectingMinMaxDesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CDetectingMinMaxBoxAlgorithmDesc);
// @END inserm-gpl
		 
OVP_Declare_End();
