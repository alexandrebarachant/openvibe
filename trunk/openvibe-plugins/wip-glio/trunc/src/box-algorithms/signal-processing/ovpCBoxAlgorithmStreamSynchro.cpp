#include "ovpCBoxAlgorithmStreamSynchro.h"
#include <algorithm>
#include <cstdio>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessing;

boolean CBoxAlgorithmStreamSynchro::initialize(void)
{
	
	m_pSignalDecoder1=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamDecoder));
    m_pSignalDecoder1->initialize();
    ip_pMemoryBufferToDecode1.initialize(m_pSignalDecoder1->getInputParameter(OVP_GD_Algorithm_SignalStreamDecoder_InputParameterId_MemoryBufferToDecode));
    op_pDecodedMatrix1.initialize(m_pSignalDecoder1->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_Matrix));
	op_ui64SamplingRate1.initialize(m_pSignalDecoder1->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_SamplingRate));
    

	m_pSignalDecoder2=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamDecoder));
    m_pSignalDecoder2->initialize();
    ip_pMemoryBufferToDecode2.initialize(m_pSignalDecoder2->getInputParameter(OVP_GD_Algorithm_SignalStreamDecoder_InputParameterId_MemoryBufferToDecode));
    op_pDecodedMatrix2.initialize(m_pSignalDecoder2->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_Matrix));
	op_ui64SamplingRate2.initialize(m_pSignalDecoder2->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_SamplingRate));


	m_pSignalEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamEncoder));
    m_pSignalEncoder->initialize();
    ip_pMatrixToEncode.initialize(m_pSignalEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_Matrix));
    op_pEncodedMemoryBuffer.initialize(m_pSignalEncoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
    ip_ui64SamplingRate.initialize(m_pSignalEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_SamplingRate));

	ip_ui64SamplingRate.setReferenceTarget(op_ui64SamplingRate1);
	
	
	//init count
	m_Chunk_sent_count	= 0;
	m_Chunk_time		= 0;
	m_headerSent		= false;

	m_2ChunksFlag_s1    = 0;
	m_2ChunksFlag_s2	= 0;

	return true;
}

boolean CBoxAlgorithmStreamSynchro::uninitialize(void)
{
	// ...
	
	op_pDecodedMatrix1.uninitialize();
    m_pSignalDecoder1->uninitialize();
	ip_pMemoryBufferToDecode1.uninitialize();
	op_ui64SamplingRate1.uninitialize();
    this->getAlgorithmManager().releaseAlgorithm(*m_pSignalDecoder1);
	
	op_pDecodedMatrix2.uninitialize();
    m_pSignalDecoder2->uninitialize();
	ip_pMemoryBufferToDecode2.uninitialize();
	op_ui64SamplingRate2.uninitialize();
    this->getAlgorithmManager().releaseAlgorithm(*m_pSignalDecoder2);

	ip_pMatrixToEncode.uninitialize();
    m_pSignalEncoder->uninitialize();
	op_pEncodedMemoryBuffer.uninitialize();
    ip_ui64SamplingRate.uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pSignalEncoder);




	return true;
}


boolean CBoxAlgorithmStreamSynchro::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}


