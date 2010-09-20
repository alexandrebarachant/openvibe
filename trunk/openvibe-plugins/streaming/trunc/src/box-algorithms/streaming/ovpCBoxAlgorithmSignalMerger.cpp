#include "ovpCBoxAlgorithmSignalMerger.h"

#include <iostream>
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Streaming;

enum
{
	DecoderType_Unknown=0,
	DecoderType_Header=1,
	DecoderType_Buffer=2,
	DecoderType_End=3,
	DecoderType_Error=10,
};

boolean CBoxAlgorithmSignalMerger::initialize(void)
{
	m_pSignalDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamDecoder));
	m_pSignalDecoder->initialize();
	ip_pMemoryBufferToDecode1.initialize(m_pSignalDecoder->getInputParameter(OVP_GD_Algorithm_SignalStreamDecoder_InputParameterId_MemoryBufferToDecode));
	op_pDecodedMatrix1.initialize(m_pSignalDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_Matrix));
	op_ui64SamplingRate1.initialize(m_pSignalDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_SamplingRate));

	m_pSignalEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamEncoder));
	m_pSignalEncoder->initialize();
	ip_pMatrixToEncode.initialize(m_pSignalEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_Matrix));
	op_pEncodedMemoryBuffer.initialize(m_pSignalEncoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
	ip_ui64SamplingRate.initialize(m_pSignalEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_SamplingRate));

	ip_ui64SamplingRate.setReferenceTarget(op_ui64SamplingRate1);
	return true;
}

boolean CBoxAlgorithmSignalMerger::uninitialize(void)
{
	op_pDecodedMatrix1.uninitialize();
	ip_pMemoryBufferToDecode1.uninitialize();
	op_ui64SamplingRate1.uninitialize();
	m_pSignalDecoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pSignalDecoder);

	ip_pMatrixToEncode.uninitialize();
	op_pEncodedMemoryBuffer.uninitialize();
	ip_ui64SamplingRate.uninitialize();
	m_pSignalEncoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pSignalEncoder);

	return true;
}

