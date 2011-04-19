#include "ovpCBoxAlgorithmStreamConcatenation.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessing;

boolean CBoxAlgorithmStreamConcatenation::initialize(void)
{
	
	//> init signal 1 input
	m_pSignalDecoder1=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamDecoder));
    m_pSignalDecoder1->initialize();
    ip_pMemoryBufferToDecode1.initialize(m_pSignalDecoder1->getInputParameter(OVP_GD_Algorithm_SignalStreamDecoder_InputParameterId_MemoryBufferToDecode));
    op_pDecodedMatrix1.initialize(m_pSignalDecoder1->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_Matrix));
	op_ui64SamplingRate1.initialize(m_pSignalDecoder1->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_SamplingRate));
    
	//> init signal 2 input
	m_pSignalDecoder2=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamDecoder));
    m_pSignalDecoder2->initialize();
    ip_pMemoryBufferToDecode2.initialize(m_pSignalDecoder2->getInputParameter(OVP_GD_Algorithm_SignalStreamDecoder_InputParameterId_MemoryBufferToDecode));
    op_pDecodedMatrix2.initialize(m_pSignalDecoder2->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_Matrix));
	op_ui64SamplingRate2.initialize(m_pSignalDecoder2->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_SamplingRate));

	//> init multiplexed signal output
	m_pSignalEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamEncoder));
    m_pSignalEncoder->initialize();
    ip_pMatrixToEncode.initialize(m_pSignalEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_Matrix));
    op_pEncodedMemoryBuffer.initialize(m_pSignalEncoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
    ip_ui64SamplingRate.initialize(m_pSignalEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_SamplingRate));

	return true;
}

boolean CBoxAlgorithmStreamConcatenation::uninitialize(void)
{
	// uninit signal 1 input
	op_pDecodedMatrix1.uninitialize();
    m_pSignalDecoder1->uninitialize();
	ip_pMemoryBufferToDecode1.uninitialize();
	op_ui64SamplingRate1.uninitialize();
    this->getAlgorithmManager().releaseAlgorithm(*m_pSignalDecoder1);
	
	// uninit signal 2 input
	op_pDecodedMatrix2.uninitialize();
    m_pSignalDecoder2->uninitialize();
	ip_pMemoryBufferToDecode2.uninitialize();
	op_ui64SamplingRate2.uninitialize();
    this->getAlgorithmManager().releaseAlgorithm(*m_pSignalDecoder2);

	// uninit multiplexed signal output
	ip_pMatrixToEncode.uninitialize();
    m_pSignalEncoder->uninitialize();
	op_pEncodedMemoryBuffer.uninitialize();
    ip_ui64SamplingRate.uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pSignalEncoder);

	return true;
}

boolean CBoxAlgorithmStreamConcatenation::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}


