#include "ovp_defines.h"
#include "ovpCEEGStreamWriterGDF.h"
#include "ovpCDataStreamWriter.h"
#include "ovpCDataStreamReader.h"

static OpenViBEPlugins::Utility::CEEGStreamWriterGDFDesc* gst_pEEGStreamWriterGDFDesc=NULL;
static OpenViBEPlugins::Utility::CDataStreamWriterDesc* gst_pDataStreamWriterDesc=NULL;
static OpenViBEPlugins::Utility::CDataStreamReaderDesc* gst_pDataStreamReaderDesc=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(void)
{
	gst_pEEGStreamWriterGDFDesc=new OpenViBEPlugins::Utility::CEEGStreamWriterGDFDesc();
	gst_pDataStreamWriterDesc=new OpenViBEPlugins::Utility::CDataStreamWriterDesc();
	gst_pDataStreamReaderDesc=new OpenViBEPlugins::Utility::CDataStreamReaderDesc();

	return true;
}

OVP_API OpenViBE::boolean onUninitialize(void)
{
	delete gst_pEEGStreamWriterGDFDesc;
	delete gst_pDataStreamWriterDesc;
	delete gst_pDataStreamReaderDesc;
	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=gst_pEEGStreamWriterGDFDesc; break;
		case 1: rpPluginObjectDescription=gst_pDataStreamWriterDesc; break;
		case 2: rpPluginObjectDescription=gst_pDataStreamReaderDesc; break;

		default: return false;
	}
	return true;
}

}
