#include "ovpCBoxAlgorithmSignalDecimation.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessing;

boolean CBoxAlgorithmSignalDecimation::initialize(void)
{
	m_i64DecimationFactor=FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 0);
	if(m_i64DecimationFactor<=1)
	{
		this->getLogManager() << LogLevel_Error << "Decimation factor should be stricly greater than 1 !\n";
		return false;
	}

	m_pStreamDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamDecoder));
	m_pStreamDecoder->initialize();

	ip_pMemoryBuffer.initialize(m_pStreamDecoder->getInputParameter(OVP_GD_Algorithm_SignalStreamDecoder_InputParameterId_MemoryBufferToDecode));
	op_pMatrix.initialize(m_pStreamDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_Matrix));
	op_ui64SamplingRate.initialize(m_pStreamDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_SamplingRate));

	m_pStreamEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamEncoder));
	m_pStreamEncoder->initialize();

	ip_ui64SamplingRate.initialize(m_pStreamEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_SamplingRate));
	ip_pMatrix.initialize(m_pStreamEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_Matrix));
	op_pMemoryBuffer.initialize(m_pStreamEncoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamEncoder_OutputParameterId_EncodedMemoryBuffer));

	return true;
}

boolean CBoxAlgorithmSignalDecimation::uninitialize(void)
{
	op_pMemoryBuffer.uninitialize();
	ip_pMatrix.uninitialize();
	ip_ui64SamplingRate.uninitialize();

	m_pStreamEncoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStreamEncoder);
	m_pStreamEncoder=NULL;

	op_ui64SamplingRate.uninitialize();
	op_pMatrix.uninitialize();
	ip_pMemoryBuffer.uninitialize();

	m_pStreamDecoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStreamDecoder);
	m_pStreamDecoder=NULL;

	return true;
}

boolean CBoxAlgorithmSignalDecimation::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CBoxAlgorithmSignalDecimation::process(void)
{
	// IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	for(uint32 i=0; i<l_rDynamicBoxContext.getInputChunkCount(0); i++)
	{
		ip_pMemoryBuffer=l_rDynamicBoxContext.getInputChunk(0, i);
		op_pMemoryBuffer=l_rDynamicBoxContext.getOutputChunk(0);

		m_pStreamDecoder->process();
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedHeader))
		{
			uint64 l_ui64SourceSamplingFrequency=op_ui64SamplingRate;
			uint64 l_ui64SourceSampleCountPerBuffer=op_pMatrix->getDimensionSize(1);
			if(l_ui64SourceSamplingFrequency%m_i64DecimationFactor != 0)
			{
				this->getLogManager() << LogLevel_Error << "Input signal sampling frequency " << l_ui64SourceSamplingFrequency << " is not a multiple of the decimation factor " << m_i64DecimationFactor << "\n";
				return false;
			}
			if(l_ui64SourceSampleCountPerBuffer%m_i64DecimationFactor != 0)
			{
				this->getLogManager() << LogLevel_ImportantWarning << "Input signal sample count per buffer " << l_ui64SourceSampleCountPerBuffer << " is not a multiple of the decimation factor " << m_i64DecimationFactor << "\n";
				// return false;
			}
			OpenViBEToolkit::Tools::Matrix::copyDescription(*ip_pMatrix, *op_pMatrix);
			ip_pMatrix->setDimensionSize(1, l_ui64SourceSampleCountPerBuffer/m_i64DecimationFactor);
			ip_ui64SamplingRate=l_ui64SourceSamplingFrequency/m_i64DecimationFactor;
			m_pStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeHeader);
		}
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedBuffer))
		{
			float64* l_pInputBuffer=op_pMatrix->getBuffer();
			float64* l_pOutputBuffer=ip_pMatrix->getBuffer();

			float64* l_pInputBufferEnd=l_pInputBuffer+op_pMatrix->getBufferElementCount();
			float64* l_pOutputBufferEnd=l_pOutputBuffer+ip_pMatrix->getBufferElementCount();

			while(l_pInputBuffer < l_pInputBufferEnd && l_pOutputBuffer < l_pOutputBufferEnd)
			{
				*l_pOutputBuffer=*l_pInputBuffer;
				l_pInputBuffer+=m_i64DecimationFactor;
				l_pOutputBuffer++;
			}

			m_pStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeBuffer);
		}
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedEnd))
		{
			m_pStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeEnd);
		}

		l_rDynamicBoxContext.markInputAsDeprecated(0, i);
		l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_rDynamicBoxContext.getInputChunkStartTime(0, i), l_rDynamicBoxContext.getInputChunkEndTime(0, i));
	}
	return true;
}
