#include "ovpCBoxAlgorithmRecalleurP300PortParallele.h"

#include <iostream>
#include <string>
#include <sstream>

#define fullLOG 0

//#define OffsetChunkEndTime uint64(0.6*(1LL<<32))

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
		operator OpenViBE::boolean (void) { return m_rConfigurationManager.expandAsBoolean(m_sSettingValue); }
		operator const CString (void) { return m_sSettingValue; }
	protected:
		IConfigurationManager& m_rConfigurationManager;
		CString m_sSettingValue;
	};
};

OpenViBE::boolean CBoxAlgorithmRecalleurP300::initialize(void)
{
	//> init Trigger input
	m_pStimulationDecoderTrigger=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
    m_pStimulationDecoderTrigger->initialize();
    ip_pMemoryBufferToDecodeTrigger.initialize(m_pStimulationDecoderTrigger->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
    op_pStimulationSetTrigger.initialize(m_pStimulationDecoderTrigger->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
    
	//> init stimlation input
	m_pStimulationDecoderRCIndex=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
    m_pStimulationDecoderRCIndex->initialize();
    ip_pMemoryBufferToDecodeRCIndex.initialize(m_pStimulationDecoderRCIndex->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
	op_pStimulationSetRowColumnIndex.initialize(m_pStimulationDecoderRCIndex->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));

	//> init signal output
	m_pStimulationEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamEncoder));
    m_pStimulationEncoder->initialize();
    ip_pStimulationsToEncode.initialize(m_pStimulationEncoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_InputParameterId_StimulationSet));
    op_pEncodedMemoryBuffer.initialize(m_pStimulationEncoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_OutputParameterId_EncodedMemoryBuffer));

	//> get user stimulation type
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	m_ui64StimulationTriggerStart=this->getTypeManager().getEnumerationEntryValueFromName(OV_TypeId_Stimulation, _AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 0));
	m_ui64StimulationToTrigStart=this->getTypeManager().getEnumerationEntryValueFromName(OV_TypeId_Stimulation, _AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 1));
	m_ui64StimulationTriggerStop=this->getTypeManager().getEnumerationEntryValueFromName(OV_TypeId_Stimulation, _AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 2));
	m_ui64StimulationToTrigStop=this->getTypeManager().getEnumerationEntryValueFromName(OV_TypeId_Stimulation, _AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 3));

	//m_ui64Stimulation     =this->getTypeManager().getEnumerationEntryValueFromName(OV_TypeId_Stimulation, _AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 0));
	//m_StimulationLabel = _AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 0);

	m_ui64lastTriggerTime=0;
	m_ui64LastChunkStartTimeTrigger=0;
	m_ui64LastChunkEndTimeTrigger=0;

	uint64 l_ui64Id=l_rStaticBoxContext.getIdentifier().toUInteger();
	std::stringstream sstr;
	sstr<<"LogGipsa/P300Recalleur_LOG_"<<l_ui64Id<<".txt";
	pFile = fopen (sstr.str().c_str() , "w");
	if (pFile == NULL) {perror ("Error opening file"); return false;}

	return true;
}

OpenViBE::boolean CBoxAlgorithmRecalleurP300::uninitialize(void)
{
	// uninit trigger input
	m_pStimulationDecoderTrigger->uninitialize();
    ip_pMemoryBufferToDecodeTrigger.uninitialize();
	op_pStimulationSetTrigger.uninitialize();
    this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationDecoderTrigger);
	
	// uninit stimulation input
	m_pStimulationDecoderRCIndex->uninitialize();
    ip_pMemoryBufferToDecodeRCIndex.uninitialize();
	op_pStimulationSetRowColumnIndex.uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationDecoderRCIndex);

	// uninit stimulation output
	m_pStimulationEncoder->uninitialize();
	ip_pStimulationsToEncode.uninitialize();
	op_pEncodedMemoryBuffer.uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationEncoder);

	//clear memory
	m_oConcernedStimP300.clear();
	m_oStimP300.clear();
	m_oStimTimeTrigger.clear();

	if (pFile != NULL) {fclose (pFile);}

	return true;
}

OpenViBE::boolean CBoxAlgorithmRecalleurP300::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}

