#include <openvibe/ov_all.h>

#include "ovpCVRPNAnalogServer.h"
#include "ovpCVRPNButtonServer.h"

OVP_Declare_Begin();

#if defined TARGET_HAS_ThirdPartyVRPN
	OVP_Declare_New(OpenViBEPlugins::VRPN::CVRPNAnalogServerDesc);
	OVP_Declare_New(OpenViBEPlugins::VRPN::CVRPNButtonServerDesc);
#endif

OVP_Declare_End();
