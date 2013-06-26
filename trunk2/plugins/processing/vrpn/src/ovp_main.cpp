#include <openvibe/ov_all.h>

#include "ovp_defines.h"
#include "box-algorithms/ovpCVRPNAnalogServer.h"
#include "box-algorithms/ovpCVRPNButtonServer.h"
#include "box-algorithms/ovpCBoxAlgorithmVRPNAnalogClient.h"
#include "box-algorithms/ovpCBoxAlgorithmVRPNButtonClient.h"

#include <vector>

OVP_Declare_Begin();

#if defined TARGET_HAS_ThirdPartyVRPN
	OVP_Declare_New(OpenViBEPlugins::VRPN::CVRPNAnalogServerDesc);
	OVP_Declare_New(OpenViBEPlugins::VRPN::CVRPNButtonServerDesc);
	OVP_Declare_New(OpenViBEPlugins::VRPN::CBoxAlgorithmVRPNAnalogClientDesc);
	OVP_Declare_New(OpenViBEPlugins::VRPN::CBoxAlgorithmVRPNButtonClientDesc);
#endif

OVP_Declare_End();
