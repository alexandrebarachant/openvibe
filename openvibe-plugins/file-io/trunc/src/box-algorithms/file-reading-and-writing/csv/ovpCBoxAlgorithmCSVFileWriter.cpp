#include "ovpCBoxAlgorithmCSVFileWriter.h"

#include <string>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::FileIO;

CBoxAlgorithmCSVFileWriter::CBoxAlgorithmCSVFileWriter(void)
	:m_pFile(NULL)
	,m_fpRealProcess(NULL)
	,m_pStreamDecoder(NULL)
{
}

boolean CBoxAlgorithmCSVFileWriter::initialize(void)
{
	this->getStaticBoxContext().getInputType(0, m_oTypeIdentifier);

	CString l_sFilename=FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 0);
	m_sSeparator=FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 1);
	m_bUseCompression=FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 2);

	m_pFile=::fopen(l_sFilename.toASCIIString(), "wb");
	if(!m_pFile)
	{
		this->getLogManager() << LogLevel_ImportantWarning << "Could not open file [" << l_sFilename << "]\n";
		return false;
	}

	if(this->getTypeManager().isDerivedFromStream(m_oTypeIdentifier, OV_TypeId_StreamedMatrix))
	{
		m_pStreamDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StreamedMatrixStreamDecoder));
		m_pStreamDecoder->initialize();
		ip_pMemoryBuffer.initialize(m_pStreamDecoder->getInputParameter(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_InputParameterId_MemoryBufferToDecode));
		op_pMatrix.initialize(m_pStreamDecoder->getOutputParameter(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputParameterId_Matrix));
		m_fpRealProcess=&CBoxAlgorithmCSVFileWriter::process_streamedMatrix;
	}
	else if(m_oTypeIdentifier==OV_TypeId_Stimulations)
	{
		m_pStreamDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
		m_pStreamDecoder->initialize();
		ip_pMemoryBuffer.initialize(m_pStreamDecoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
		op_pStimulationSet.initialize(m_pStreamDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
		m_fpRealProcess=&CBoxAlgorithmCSVFileWriter::process_stimulation;
	}
	else
	{
		this->getLogManager() << LogLevel_ImportantWarning << "Invalid input type identifier " << this->getTypeManager().getTypeName(m_oTypeIdentifier) << "\n";
		return false;
	}

	if(m_bUseCompression)
	{
		this->getLogManager() << LogLevel_Warning << "Compression flag not used yet, the file will be flagged uncompressed and stored as is\n";
	}

	return true;
}

boolean CBoxAlgorithmCSVFileWriter::uninitialize(void)
{
	if(m_pFile)
	{
		::fclose(m_pFile);
		m_pFile=NULL;
	}

	op_pStimulationSet.uninitialize();
	op_pMatrix.uninitialize();
	ip_pMemoryBuffer.uninitialize();

	if(m_pStreamDecoder)
	{
		m_pStreamDecoder->uninitialize();
		this->getAlgorithmManager().releaseAlgorithm(*m_pStreamDecoder);
		m_pStreamDecoder=NULL;
	}

	return true;
}

boolean CBoxAlgorithmCSVFileWriter::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CBoxAlgorithmCSVFileWriter::process(void)
{
	return (this->*m_fpRealProcess)();
}

boolean CBoxAlgorithmCSVFileWriter::process_streamedMatrix(void)
{
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	for(uint32 i=0; i<l_rDynamicBoxContext.getInputChunkCount(0); i++)
	{
		uint64 l_ui64StartTime=l_rDynamicBoxContext.getInputChunkStartTime(0, i);
		uint64 l_ui64EndTime=l_rDynamicBoxContext.getInputChunkEndTime(0, i);
		ip_pMemoryBuffer=l_rDynamicBoxContext.getInputChunk(0, i);
		m_pStreamDecoder->process();
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputTriggerId_ReceivedHeader))
		{
			if(op_pMatrix->getDimensionCount() != 2)
			{
				this->getLogManager() << LogLevel_ImportantWarning << "Input matrix does not have 2 dimensions - Could not write content in CSV file...\n";
				return false;
			}

			::fprintf(m_pFile, "Time (s)");
			for(uint32 c=0; c<op_pMatrix->getDimensionSize(0); c++)
			{
				std::string l_sLabel(op_pMatrix->getDimensionLabel(0, c));
				while(l_sLabel.length()>0 && l_sLabel[l_sLabel.length()-1]==' ')
				{
					l_sLabel.erase(l_sLabel.length()-1);
				}
				::fprintf(m_pFile,
					"%s%s",
					m_sSeparator.toASCIIString(),
					l_sLabel.c_str());
			}
			::fprintf(m_pFile, "\n");
		}
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputTriggerId_ReceivedBuffer))
		{
			for(uint32 s=0; s<op_pMatrix->getDimensionSize(1); s++)
			{
				if(m_oTypeIdentifier==OV_TypeId_Signal)   ::fprintf(m_pFile, "%f", ((l_ui64StartTime+((s*(l_ui64EndTime-l_ui64StartTime))/op_pMatrix->getDimensionSize(1)))>>16)/65536.);
				if(m_oTypeIdentifier==OV_TypeId_Spectrum) ::fprintf(m_pFile, "%f", (l_ui64EndTime>>16)/65536.);
				for(uint32 c=0; c<op_pMatrix->getDimensionSize(0); c++)
				{
					::fprintf(m_pFile,
						"%s%f",
						m_sSeparator.toASCIIString(),
						op_pMatrix->getBuffer()[c*op_pMatrix->getDimensionSize(1)+s]);
				}
				::fprintf(m_pFile, "\n");
			}
		}
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputTriggerId_ReceivedEnd))
		{
		}
		l_rDynamicBoxContext.markInputAsDeprecated(0, i);
	}

	return true;
}

boolean CBoxAlgorithmCSVFileWriter::process_stimulation(void)
{
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	for(uint32 i=0; i<l_rDynamicBoxContext.getInputChunkCount(0); i++)
	{
		ip_pMemoryBuffer=l_rDynamicBoxContext.getInputChunk(0, i);
		m_pStreamDecoder->process();
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedHeader))
		{
			::fprintf(m_pFile,
				"Time (s)%sIdentifier%sDuration\n",
				m_sSeparator.toASCIIString(),
				m_sSeparator.toASCIIString());
		}
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
		{
			for(uint32 j=0; j<op_pStimulationSet->getStimulationCount(); j++)
			{
				::fprintf(m_pFile,
					"%f%s%llu%s%f\n",
					(op_pStimulationSet->getStimulationDate(j)>>16)/65536.0,
					m_sSeparator.toASCIIString(),
					op_pStimulationSet->getStimulationIdentifier(j),
					m_sSeparator.toASCIIString(),
					(op_pStimulationSet->getStimulationDuration(j)>>16)/65536.0);
			}
		}
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedEnd))
		{
		}
		l_rDynamicBoxContext.markInputAsDeprecated(0, i);
	}

	return true;
}
