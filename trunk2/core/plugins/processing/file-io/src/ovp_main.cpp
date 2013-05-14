#include "ovp_defines.h"
#include "box-algorithms/ovpCGDFFileReader.h"
#include "box-algorithms/ovpCGDFFileWriter.h"
#include "box-algorithms/ovpCBCICompetitionIIIbReader.h"

#include "algorithms/brainamp/ovpCAlgorithmBrainampFileReader.h"

#include "algorithms/ovpCAlgorithmOVMatrixFileReader.h"
#include "algorithms/ovpCAlgorithmOVMatrixFileWriter.h"

#include "algorithms/xml-scenario/ovpCAlgorithmXMLScenarioExporter.h"
#include "algorithms/xml-scenario/ovpCAlgorithmXMLScenarioImporter.h"

#include "box-algorithms/brainamp/ovpCBoxAlgorithmBrainampFileReader.h"

#include "box-algorithms/csv/ovpCBoxAlgorithmCSVFileWriter.h"
#include "box-algorithms/csv/ovpCBoxAlgorithmCSVFileReader.h"

#include "box-algorithms/openvibe/ovpCBoxAlgorithmGenericStreamReader.h"
#include "box-algorithms/openvibe/ovpCBoxAlgorithmGenericStreamWriter.h"

#include "box-algorithms/ovpCBoxAlgorithmElectrodeLocalizationFileReader.h"

#include "box-algorithms/bci2000reader/ovpCBoxAlgorithmBCI2000Reader.h"

#include "box-algorithms/ovpCBoxAlgorithmSignalConcatenation.h"

OVP_Declare_Begin()
	OVP_Declare_New(OpenViBEPlugins::FileIO::CGDFFileReaderDesc)
	OVP_Declare_New(OpenViBEPlugins::FileIO::CGDFFileWriterDesc)

	OVP_Declare_New(OpenViBEPlugins::FileIO::CBCICompetitionIIIbReaderDesc)

	OVP_Declare_New(OpenViBEPlugins::FileIO::CAlgorithmBrainampFileReaderDesc)

	OVP_Declare_New(OpenViBEPlugins::FileIO::CAlgorithmOVMatrixFileReaderDesc)
	OVP_Declare_New(OpenViBEPlugins::FileIO::CAlgorithmOVMatrixFileWriterDesc)

	OVP_Declare_New(OpenViBEPlugins::FileIO::CAlgorithmXMLScenarioExporterDesc)
	OVP_Declare_New(OpenViBEPlugins::FileIO::CAlgorithmXMLScenarioImporterDesc)

	OVP_Declare_New(OpenViBEPlugins::FileIO::CBoxAlgorithmBrainampFileReaderDesc)

	OVP_Declare_New(OpenViBEPlugins::FileIO::CBoxAlgorithmCSVFileWriterDesc)
	OVP_Declare_New(OpenViBEPlugins::FileIO::CBoxAlgorithmCSVFileReaderDesc)

	OVP_Declare_New(OpenViBEPlugins::FileIO::CBoxAlgorithmGenericStreamReaderDesc)
	OVP_Declare_New(OpenViBEPlugins::FileIO::CBoxAlgorithmGenericStreamWriterDesc)

	OVP_Declare_New(OpenViBEPlugins::FileIO::CBoxAlgorithmElectrodeLocalisationFileReaderDesc)
	
	OVP_Declare_New(OpenViBEPlugins::FileIO::CBoxAlgorithmBCI2000ReaderDesc)

	OVP_Declare_New(OpenViBEPlugins::FileIO::CBoxAlgorithmSignalConcatenationDesc)

OVP_Declare_End()
