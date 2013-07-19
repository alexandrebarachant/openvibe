
#include <vector>
#include <openvibe/ov_all.h>
#include "ovp_defines.h"

// @BEGIN inserm-gpl

#include "algorithms/ovpCComputeFisherLdaFunction.h"
#include "algorithms/ovpCApplyFisherLdaFunction.h"
#include "algorithms/ovpCFeatureExtractionLda.h"
#include "box-algorithms/ovpCLDABoxAlgorithm.h"

#include "algorithms/ovpCNaiveBayesApplyFunction.h"
#include "box-algorithms/ovpCNaiveBayesApplyBoxAlgorithm.h"

// @END inserm-gpl


OVP_Declare_Begin();

// @BEGIN inserm-gpl
#if defined TARGET_HAS_ThirdPartyITPP
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CComputeFisherLdaFunctionDesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CApplyFisherLdaFunctionDesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CFeatureExtractionLdaDesc);

	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CNaiveBayesApplyFunctionDesc);			// Requires ITPP
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CNaiveBayesApplyBoxAlgorithmDesc);		// Depends on the previous
#endif // TARGET_HAS_ThirdPartyITPP

	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CLDABoxAlgorithmDesc);
// @END inserm-gpl
		 
OVP_Declare_End();
