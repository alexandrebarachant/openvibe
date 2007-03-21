#include "ovp_defines.h"
#include "ovpCGDFReader.h"
#include "ovpCEEGStreamWriterGDF.h"
#include "ovpCDataStreamReader.h"
#include "ovpCDataStreamWriter.h"

static OpenViBEPlugins::FileIO::CGDFReaderDesc* gst_pGDFReaderDesc=NULL;
static OpenViBEPlugins::FileIO::CEEGStreamWriterGDFDesc* gst_pEEGStreamWriterGDFDesc=NULL;

static OpenViBEPlugins::FileIO::CDataStreamReaderDesc* gst_pDataStreamReaderDesc=NULL;
static OpenViBEPlugins::FileIO::CDataStreamWriterDesc* gst_pDataStreamWriterDesc=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(void)
{
	gst_pGDFReaderDesc=new OpenViBEPlugins::FileIO::CGDFReaderDesc();
	gst_pEEGStreamWriterGDFDesc=new OpenViBEPlugins::FileIO::CEEGStreamWriterGDFDesc();

	gst_pDataStreamReaderDesc=new OpenViBEPlugins::FileIO::CDataStreamReaderDesc();
	gst_pDataStreamWriterDesc=new OpenViBEPlugins::FileIO::CDataStreamWriterDesc();

	return true;
}

OVP_API OpenViBE::boolean onUninitialize(void)
{
	delete gst_pDataStreamWriterDesc;
	delete gst_pDataStreamReaderDesc;

	delete gst_pEEGStreamWriterGDFDesc;
	delete gst_pGDFReaderDesc;

	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=gst_pGDFReaderDesc; break;
		case 1: rpPluginObjectDescription=gst_pEEGStreamWriterGDFDesc; break;

		case 2: rpPluginObjectDescription=gst_pDataStreamWriterDesc; break;
		case 3: rpPluginObjectDescription=gst_pDataStreamReaderDesc; break;

		default: return false;
	}
	return true;
}

}
