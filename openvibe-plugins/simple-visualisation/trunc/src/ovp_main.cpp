#include "ovp_defines.h"
#include <openvibe/ov_all.h>

#include "ovpCSignalDisplay.h"
#include "ovpCGrazVisualization.h"
#include "ovpCFrequencySpectrumDisplay.h"

static OpenViBEPlugins::SimpleVisualisation::CSignalDisplayDesc* gst_pSignalDisplayDesc=NULL;
static OpenViBEPlugins::SimpleVisualisation::CGrazVisualizationDesc* gst_pGrazVisualizationDesc=NULL;
static OpenViBEPlugins::SimpleVisualisation::CFrequencySpectrumDisplayDesc * gst_pFrequencySpectrumDisplayDesc=NULL;


extern "C"
{

OVP_API OpenViBE::boolean onInitialize()
{
	gst_pSignalDisplayDesc=new OpenViBEPlugins::SimpleVisualisation::CSignalDisplayDesc();
	gst_pGrazVisualizationDesc=new OpenViBEPlugins::SimpleVisualisation::CGrazVisualizationDesc();
	gst_pFrequencySpectrumDisplayDesc=new OpenViBEPlugins::SimpleVisualisation::CFrequencySpectrumDisplayDesc();

	return true;
}

OVP_API OpenViBE::boolean onUninitialize()
{
	delete gst_pSignalDisplayDesc;
	delete gst_pGrazVisualizationDesc;
	delete gst_pFrequencySpectrumDisplayDesc;
	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext, OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=gst_pSignalDisplayDesc; break;
		case 1: rpPluginObjectDescription=gst_pGrazVisualizationDesc; break;
		case 2: rpPluginObjectDescription=gst_pFrequencySpectrumDisplayDesc; break;
		default: return false;
	}
	return true;
}

}
