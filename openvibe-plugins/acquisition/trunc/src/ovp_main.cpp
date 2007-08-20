#include "ovp_defines.h"
#include "ovpCGenericNetworkAcquisition.h"

static OpenViBEPlugins::Acquisition::CGenericNetworkAcquisitionDesc* gst_pGenericNetworkAcquisitionDesc=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext)
{
	gst_pGenericNetworkAcquisitionDesc=new OpenViBEPlugins::Acquisition::CGenericNetworkAcquisitionDesc();

	return true;
}

OVP_API OpenViBE::boolean onUninitialize(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext)
{
	delete gst_pGenericNetworkAcquisitionDesc;

	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext, OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=gst_pGenericNetworkAcquisitionDesc; break;

		default: return false;
	}
	return true;
}

}
