#include "ovp_defines.h"
#include <openvibe/ov_all.h>
#include "ovpCXMLStimulationScenarioPlayer.h"
#include "ovpCKeyboardStimulator.h"

static OpenViBEPlugins::Stimulation::CXMLStimulationScenarioPlayerDesc* gst_pXMLStimulationScenarioPlayerDesc=NULL;
static OpenViBEPlugins::Stimulation::CKeyboardStimulatorDesc* gst_pKeyboardStimulatorDesc=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(void)
{
	gst_pXMLStimulationScenarioPlayerDesc=new OpenViBEPlugins::Stimulation::CXMLStimulationScenarioPlayerDesc();
	gst_pKeyboardStimulatorDesc=new OpenViBEPlugins::Stimulation::CKeyboardStimulatorDesc();
	return true;
}

OVP_API OpenViBE::boolean onUninitialize(void)
{
	delete gst_pXMLStimulationScenarioPlayerDesc;
	delete gst_pKeyboardStimulatorDesc;
	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{

	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=gst_pXMLStimulationScenarioPlayerDesc; break;
		case 1: rpPluginObjectDescription=gst_pKeyboardStimulatorDesc; break;
		default: return false;
	}
	return true;
}

}
