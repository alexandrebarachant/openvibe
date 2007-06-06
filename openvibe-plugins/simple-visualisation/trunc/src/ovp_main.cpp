#include "ovp_defines.h"
#include <openvibe/ov_all.h>

#include "ovpCSignalDisplay.h"
#include "ovpCGrazVisualization.h"

static OpenViBEPlugins::SimpleVisualisation::CSignalDisplayDesc* gst_pSignalDisplayDesc=NULL;
static OpenViBEPlugins::SimpleVisualisation::CGrazVisualizationDesc* gst_pGrazVisualizationDesc=NULL;


extern "C"
{

OVP_API OpenViBE::boolean onInitialize(void)
{
	gst_pSignalDisplayDesc=new OpenViBEPlugins::SimpleVisualisation::CSignalDisplayDesc();
	gst_pGrazVisualizationDesc=new OpenViBEPlugins::SimpleVisualisation::CGrazVisualizationDesc();

	return true;
}

OVP_API OpenViBE::boolean onUninitialize(void)
{
	delete gst_pSignalDisplayDesc;
	delete gst_pGrazVisualizationDesc;
	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=gst_pSignalDisplayDesc; break;
		case 1: rpPluginObjectDescription=gst_pGrazVisualizationDesc; break;

		default: return false;
	}
	return true;
}

}
