#include "ovp_defines.h"

#include "box-algorithms/filter/ovpCBoxAlgorithmXDAWNSpatialFilterTrainer.h"

#include "box-algorithms/basic/ovpCBoxAlgorithmIFFTbox.h"
#include "box-algorithms/basic/ovpCBoxAlgorithmEnvelope.h"

#include "algorithms/basic/ovpCAlgorithmARBurgMethod.h"
#include "box-algorithms/basic/ovpCBoxAlgorithmARFeatures.h"



OVP_Declare_Begin();

#if defined TARGET_HAS_ThirdPartyITPP
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CBoxAlgorithmXDAWNSpatialFilterTrainerDesc);

	OVP_Declare_New(OpenViBEPlugins::SignalProcessingBasic::CBoxAlgorithmIFFTboxDesc);
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingBasic::CBoxAlgorithmEnvelopeDesc);

#endif // TARGET_HAS_ThirdPartyITPP


#ifdef __OpenViBEPlugins_BoxAlgorithm_ARFeatures_H__
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CBoxAlgorithmARFeaturesDesc);
#endif
#ifdef __OpenViBEPlugins_Algorithm_ARBurgMethod_H__
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CAlgorithmARBurgMethodDesc);
#endif

	
	OVP_Declare_End();
