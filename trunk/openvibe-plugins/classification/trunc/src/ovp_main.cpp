#include "ovp_defines.h"

#include "box-algorithms/classification/ovpCBoxAlgorithmVotingClassifier.h"
#include "box-algorithms/classification/ovpCBoxAlgorithmClassifierTrainer.h"
#include "box-algorithms/classification/ovpCBoxAlgorithmClassifierProcessor.h"
#include "box-algorithms/confusion-matrix/ovpCBoxAlgorithmConfusionMatrix.h"

#include "algorithms/confusion-matrix/ovpCAlgorithmConfusionMatrix.h"

#include <vector>

// #include "ovpCLDAClassifier.h"

OVP_Declare_Begin();
	OVP_Declare_New(OpenViBEPlugins::Classification::CBoxAlgorithmVotingClassifierDesc);
	OVP_Declare_New(OpenViBEPlugins::Classification::CBoxAlgorithmClassifierTrainerDesc);
	OVP_Declare_New(OpenViBEPlugins::Classification::CBoxAlgorithmClassifierProcessorDesc);
	OVP_Declare_New(OpenViBEPlugins::Classification::CBoxAlgorithmConfusionMatrixDesc);

	// OVP_Declare_New(OpenViBEPlugins::Classification::CLDAClassifierDesc);

	OVP_Declare_New(OpenViBEPlugins::Classification::CAlgorithmConfusionMatrixDesc);
OVP_Declare_End();
