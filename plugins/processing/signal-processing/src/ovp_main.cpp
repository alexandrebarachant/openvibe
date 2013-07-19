
#include "algorithms/basic/ovpCMatrixAverage.h"
#include "algorithms/epoching/ovpCAlgorithmStimulationBasedEpoching.h"
//#include "algorithms/filters/ovpCApplySpatialFilter.h"

#include "box-algorithms/basic/ovpCBoxAlgorithmChannelRename.h"
#include "box-algorithms/basic/ovpCBoxAlgorithmChannelSelector.h"
#include "box-algorithms/basic/ovpCBoxAlgorithmEpochAverage.h"
#include "box-algorithms/basic/ovpCBoxAlgorithmCrop.h"
#include "box-algorithms/basic/ovpCBoxAlgorithmSignalDecimation.h"
#include "box-algorithms/basic/ovpCBoxAlgorithmReferenceChannel.h"
#include "box-algorithms/basic/ovpCBoxAlgorithmDifferentialIntegral.h"
#include "box-algorithms/epoching/ovpCBoxAlgorithmStimulationBasedEpoching.h"
//#include "box-algorithms/filters/ovpCSpatialFilterBoxAlgorithm.h"
#include "box-algorithms/filters/ovpCBoxAlgorithmCommonAverageReference.h"
#include "box-algorithms/filters/ovpCBoxAlgorithmSpatialFilter.h"

#include "box-algorithms/spectral-analysis/ovpCBoxAlgorithmFrequencyBandSelector.h"
#include "box-algorithms/spectral-analysis/ovpCBoxAlgorithmSpectrumAverage.h"

#include "box-algorithms/ovpCTimeBasedEpoching.h"
#include "box-algorithms/ovpCReferenceChannel.h"
#include "box-algorithms/ovpCChannelSelector.h"
#include "box-algorithms/ovpCSimpleDSP.h"
#include "box-algorithms/ovpCSignalAverage.h"
#include "box-algorithms/ovpCSignalConcat.h"
#include "box-algorithms/ovpCFirstDifferenceDetrending.h"
#include "box-algorithms/ovpCSecondDifferenceDetrending.h"
#include "box-algorithms/ovpCBoxAlgorithmQuadraticForm.h"

#include "box-algorithms/filter/ovpCBoxAlgorithmXDAWNSpatialFilterTrainer.h"

#include "box-algorithms/basic/ovpCBoxAlgorithmIFFTbox.h"
#include "box-algorithms/basic/ovpCBoxAlgorithmEnvelope.h"

#include "algorithms/basic/ovpCAlgorithmARBurgMethod.h"
#include "box-algorithms/basic/ovpCBoxAlgorithmARCoefficients.h"

OVP_Declare_Begin()

	rPluginModuleContext.getTypeManager().registerEnumerationType (OVP_TypeId_EpochAverageMethod, "Epoch Average method");
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_EpochAverageMethod, "Moving epoch average", OVP_TypeId_EpochAverageMethod_MovingAverage.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_EpochAverageMethod, "Moving epoch average (Immediate)", OVP_TypeId_EpochAverageMethod_MovingAverageImmediate.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_EpochAverageMethod, "Epoch block average", OVP_TypeId_EpochAverageMethod_BlockAverage.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_EpochAverageMethod, "Cumulative average", OVP_TypeId_EpochAverageMethod_CumulativeAverage.toUInteger());

	rPluginModuleContext.getTypeManager().registerEnumerationType (OVP_TypeId_CropMethod, "Crop method");
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_CropMethod, "Min",     OVP_TypeId_CropMethod_Min.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_CropMethod, "Max",     OVP_TypeId_CropMethod_Max.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_CropMethod, "Min/Max", OVP_TypeId_CropMethod_MinMax.toUInteger());


	rPluginModuleContext.getTypeManager().registerEnumerationType (OVP_TypeId_SelectionMethod, "Selection method");
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_SelectionMethod, "Select", OVP_TypeId_SelectionMethod_Select.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_SelectionMethod, "Reject", OVP_TypeId_SelectionMethod_Reject.toUInteger());

	rPluginModuleContext.getTypeManager().registerEnumerationType (OVP_TypeId_MatchMethod, "Match method");
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_MatchMethod, "Name",  OVP_TypeId_MatchMethod_Name.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_MatchMethod, "Index", OVP_TypeId_MatchMethod_Index.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_MatchMethod, "Smart", OVP_TypeId_MatchMethod_Smart.toUInteger());

	rPluginModuleContext.getTypeManager().registerEnumerationType (OVP_TypeId_DifferentialIntegralOperation, "Differential/Integral select");
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_DifferentialIntegralOperation, "Differential", OVP_TypeId_DifferentialIntegralOperation_Differential.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_DifferentialIntegralOperation, "Integral",     OVP_TypeId_DifferentialIntegralOperation_Integral.toUInteger());

	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CTimeBasedEpochingDesc);

	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CMatrixAverageDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CAlgorithmStimulationBasedEpochingDesc)
//	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CApplySpatialFilterDesc)

	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmChannelRenameDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmChannelSelectorDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmReferenceChannelDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmDifferentialIntegralDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CEpochAverageDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmCropDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmSignalDecimationDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmStimulationBasedEpochingDesc)
//	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CSpatialFilterBoxAlgorithmDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmCommonAverageReferenceDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmSpatialFilterDesc)

	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CReferenceChannelOldDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CChannelSelectorDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CSimpleDSPDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CSignalAverageDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CSignalConcatenationDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CFirstDifferenceDetrendingDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CSecondDifferenceDetrendingDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmQuadraticFormDesc)

	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmFrequencyBandSelectorDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBoxAlgorithmSpectrumAverageDesc)

#if defined TARGET_HAS_ThirdPartyITPP
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CBoxAlgorithmXDAWNSpatialFilterTrainerDesc);

	OVP_Declare_New(OpenViBEPlugins::SignalProcessingBasic::CBoxAlgorithmIFFTboxDesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingBasic::CBoxAlgorithmEnvelopeDesc);

#endif // TARGET_HAS_ThirdPartyITPP


#ifdef __OpenViBEPlugins_BoxAlgorithm_ARCoefficients_H__
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CBoxAlgorithmARCoefficientsDesc);
#endif
#ifdef __OpenViBEPlugins_Algorithm_ARBurgMethod_H__
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CAlgorithmARBurgMethodDesc);
#endif
	
OVP_Declare_End()
