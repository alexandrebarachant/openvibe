#include "ova_defines.h"
#include "OBTPluginInformation.h"

OMK::Name g_sVrpnButtonStateUpdate("vrpn_button state update");
OMK::Name g_sVrpnAnalogStateUpdate("vrpn_analog state update");

/*
OMK::Name g_s3DEntityManipulator_Reset("3d_entity_manipulator reset");

OMK::Name g_sManipulate3DEntity_Goal("manipulable_3d_entity goal");

OMK::Name g_sManipulate3DEntity_Translate("manipulable_3d_entity translate");
OMK::Name g_sManipulate3DEntity_SetPosition("manipulable_3d_entity set position");
OMK::Name g_sManipulate3DEntity_Rotate("manipulable_3d_entity rotate");
OMK::Name g_sManipulate3DEntity_SetOrientation("manipulable_3d_entity set orientation");
OMK::Name g_sManipulate3DEntity_Scale("manipulable_3d_entity scale");
OMK::Name g_sManipulate3DEntity_SetScale("manipulable_3d_entity set scale");
OMK::Name g_sManipulate3DEntity_Reset("manipulable_3d_entity reset");
*/

extern "C"
{
	OVA_API const OBT::PluginInformation& getPluginInformation(void)
	{
		static OBT::PluginInformation lst_oPluginInfo("OpenViBE-vr-demo-dynamic", "OpenViBE Plugin", 0, 1, "", "");
		return lst_oPluginInfo;
	}

	OVA_API bool initPlugin(const char* sConfigurationFile)
	{
		return true ;
	}

	OVA_API bool finishPlugin(void)
	{
		return true ;
	}
}
