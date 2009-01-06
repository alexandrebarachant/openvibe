#include "ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#if defined TARGET_HAS_ThirdPartyBLiFF
#include "algorithms/classification/ovpCAlgorithmClassifierTrainerBliffLDA.h"
#include "algorithms/classification/ovpCAlgorithmClassifierProcessorBliffLDA.h"
#endif // TARGET_HAS_ThirdPartyBLiFF

#if defined TARGET_HAS_ThirdPartyITPP
#include "algorithms/classification/ovpCAlgorithmClassifierTrainerLDA.h"
#include "algorithms/classification/ovpCAlgorithmClassifierProcessorLDA.h"
#endif // TARGET_HAS_ThirdPartyITPP

#include <vector>

OVP_Declare_Begin()

#if defined TARGET_HAS_ThirdPartyBLiFF
	// rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVTK_TypeId_ClassificationProcessorAlgorithm, "BLiFF - Linear Discrimimant Analysis (LDA)", OVP_ClassId_Algorithm_ClassifierProcessorBliffLDA.toUInteger());
	// rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVTK_TypeId_ClassificationTrainerAlgorithm,   "BLiFF - Linear Discrimimant Analysis (LDA)", OVP_ClassId_Algorithm_ClassifierTrainerBliffLDA.toUInteger());

	// OVP_Declare_New(OpenViBEPlugins::Local::CAlgorithmClassifierTrainerBliffLDADesc);
	// OVP_Declare_New(OpenViBEPlugins::Local::CAlgorithmClassifierProcessorBliffLDADesc);
#endif // TARGET_HAS_ThirdPartyBLiFF

#if defined TARGET_HAS_ThirdPartyITPP
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVTK_TypeId_ClassificationProcessorAlgorithm, "Linear Discrimimant Analysis (LDA)", OVP_ClassId_Algorithm_ClassifierProcessorLDA.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVTK_TypeId_ClassificationTrainerAlgorithm,   "Linear Discrimimant Analysis (LDA)", OVP_ClassId_Algorithm_ClassifierTrainerLDA.toUInteger());

	OVP_Declare_New(OpenViBEPlugins::Local::CAlgorithmClassifierTrainerLDADesc);
	OVP_Declare_New(OpenViBEPlugins::Local::CAlgorithmClassifierProcessorLDADesc);
#endif // TARGET_HAS_ThirdPartyITPP

OVP_Declare_End()
