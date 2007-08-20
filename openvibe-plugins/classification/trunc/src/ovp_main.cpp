#include "ovpCLDAClassifier.h"

#include "ovp_defines.h"
#include <openvibe/ov_all.h>

static OpenViBEPlugins::Classification::CLDAClassifierDesc* gst_pLDACLassifierDesc=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext)
{
	gst_pLDACLassifierDesc = new OpenViBEPlugins::Classification::CLDAClassifierDesc();

	return true;
}

OVP_API OpenViBE::boolean onUninitialize(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext)
{
	delete gst_pLDACLassifierDesc;

	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext, OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{

	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription = gst_pLDACLassifierDesc; break;

		default: return false;
	}
	return true;
}

}
