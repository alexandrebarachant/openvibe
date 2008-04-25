#include "ovp_defines.h"
#include "ovpCGDFFileReader.h"
#include "ovpCGDFFileWriter.h"
#include "ovpCGenericStreamReader.h"
#include "ovpCGenericStreamWriter.h"
#include "ovpCBCICompetitionIIIbReader.h"

#include "algorithms/file-reading-and-writing/brainamp/ovpCAlgorithmBrainampFileReader.h"

#include "box-algorithms/file-reading-and-writing/brainamp/ovpCBoxAlgorithmBrainampFileReader.h"

OVP_Declare_Begin()
	OVP_Declare_New(OpenViBEPlugins::FileIO::CGDFFileReaderDesc)
	OVP_Declare_New(OpenViBEPlugins::FileIO::CGDFFileWriterDesc)

	OVP_Declare_New(OpenViBEPlugins::FileIO::CGenericStreamReaderDesc)
	OVP_Declare_New(OpenViBEPlugins::FileIO::CGenericStreamWriterDesc)

	OVP_Declare_New(OpenViBEPlugins::FileIO::CBCICompetitionIIIbReaderDesc)

	OVP_Declare_New(OpenViBEPlugins::FileIO::CAlgorithmBrainampFileReaderDesc)
	OVP_Declare_New(OpenViBEPlugins::FileIO::CBoxAlgorithmBrainampFileReaderDesc)
OVP_Declare_End()
