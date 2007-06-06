#include "ovp_defines.h"
#include "ovpCChannelSelector.h"
#include "ovpCSimpleDSP.h"
#include "ovpCSignalAverage.h"

static OpenViBEPlugins::SignalProcessing::CChannelSelectorDesc* gst_pChannelSelectorDesc=NULL;
static OpenViBEPlugins::SignalProcessing::CSimpleDSPDesc* gst_pSimpleDSPDesc=NULL;
static OpenViBEPlugins::SignalProcessing::CSignalAverageDesc* gst_pSignalAverageDesc=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(void)
{
	gst_pChannelSelectorDesc=new OpenViBEPlugins::SignalProcessing::CChannelSelectorDesc();
	gst_pSimpleDSPDesc=new OpenViBEPlugins::SignalProcessing::CSimpleDSPDesc();
	gst_pSignalAverageDesc=new OpenViBEPlugins::SignalProcessing::CSignalAverageDesc();

	return true;
}

OVP_API OpenViBE::boolean onUninitialize(void)
{
	delete gst_pChannelSelectorDesc;
	delete gst_pSimpleDSPDesc;
	delete gst_pSignalAverageDesc;

	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=gst_pChannelSelectorDesc; break;
		case 1: rpPluginObjectDescription=gst_pSimpleDSPDesc; break;
		case 2: rpPluginObjectDescription=gst_pSignalAverageDesc; break;
		default: return false;
	}
	return true;
}

}
