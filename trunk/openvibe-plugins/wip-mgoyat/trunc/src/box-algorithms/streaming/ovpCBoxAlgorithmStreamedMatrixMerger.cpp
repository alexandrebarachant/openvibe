#include "ovpCBoxAlgorithmStreamedMatrixMerger.h"

#include <iostream>
#include <sstream>

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

boolean CBoxAlgorithmStreamedMatrixMerger::initialize(void)
{
	m_pSignalDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StreamedMatrixStreamDecoder));
	m_pSignalDecoder->initialize();
	ip_pMemoryBufferToDecode1.initialize(m_pSignalDecoder->getInputParameter(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_InputParameterId_MemoryBufferToDecode));
	op_pDecodedMatrix1.initialize(m_pSignalDecoder->getOutputParameter(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputParameterId_Matrix));

	m_pSignalEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StreamedMatrixStreamEncoder));
	m_pSignalEncoder->initialize();
	ip_pMatrixToEncode.initialize(m_pSignalEncoder->getInputParameter(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_InputParameterId_Matrix));
	op_pEncodedMemoryBuffer.initialize(m_pSignalEncoder->getOutputParameter(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_OutputParameterId_EncodedMemoryBuffer));

	IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	CString l_sSettingValue;
	l_rStaticBoxContext.getSettingValue(0, l_sSettingValue);
	m_ui32MergingDimension=(uint32)this->getConfigurationManager().expandAsUInteger(l_sSettingValue);
	
	uint64 l_ui64Id=l_rStaticBoxContext.getIdentifier().toUInteger();
	std::stringstream sstr;
	sstr<<"LogGipsa/Mx_MergerLOG_"<<l_ui64Id<<".txt";
	pFile = fopen (sstr.str().c_str() , "w");
	if (pFile == NULL) {perror ("Error opening file"); return false;}
	
	return true;
}

boolean CBoxAlgorithmStreamedMatrixMerger::uninitialize(void)
{
	op_pDecodedMatrix1.uninitialize();
	ip_pMemoryBufferToDecode1.uninitialize();
	m_pSignalDecoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pSignalDecoder);

	ip_pMatrixToEncode.uninitialize();
	op_pEncodedMemoryBuffer.uninitialize();
	m_pSignalEncoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pSignalEncoder);

	if (pFile != NULL) {fclose (pFile);}
	
	return true;
}

