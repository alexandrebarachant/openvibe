#include "ovpCBoxAlgorithmVotingClassifier.h"

#include <system/Memory.h>

#include <list>
#include <vector>
#include <string>
#include <algorithm>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Classification;

boolean CBoxAlgorithmVotingClassifier::initialize(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();
	uint32 i;

	m_pClassificationChoiceEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamEncoder));
	m_pClassificationChoiceEncoder->initialize();

	ip_pClassificationChoiceStimulationSet.initialize(m_pClassificationChoiceEncoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_InputParameterId_StimulationSet));
	op_pClassificationChoiceMemoryBuffer.initialize(m_pClassificationChoiceEncoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_OutputParameterId_EncodedMemoryBuffer));

	for(i=0; i<l_rStaticBoxContext.getInputCount(); i++)
	{
		SInput& l_rInput=m_vClassificationResults[i];
		l_rInput.m_pDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
		l_rInput.m_pDecoder->initialize();

		l_rInput.ip_pMemoryBuffer.initialize(l_rInput.m_pDecoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
		l_rInput.op_pStimulationSet.initialize(l_rInput.m_pDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
	}

	m_ui64NumberOfRepetitions  =FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 0);
	m_ui64TargetClassLabel     =FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 1);
	m_ui64NonTargetClassLabel  =FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 2);
	m_ui64RejectClassLabel     =FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 3);
	m_ui64ResultClassLabelBase =FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 4);

	m_ui64LastTime=0;

	m_pClassificationChoiceEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeHeader);
	l_rDynamicBoxContext.markOutputAsReadyToSend(0, m_ui64LastTime, this->getPlayerContext().getCurrentTime());

	return true;
}

boolean CBoxAlgorithmVotingClassifier::uninitialize(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	// IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();
	uint32 i;

	for(i=0; i<l_rStaticBoxContext.getInputCount(); i++)
	{
		SInput& l_rInput=m_vClassificationResults[i];
		l_rInput.m_pDecoder->uninitialize();
		this->getAlgorithmManager().releaseAlgorithm(*l_rInput.m_pDecoder);
	}

	m_pClassificationChoiceEncoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pClassificationChoiceEncoder);

	return true;
}

boolean CBoxAlgorithmVotingClassifier::processInput(uint32 ui32Index)
{
	this->getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}

boolean CBoxAlgorithmVotingClassifier::process(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();
	uint32 i, j, k;

	boolean l_bCanChoose=true;

	for(i=0; i<l_rStaticBoxContext.getInputCount(); i++)
	{
		SInput& l_rInput=m_vClassificationResults[i];
		for(j=0; j<l_rDynamicBoxContext.getInputChunkCount(i); j++)
		{
			l_rInput.ip_pMemoryBuffer=l_rDynamicBoxContext.getInputChunk(i, j);
			l_rInput.m_pDecoder->process();
			if(l_rInput.m_pDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedHeader))
			{
			}
			if(l_rInput.m_pDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
			{
				for(k=0; k<l_rInput.op_pStimulationSet->getStimulationCount(); k++)
				{
					uint64 l_ui64StimulationIdentifier=l_rInput.op_pStimulationSet->getStimulationIdentifier(k);
					if(l_ui64StimulationIdentifier == m_ui64TargetClassLabel || l_ui64StimulationIdentifier == m_ui64NonTargetClassLabel)
					{
						l_rInput.m_vStimulationId.push_back(std::pair<uint64, uint64>(l_ui64StimulationIdentifier, l_rInput.op_pStimulationSet->getStimulationDate(k)));
					}
				}
			}
			if(l_rInput.m_pDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedEnd))
			{
			}
			l_rDynamicBoxContext.markInputAsDeprecated(i, j);
		}

		if(l_rInput.m_vStimulationId.size() < m_ui64NumberOfRepetitions)
		{
			l_bCanChoose=false;
		}
	}

	if(l_bCanChoose)
	{
		uint64 l_ui64ResultScore = 0;
		uint64 l_ui64ResultClassLabel = m_ui64RejectClassLabel;
		uint64 l_ui64Time = 0;

		std::map < uint32, uint32 > l_vScores;
		for(i=0; i<l_rStaticBoxContext.getInputCount(); i++)
		{
			SInput& l_rInput=m_vClassificationResults[i];
			for(j=0; j<m_ui64NumberOfRepetitions; j++)
			{
				if(l_rInput.m_vStimulationId[j].first == m_ui64TargetClassLabel)
				{
					l_vScores[i]++;
				}
			}

			if(l_vScores[i] > l_ui64ResultScore)
			{
				l_ui64ResultScore = l_vScores[i];
				l_ui64ResultClassLabel = m_ui64ResultClassLabelBase + i;
				l_ui64Time = l_rInput.m_vStimulationId[m_ui64NumberOfRepetitions-1].second;
			}
			else if(l_vScores[i] == l_ui64ResultScore)
			{
				l_ui64ResultScore = l_vScores[i];
				l_ui64ResultClassLabel = m_ui64RejectClassLabel;
				l_ui64Time = l_rInput.m_vStimulationId[m_ui64NumberOfRepetitions-1].second;
			}

			this->getLogManager() << LogLevel_Trace << "Input " << i << " got score " << l_vScores[i] << "\n";

			l_rInput.m_vStimulationId.erase(l_rInput.m_vStimulationId.begin(), l_rInput.m_vStimulationId.begin()+m_ui64NumberOfRepetitions);
		}

		if(l_ui64ResultClassLabel != m_ui64RejectClassLabel)
		{
			this->getLogManager() << LogLevel_Trace << "Chosed " << this->getTypeManager().getEnumerationEntryNameFromValue(OV_TypeId_Stimulation, l_ui64ResultClassLabel) << " with score " << l_ui64ResultScore << "\n";
		}
		else
		{
			this->getLogManager() << LogLevel_Trace << "Chosed rejection " << this->getTypeManager().getEnumerationEntryNameFromValue(OV_TypeId_Stimulation, l_ui64ResultClassLabel) << "\n";
		}

		for(i=0; i<l_rStaticBoxContext.getInputCount(); i++)
		{
			this->getLogManager() << LogLevel_Trace << "Input " << i << " got score " << l_vScores[i] << "\n";
		}


		CStimulationSet l_oStimulationSet;
		l_oStimulationSet.appendStimulation(l_ui64ResultClassLabel, l_ui64Time, 0);
		ip_pClassificationChoiceStimulationSet=&l_oStimulationSet;
		op_pClassificationChoiceMemoryBuffer=l_rDynamicBoxContext.getOutputChunk(0);
		m_pClassificationChoiceEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeBuffer);
		l_rDynamicBoxContext.markOutputAsReadyToSend(0, m_ui64LastTime, l_ui64Time);
		m_ui64LastTime=l_ui64Time;
	}

	return true;
}