boolean CBoxAlgorithmStreamSynchro::process(void)
{
	

	//récupérer les entrées/sorties de la boîte
	//prendre le plus petit nombre de morceau disponible sur M1 et M2
	//pour chacun de ces morceaux faire
		//obtenir les morceaux de M1 et décoder
		//obtenir les morceaux de M2 et décoder
		//si les 2 morceaux appartiennent au header, faire l'initialisation et encoder
		//si les 2 morceaux sont au buffer, faire le traitement et encoder
		//transférer le flux traité vers la sortie et marquer la sortie comme prête
		//marquer les deux morceaux à l'entrée comme lu


	//récupérer les entrées/sorties de la boîte
	IBoxIO* l_pDynamicBoxContext	= getBoxAlgorithmContext()->getDynamicBoxContext();

	//prendre le plus petit nombre de morceau disponible sur M1 et M2
	uint32 nombreMorceauM1			= l_pDynamicBoxContext->getInputChunkCount(0);
	uint32 nombreMorceauM2			= l_pDynamicBoxContext->getInputChunkCount(1);

	//if((nombreMorceauM1>2)&&(nombreMorceauM2<2))nombreMorceauM1==0;
	//if((nombreMorceauM2>2)&&(nombreMorceauM1<2))nombreMorceauM2==0;

	//printf("Chunk en attente stream 1 = %d\n",nombreMorceauM1);
	//printf("Chunk en attente stream 2 = %d\n",nombreMorceauM2);

	if(!m_engine.IsInitialized() && (nombreMorceauM1 || nombreMorceauM2))
	{
		if(nombreMorceauM1)
		{	ip_pMemoryBufferToDecode1=l_pDynamicBoxContext->getInputChunk(0, 0);
			m_pSignalDecoder1->process();
			//printf("Chunk in 1 - first\n");

			if(m_pSignalDecoder1->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedHeader))
			{	m_engine.Build(0, op_ui64SamplingRate1, op_pDecodedMatrix1->getDimensionSize(0), op_pDecodedMatrix1->getDimensionSize(1), 1);
				printf("Chunk in 1 - header\n");
				//marquer les morceaux à l'entrée comme lus
				l_pDynamicBoxContext->markInputAsDeprecated(0,0);
		}	}
		
		if(nombreMorceauM2)
		{	ip_pMemoryBufferToDecode2=l_pDynamicBoxContext->getInputChunk(1, 0);
			m_pSignalDecoder2->process();
			//printf("Chunk in 2 - first\n");

			if(m_pSignalDecoder2->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedHeader))
			{	m_engine.Build(1, op_ui64SamplingRate2, op_pDecodedMatrix2->getDimensionSize(0), op_pDecodedMatrix2->getDimensionSize(1), 1);
				printf("Chunk in 2 - header\n");
				//marquer les morceaux à l'entrée comme lus
				l_pDynamicBoxContext->markInputAsDeprecated(1, 0);
		}	}

		return true;
	}


	for(uint32 ii=0; ii < nombreMorceauM1; ii++)
	{	ip_pMemoryBufferToDecode1=l_pDynamicBoxContext->getInputChunk(0, ii);
		m_pSignalDecoder1->process();

		if(m_pSignalDecoder1->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedBuffer))
		{	if(!m_engine.IsDetected(0))
			{	m_engine.Push(0, *op_pDecodedMatrix1);
				l_pDynamicBoxContext->markInputAsDeprecated(0, ii);
		}	}
		else
			l_pDynamicBoxContext->markInputAsDeprecated(0, ii);
	}
			
	for(uint32 ii=0; ii < nombreMorceauM2; ii++)
	{	ip_pMemoryBufferToDecode2=l_pDynamicBoxContext->getInputChunk(1, ii);
		m_pSignalDecoder2->process();

		if(m_pSignalDecoder2->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedBuffer))
		{	if(!m_engine.IsDetected(1))
			{	m_engine.Push(1, *op_pDecodedMatrix2);
				l_pDynamicBoxContext->markInputAsDeprecated(1, ii);
		}	}
		else
			l_pDynamicBoxContext->markInputAsDeprecated(1, ii);
	}
	
	if(!m_headerSent)
	{			
		m_headerSent = true;
		m_OutputChunkSize= m_engine.NbSamples(0) <= m_engine.NbSamples(1) ? m_engine.NbSamples(0) : m_engine.NbSamples(1);
		
		ip_pMatrixToEncode->setDimensionCount(2);
		ip_pMatrixToEncode->setDimensionSize(0, m_engine.NbChannels(0) + m_engine.NbChannels(1));
		ip_pMatrixToEncode->setDimensionSize(1, m_OutputChunkSize);

		op_pEncodedMemoryBuffer=l_pDynamicBoxContext->getOutputChunk(0);
		m_pSignalEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeHeader);
		l_pDynamicBoxContext->markOutputAsReadyToSend(0,0,0);
	}

	OpenViBE::uint32 nbChunks = m_engine.NbChunks(m_OutputChunkSize);

	if(nbChunks)
	{
		while(nbChunks--)
		{
			m_engine.GetResult(*ip_pMatrixToEncode);
			
			// Prepare encoder
			op_pEncodedMemoryBuffer	= l_pDynamicBoxContext->getOutputChunk(0);
			m_pSignalEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeBuffer);

			// Calculate the chunk time in s.
			m_Chunk_time			= m_Chunk_sent_count*m_engine.NbSamples(0)*(1/(float)op_ui64SamplingRate1);
			
			// Convert the (float) time in the OpenVibe time format.
			uint64 l_ui64StartTime1	= uint64(m_Chunk_time*(1ll<<32));
			uint64 l_ui64EndTime1	= uint64(((m_Chunk_sent_count+1)*m_engine.NbSamples(0)*(1/(float)op_ui64SamplingRate1))*(1ll<<32));
			l_pDynamicBoxContext->markOutputAsReadyToSend(0, l_ui64StartTime1 , l_ui64EndTime1 );
			
			// update the number of chunk sent.
			m_Chunk_sent_count++;
		}
		m_engine.TagUndetected();
	}//End if nbChunks

	return true;
}