OpenViBE::boolean CBoxAlgorithmRecalleurP300::process(void)
{
	//> Get dynamic box context
	IBoxIO* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();

	//> Build time consistant package of Stimulation P300
	m_oStimP300.clear();
	for(uint32 j=0; j<l_pDynamicBoxContext->getInputChunkCount(1); j++)
	  {
		ip_pMemoryBufferToDecodeRCIndex=l_pDynamicBoxContext->getInputChunk(1, j);
		m_pStimulationDecoderRCIndex->process();
		if(m_pStimulationDecoderRCIndex->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
		  {
			uint64 l_ui64ChunkStartTime=l_pDynamicBoxContext->getInputChunkStartTime(1, j);
			uint64 l_ui64ChunkEndTime=l_pDynamicBoxContext->getInputChunkEndTime(1, j);
			//Semer les graines
			for(uint32 stim=0; stim<op_pStimulationSetRowColumnIndex->getStimulationCount(); stim++)
			  {
				if(op_pStimulationSetRowColumnIndex->getStimulationIdentifier(stim)==m_ui64StimulationToTrigStart||
					op_pStimulationSetRowColumnIndex->getStimulationIdentifier(stim)==m_ui64StimulationToTrigStop)
				  {
					if(AsNotTime(op_pStimulationSetRowColumnIndex->getStimulationDate(stim)))
					    {
							StructureStimulationP300AtTime *ssp300Time=new StructureStimulationP300AtTime;
							ssp300Time->stimSet.appendStimulation(op_pStimulationSetRowColumnIndex->getStimulationIdentifier(stim),
												op_pStimulationSetRowColumnIndex->getStimulationDate(stim),
												op_pStimulationSetRowColumnIndex->getStimulationDuration(stim));
							ssp300Time->chunkStartTime=l_ui64ChunkStartTime;
							ssp300Time->chunkEndTime=l_ui64ChunkEndTime;
							m_oStimP300.push_back(ssp300Time);

							//
							std::stringstream sstr;
							sstr<<"seed : "<<op_pStimulationSetRowColumnIndex->getStimulationIdentifier(stim)<<" "
											<<op_pStimulationSetRowColumnIndex->getStimulationDate(stim)<<"\n";
							fwrite (sstr.str().c_str() , 1 , sstr.str().size() , pFile );
							//

							//retirer la stim pour ne pas la relire ensuite
							op_pStimulationSetRowColumnIndex->removeStimulation(stim);
							stim--;

						}
				  }
			  }
			//Faire croitre les pousses
			for(uint32 stim=0; !m_oStimP300.empty() && stim<op_pStimulationSetRowColumnIndex->getStimulationCount(); stim++)
			  {
				if(!aggregateStim(op_pStimulationSetRowColumnIndex->getStimulationIdentifier(stim),
								op_pStimulationSetRowColumnIndex->getStimulationDate(stim),
								op_pStimulationSetRowColumnIndex->getStimulationDuration(stim)))
				  {
						//garder de côté les stimulations non concerné pour conserver la chronologie
						StructureStimulationP300AtTime *ssOtherTime=new StructureStimulationP300AtTime;
						ssOtherTime->stimSet.appendStimulation(op_pStimulationSetRowColumnIndex->getStimulationIdentifier(stim),
											op_pStimulationSetRowColumnIndex->getStimulationDate(stim),
											op_pStimulationSetRowColumnIndex->getStimulationDuration(stim));
						ssOtherTime->chunkStartTime=l_ui64ChunkStartTime;
						ssOtherTime->chunkEndTime=l_ui64ChunkEndTime;
						m_oStimOthers.push_back(ssOtherTime);
				  }
			  }
		  }

		l_pDynamicBoxContext->markInputAsDeprecated(1,j);
	  }

	//> Eliminer les paquets sans les marqueurs, stocker sur le long terme les paquets restants
	for(uint32 i=0; i<m_oStimP300.size(); i++)
	  {
		boolean AsMarqueur=false;
		for(uint32 j=0; j<m_oStimP300[i]->stimSet.getStimulationCount() ; j++)
		  {
			if(m_oStimP300[i]->stimSet.getStimulationIdentifier(j)==m_ui64StimulationToTrigStart ||
				m_oStimP300[i]->stimSet.getStimulationIdentifier(j)==m_ui64StimulationToTrigStop)
			  {
				AsMarqueur=true;
				break;
			  }
		  }
		//
		if(!AsMarqueur)
		  {
			uint64 l_ui64StartTime=m_oStimP300[i]->chunkStartTime;
			uint64 l_ui64EndTime=m_oStimP300[i]->chunkEndTime;
			ip_pStimulationsToEncode=&m_oStimP300[i]->stimSet;
			op_pEncodedMemoryBuffer=l_pDynamicBoxContext->getOutputChunk(0);
			m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeBuffer);
			l_pDynamicBoxContext->markOutputAsReadyToSend(0,l_ui64StartTime ,l_ui64EndTime );
			//
			delete m_oStimP300[i];
		  }
		else
		  {
			m_oConcernedStimP300.push_back(m_oStimP300[i]);
		  }
	  }

	//> build time consistant Trigger Stimulation
	for(uint32 j=0; j<l_pDynamicBoxContext->getInputChunkCount(0); j++)
	  {
		ip_pMemoryBufferToDecodeTrigger=l_pDynamicBoxContext->getInputChunk(0, j);
		m_pStimulationDecoderTrigger->process();
		if(m_pStimulationDecoderTrigger->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
		  {
			uint64 l_ui64ChunkStartTime=l_pDynamicBoxContext->getInputChunkStartTime(0, j);
			uint64 l_ui64ChunkEndTime=l_pDynamicBoxContext->getInputChunkEndTime(0, j);
			m_ui64LastChunkStartTimeTrigger=l_ui64ChunkStartTime;
			m_ui64LastChunkEndTimeTrigger=l_ui64ChunkEndTime;
			//
			for(uint32 stim=0; stim<op_pStimulationSetTrigger->getStimulationCount(); stim++)
			  {
				if(op_pStimulationSetTrigger->getStimulationIdentifier(stim)==m_ui64StimulationTriggerStart)
				  {
					StructureTimeTrigger stt;
					stt.time=op_pStimulationSetTrigger->getStimulationDate(stim);
					stt.itIsAStart=true;
					stt.chunkStartTime=l_ui64ChunkStartTime;
					stt.chunkEndTime=l_ui64ChunkEndTime;
					m_oStimTimeTrigger.push_back(stt);
				  }
				else if(op_pStimulationSetTrigger->getStimulationIdentifier(stim)==m_ui64StimulationTriggerStop)
				  {
					StructureTimeTrigger stt;
					stt.time=op_pStimulationSetTrigger->getStimulationDate(stim);
					stt.itIsAStart=false;
					stt.chunkStartTime=l_ui64ChunkStartTime;
					stt.chunkEndTime=l_ui64ChunkEndTime;
					m_oStimTimeTrigger.push_back(stt);
				  }
			  }
		  }

		l_pDynamicBoxContext->markInputAsDeprecated(0,j);
	  }

	//std::cout<<"State : "<<m_oStimTimeTrigger.size()<<" | "<<m_oConcernedStimP300.size()<<std::endl;

	//> pour chaque TimeTrigger find Stim P300 correspondante, puis changer les temps et envoyer et mettre à jour les listes
	//ne pas oublier les stimulations autres dont le temps est passé
	boolean sendAny=true;
	for(uint32 k=0; k<m_oStimTimeTrigger.size() && k<m_oConcernedStimP300.size(); k++)
	  {
		int32 l_idx=findStimP300IndexWith(m_oStimTimeTrigger[k].itIsAStart);
		if(l_idx<0) {continue;}
		FindAndSendOthersBeforeThisTrigger(l_pDynamicBoxContext,m_oStimTimeTrigger[k],l_idx);
		ChangeTimeAtIdx(m_oStimTimeTrigger[k],l_idx);
		//
		uint64 l_ui64StartTime=m_oConcernedStimP300[l_idx]->chunkStartTime;
		uint64 l_ui64EndTime=m_oConcernedStimP300[l_idx]->chunkEndTime;
		ip_pStimulationsToEncode=&m_oConcernedStimP300[l_idx]->stimSet;
		op_pEncodedMemoryBuffer=l_pDynamicBoxContext->getOutputChunk(0);
		m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeBuffer);
		l_pDynamicBoxContext->markOutputAsReadyToSend(0,l_ui64StartTime ,l_ui64EndTime );
		//
		sendAny=false;
		//
		m_ui64lastTriggerTime=m_oStimTimeTrigger[k].time;
		delete m_oConcernedStimP300[l_idx];
		m_oConcernedStimP300.erase(m_oConcernedStimP300.begin()+l_idx);
		m_oStimTimeTrigger.erase(m_oStimTimeTrigger.begin()+k);
		k--;

	  }
	if(m_oConcernedStimP300.empty() && m_oStimTimeTrigger.empty())
	  {
	    sendAllOthers(l_pDynamicBoxContext);
		flushAllOthers();
	  }
	if(sendAny && m_oStimTimeTrigger.empty())
	{
		uint64 l_ui64StartTime=m_ui64LastChunkStartTimeTrigger;
		uint64 l_ui64EndTime=m_ui64LastChunkEndTimeTrigger;
		OpenViBE::CStimulationSet stimSet;
		stimSet.appendStimulation(0,l_ui64StartTime+(l_ui64EndTime-l_ui64StartTime)/2,0);
		ip_pStimulationsToEncode=&stimSet;
		op_pEncodedMemoryBuffer=l_pDynamicBoxContext->getOutputChunk(0);
		m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeBuffer);
		l_pDynamicBoxContext->markOutputAsReadyToSend(0,l_ui64StartTime ,l_ui64EndTime );

		#if fullLOG
		std::stringstream sstr;
		sstr<<"Send an empty StimSet :"<<l_ui64StartTime<<"-"<<l_ui64EndTime<<"\t";
		fwrite (sstr.str().c_str() , 1 , sstr.str().size() , pFile );
		#endif
	}

	//std::cout<<"end process"<<std::endl;
	return true;
}// End Process

