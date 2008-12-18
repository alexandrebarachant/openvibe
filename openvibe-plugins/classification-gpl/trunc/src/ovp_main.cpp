#if defined TARGET_HAS_ThirdPartyBLiFF
#include "algorithms/classification/ovpCAlgorithmClassifierTrainerLDA.h"
#include "algorithms/classification/ovpCAlgorithmClassifierProcessorLDA.h"
#endif // TARGET_HAS_ThirdPartyBLiFF

OVP_Declare_Begin()

#if defined TARGET_HAS_ThirdPartyBLiFF
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVTK_TypeId_ClassificationProcessorAlgorithm, "Linear Discrimimant Analysis (LDA)", OVP_ClassId_Algorithm_ClassifierProcessorLDA.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVTK_TypeId_ClassificationTrainerAlgorithm,   "Linear Discrimimant Analysis (LDA)", OVP_ClassId_Algorithm_ClassifierTrainerLDA.toUInteger());

	OVP_Declare_New(OpenViBEPlugins::Local::CAlgorithmClassifierTrainerLDADesc);
	OVP_Declare_New(OpenViBEPlugins::Local::CAlgorithmClassifierProcessorLDADesc);
#endif // TARGET_HAS_ThirdPartyBLiFF

OVP_Declare_End()
