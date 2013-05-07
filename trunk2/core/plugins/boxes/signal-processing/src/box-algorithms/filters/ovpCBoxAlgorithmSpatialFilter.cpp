#include "ovpCBoxAlgorithmSpatialFilter.h"

#include <system/Memory.h>

#include <sstream>
#include <string>
#include <cstdio>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessing;

OpenViBE::uint32 CBoxAlgorithmSpatialFilter::loadCoefficients(const OpenViBE::CString &rCoefficients, const char c1, const char c2) 
{
	this->getLogManager() << LogLevel_Trace << "Parsing coefficients matrix\n";

	m_vCoefficient.clear();

	// Count the number of entries
	// @Note To avoid doing a ton of subsequent memory allocations (very slow on Windows debug builds), we first count the number of entries in the vector. If the file format had specified the vector dimension, we wouldn't have to do this step.
	this->getLogManager() << LogLevel_Trace << "Counting the number of coefficients\n";
	uint32 l_u32count = 0;
	const char *l_sPtr = rCoefficients.toASCIIString();
	while(*l_sPtr!=0) 
	{
		// Skip separator characters
		while(*l_sPtr==c1 || *l_sPtr==c2) 
		{ 
			l_sPtr++; 
		}
		if(*l_sPtr==0) 
		{
			break;
		}
		// Ok, we have reached something that is not NULL or separator, assume its a number
		l_u32count++;
		// Skip the normal characters
		while(*l_sPtr!=c1 && *l_sPtr!=c2 && *l_sPtr!=0) 
		{ 
			l_sPtr++; 
		}
	}

	// Resize in one step for efficiency.
	m_vCoefficient.resize(l_u32count);

	// Ok, convert to floats
	this->getLogManager() << LogLevel_Trace << "Converting the coefficients to a float vector\n";
	l_sPtr = rCoefficients.toASCIIString();
	uint32 l_u32currentIdx = 0;
	while(*l_sPtr!=0) 
	{
		const int BUFFSIZE=1024;
		char l_sBuffer[BUFFSIZE];
		// Skip separator characters
		while(*l_sPtr==c1 || *l_sPtr==c2) 
		{ 
			l_sPtr++; 
		}
		if(*l_sPtr==0) 
		{
			break;
		}
		// Copy the normal characters, don't exceed buffer size
		int i=0;
		while(*l_sPtr!=c1 && *l_sPtr!=c2 && *l_sPtr!=0) 
		{ 
			if(i<BUFFSIZE-1) {
				l_sBuffer[i++] = *l_sPtr; 
			}
			l_sPtr++;
		}
		l_sBuffer[i]=0;
		// Finally, convert
		if(!sscanf(l_sBuffer, "%lf", &m_vCoefficient[l_u32currentIdx])) 
		{
			this->getLogManager() << LogLevel_Error << "Error parsing coefficient nr. " << l_u32currentIdx << ", stopping.\n";
			break;
		}
		l_u32currentIdx++;
	}
	if(l_u32currentIdx != l_u32count) {
		this->getLogManager() << LogLevel_Warning << "Number of coefficients expected did not match the number read\n";
	}

	return l_u32currentIdx;
}

boolean CBoxAlgorithmSpatialFilter::initialize(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();

	m_pStreamDecoder=NULL;
	m_pStreamEncoder=NULL;

	boolean l_bValid=false;
	CIdentifier l_oIdentifier;
	l_rStaticBoxContext.getInputType(0, l_oIdentifier);

	if(l_oIdentifier==OV_TypeId_StreamedMatrix)
	{
		l_bValid=true;

		m_pStreamDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StreamedMatrixStreamDecoder));
		m_pStreamEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StreamedMatrixStreamEncoder));

		m_pStreamDecoder->initialize();
		m_pStreamEncoder->initialize();
	}

	if(l_oIdentifier==OV_TypeId_Signal)
	{
		l_bValid=true;

		m_pStreamDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamDecoder));
		m_pStreamEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamEncoder));

		m_pStreamDecoder->initialize();
		m_pStreamEncoder->initialize();

		TParameterHandler < uint64 > op_pSamplingFrequency(m_pStreamDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_SamplingRate));
		TParameterHandler < uint64 > ip_pSamplingFrequency(m_pStreamEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_SamplingRate));
		ip_pSamplingFrequency.setReferenceTarget(op_pSamplingFrequency);
	}

	if(l_oIdentifier==OV_TypeId_Spectrum)
	{
		l_bValid=true;

		m_pStreamDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SpectrumStreamDecoder));
		m_pStreamEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SpectrumStreamEncoder));

		m_pStreamDecoder->initialize();
		m_pStreamEncoder->initialize();

		TParameterHandler < IMatrix* > op_pBandMatrix(m_pStreamDecoder->getOutputParameter(OVP_GD_Algorithm_SpectrumStreamDecoder_OutputParameterId_MinMaxFrequencyBands));
		TParameterHandler < IMatrix* > ip_pBandMatrix(m_pStreamEncoder->getInputParameter(OVP_GD_Algorithm_SpectrumStreamEncoder_InputParameterId_MinMaxFrequencyBands));
		ip_pBandMatrix.setReferenceTarget(op_pBandMatrix);
	}

	if(!l_bValid)
	{
		this->getLogManager() << LogLevel_Error << "Unhandled input stream type " << l_oIdentifier << "\n";
		return false;
	}

	ip_pMemoryBuffer.initialize(m_pStreamDecoder->getInputParameter(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_InputParameterId_MemoryBufferToDecode));
	op_pMatrix.initialize(m_pStreamDecoder->getOutputParameter(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputParameterId_Matrix));

	ip_pMatrix.initialize(m_pStreamEncoder->getInputParameter(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_InputParameterId_Matrix));
	op_pMemoryBuffer.initialize(m_pStreamEncoder->getOutputParameter(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_OutputParameterId_EncodedMemoryBuffer));

	return true;
}

