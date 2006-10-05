#include "ovp_defines.h"
#include "ovCGenericNetworkAcquisition.h"
#include "ovCStandardAcquisitionBehavior.h"

static OpenViBEPlugins::Acquisition::CGenericNetworkAcquisitionDesc* gst_pGenericNetworkAcquisitionDesc=NULL;
static OpenViBEPlugins::Acquisition::CStandardAcquisitionBehaviorDesc* gst_pStandardAcquisitionBehaviorDesc=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(void)
{
	gst_pGenericNetworkAcquisitionDesc=new OpenViBEPlugins::Acquisition::CGenericNetworkAcquisitionDesc();
	gst_pStandardAcquisitionBehaviorDesc=new OpenViBEPlugins::Acquisition::CStandardAcquisitionBehaviorDesc();
	return true;
}

OVP_API OpenViBE::boolean onUninitialize(void)
{
	delete gst_pGenericNetworkAcquisitionDesc;
	delete gst_pStandardAcquisitionBehaviorDesc;
	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=gst_pGenericNetworkAcquisitionDesc; break;
		case 1: rpPluginObjectDescription=gst_pStandardAcquisitionBehaviorDesc; break;
		default: return false;
	}
	return true;
}

}
