#include "ovp_defines.h"
#include "ovpCEBMLStreamSpy.h"

static OpenViBEPlugins::Tools::CEBMLStreamSpyDesc* gst_pEBMLStreamSpyDesc=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext)
{
	gst_pEBMLStreamSpyDesc=new OpenViBEPlugins::Tools::CEBMLStreamSpyDesc();

	return true;
}

OVP_API OpenViBE::boolean onUninitialize(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext)
{
	delete gst_pEBMLStreamSpyDesc;

	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext, OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=gst_pEBMLStreamSpyDesc; break;

		default: return false;
	}
	return true;
}

}
