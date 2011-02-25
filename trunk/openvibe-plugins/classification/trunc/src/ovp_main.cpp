#include "ovp_defines.h"

#include "algorithms/classification/ovpCAlgorithmClassifierNULL.h"
#include "algorithms/classification/ovpCAlgorithmClassifierSVM.h"

#include "algorithms/confusion-matrix/ovpCAlgorithmConfusionMatrix.h"

#include "box-algorithms/classification/ovpCBoxAlgorithmVotingClassifier.h"
#include "box-algorithms/classification/ovpCBoxAlgorithmClassifierTrainer.h"
#include "box-algorithms/classification/ovpCBoxAlgorithmClassifierProcessor.h"
#include "box-algorithms/confusion-matrix/ovpCBoxAlgorithmConfusionMatrix.h"

#include <vector>

OVP_Declare_Begin();

//	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVTK_TypeId_ClassificationAlgorithm, "NULL Classifier (does nothing)",OVP_ClassId_Algorithm_ClassifierNULL.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVTK_TypeId_ClassificationAlgorithm, "Support Vector Machine (SVM)",OVP_ClassId_Algorithm_ClassifierSVM.toUInteger());

//	OVP_Declare_New(OpenViBEPlugins::Classification::CAlgorithmClassifierNULLDesc);
	OVP_Declare_New(OpenViBEPlugins::Classification::CAlgorithmClassifierSVMDesc);

	OVP_Declare_New(OpenViBEPlugins::Classification::CAlgorithmConfusionMatrixDesc);

	OVP_Declare_New(OpenViBEPlugins::Classification::CBoxAlgorithmVotingClassifierDesc);
	OVP_Declare_New(OpenViBEPlugins::Classification::CBoxAlgorithmClassifierTrainerDesc);
	OVP_Declare_New(OpenViBEPlugins::Classification::CBoxAlgorithmClassifierProcessorDesc);
	OVP_Declare_New(OpenViBEPlugins::Classification::CBoxAlgorithmConfusionMatrixDesc);

OVP_Declare_End();
