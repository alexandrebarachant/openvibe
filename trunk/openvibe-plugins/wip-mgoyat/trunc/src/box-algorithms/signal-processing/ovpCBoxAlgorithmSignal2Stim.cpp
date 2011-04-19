#include "ovpCBoxAlgorithmSignal2Stim.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessing;

#include <iostream>

namespace
{
	class _AutoCast_
	{
	public:
		_AutoCast_(IBox& rBox, IConfigurationManager& rConfigurationManager, const uint32 ui32Index) : m_rConfigurationManager(rConfigurationManager) { rBox.getSettingValue(ui32Index, m_sSettingValue); }
		operator uint64 (void) { return m_rConfigurationManager.expandAsUInteger(m_sSettingValue); }
		operator int64 (void) { return m_rConfigurationManager.expandAsInteger(m_sSettingValue); }
		operator float64 (void) { return m_rConfigurationManager.expandAsFloat(m_sSettingValue); }
		operator OpenViBE::boolean (void) { return m_rConfigurationManager.expandAsBoolean(m_sSettingValue); }
		operator const CString (void) { return m_sSettingValue; }
	protected:
		IConfigurationManager& m_rConfigurationManager;
		CString m_sSettingValue;
	};
};

OpenViBE::boolean CBoxAlgorithmSignal2Stim::initialize(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();

	//> init Trigger input
	m_pSignalDecoderTrigger=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamDecoder));
	m_pSignalDecoderTrigger->initialize();
	ip_pMemoryBufferToDecodeTrigger.initialize(m_pSignalDecoderTrigger->getInputParameter(OVP_GD_Algorithm_SignalStreamDecoder_InputParameterId_MemoryBufferToDecode));
	op_pDecodedMatrixTrigger.initialize(m_pSignalDecoderTrigger->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_Matrix));
	op_ui64SamplingRateTrigger.initialize(m_pSignalDecoderTrigger->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_SamplingRate));
	
	//> init stimulation output
	m_pStimulationEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamEncoder));
	m_pStimulationEncoder->initialize();
	ip_pStimulationsToEncode.initialize(m_pStimulationEncoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_InputParameterId_StimulationSet));
	op_pEncodedMemoryBuffer.initialize(m_pStimulationEncoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_OutputParameterId_EncodedMemoryBuffer));

	//> get user stimulation type
	m_ui64StimulationToTrigUp=this->getTypeManager().getEnumerationEntryValueFromName(OV_TypeId_Stimulation, _AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 0));
	m_ui64StimulationToTrigDown=this->getTypeManager().getEnumerationEntryValueFromName(OV_TypeId_Stimulation, _AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 1));

	m_f64MemFront=0;

	return true;
}

OpenViBE::boolean CBoxAlgorithmSignal2Stim::uninitialize(void)
{
	// uninit trigger input
	m_pSignalDecoderTrigger->uninitialize();
	ip_pMemoryBufferToDecodeTrigger.uninitialize();
	op_pDecodedMatrixTrigger.uninitialize();
	op_ui64SamplingRateTrigger.uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pSignalDecoderTrigger);
	
	// uninit stimulation output
	m_pStimulationEncoder->uninitialize();
	ip_pStimulationsToEncode.uninitialize();
	op_pEncodedMemoryBuffer.uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationEncoder);

	return true;
}

OpenViBE::boolean CBoxAlgorithmSignal2Stim::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}

OpenViBE::boolean CBoxAlgorithmSignal2Stim::process(void)
{
	//> Get dynamic box context
	IBoxIO* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();

	//> extraire les dates de stimuli par rapport au trigger
	for(uint32 j=0; j<l_pDynamicBoxContext->getInputChunkCount(0); j++)
	  {
		ip_pMemoryBufferToDecodeTrigger=l_pDynamicBoxContext->getInputChunk(0,j);
		m_pSignalDecoderTrigger->process();
		ip_pStimulationsToEncode->clear();

		if(m_pSignalDecoderTrigger->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedBuffer))
		  {
			uint64 l_ui64ChunkStartTime=l_pDynamicBoxContext->getInputChunkStartTime(0, j);
			uint64 l_ui64ChunkEndTime=l_pDynamicBoxContext->getInputChunkEndTime(0, j);

			for(uint32 i=0; i<op_pDecodedMatrixTrigger->getDimensionSize(1); i++)
			  {
				float64 l_f64value=op_pDecodedMatrixTrigger->getBuffer()[i];
				if(m_f64MemFront!=l_f64value) //nouveau front
				{
					//sample index to time OV :
					uint64 l_ui64sampleTime=l_ui64ChunkStartTime+ uint64((i/float64(op_ui64SamplingRateTrigger))*(1LL<<32)); //a verifier
					//Stim id :
					uint64 l_StimId=0;
					if(l_f64value==0){l_StimId=m_ui64StimulationToTrigDown;}  //front descendant
					else {l_StimId=m_ui64StimulationToTrigUp;}  //front montant
					//save
					ip_pStimulationsToEncode->appendStimulation(l_StimId,l_ui64sampleTime, 0);
				}
				m_f64MemFront=l_f64value;
			  }

			if(ip_pStimulationsToEncode->getStimulationCount()<=0)
			  {ip_pStimulationsToEncode->appendStimulation(0,l_ui64ChunkStartTime+(l_ui64ChunkEndTime-l_ui64ChunkStartTime)/2, 0);}
			op_pEncodedMemoryBuffer=l_pDynamicBoxContext->getOutputChunk(0);
			m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeBuffer);
			l_pDynamicBoxContext->markOutputAsReadyToSend(0,l_ui64ChunkStartTime ,l_ui64ChunkEndTime );
		  }

		l_pDynamicBoxContext->markInputAsDeprecated(0,j);
	  }

	return true;
}// End Process