boolean CBoxAlgorithmStreamedMatrixMerger::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CBoxAlgorithmStreamedMatrixMerger::process(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();

	uint32 i, j, k;
	uint32 l_ui32MinimumChunkInputCount=l_pDynamicBoxContext->getInputChunkCount(0);
	for(i=1; i<l_rStaticBoxContext.getInputCount(); i++)
	{
		l_ui32MinimumChunkInputCount=std::min(l_pDynamicBoxContext->getInputChunkCount(i),l_ui32MinimumChunkInputCount);
	}

	//for(i=0; i<l_rStaticBoxContext.getInputCount(); i++)
	  //{
		//std::cout<<"input "<<l_pDynamicBoxContext->getInputChunkCount(i)<<std::endl;
	  //}
	//std::cout<<"minInputChunkCount="<<l_ui32MinimumChunkInputCount<<std::endl;

	//LOG
	std::stringstream sstr;
	sstr<<"Input : ";
	for(k=0; k<l_rStaticBoxContext.getInputCount(); k++)
	  {
		sstr<<l_pDynamicBoxContext->getInputChunkCount(k)<<" ";
	  }
	sstr<<"\n";
	if(pFile) {fwrite (sstr.str().c_str() , 1 , sstr.str().size() , pFile );}
	//
	
	for(j=0; j<l_ui32MinimumChunkInputCount; j++)
	{
		uint32 l_ui32DecoderType=DecoderType_Unknown;
		uint64 l_ui64SamplingRate=uint64(-1);
		std::vector<uint32> l_vui32dimensionCount;
		//uint32 l_ui32ChannelCount=0;
		//uint32 l_ui32SampleCount=0;
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
			//std::cout<<"DecoderType == "<<( l_ui32DecoderType==DecoderType_Unknown ? "Unknown":
				//							l_ui32DecoderType==DecoderType_Header ? "Header":
				//							l_ui32DecoderType==DecoderType_Buffer ? "Buffer":
				//							l_ui32DecoderType==DecoderType_End ? "End":"Error"
				//							)<<std::endl;

			// action
			if(l_ui32DecoderType==DecoderType_Header)
			{
				// Test chunk size coherency
				if(l_vui32dimensionCount.empty())
				{
					for(uint32 h=0; h<op_pDecodedMatrix1->getDimensionCount();h++)
					  {l_vui32dimensionCount.push_back(op_pDecodedMatrix1->getDimensionSize(h));}
					//
					if(m_ui32MergingDimension>=l_vui32dimensionCount.size())
					  {
						this->getLogManager() << LogLevel_Error << "Merging dimensions out of range : got " << m_ui32MergingDimension << "th dimension - wanted " << (uint32)l_vui32dimensionCount.size() << " dimensions maximum.\n";
						return false;
					  }
				}
				else if(l_vui32dimensionCount.size()!=op_pDecodedMatrix1->getDimensionCount())
				{
					this->getLogManager() << LogLevel_Error << "Dimensions mismatch : wanted " << (uint32)l_vui32dimensionCount.size() << " dimensions - got " << op_pDecodedMatrix1->getDimensionCount() << " dimensions.\n";
					return false;
				}
				else
				{
					for(uint32 h=0; h<l_vui32dimensionCount.size(); h++)
					  {
						if(h==m_ui32MergingDimension) {continue;}
					    if(l_vui32dimensionCount.at(h)!=op_pDecodedMatrix1->getDimensionSize(h))
						  {
						  	this->getLogManager() << LogLevel_Error << "Dimension size "<<h<<" mismatch : wanted " << (uint32)l_vui32dimensionCount.at(h) << " - got " << op_pDecodedMatrix1->getDimensionSize(h) << ".\n";
							return false;
						  }
					  }
				}
				
				/*std::cout<<"setting : "<<l_vui32dimensionCount.size()<<"dimensions | "<<op_pDecodedMatrix1->getDimensionCount()<<std::endl;
				std::cout<<"\t dimension = ";
				for(uint32 g=0; g<l_vui32dimensionCount.size(); g++)
				  {std::cout<<l_vui32dimensionCount.at(g)<<"|"<<op_pDecodedMatrix1->getDimensionSize(g)<<" ; "<<std::endl;}
				std::cout<<std::endl;
				std::cout<<"\t mergingdimension = "<<m_ui32MergingDimension<<std::endl;*/

				// Memory reservation
				m_vMergingDimensionCount.push_back(op_pDecodedMatrix1->getDimensionSize(m_ui32MergingDimension));

				// Channel names
				for(k=0; k<op_pDecodedMatrix1->getDimensionSize(0); k++)
				{
					m_vChannelName.push_back(op_pDecodedMatrix1->getDimensionLabel(0, k));
				}

				// send data
				//std::cout<<"i="<<i<<" | "<<l_rStaticBoxContext.getInputCount()-1<<std::endl;
				if(i==l_rStaticBoxContext.getInputCount()-1)
				{
					//std::cout<<"send data menu"<<std::endl;
					ip_pMatrixToEncode->setDimensionCount(l_vui32dimensionCount.size());
					for(uint32 h=0; h<l_vui32dimensionCount.size(); h++)
					  {
					  	if(h==m_ui32MergingDimension) 
						  {
							uint32 l_ui32ChannelCount=0;
							for(uint32 p=0; p<m_vMergingDimensionCount.size(); p++) {l_ui32ChannelCount+=m_vMergingDimensionCount.at(p);}
							ip_pMatrixToEncode->setDimensionSize(m_ui32MergingDimension, l_ui32ChannelCount);
						  }
						else
						  {ip_pMatrixToEncode->setDimensionSize(h, l_vui32dimensionCount.at(h));}
					  }
					  
					for(k=0; k<op_pDecodedMatrix1->getDimensionSize(0); k++)
					{
						ip_pMatrixToEncode->setDimensionLabel(0, k, m_vChannelName[k].c_str());
					}

					//std::cout<<"send Header"<<std::endl;
					m_pSignalEncoder->process(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_InputTriggerId_EncodeHeader);
					l_pDynamicBoxContext->markOutputAsReadyToSend(0, l_ui64StartTime, l_ui64EndTime);
				}
			}

			if(l_ui32DecoderType==DecoderType_Buffer)
			{
				/*if( (l_ui64StartTime !=0 && l_ui64StartTime!=l_pDynamicBoxContext->getInputChunkStartTime(i, j))
				    || (l_ui64EndTime!=0 && l_ui64EndTime!=l_pDynamicBoxContext->getInputChunkEndTime(i, j)) )
				{
					this->getLogManager() << LogLevel_Warning << "Chunk time mismatch : ["
						<< l_pDynamicBoxContext->getInputChunkStartTime(i, j) << " | " << l_pDynamicBoxContext->getInputChunkEndTime(i, j)
						<< "] - wanted [" << l_ui64StartTime << " | " << l_ui64EndTime << "]\n";
					return false;
				}*/

				//LOG
				std::stringstream sstr;
				sstr<<"Buffer : "<<i<<"|"<<l_rStaticBoxContext.getInputCount()<<"\t";
				//
				
				uint32 l_ui32MatrixSize=op_pDecodedMatrix1->getDimensionSize(0);
				for(uint32 p=1; p<op_pDecodedMatrix1->getDimensionCount(); p++) {l_ui32MatrixSize*=op_pDecodedMatrix1->getDimensionSize(p);}
				//
				sstr<<"Size="<<l_ui32MatrixSize<<"\t";
				//
				for(k=0; k<l_ui32MatrixSize; k++)
				{
					ip_pMatrixToEncode->getBuffer()[l_ui64IndexCount+k]=op_pDecodedMatrix1->getBuffer()[k];
					//std::cout<<"Add "<<op_pDecodedMatrix1->getBuffer()[k]<<" ";
					sstr<<op_pDecodedMatrix1->getBuffer()[k]<<" ";
				}
				//std::cout<<std::endl;
				l_ui64IndexCount+=l_ui32MatrixSize;

				//std::cout<<"i="<<i<<" | "<<l_rStaticBoxContext.getInputCount()-1<<std::endl;
				if(i==l_rStaticBoxContext.getInputCount()-1)
				{
					sstr<<"\n Output Buffer : ";
					sstr<<ip_pMatrixToEncode->getDimensionCount()<<" = ";
					for(uint32 h=0; h<ip_pMatrixToEncode->getDimensionCount();h++)
					  {
						sstr<<ip_pMatrixToEncode->getDimensionSize(h);
						if(h+1!=ip_pMatrixToEncode->getDimensionCount())  {sstr<<"*";}
						else {sstr<<"\t";}
					  }
					uint32 l_ui32MatrixSizeOutput=ip_pMatrixToEncode->getDimensionSize(0);
					for(uint32 p=1; p<ip_pMatrixToEncode->getDimensionCount(); p++) {l_ui32MatrixSizeOutput*=ip_pMatrixToEncode->getDimensionSize(p);}
					for(k=0; k<l_ui32MatrixSizeOutput; k++)
					  {
						sstr<<ip_pMatrixToEncode->getBuffer()[k]<<" ";
					  }
					sstr<<" TO SEND ";
				
					//std::cout<<"send Buffer"<<std::endl;
					m_pSignalEncoder->process(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_InputTriggerId_EncodeBuffer);
					l_pDynamicBoxContext->markOutputAsReadyToSend(0, l_pDynamicBoxContext->getInputChunkStartTime(i, j), l_pDynamicBoxContext->getInputChunkEndTime(i, j));
				}

				sstr<<"\n";				
				if(pFile) {fwrite (sstr.str().c_str() , 1 , sstr.str().size() , pFile );}
				//
			}

			if(l_ui32DecoderType==DecoderType_End)
			{
				if(i==l_rStaticBoxContext.getInputCount()-1)
				{
					m_pSignalEncoder->process(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_InputTriggerId_EncodeEnd);
					l_pDynamicBoxContext->markOutputAsReadyToSend(0, l_pDynamicBoxContext->getInputChunkStartTime(i, j), l_pDynamicBoxContext->getInputChunkEndTime(i, j));
				}
			}

			l_pDynamicBoxContext->markInputAsDeprecated(i, j);
		}
	}

	return true;
}
