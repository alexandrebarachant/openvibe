#include "ovp_defines.h"
#include "ovCGenericNetworkAcquisition.h"

static OpenViBEPlugins::Acquisition::CGenericNetworkAcquisitionDesc* gst_pGenericNetworkAcquisitionDesc=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(void)
{
	gst_pGenericNetworkAcquisitionDesc=new OpenViBEPlugins::Acquisition::CGenericNetworkAcquisitionDesc();
	return true;
}

OVP_API OpenViBE::boolean onUninitialize(void)
{
	delete gst_pGenericNetworkAcquisitionDesc;
	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=gst_pGenericNetworkAcquisitionDesc; break;
		default: return false;
	}
	return true;
}

}