boolean CBoxAlgorithmSpatialFilter::uninitialize(void)
{
	if(m_pStreamDecoder)
	{
		m_pStreamDecoder->uninitialize();
		this->getAlgorithmManager().releaseAlgorithm(*m_pStreamDecoder);
		m_pStreamDecoder=NULL;
	}

	if(m_pStreamEncoder)
	{
		m_pStreamEncoder->uninitialize();
		this->getAlgorithmManager().releaseAlgorithm(*m_pStreamEncoder);
		m_pStreamEncoder=NULL;
	}

	return true;
}

boolean CBoxAlgorithmSpatialFilter::processInput(uint32 ui32InputIndex)
{
	this->getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CBoxAlgorithmSpatialFilter::process(void)
{
	// IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	uint32 i, j, k, l;

	for(i=0; i<l_rDynamicBoxContext.getInputChunkCount(0); i++)
	{
		ip_pMemoryBuffer=l_rDynamicBoxContext.getInputChunk(0, i);
		op_pMemoryBuffer=l_rDynamicBoxContext.getOutputChunk(0);

		m_pStreamDecoder->process();
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputTriggerId_ReceivedHeader))
		{
			CString l_sCoefficient=FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 0);
			
			loadCoefficients(l_sCoefficient, ' ', OV_Value_EnumeratedStringSeparator);

			uint32 l_ui32OutputChannelCountSetting=(uint32)(uint64)FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 1);
			uint32 l_ui32InputChannelCountSetting=(uint32)(uint64)FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 2);

			uint32 l_ui32OutputChannelCount=m_vCoefficient.size() / op_pMatrix->getDimensionSize(0);
			uint32 l_ui32InputChannelCount=op_pMatrix->getDimensionSize(0);

			if(l_ui32InputChannelCount!=l_ui32InputChannelCountSetting)
			{
				this->getLogManager() << LogLevel_Error  << "Bad matrix size - Expected " << l_ui32InputChannelCountSetting << " input channels and received " << l_ui32InputChannelCount << " input channels\n";
				return false;
			}

			if(m_vCoefficient.size() != l_ui32OutputChannelCountSetting*l_ui32InputChannelCountSetting)
			{
				this->getLogManager() << LogLevel_Error << "Bad matrix size - The number of coefficients of the filter (" << uint32(m_vCoefficient.size()) << ") does not match the input/output channel count settings (" << l_ui32InputChannelCountSetting << "x" << l_ui32OutputChannelCountSetting << ")\n";
				return false;
			}

			ip_pMatrix->setDimensionCount(2);
			ip_pMatrix->setDimensionSize(0, l_ui32OutputChannelCount);
			ip_pMatrix->setDimensionSize(1, op_pMatrix->getDimensionSize(1));

			m_pStreamEncoder->process(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_InputTriggerId_EncodeHeader);
		}
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputTriggerId_ReceivedBuffer))
		{
			float64* l_pMatrix=op_pMatrix->getBuffer();
			float64* l_pFilteredMatrix=ip_pMatrix->getBuffer();
			uint32 l_ui32InputChannelCount=op_pMatrix->getDimensionSize(0);
			uint32 l_ui32OutputChannelCount=ip_pMatrix->getDimensionSize(0);
			uint32 l_ui32SampleCount=ip_pMatrix->getDimensionSize(1);

			System::Memory::set(l_pFilteredMatrix, l_ui32SampleCount*l_ui32OutputChannelCount*sizeof(float64), 0);

			for(j=0; j<l_ui32OutputChannelCount; j++)
			{
				for(k=0; k<l_ui32InputChannelCount; k++)
				{
					for(l=0; l<l_ui32SampleCount; l++)
					{
						l_pFilteredMatrix[j*l_ui32SampleCount+l]+=m_vCoefficient[j*l_ui32InputChannelCount+k]*l_pMatrix[k*l_ui32SampleCount+l];
					}
				}
			}

			m_pStreamEncoder->process(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_InputTriggerId_EncodeBuffer);
		}
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputTriggerId_ReceivedEnd))
		{
			m_pStreamEncoder->process(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_InputTriggerId_EncodeEnd);
		}

		l_rDynamicBoxContext.markInputAsDeprecated(0, i);
		l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_rDynamicBoxContext.getInputChunkStartTime(0, i), l_rDynamicBoxContext.getInputChunkEndTime(0, i));
	}

	return true;
}