boolean CBoxAlgorithmSignalMerger::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CBoxAlgorithmSignalMerger::process(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();

	uint32 i, j, k;
	uint32 l_ui32MinimumChunkInputCount=l_pDynamicBoxContext->getInputChunkCount(0);
	for(i=1; i<l_rStaticBoxContext.getInputCount(); i++)
	{
		l_ui32MinimumChunkInputCount=std::min(l_pDynamicBoxContext->getInputChunkCount(i),l_ui32MinimumChunkInputCount);
	}

	for(j=0; j<l_ui32MinimumChunkInputCount; j++)
	{
		uint32 l_ui32DecoderType=DecoderType_Unknown;
		uint64 l_ui64SamplingRate=uint64(-1);
		uint32 l_ui32ChannelCount=0;
		uint32 l_ui32SampleCount=0;
		uint64 l_ui64IndexCount=0;
		uint64 l_ui64StartTime=l_pDynamicBoxContext->getInputChunkStartTime(0, j);
		uint64 l_ui64EndTime=l_pDynamicBoxContext->getInputChunkEndTime(0, j);

		op_pEncodedMemoryBuffer=l_pDynamicBoxContext->getOutputChunk(0);

		for(i=0; i<l_rStaticBoxContext.getInputCount(); i++)
		{
			// décodage
			ip_pMemoryBufferToDecode1=l_pDynamicBoxContext->getInputChunk(i, j);
			m_pSignalDecoder->process();

			// uniformisation
			if(l_ui32DecoderType!=DecoderType_Error)
			{
				if(m_pSignalDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedHeader))
				{
					l_ui32DecoderType=(l_ui32DecoderType==DecoderType_Unknown ? DecoderType_Header : (l_ui32DecoderType!=DecoderType_Header ? DecoderType_Error : l_ui32DecoderType));
				}
				if(m_pSignalDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedBuffer))
				{
					l_ui32DecoderType=(l_ui32DecoderType==DecoderType_Unknown ? DecoderType_Buffer : (l_ui32DecoderType!=DecoderType_Buffer ? DecoderType_Error : l_ui32DecoderType));
				}
				if(m_pSignalDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedEnd))
				{
					l_ui32DecoderType=(l_ui32DecoderType==DecoderType_Unknown ? DecoderType_End : (l_ui32DecoderType!=DecoderType_End ? DecoderType_Error : l_ui32DecoderType));
				}
			}

			// action
			if(l_ui32DecoderType==DecoderType_Header)
			{
				// Test of entry flux sampling rate coherency
				if(l_ui64SamplingRate==uint64(-1))
				{
					l_ui64SamplingRate=op_ui64SamplingRate1;
				}
				if(l_ui64SamplingRate!=op_ui64SamplingRate1)
				{
					this->getLogManager() << LogLevel_Error << "Sampling rate mismatch : got " << l_ui64SamplingRate << "Hz - wanted " << op_ui64SamplingRate1 << "Hz.\n";
					return false;
				}

				// Test chunk size coherency
				if(l_ui32SampleCount==0)
				{
					l_ui32SampleCount=op_pDecodedMatrix1->getDimensionSize(1);
				}
				if(l_ui32SampleCount!=op_pDecodedMatrix1->getDimensionSize(1))
				{
					this->getLogManager() << LogLevel_Error << "Epoch size mismatch : got " << l_ui32SampleCount << " samples - wanted " << op_pDecodedMatrix1->getDimensionSize(1) << " samples.\n";
					return false;
				}

				// Memory reservation
				m_vChannelCount.push_back(op_pDecodedMatrix1->getDimensionSize(0));
				l_ui32ChannelCount+=op_pDecodedMatrix1->getDimensionSize(0);

				// Channel names
				for(k=0; k<op_pDecodedMatrix1->getDimensionSize(0); k++)
				{
					m_vChannelName.push_back(op_pDecodedMatrix1->getDimensionLabel(0, k));
				}

				// send data
				if(i==l_rStaticBoxContext.getInputCount()-1)
				{
					ip_pMatrixToEncode->setDimensionCount(2);
					ip_pMatrixToEncode->setDimensionSize(0, l_ui32ChannelCount);
					ip_pMatrixToEncode->setDimensionSize(1, l_ui32SampleCount);

					for(k=0; k<l_ui32ChannelCount; k++)
					{
						ip_pMatrixToEncode->setDimensionLabel(0, k, m_vChannelName[k].c_str());
					}

					m_pSignalEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeHeader);
					l_pDynamicBoxContext->markOutputAsReadyToSend(0, l_ui64StartTime, l_ui64EndTime);
				}
			}

			if(l_ui32DecoderType==DecoderType_Buffer)
			{
				if( (l_ui64StartTime !=0 && l_ui64StartTime!=l_pDynamicBoxContext->getInputChunkStartTime(i, j))
				    || (l_ui64EndTime!=0 && l_ui64EndTime!=l_pDynamicBoxContext->getInputChunkEndTime(i, j)) )
				{
					this->getLogManager() << LogLevel_Warning << "Chunk time mismatch : ["
						<< l_pDynamicBoxContext->getInputChunkStartTime(i, j) << " | " << l_pDynamicBoxContext->getInputChunkEndTime(i, j)
						<< "] - wanted [" << l_ui64StartTime << " | " << l_ui64EndTime << "]\n";
					return false;
				}

				for(k=0; k<m_vChannelCount.at(i)*op_pDecodedMatrix1->getDimensionSize(1); k++)
				{
					ip_pMatrixToEncode->getBuffer()[l_ui64IndexCount+k]=op_pDecodedMatrix1->getBuffer()[k];
				}
				l_ui64IndexCount+=m_vChannelCount.at(i)*op_pDecodedMatrix1->getDimensionSize(1);
				if(l_ui64IndexCount>ip_pMatrixToEncode->getDimensionSize(0)*ip_pMatrixToEncode->getDimensionSize(1))
				{
					// This should never happen
					this->getLogManager() << LogLevel_Warning << "Buffer overflow : " << l_ui64IndexCount << " / " << ip_pMatrixToEncode->getDimensionSize(0)*ip_pMatrixToEncode->getDimensionSize(1) << "\n";
					return false;
				}

				if(i==l_rStaticBoxContext.getInputCount()-1)
				{
					m_pSignalEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeBuffer);
					l_pDynamicBoxContext->markOutputAsReadyToSend(0, l_pDynamicBoxContext->getInputChunkStartTime(i, j), l_pDynamicBoxContext->getInputChunkEndTime(i, j));
				}
			}

			if(l_ui32DecoderType==DecoderType_End)
			{
				if(i==l_rStaticBoxContext.getInputCount()-1)
				{
					m_pSignalEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeEnd);
					l_pDynamicBoxContext->markOutputAsReadyToSend(0, l_pDynamicBoxContext->getInputChunkStartTime(i, j), l_pDynamicBoxContext->getInputChunkEndTime(i, j));
				}
			}

			l_pDynamicBoxContext->markInputAsDeprecated(i, j);
		}
	}

	return true;
}
