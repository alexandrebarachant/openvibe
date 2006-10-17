#include "ovp_defines.h"
#include "ovpCEEGStreamWriterGDF.h"

static OpenViBEPlugins::Utility::CEEGStreamWriterGDFDesc* gst_pEEGStreamWriterGDFDesc=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(void)
{
	gst_pEEGStreamWriterGDFDesc=new OpenViBEPlugins::Utility::CEEGStreamWriterGDFDesc();
	return true;
}

OVP_API OpenViBE::boolean onUninitialize(void)
{
	delete gst_pEEGStreamWriterGDFDesc;
	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=gst_pEEGStreamWriterGDFDesc; break;
		default: return false;
	}
	return true;
}

}
