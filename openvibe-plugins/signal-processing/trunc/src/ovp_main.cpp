#include "ovp_defines.h"
#include "ovpCChannelSelector.h"

static OpenViBEPlugins::SignalProcessing::CChannelSelectorDesc* gst_pChannelSelectorDesc=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(void)
{
	gst_pChannelSelectorDesc=new OpenViBEPlugins::SignalProcessing::CChannelSelectorDesc();

	return true;
}

OVP_API OpenViBE::boolean onUninitialize(void)
{
	delete gst_pChannelSelectorDesc;

	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=gst_pChannelSelectorDesc; break;

		default: return false;
	}
	return true;
}

}
