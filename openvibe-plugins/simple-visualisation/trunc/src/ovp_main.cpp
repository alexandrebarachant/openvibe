#include "ovp_defines.h"
#include <openvibe/ov_all.h>

#include "ovpCSignalDisplay.h"
#include "ovpCGrazVisualization.h"
#include "ovpCFrequencySpectrumDisplay.h"
#include "ovpCPowerSpectrumDisplay.h"

static OpenViBEPlugins::SimpleVisualisation::CSignalDisplayDesc g_oSignalDisplayDesc;
static OpenViBEPlugins::SimpleVisualisation::CGrazVisualizationDesc g_oGrazVisualizationDesc;
static OpenViBEPlugins::SimpleVisualisation::CFrequencySpectrumDisplayDesc g_oFrequencySpectrumDisplayDesc;
static OpenViBEPlugins::SimpleVisualisation::CPowerSpectrumDisplayDesc g_oPowerSpectrumDisplayDesc;


extern "C"
{

OVP_API OpenViBE::boolean onInitialize(void)
{
	return true;
}

OVP_API OpenViBE::boolean onUninitialize(void)
{
	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext, OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=&g_oSignalDisplayDesc; break;
		case 1: rpPluginObjectDescription=&g_oGrazVisualizationDesc; break;
		case 2: rpPluginObjectDescription=&g_oFrequencySpectrumDisplayDesc; break;
		case 3: rpPluginObjectDescription=&g_oPowerSpectrumDisplayDesc; break;
		default: return false;
	}
	return true;
}

}
