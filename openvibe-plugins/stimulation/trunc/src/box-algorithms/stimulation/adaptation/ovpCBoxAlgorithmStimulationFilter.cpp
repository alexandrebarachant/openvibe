#include "ovpCBoxAlgorithmStimulationFilter.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Stimulation;

boolean CBoxAlgorithmStimulationFilter::initialize(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();

	m_ui64DefaultAction=FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 0);
	for(uint32 i=1; i<l_rStaticBoxContext.getSettingCount(); i+=5)
	{
		SRule l_oRule;
		l_oRule.ui64Action            =FSettingValueAutoCast(*this->getBoxAlgorithmContext(), i  );
		l_oRule.ui64StartStimulationId=FSettingValueAutoCast(*this->getBoxAlgorithmContext(), i+1);
		l_oRule.ui64EndStimulationId  =FSettingValueAutoCast(*this->getBoxAlgorithmContext(), i+2);
		l_oRule.ui64StartTime         =(uint64)(((float64)FSettingValueAutoCast(*this->getBoxAlgorithmContext(), i+3))*(1LL<<32));
		l_oRule.ui64EndTime           =(uint64)(((float64)FSettingValueAutoCast(*this->getBoxAlgorithmContext(), i+4))*(1LL<<32));
		m_vRules.push_back(l_oRule);
	}

	m_pStimulationEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamEncoder));
	m_pStimulationEncoder->initialize();

	m_pStimulationDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
	m_pStimulationDecoder->initialize();

	ip_pMemoryBuffer.initialize(m_pStimulationDecoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
	op_pStimulationSet.initialize(m_pStimulationDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
	ip_pStimulationSet.initialize(m_pStimulationEncoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_InputParameterId_StimulationSet));
	op_pMemoryBuffer.initialize(m_pStimulationEncoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_OutputParameterId_EncodedMemoryBuffer));

	return true;
}

boolean CBoxAlgorithmStimulationFilter::uninitialize(void)
{
	op_pMemoryBuffer.uninitialize();
	ip_pStimulationSet.uninitialize();
	op_pStimulationSet.uninitialize();
	ip_pMemoryBuffer.uninitialize();

	m_pStimulationDecoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationDecoder);

	m_pStimulationEncoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationEncoder);

	m_vRules.clear();

	return true;
}

boolean CBoxAlgorithmStimulationFilter::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}

boolean CBoxAlgorithmStimulationFilter::process(void)
{
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	for(uint32 i=0; i<l_rDynamicBoxContext.getInputChunkCount(0); i++)
	{
		ip_pMemoryBuffer=l_rDynamicBoxContext.getInputChunk(0, i);
		op_pMemoryBuffer=l_rDynamicBoxContext.getOutputChunk(0);
		m_pStimulationDecoder->process();
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedHeader))
		{
			m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeHeader);
		}
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
		{
			ip_pStimulationSet->setStimulationCount(0);
			for(uint64 s=0; s<op_pStimulationSet->getStimulationCount(); s++)
			{
				uint64 l_ui64StimulationId=op_pStimulationSet->getStimulationIdentifier(s);
				uint64 l_ui64StimulationDate=op_pStimulationSet->getStimulationDate(s);
				uint64 l_ui64Action=m_ui64DefaultAction;

				for(size_t r=0; r<m_vRules.size(); r++)
				{
					const SRule& l_rRule=m_vRules[i];
					if(l_rRule.ui64StartStimulationId <= l_ui64StimulationId && l_ui64StimulationId <= l_rRule.ui64EndStimulationId)
					{
						if(l_rRule.ui64StartTime <= l_ui64StimulationDate && l_ui64StimulationDate <= l_rRule.ui64EndTime)
						{
							l_ui64Action=l_rRule.ui64Action;
						}
					}
				}

				if(l_ui64Action==OVP_TypeId_StimulationFilterAction_Select.toUInteger())
				{
					ip_pStimulationSet->appendStimulation(
						l_ui64StimulationId,
						l_ui64StimulationDate,
						op_pStimulationSet->getStimulationDuration(s));
				}
				if(l_ui64Action==OVP_TypeId_StimulationFilterAction_Reject.toUInteger())
				{
				}
			}

			m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeBuffer);
		}
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedEnd))
		{
			m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeEnd);
		}
		l_rDynamicBoxContext.markInputAsDeprecated(0, i);
		l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_rDynamicBoxContext.getInputChunkStartTime(0, i), l_rDynamicBoxContext.getInputChunkEndTime(0, i));
	}

	return true;
}