boolean CBoxAlgorithmRecalleurP300::AsNotTime(uint64 time)
{
  for(uint32 i=0; i<m_oStimP300.size(); i++)
   {
	for(uint32 j=0; j<m_oStimP300[i]->stimSet.getStimulationCount() ; j++)
	  {
		if(m_oStimP300[i]->stimSet.getStimulationDate(j)==time)
		  {return false;}
	  }
   }
 return true;
}

OpenViBE::boolean CBoxAlgorithmRecalleurP300::aggregateStim(OpenViBE::uint64 id, OpenViBE::uint64 date, OpenViBE::uint64 duration)
{
 for(uint32 i=0; i<m_oStimP300.size(); i++)
   {
	if(m_oStimP300[i]->stimSet.getStimulationDate(0)==date)
	  {
		m_oStimP300[i]->stimSet.appendStimulation(id,date,duration);
		return true;
	  }
   }
 return false;
}

OpenViBE::int32 CBoxAlgorithmRecalleurP300::findStimP300IndexWith(OpenViBE::boolean Bstart)
{
	for(uint32 i=0; i<m_oConcernedStimP300.size(); i++)
	  {
		if((Bstart && m_oConcernedStimP300[i]->stimSet.getStimulationIdentifier(0)==m_ui64StimulationToTrigStart) ||
			(!Bstart && m_oConcernedStimP300[i]->stimSet.getStimulationIdentifier(0)==m_ui64StimulationToTrigStop))
			{return int32(i);}
	  }
	return -1;
}

