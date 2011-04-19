#include "ovpCBoxAlgorithmEMA.h"
#include "../../algorithms/signal-processing/ovpCAlgorithmEMA.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessing;

boolean CBoxAlgorithmEMA::initialize(void)
{
	m_pSignalDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamDecoder));
    m_pSignalDecoder->initialize();
    ip_pMemoryBufferToDecode.initialize(m_pSignalDecoder->getInputParameter(OVP_GD_Algorithm_SignalStreamDecoder_InputParameterId_MemoryBufferToDecode));
    op_pDecodedMatrix.initialize(m_pSignalDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_Matrix));
	op_ui64SamplingRate.initialize(m_pSignalDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_SamplingRate));

	m_pSignalEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamEncoder));
    m_pSignalEncoder->initialize();
    ip_pMatrixToEncode.initialize(m_pSignalEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_Matrix));
    op_pEncodedMemoryBuffer.initialize(m_pSignalEncoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
	ip_ui64SamplingRate.initialize(m_pSignalEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_SamplingRate));

	m_pSommeAlgorithm=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_ClassId_AlgoEMA));
	m_pSommeAlgorithm->initialize();
    ip_pMatrixAlgo.initialize(m_pSommeAlgorithm->getInputParameter(OVP_Algorithm_EMA_InputParameter));
    op_pMatrixAlgo.initialize(m_pSommeAlgorithm->getOutputParameter(OVP_Algorithm_EMA_OutputParameter));
	
	ip_pParameter.initialize(m_pSommeAlgorithm->getInputParameter(OVP_Algorithm_EMA_Parameter));
	
	CString l_sSettingValue;
	getStaticBoxContext().getSettingValue(0, l_sSettingValue);
	OpenViBE::float64 m_f64InfCropValue=::atof(l_sSettingValue);
	ip_pParameter=m_f64InfCropValue;
	
	//connecter les passerelles : 
	ip_ui64SamplingRate.setReferenceTarget(op_ui64SamplingRate);
	ip_pMatrixAlgo.setReferenceTarget(op_pDecodedMatrix);
    ip_pMatrixToEncode.setReferenceTarget(op_pMatrixAlgo);

	return true;
}

boolean CBoxAlgorithmEMA::uninitialize(void)
{
	// ...
	op_ui64SamplingRate.uninitialize();
	op_pDecodedMatrix.uninitialize();
    m_pSignalDecoder->uninitialize();
    this->getAlgorithmManager().releaseAlgorithm(*m_pSignalDecoder);

	ip_ui64SamplingRate.uninitialize();
	ip_pMatrixToEncode.uninitialize();
    m_pSignalEncoder->uninitialize();
    this->getAlgorithmManager().releaseAlgorithm(*m_pSignalEncoder);

	ip_pMatrixAlgo.uninitialize();
	op_pMatrixAlgo.uninitialize();
    m_pSommeAlgorithm->uninitialize();
    this->getAlgorithmManager().releaseAlgorithm(*m_pSommeAlgorithm);

	ip_pParameter.uninitialize();

	return true;
}

boolean CBoxAlgorithmEMA::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}


boolean CBoxAlgorithmEMA::process(void)
{
	//récupérer les entrées/sorties de la boîte
	IBoxIO* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();

	//prendre nombre de morceau disponible 
	uint32 nombreMorceau=l_pDynamicBoxContext->getInputChunkCount(0);

	uint64 l_ui64StartTime=0,l_ui64EndTime=0;

	//pour chacun de ces morceaux faire
	for(uint32 j=0; j<nombreMorceau; j++)
		{
			//obtenir les morceaux et décoder
			ip_pMemoryBufferToDecode=l_pDynamicBoxContext->getInputChunk(0, j);
			m_pSignalDecoder->process();

			//si le morceau appartient au header, faire l'initialisation et encoder
			if(m_pSignalDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedHeader))
				{
					//initialiser les matrices encore non définies :
					m_pSommeAlgorithm->process(OVP_Algorithm_EMA_InputTriggerId_Initialize);
					///
					op_pEncodedMemoryBuffer=l_pDynamicBoxContext->getOutputChunk(0);
					m_pSignalEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeHeader);
					l_ui64StartTime=l_pDynamicBoxContext->getInputChunkStartTime(0, j);
					l_ui64EndTime=l_pDynamicBoxContext->getInputChunkEndTime(0, j);
					l_pDynamicBoxContext->markOutputAsReadyToSend(0,l_ui64StartTime ,l_ui64EndTime );
				}

			//si les morceaux sont au buffer, faire l'opération et encoder
			if(m_pSignalDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedBuffer))
				{
					//faire l'opération
					m_pSommeAlgorithm->process(OVP_Algorithm_EMA_InputTriggerId_Process);
					///
					//transférer la somme vers la sortie et marquer la sortie comme prête
					op_pEncodedMemoryBuffer=l_pDynamicBoxContext->getOutputChunk(0);
					m_pSignalEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeBuffer);
					l_ui64StartTime=l_pDynamicBoxContext->getInputChunkStartTime(0, j);
					l_ui64EndTime=l_pDynamicBoxContext->getInputChunkEndTime(0, j);
					l_pDynamicBoxContext->markOutputAsReadyToSend(0,l_ui64StartTime,l_ui64EndTime);
				}

			//si les morceaux appartiennent au ender, transférer
			if(m_pSignalDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedEnd))
				{
					m_pSignalEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeEnd);
					l_ui64StartTime=l_pDynamicBoxContext->getInputChunkStartTime(0, j);
					l_ui64EndTime=l_pDynamicBoxContext->getInputChunkEndTime(0, j);
					l_pDynamicBoxContext->markOutputAsReadyToSend(0,l_ui64StartTime,l_ui64EndTime);
				}

			//marquer les morceaux à l'entrée comme lus
			l_pDynamicBoxContext->markInputAsDeprecated(0,j);
		}

	return true;
}
