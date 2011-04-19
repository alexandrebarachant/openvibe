#include "ovpCBoxAlgorithmStim2Matrix.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessing;

namespace
{
	class _AutoCast_
	{
	public:
		_AutoCast_(IBox& rBox, IConfigurationManager& rConfigurationManager, const uint32 ui32Index) : m_rConfigurationManager(rConfigurationManager) { rBox.getSettingValue(ui32Index, m_sSettingValue); }
		operator uint64 (void) { return m_rConfigurationManager.expandAsUInteger(m_sSettingValue); }
		operator int64 (void) { return m_rConfigurationManager.expandAsInteger(m_sSettingValue); }
		operator float64 (void) { return m_rConfigurationManager.expandAsFloat(m_sSettingValue); }
		operator boolean (void) { return m_rConfigurationManager.expandAsBoolean(m_sSettingValue); }
		operator const CString (void) { return m_sSettingValue; }
	protected:
		IConfigurationManager& m_rConfigurationManager;
		CString m_sSettingValue;
	};
}

OpenViBE::boolean CBoxAlgorithmStim2Matrix::initialize(void)
{
	//> get Static box context to access to user choices
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();

	//> init stimlation input
	m_pStimulationDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
    m_pStimulationDecoder->initialize();
    ip_pMemoryBufferToDecode.initialize(m_pStimulationDecoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
	op_pStimulationSet.initialize(m_pStimulationDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));

	//> init signal output
	m_pSignalEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StreamedMatrixStreamEncoder));
    m_pSignalEncoder->initialize();
    ip_pMatrixToEncode.initialize(m_pSignalEncoder->getInputParameter(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_InputParameterId_Matrix));
    op_pEncodedMemoryBuffer.initialize(m_pSignalEncoder->getOutputParameter(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
	
	m_oMatrix=new CMatrix();
	m_oMatrix->setDimensionCount(1);
	m_oMatrix->setDimensionSize(0,1);
	
	m_ui64StimulationStart=this->getTypeManager().getEnumerationEntryValueFromName(OV_TypeId_Stimulation, _AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 0));
	m_ui64StimulationEnd=this->getTypeManager().getEnumerationEntryValueFromName(OV_TypeId_Stimulation, _AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 1));
	//m_ui64StimulationStart=OVTK_StimulationId_Label_00;
	//m_ui64StimulationEnd=OVTK_StimulationId_Label_1F;
	return true;
}

OpenViBE::boolean CBoxAlgorithmStim2Matrix::uninitialize(void)
{
	// uninit stimulation input
	op_pStimulationSet.uninitialize();
    m_pStimulationDecoder->uninitialize();
	ip_pMemoryBufferToDecode.uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationDecoder);

	// uninit signal output
	ip_pMatrixToEncode.uninitialize();
    m_pSignalEncoder->uninitialize();
	op_pEncodedMemoryBuffer.uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pSignalEncoder);
	
	return true;
}

OpenViBE::boolean CBoxAlgorithmStim2Matrix::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}

OpenViBE::boolean CBoxAlgorithmStim2Matrix::process(void)
{
	//> Get dynamic box context
	IBoxIO* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();

	//> Init local variables
	uint64 l_ui64StartTime=0;
	uint64 l_ui64EndTime=0;
	uint64 l_ui64ChunkSize=0;
	uint64 l_ui64StimulationDate=0;


	for(uint32 j=0; j<l_pDynamicBoxContext->getInputChunkCount(0); j++)
		{
			ip_pMemoryBufferToDecode=l_pDynamicBoxContext->getInputChunk(0, j);
			op_pEncodedMemoryBuffer=l_pDynamicBoxContext->getOutputChunk(0);
			m_pStimulationDecoder->process();

			l_ui64StartTime=l_pDynamicBoxContext->getInputChunkStartTime(0, j);
			l_ui64EndTime=l_pDynamicBoxContext->getInputChunkEndTime(0, j);
					
			if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedHeader))
				{					
					ip_pMatrixToEncode=m_oMatrix;

					m_pSignalEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeHeader);
					l_pDynamicBoxContext->markOutputAsReadyToSend(0,l_ui64StartTime ,l_ui64EndTime );
				}
			if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
				{
					ip_pMatrixToEncode=m_oMatrix;
					op_pEncodedMemoryBuffer=l_pDynamicBoxContext->getOutputChunk(0);
					
					for(uint32 stim=0; stim<op_pStimulationSet->getStimulationCount(); stim++)
					  {
						// std::cout<<"stimulation detected"<<std::endl;
						if(op_pStimulationSet->getStimulationIdentifier(stim)>=m_ui64StimulationStart
							&& op_pStimulationSet->getStimulationIdentifier(stim)<=m_ui64StimulationEnd)
						  {
							uint32 stim_index=(uint32)(op_pStimulationSet->getStimulationIdentifier(stim)-m_ui64StimulationStart);
							m_oMatrix->getBuffer()[0]=stim_index;
							// std::cout<<"should send a "<<stim_index<<std::endl;
							m_pSignalEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeBuffer);
							l_pDynamicBoxContext->markOutputAsReadyToSend(0,l_ui64StartTime ,l_ui64EndTime );
						  }
 					  }
				}
			if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedEnd))
				{
				}
			
			l_pDynamicBoxContext->markInputAsDeprecated(0,j);
		}
	return true;
}