boolean CBoxAlgorithmStreamConcatenation::process(void)
{
	//> Get dynamic box context
	IBoxIO* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();

	//uint32 nombreMorceauM1=l_pDynamicBoxContext->getInputChunkCount(0);
	//uint32 nombreMorceauM2=l_pDynamicBoxContext->getInputChunkCount(1);
	//uint32 minMorceauM1M2=nombreMorceauM1<nombreMorceauM2 ? nombreMorceauM1 : nombreMorceauM2;
	
	//> Process the same number of Signal 1 chunks(nbChunkSignal1) and Signal 2 chunks (nbChunkSignal2)
	uint32 nbChunkSignal1=l_pDynamicBoxContext->getInputChunkCount(0);
	uint32 nbChunkSignal2=l_pDynamicBoxContext->getInputChunkCount(1);
	uint32 minChunkBothInput=nbChunkSignal1<nbChunkSignal2 ? nbChunkSignal1 : nbChunkSignal2;

	//> Init local variables
	uint64 l_ui64StartTime1=0,l_ui64EndTime1=0,l_ui64ChunkSize1=0;
	uint64 l_ui64StartTime2=0,l_ui64EndTime2=0,l_ui64ChunkSize2=0;
	const uint8* l_pChunkBuffer1=NULL;
	const uint8* l_pChunkBuffer2=NULL;

	//> For each ChunkSignal
	for(uint32 j=0; j<minChunkBothInput; j++)
		{
			//> Get Signal 1 chunks and uncode them.
			ip_pMemoryBufferToDecode1=l_pDynamicBoxContext->getInputChunk(0, j);
			m_pSignalDecoder1->process();
			//> Get Signal 2 chunks and uncode them.
			ip_pMemoryBufferToDecode2=l_pDynamicBoxContext->getInputChunk(1, j);
			m_pSignalDecoder2->process();

			//> If both chunks are headers...
			if(m_pSignalDecoder1->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedHeader)
				&&
				m_pSignalDecoder2->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedHeader))
				{

						// Test of entry flux sampling rate coherency
						if(op_ui64SamplingRate1!=op_ui64SamplingRate2) 
						{
						getLogManager() << LogLevel_Warning << "Sampling rate 1 = "<<op_ui64SamplingRate1<< " Hz.\n";
						getLogManager() << LogLevel_Warning << "Sampling rate 2 = "<<op_ui64SamplingRate2<< " Hz.\n";
						getLogManager() << LogLevel_ImportantWarning << "The two streams must have the same sampling rate.\n";
						return false;
						}
					
							OpenViBE::uint32 l_nbChannels1=op_pDecodedMatrix1->getDimensionSize(0);
							OpenViBE::uint32 l_nbChannels2=op_pDecodedMatrix2->getDimensionSize(0);
							OpenViBE::uint32 l_nbChannels3=l_nbChannels1+l_nbChannels2;
							OpenViBE::uint32 l_nbSamples=op_pDecodedMatrix1->getDimensionSize(1);
							OpenViBE::uint32 l_nbSamples2=op_pDecodedMatrix2->getDimensionSize(1);

						// Test chunk size coherency
						if(l_nbSamples!=l_nbSamples2) 
						{
						getLogManager() << LogLevel_Warning << "Epoch 1 size = "<<l_nbSamples<<" samples.\n";
						getLogManager() << LogLevel_Warning << "Epoch 2 size = "<<l_nbSamples2<<" samples.\n";
						getLogManager() << LogLevel_ImportantWarning << "The two streams must have the same epoch size.\n";
						return false;
						}

							ip_pMatrixToEncode->setDimensionCount(2);
							ip_pMatrixToEncode->setDimensionSize(0,l_nbChannels3);
							ip_pMatrixToEncode->setDimensionSize(1,l_nbSamples);

					const char* l_ChannelLabel;
					for (uint32 a=0; a<l_nbChannels1; a++)
					{
						l_ChannelLabel=op_pDecodedMatrix1->getDimensionLabel(0,a);
						ip_pMatrixToEncode->setDimensionLabel(0,a,l_ChannelLabel);
					}
					for (uint32 a=0; a<l_nbChannels2; a++)
					{
						l_ChannelLabel=op_pDecodedMatrix2->getDimensionLabel(0,a);
						ip_pMatrixToEncode->setDimensionLabel(0,l_nbChannels1+a,l_ChannelLabel);
					}

					//printf("Nb_channels 1= %d\n",l_nbChannels1);
					//printf("Nb_channels 2= %d\n",l_nbChannels2);
					
					op_pEncodedMemoryBuffer=l_pDynamicBoxContext->getOutputChunk(0);
					m_pSignalEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeHeader);
					l_ui64StartTime1=l_pDynamicBoxContext->getInputChunkStartTime(0, j);
					l_ui64EndTime1=l_pDynamicBoxContext->getInputChunkEndTime(0, j);
					l_pDynamicBoxContext->markOutputAsReadyToSend(0,l_ui64StartTime1 ,l_ui64EndTime1 );
				}

			//> If both chunks are data, process...
			if(m_pSignalDecoder1->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedBuffer)
				&&
				m_pSignalDecoder2->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedBuffer))
				{
					OpenViBE::uint32 l_nbChannels1=op_pDecodedMatrix1->getDimensionSize(0);
					OpenViBE::uint32 l_nbChannels2=op_pDecodedMatrix2->getDimensionSize(0);
					OpenViBE::uint32 l_nbChannels3=l_nbChannels1+l_nbChannels2;
					OpenViBE::uint32 l_nbSamples=op_pDecodedMatrix1->getDimensionSize(1);

					ip_pMatrixToEncode->setDimensionCount(2);
					ip_pMatrixToEncode->setDimensionSize(0,l_nbChannels3);
					ip_pMatrixToEncode->setDimensionSize(1,l_nbSamples);

				
					
					for (uint32 a=0; a<l_nbChannels1; a++)
					{
						for (uint32 b=0; b<l_nbSamples; b++)
						{
							ip_pMatrixToEncode->getBuffer()[a*l_nbSamples+b]=op_pDecodedMatrix1->getBuffer()[a*l_nbSamples+b];
						}
					}
					for (uint32 a=0; a<l_nbChannels2; a++)
					{
						for (uint32 b=0; b<l_nbSamples; b++)
						{
							ip_pMatrixToEncode->getBuffer()[a*l_nbSamples+b+l_nbSamples*l_nbChannels1]=op_pDecodedMatrix2->getBuffer()[a*l_nbSamples+b];
						}
					}
					
					
					op_pEncodedMemoryBuffer=l_pDynamicBoxContext->getOutputChunk(0);
					m_pSignalEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeBuffer);
					l_ui64StartTime1=l_pDynamicBoxContext->getInputChunkStartTime(0, j);
					l_ui64EndTime1=l_pDynamicBoxContext->getInputChunkEndTime(0, j);
					l_pDynamicBoxContext->markOutputAsReadyToSend(0,l_ui64StartTime1 ,l_ui64EndTime1 );

					
				}

			//> If both chunks are footers...
			if(m_pSignalDecoder1->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedEnd)
				&&
				m_pSignalDecoder2->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedEnd))
				{
				}

			//> Mark both inputs as deprecated.
			l_pDynamicBoxContext->markInputAsDeprecated(0,j);
			l_pDynamicBoxContext->markInputAsDeprecated(1,j);
			
		}

	return true;
}
