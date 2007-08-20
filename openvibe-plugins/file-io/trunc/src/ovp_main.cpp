#include "ovp_defines.h"
#include "ovpCGDFFileReader.h"
#include "ovpCGDFFileWriter.h"
#include "ovpCGenericStreamReader.h"
#include "ovpCGenericStreamWriter.h"
#include "ovpCBCICompetitionIIIbReader.h"

static OpenViBEPlugins::FileIO::CGDFFileReaderDesc* gst_pGDFFileReaderDesc=NULL;
static OpenViBEPlugins::FileIO::CGDFFileWriterDesc* gst_pGDFFileWriterDesc=NULL;

static OpenViBEPlugins::FileIO::CGenericStreamReaderDesc* gst_pDataStreamReaderDesc=NULL;
static OpenViBEPlugins::FileIO::CGenericStreamWriterDesc* gst_pDataStreamWriterDesc=NULL;
static OpenViBEPlugins::FileIO::CBCICompetitionIIIbReaderDesc* gst_pBCICompetitionIIIbReaderDesc=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext)
{
	gst_pGDFFileReaderDesc=new OpenViBEPlugins::FileIO::CGDFFileReaderDesc();
	gst_pGDFFileWriterDesc=new OpenViBEPlugins::FileIO::CGDFFileWriterDesc();

	gst_pDataStreamReaderDesc=new OpenViBEPlugins::FileIO::CGenericStreamReaderDesc();
	gst_pDataStreamWriterDesc=new OpenViBEPlugins::FileIO::CGenericStreamWriterDesc();

	gst_pBCICompetitionIIIbReaderDesc=new OpenViBEPlugins::FileIO::CBCICompetitionIIIbReaderDesc();

	return true;
}

OVP_API OpenViBE::boolean onUninitialize(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext)
{
	delete gst_pDataStreamWriterDesc;
	delete gst_pDataStreamReaderDesc;

	delete gst_pGDFFileWriterDesc;
	delete gst_pGDFFileReaderDesc;

	delete gst_pBCICompetitionIIIbReaderDesc;

	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(const OpenViBE::Kernel::IPluginModuleContext& rPluginModuleContext, OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=gst_pGDFFileReaderDesc; break;
		case 1: rpPluginObjectDescription=gst_pGDFFileWriterDesc; break;

		case 2: rpPluginObjectDescription=gst_pDataStreamReaderDesc; break;
		case 3: rpPluginObjectDescription=gst_pDataStreamWriterDesc; break;

		case 4: rpPluginObjectDescription=gst_pBCICompetitionIIIbReaderDesc; break;

		default: return false;
	}
	return true;
}

}
