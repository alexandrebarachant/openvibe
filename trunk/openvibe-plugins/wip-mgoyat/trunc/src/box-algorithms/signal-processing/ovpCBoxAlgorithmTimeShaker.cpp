#include "ovpCBoxAlgorithmTimeShaker.h"
#include <cstdlib>
#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessing;

boolean CBoxTimeMover::initialize(void)
{
	m_pStreamDecoder=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamDecoder));
	m_pStreamDecoder->initialize();

	m_pStreamEncoder=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamEncoder));
	m_pStreamEncoder->initialize();

	m_oInputMemoryBufferHandle.initialize(m_pStreamDecoder->getInputParameter(OVP_GD_Algorithm_SignalStreamDecoder_InputParameterId_MemoryBufferToDecode));
	m_oOutputMemoryBufferHandle.initialize(m_pStreamEncoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
	m_pStreamEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_Matrix)->setReferenceTarget(m_pStreamDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_Matrix));
	iop_pSamplingRate.initialize(m_pStreamDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_SamplingRate));	
	m_pStreamEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_SamplingRate)->setReferenceTarget(m_pStreamDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_SamplingRate));
	
	CString l_sSettings;
	getBoxAlgorithmContext()->getStaticBoxContext()->getSettingValue(0, l_sSettings);
	m_fDilatation=::atof(l_sSettings);
	//
	getBoxAlgorithmContext()->getStaticBoxContext()->getSettingValue(1, l_sSettings);
	m_foffStart=::atof(l_sSettings);
	//
	getBoxAlgorithmContext()->getStaticBoxContext()->getSettingValue(2, l_sSettings);
	// m_foffEnd=::atof(l_sSettings);
	m_foffEnd=m_foffStart;// en attendant de gérer la dilatation supplémentaire

	return true;
}

boolean CBoxTimeMover::uninitialize(void)
{
	iop_pSamplingRate.uninitialize();
	m_oInputMemoryBufferHandle.uninitialize();
	m_oOutputMemoryBufferHandle.uninitialize();

	m_pStreamEncoder->uninitialize();
	m_pStreamDecoder->uninitialize();

	getAlgorithmManager().releaseAlgorithm(*m_pStreamEncoder);
	getAlgorithmManager().releaseAlgorithm(*m_pStreamDecoder);

	return true;
}

boolean CBoxTimeMover::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CBoxTimeMover::process(void)
{
	// std::cout<<"process"<<std::endl;
	
	IBoxIO& l_rDynamicBoxContext=getDynamicBoxContext();
	// IBox& l_rStaticBoxContext=getStaticBoxContext();
	
	for(uint32 j=0; j<l_rDynamicBoxContext.getInputChunkCount(0); j++)
	  {
		m_oInputMemoryBufferHandle=l_rDynamicBoxContext.getInputChunk(0, j);
		m_oOutputMemoryBufferHandle=l_rDynamicBoxContext.getOutputChunk(0);
		uint64 l_ui64StartTime=l_rDynamicBoxContext.getInputChunkStartTime(0, j);
		uint64 l_ui64EndTime=l_rDynamicBoxContext.getInputChunkEndTime(0, j);
		uint64 l_ui64DeltaTime=DilatationTime3232(l_ui64EndTime-l_ui64StartTime,m_fDilatation);
		uint64 l_ui64StartTimeOff7=TranslationTime3232(l_ui64StartTime,m_foffStart);
		uint64 l_ui64EndTimeOff7=TranslationTime3232(l_ui64StartTime+l_ui64DeltaTime,m_foffEnd);
		//plus la dilatation engendré par deltaTimeOff...
		//l_ui64EndTimeOff7-l_ui64StartTimeOff7
			
		m_pStreamDecoder->process();
	
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedHeader))
			{
				std::cout<<"DownSampling information : "<<iop_pSamplingRate<<"*"<<1/m_fDilatation<<"=>"<<iop_pSamplingRate/m_fDilatation<<std::endl;
				iop_pSamplingRate=iop_pSamplingRate/m_fDilatation;
				if(iop_pSamplingRate==0)
				  {std::cout<<"nul sampling Rate, it could produce damage in next boxes"<<std::endl;}
				//
				m_pStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeHeader);
				l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_ui64StartTime, l_ui64EndTime);
			}//end header
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedBuffer))
			{
				m_pStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeBuffer);
				l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_ui64StartTimeOff7, l_ui64EndTimeOff7);
				std::cout<<"chunkTime : "<<l_ui64StartTimeOff7<<"->"<<l_ui64EndTimeOff7<<std::endl;
			}
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedEnd))
			{
				m_pStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeEnd);
				l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_ui64StartTime, l_ui64EndTime);
			}

		l_rDynamicBoxContext.markInputAsDeprecated(0, j);
	  }
	  
	// std::cout<<"end process"<<std::endl;
	return true;
}

uint64 CBoxTimeMover::TranslationTime3232(OpenViBE::uint64 base, OpenViBE::float64 times)
{
	if(times>=0) return base+times*(1LL<<32);
	if(base<(-times)*(1LL<<32)) return 0;
	return base-((-times)*(1LL<<32));
}

uint64 CBoxTimeMover::DilatationTime3232(OpenViBE::uint64 base, OpenViBE::float64 factor)
{
	uint64 baseS=uint64(((base>>32)<<32)*factor);
	uint64 baseMS=uint64((factor*float64(((((base<<32)>>32)+65536)*1000)>>32)/1000)*(1LL<<32));
	return baseS+baseMS;
}

