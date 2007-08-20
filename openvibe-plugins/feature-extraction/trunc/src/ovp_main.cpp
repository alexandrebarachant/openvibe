#include "ovpCFeatureAggregator.h"

#include "ovp_defines.h"
#include <openvibe/ov_all.h>

static OpenViBEPlugins::FeatureExtraction::CFeatureAggregatorDesc* gst_pFeatureAggregatorDesc=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext)
{
	gst_pFeatureAggregatorDesc = new OpenViBEPlugins::FeatureExtraction::CFeatureAggregatorDesc();

	return true;
}

OVP_API OpenViBE::boolean onUninitialize(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext)
{
	delete gst_pFeatureAggregatorDesc;

	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext, OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{

	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription = gst_pFeatureAggregatorDesc; break;

		default: return false;
	}
	return true;
}

}