void CBoxAlgorithmRecalleurP300::ChangeTimeAtIdx(const StructureTimeTrigger& stt,OpenViBE::int32 idx)
{
	if(idx<0 || idx>=(int)m_oConcernedStimP300.size()) {return;}
	//
	uint64 chunkTimeStim=m_oConcernedStimP300[idx]->chunkStartTime;
	int64 chunkDiffTime=(stt.chunkStartTime>=chunkTimeStim)?((1000*(stt.chunkStartTime-chunkTimeStim))>>32):-((1000*(chunkTimeStim-stt.chunkStartTime))>>32);

	m_oConcernedStimP300[idx]->chunkStartTime=stt.chunkStartTime;
	m_oConcernedStimP300[idx]->chunkEndTime=stt.chunkEndTime;
	//
	uint64 timeStim;
	int64 diffTime;
	for(uint32 i=0; i<m_oConcernedStimP300[idx]->stimSet.getStimulationCount(); i++)
	  {
		///marque la différence de temps entre Trigger et Stim dans le fichier
		timeStim=m_oConcernedStimP300[idx]->stimSet.getStimulationDate(i);
		diffTime=(stt.time>=timeStim)?((1000*(stt.time-timeStim))>>32):-((1000*(timeStim-stt.time))>>32);

		///change le temps
		m_oConcernedStimP300[idx]->stimSet.setStimulationDate(i,stt.time);
	  }
	//save des temps
	if(m_oConcernedStimP300[idx]->stimSet.getStimulationCount()!=0)
	  {
		std::stringstream sstr;
		#if fullLOG
		sstr<<"\n";
		#endif
		sstr<<stt.time<<" "<<timeStim<<" :=: "<<diffTime<<" . In chunkStartTime : "
			<<stt.chunkStartTime<<" "<<chunkTimeStim<<" :=: "<<chunkDiffTime<<"\n";
		for(uint32 i=0; i<m_oConcernedStimP300[idx]->stimSet.getStimulationCount(); i++)
		  {sstr<<m_oConcernedStimP300[idx]->stimSet.getStimulationIdentifier(i)<<" ";}
		sstr<<"\n";
		fwrite (sstr.str().c_str() , 1 , sstr.str().size() , pFile );
	  }

}

