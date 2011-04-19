#include "ovpCBoxAlgorithmTestAhmed.h"

#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessing;

boolean CBoxAlgorithmTestAhmed::initialize(void)
{
	m_pStimulationDecoderRow=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
    m_pStimulationDecoderRow->initialize();
    ip_pMemoryBufferToDecodeRow.initialize(m_pStimulationDecoderRow->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
    op_pStimulationSetRow.initialize(m_pStimulationDecoderRow->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));

	m_pStimulationDecoderColumn=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
    m_pStimulationDecoderColumn->initialize();
    ip_pMemoryBufferToDecodeColumn.initialize(m_pStimulationDecoderColumn->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
    op_pStimulationSetColumn.initialize(m_pStimulationDecoderColumn->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));

	
	m_pStimulationEncoderCommand=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamEncoder));
    m_pStimulationEncoderCommand->initialize();
    ip_pStimulationSetCommand.initialize(m_pStimulationEncoderCommand->getInputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_InputParameterId_StimulationSet));
    op_pEncodedMemoryBufferCommand.initialize(m_pStimulationEncoderCommand->getOutputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_OutputParameterId_EncodedMemoryBuffer));

	RowHeader=false;
	ColumnHeader=false;
	HeaderSent=false;
	CommandDetected=false;
	FirstLetterOK=false;
	return true;
}

boolean CBoxAlgorithmTestAhmed::uninitialize(void)
{
	queueRow.clear();
	queueColumn.clear();
	
	// ...
	op_pStimulationSetRow.uninitialize();
	ip_pMemoryBufferToDecodeRow.uninitialize();
    m_pStimulationDecoderRow->uninitialize();
    this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationDecoderRow);
	
	op_pStimulationSetColumn.uninitialize();
	ip_pMemoryBufferToDecodeColumn.uninitialize();
    m_pStimulationDecoderColumn->uninitialize();
    this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationDecoderColumn);
	
	op_pEncodedMemoryBufferCommand.uninitialize();
	ip_pStimulationSetCommand.uninitialize();
    m_pStimulationEncoderCommand->uninitialize();
    this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationEncoderCommand);

	return true;
}

boolean CBoxAlgorithmTestAhmed::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}


boolean CBoxAlgorithmTestAhmed::process(void)
{
	//récupérer les entrées/sorties de la boîte
	IBoxIO* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();
//Row
	for(uint32 j=0; j<l_pDynamicBoxContext->getInputChunkCount(0); j++)
		{
			//obtenir les morceaux et décoder
			ip_pMemoryBufferToDecodeRow=l_pDynamicBoxContext->getInputChunk(0, j);
			m_pStimulationDecoderRow->process();
			
			if(m_pStimulationDecoderRow->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedHeader))
				{
					RowHeader=true;
				}
			if(m_pStimulationDecoderRow->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
				{ //chunk
					for(uint32 i=0; i<op_pStimulationSetRow->getStimulationCount();i++)
					{
						op_pStimulationSetRow->getStimulationIdentifier(i);
						op_pStimulationSetRow->getStimulationDate(i);
						op_pStimulationSetRow->getStimulationDuration(i);
						if(op_pStimulationSetRow->getStimulationIdentifier(i)>=OVTK_StimulationId_Label_01
						&& op_pStimulationSetRow->getStimulationIdentifier(i)<=OVTK_StimulationId_Label_06)
							{ 
								int32 x=op_pStimulationSetRow->getStimulationIdentifier(i) - OVTK_StimulationId_Label_00;
								std::cout<<"Ligne "<<x<<" detected"<<std::endl;
								queueRow.push_back(x);
							}

					}
			  
				}
			if(m_pStimulationDecoderRow->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedEnd))
				{
				}
				
			l_pDynamicBoxContext->markInputAsDeprecated(0, j);
		}
		// column
	for(uint32 j=0; j<l_pDynamicBoxContext->getInputChunkCount(1); j++)
		{
			//obtenir les morceaux et décoder
			ip_pMemoryBufferToDecodeColumn=l_pDynamicBoxContext->getInputChunk(1, j);
			m_pStimulationDecoderColumn->process();
			
			if(m_pStimulationDecoderColumn->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedHeader))
				{
					ColumnHeader=true;
				}
			if(m_pStimulationDecoderColumn->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
				{
					for(uint32 i=0; i<op_pStimulationSetColumn->getStimulationCount();i++)
					{
						op_pStimulationSetColumn->getStimulationIdentifier(i);
						op_pStimulationSetColumn->getStimulationDate(i);
						op_pStimulationSetColumn->getStimulationDuration(i);
						if(op_pStimulationSetRow->getStimulationIdentifier(i)>=OVTK_StimulationId_Label_07
						&& op_pStimulationSetRow->getStimulationIdentifier(i)<=OVTK_StimulationId_Label_12)
							{ 
								int32 y=op_pStimulationSetRow->getStimulationIdentifier(i) - OVTK_StimulationId_Label_00;
								std::cout<<"Column "<<y<<" detected"<<std::endl;
								queueColumn.push_back(y);
							}
					}
				}
			if(m_pStimulationDecoderColumn->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedEnd))
				{
				}
			
			l_pDynamicBoxContext->markInputAsDeprecated(1, j);
		}

 //Output
		op_pEncodedMemoryBufferCommand=l_pDynamicBoxContext->getOutputChunk(0);
		//
		if(RowHeader && ColumnHeader)
		  {
			uint64 l_ui64StartTime=0;
			uint64 l_ui64EndTime=0;
			m_pStimulationEncoderCommand->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeHeader);
			l_pDynamicBoxContext->markOutputAsReadyToSend(0, l_ui64StartTime, l_ui64EndTime);
			RowHeader=false;
			ColumnHeader=false;
			HeaderSent=true;
		  }
		
		if(HeaderSent)
		  {
			//traiter les données
			if(queueRow.size()>0 && queueColumn.size()>0)
			{
				if(FirstLetterOK && queueRow.front()==2 && queueColumn.front()==12)//lettres CL
					{
						CommandDetected=true;
						FirstLetterOK=false;
					}
				if(queueRow.front()==1 && queueColumn.front()==9)
					{
						FirstLetterOK=true;
					}
				queueRow.pop_front();
				queueColumn.pop_front();
			}
			//gérer la commande
			if(CommandDetected)
				{
					//ajouter la stimulation de commande
					ip_pStimulationSetCommand->appendStimulation (OVTK_StimulationId_ExperimentStop, 0, 0);		
					//envoyer les stimulations	
					uint64 l_ui64StartTime=0;
					uint64 l_ui64EndTime=0;
					m_pStimulationEncoderCommand->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeBuffer);
					l_pDynamicBoxContext->markOutputAsReadyToSend(0, l_ui64StartTime, l_ui64EndTime);
				}
		  }

	return true;
}
