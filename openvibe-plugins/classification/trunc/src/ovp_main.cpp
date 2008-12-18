#include "ovp_defines.h"

#include "box-algorithms/classification/ovpCBoxAlgorithmClassifierProcessor.h"
#include "box-algorithms/classification/ovpCBoxAlgorithmClassifierTrainer.h"

#include <vector>

// #include "ovpCLDAClassifier.h"

OVP_Declare_Begin();
	OVP_Declare_New(OpenViBEPlugins::Local::CBoxAlgorithmClassifierTrainerDesc);
	OVP_Declare_New(OpenViBEPlugins::Local::CBoxAlgorithmClassifierProcessorDesc);

	// OVP_Declare_New(OpenViBEPlugins::Classification::CLDAClassifierDesc);
OVP_Declare_End();