void CBoxAlgorithmRecalleurP300::FindAndSendOthersBeforeThisTrigger(IBoxIO* iobox, const StructureTimeTrigger& stt,OpenViBE::int32 idx)
{
	//find Stim before StimP300
	uint64 timeStimP300=m_oConcernedStimP300[idx]->stimSet.getStimulationDate(0);

	int32 l_i32IdxOther=findOthersBefore(timeStimP300);
	while(l_i32IdxOther>=0)
	  {
		//manage time of this Stimulation
		uint64 timePrecedentStimP300=m_oStimOthers[l_i32IdxOther]->stimSet.getStimulationDate(0);
		uint64 newTimeForOther=stt.time>0 ? stt.time-(timeStimP300-timePrecedentStimP300) : 0;
		//
		std::stringstream sstr;
		sstr<<"intercal : "<<timeStimP300<<" | "<<stt.time<<" | "<<timePrecedentStimP300<<" => "<<newTimeForOther<<"\n";
		fwrite (sstr.str().c_str() , 1 , sstr.str().size() , pFile );
		//
		setTimeOthers(newTimeForOther,l_i32IdxOther);
		m_oStimOthers[l_i32IdxOther]->chunkStartTime=stt.chunkStartTime-(timeStimP300-timePrecedentStimP300);
		m_oStimOthers[l_i32IdxOther]->chunkEndTime=stt.chunkEndTime-(timeStimP300-timePrecedentStimP300);
		//send this stimulation
		sendStimulationOthers(iobox,l_i32IdxOther);
		//kill this stimulation
		delete m_oStimOthers[l_i32IdxOther];
		m_oStimOthers.erase(m_oStimOthers.begin()+l_i32IdxOther);

		//update while
		l_i32IdxOther=findOthersBefore(timeStimP300);
	  }

}

int32 CBoxAlgorithmRecalleurP300::findOthersBefore(OpenViBE::uint64 date)
{
	for(uint32 i=0; i<m_oStimOthers.size(); i++)
	  {
		if( m_oStimOthers[i]->stimSet.getStimulationDate(0)<date )
			{return int32(i);}
	  }
	return -1;
}

void CBoxAlgorithmRecalleurP300::setTimeOthers(OpenViBE::uint64 date, OpenViBE::int32 idx)
{
	for(uint32 i=0; i<m_oStimOthers[idx]->stimSet.getStimulationCount(); i++)
	  {
		m_oStimOthers[idx]->stimSet.setStimulationDate(i,date);
	  }
}

void CBoxAlgorithmRecalleurP300::sendStimulationOthers(IBoxIO* iobox, OpenViBE::int32 idx)
{
	uint64 l_ui64StartTime=m_oStimOthers[idx]->chunkStartTime;
	uint64 l_ui64EndTime=m_oStimOthers[idx]->chunkEndTime;
	ip_pStimulationsToEncode=&m_oStimOthers[idx]->stimSet;
	op_pEncodedMemoryBuffer=iobox->getOutputChunk(0);
	m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeBuffer);
	iobox->markOutputAsReadyToSend(0,l_ui64StartTime ,l_ui64EndTime );
}

void CBoxAlgorithmRecalleurP300::sendAllOthers(IBoxIO* iobox)
{
	if(m_oStimOthers.size()>0)
	  {
		std::stringstream sstr;
		sstr<<"send Others \n";
		fwrite (sstr.str().c_str() , 1 , sstr.str().size() , pFile );
	  }

	//

	for(uint32 idx=0; idx<m_oStimOthers.size();idx++)
	  {
		for(uint32 i=0; i<m_oStimOthers[idx]->stimSet.getStimulationCount(); i++)
		  {
			m_oStimOthers[idx]->stimSet.setStimulationDate(i,m_ui64lastTriggerTime);
		  }
		sendStimulationOthers(iobox,idx);
	  }
}

void CBoxAlgorithmRecalleurP300::flushAllOthers()
{
	for(uint32 idx=0; idx<m_oStimOthers.size();idx++)
	  {delete m_oStimOthers[idx];}
	m_oStimOthers.clear();
}


