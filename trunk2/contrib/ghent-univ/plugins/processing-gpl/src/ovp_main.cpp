
#include <vector>

#include <ov_all.h>

#include "ovp_defines.h"

#include "box-algorithms/ovpCBoxAlgorithmCSPSpatialFilterTrainer.h"

OVP_Declare_Begin();

#if defined TARGET_HAS_ThirdPartyITPP
	OVP_Declare_New(OpenViBEPlugins::SignalProcessingGpl::CBoxAlgorithmCSPSpatialFilterTrainerDesc);
#endif	

OVP_Declare_End();
