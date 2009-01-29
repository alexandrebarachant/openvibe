#include <openvibe/ov_all.h>

#include "ovp_defines.h"

#if defined TARGET_HAS_ThirdPartyMatlab
#include "box-algorithms/tools/ovpCBoxAlgorithmMatlabFilter.h"
#endif // TARGET_HAS_ThirdPartyMatlab

#include <vector>

OVP_Declare_Begin()

#if defined TARGET_HAS_ThirdPartyMatlab
	OVP_Declare_New(OpenViBEPlugins::Local::CBoxAlgorithmMatlabFilterDesc);
#endif // TARGET_HAS_ThirdPartyMatlab

OVP_Declare_End()
