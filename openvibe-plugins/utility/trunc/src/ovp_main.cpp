#include "ovp_defines.h"
#include "ovpCEEGStreamWriterEDF.h"

static OpenViBEPlugins::Utility::CEEGStreamWriterEDFDesc* gst_pEEGStreamWriterEDFDesc=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(void)
{
	gst_pEEGStreamWriterEDFDesc=new OpenViBEPlugins::Utility::CEEGStreamWriterEDFDesc();
	return true;
}

OVP_API OpenViBE::boolean onUninitialize(void)
{
	delete gst_pEEGStreamWriterEDFDesc;
	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=gst_pEEGStreamWriterEDFDesc; break;
		default: return false;
	}
	return true;
}

}
