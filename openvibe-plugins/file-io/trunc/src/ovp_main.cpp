#include "ovp_defines.h"
#include "ovpCGDFFileReader.h"
#include "ovpCGDFFileWriter.h"
#include "ovpCGenericStreamReader.h"
#include "ovpCGenericStreamWriter.h"

static OpenViBEPlugins::FileIO::CGDFFileReaderDesc* gst_pGDFFileReaderDesc=NULL;
static OpenViBEPlugins::FileIO::CGDFFileWriterDesc* gst_pGDFFileWriterDesc=NULL;

static OpenViBEPlugins::FileIO::CGenericStreamReaderDesc* gst_pDataStreamReaderDesc=NULL;
static OpenViBEPlugins::FileIO::CGenericStreamWriterDesc* gst_pDataStreamWriterDesc=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(void)
{
	gst_pGDFFileReaderDesc=new OpenViBEPlugins::FileIO::CGDFFileReaderDesc();
	gst_pGDFFileWriterDesc=new OpenViBEPlugins::FileIO::CGDFFileWriterDesc();

	gst_pDataStreamReaderDesc=new OpenViBEPlugins::FileIO::CGenericStreamReaderDesc();
	gst_pDataStreamWriterDesc=new OpenViBEPlugins::FileIO::CGenericStreamWriterDesc();

	return true;
}

OVP_API OpenViBE::boolean onUninitialize(void)
{
	delete gst_pDataStreamWriterDesc;
	delete gst_pDataStreamReaderDesc;

	delete gst_pGDFFileWriterDesc;
	delete gst_pGDFFileReaderDesc;

	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=gst_pGDFFileReaderDesc; break;
		case 1: rpPluginObjectDescription=gst_pGDFFileWriterDesc; break;

		case 2: rpPluginObjectDescription=gst_pDataStreamReaderDesc; break;
		case 3: rpPluginObjectDescription=gst_pDataStreamWriterDesc; break;

		default: return false;
	}
	return true;
}

}
