#include "ovpCBoxAlgorithmSequenceGenerateur.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Stimulation;

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
};

boolean CBoxAlgorithmSequenceGenerateur::initialize(void)
{	
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();

	m_pStimulationEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamEncoder));
	m_pStimulationEncoder->initialize();

	m_pStimulationDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
	m_pStimulationDecoder->initialize();

	iop_pStimulationSet.initialize(m_pStimulationDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
	m_pStimulationEncoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_InputParameterId_StimulationSet)->setReferenceTarget(m_pStimulationDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------
	
	CString strWord	=_AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 0);
	CString strSeq	=_AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 1);
	
	m_fileWords.open (strWord.toASCIIString(), std::fstream::in);
	if (!m_fileWords.is_open())
	  {
	   std::cout << "Error opening word file "<<strWord<<std::endl;
	   return false;
	  }
	//
	m_fileSequence.open (strSeq.toASCIIString(), std::fstream::in);
	if (!m_fileSequence.is_open())
	  {
	   std::cout << "Error opening sequence file "<<strSeq<<std::endl;
	   return false;
	  }

	getIndexCharTab();
	getCharList();
	m_ui32IndexCurrentChar=-1;
	
	return true;
}

boolean CBoxAlgorithmSequenceGenerateur::uninitialize(void)
{
	iop_pStimulationSet.uninitialize();

	m_pStimulationDecoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationDecoder);

	m_pStimulationEncoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationEncoder);

	if(m_fileSequence.is_open()) {m_fileSequence.close();}
	if(m_fileWords.is_open()) {m_fileWords.close();}
	return true;
}

boolean CBoxAlgorithmSequenceGenerateur::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CBoxAlgorithmSequenceGenerateur::process(void)
{	
	// std::cout<<"start Process"<<std::endl;

	// IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	m_cStimulationToWrite.clear();
	for(uint32 i=0; i<l_rDynamicBoxContext.getInputChunkCount(0); i++)
	{
		TParameterHandler < const IMemoryBuffer* > ip_pMemoryBufferToDecode(m_pStimulationDecoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
		TParameterHandler < IMemoryBuffer* > op_pMemoryBuffer(m_pStimulationEncoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
		ip_pMemoryBufferToDecode=l_rDynamicBoxContext.getInputChunk(0, i);
		op_pMemoryBuffer=l_rDynamicBoxContext.getOutputChunk(0);
		m_pStimulationDecoder->process();

		uint64 l_ui64StartTime=l_rDynamicBoxContext.getInputChunkStartTime(0, i);
		uint64 l_ui64EndTime=l_rDynamicBoxContext.getInputChunkEndTime(0, i);
					
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedHeader))
		  {
			m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeHeader);
			l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_ui64StartTime, l_ui64EndTime);
		  }
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
		  {
			//enregistrer une cible par pause entre trial
			while(iop_pStimulationSet->getStimulationCount()!=0)
			  {
				if(iop_pStimulationSet->getStimulationIdentifier(0) == OVTK_StimulationId_RestStart)
				  {
				  	// std::cout<<"get next line of sequence"<<std::endl;
					if(getNextTarget())
					  {
						uint64 	date=iop_pStimulationSet->getStimulationDate(0),
								duration=iop_pStimulationSet->getStimulationDuration(0);
						
						//coller les deux indices de ligne/colonne de la cible
						m_cStimulationToWrite.appendStimulation (OVTK_StimulationId_Label_00+m_ui32Row, date, duration);
						m_cStimulationToWrite.appendStimulation (OVTK_StimulationId_Label_06+m_ui32Column, date, duration);
					  }
				  }
				if(iop_pStimulationSet->getStimulationIdentifier(0) == OVTK_StimulationId_ExperimentStop)
				  {
					uint64 	date=iop_pStimulationSet->getStimulationDate(0),
							duration=iop_pStimulationSet->getStimulationDuration(0);
					m_cStimulationToWrite.appendStimulation (OVTK_StimulationId_Train, date, duration);
				  }
				iop_pStimulationSet->removeStimulation(0);
			  }

			// réécrire les stimulations sur le support handler
			for(uint32 h=0; h<m_cStimulationToWrite.getStimulationCount(); h++)
			  {
				iop_pStimulationSet->appendStimulation(m_cStimulationToWrite.getStimulationIdentifier(h),m_cStimulationToWrite.getStimulationDate(h),m_cStimulationToWrite.getStimulationDuration(h));
			  }
			  
			  
			//émettre
			m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeBuffer);
			l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_ui64StartTime, l_ui64EndTime);
		  }
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedEnd))
		  {
		  }
		  
		l_rDynamicBoxContext.markInputAsDeprecated(0, i);
	}

	return true;
}


void CBoxAlgorithmSequenceGenerateur::getIndexCharTab()
{
	std::string text; 
	while(!m_fileSequence.eof()) 
	  {
		//extraire une ligne
		getline(m_fileSequence,text); 
		//std::cout<<"get line : "<<text<<"."<<std::endl;
		//extraire les trois segments
		int32 idxRow=atoi(text.c_str());
		if(!idxRow) {std::cout<<"bad first index"<<std::endl; break;} //attention, ne lis pas les 0...
		//
		int32 idx=text.find(" ", 0);
		if(idx==std::string::npos) {std::cout<<"any second index"<<std::endl; break;}
		int32 idxColumn=atoi(text.c_str()+idx+1);
		if(!idxColumn) {std::cout<<"bad second index"<<std::endl; break;} //attention, ne lis pas les 0...
		//
		idx=text.find(" ", idx+1);
		if(idx==std::string::npos) {std::cout<<"any second index"<<std::endl; break;}
		int32 idxFin=text.find(" ", idx+1);
		if(idxFin==std::string::npos) {idxFin=text.length()+1;}
		std::string symbol=text.substr(idx+1, idxFin-idx-1);
		if(symbol.empty()) {std::cout<<"no symbol"<<std::endl; break;}
		//
		//std::cout<<"indexCharTab extract : "<<idxRow<<","<<idxColumn<<". -> "<<symbol.c_str()<<"."<<std::endl;
		//les rentrer dans les map
		uint32 idxMap=m_mapIndexTab.size();
		m_mapIndexTab[idxMap]=std::pair<uint32,uint32> (idxRow,idxColumn);
		m_mapCharTab[symbol]=idxMap;
	  }
	
	/*std::map<OpenViBE::uint32, std::pair<OpenViBE::uint32,OpenViBE::uint32> >::iterator it	;
	for (it=m_mapIndexTab.begin() ; it != m_mapIndexTab.end(); it++ )
      {std::cout << (*it).first << " => " << (*it).second.first<<","<<(*it).second.second << std::endl;}

	 std::map<std::string, OpenViBE::uint32>::iterator it2;
	for ( it2=m_mapCharTab.begin() ; it2 != m_mapCharTab.end(); it2++ )
      {std::cout << (*it2).first.c_str() << " => " << (*it2).second<< std::endl;}*/
	  
	std::cout<<"Taille des map : _INDEX = "<<m_mapIndexTab.size()<<" _CHAR = "<<m_mapCharTab.size()<< std::endl;
}

void CBoxAlgorithmSequenceGenerateur::getCharList()
{
	std::string text; 
	while(!m_fileWords.eof()) 
	  {
		//extraire une ligne
		getline(m_fileWords,text); 
		//std::cout<<"get line : "<<text<<"."<<std::endl;
		//remplir le stockeur des symboles
		m_vectChar.push_back(text);
	  }
	std::cout<<"Taille de la liste des CHAR : "<<m_vectChar.size()<< std::endl;
}

boolean CBoxAlgorithmSequenceGenerateur::getNextTarget()
{
	//identification du symbol suivant reconnu
	uint32 idxTest=m_ui32IndexCurrentChar+1;
	if(m_vectChar.empty() || m_vectChar.size()<=idxTest) {std::cout<<"end of vector of Symbol"<<std::endl; return false;}
	std::string symbol=m_vectChar[idxTest];
	while(m_mapCharTab.find(symbol)==m_mapCharTab.end())
	  {
		std::cout<<symbol.c_str()<<" not found in map"<<std::endl;
		idxTest++;
		if(m_vectChar.size()<=idxTest) {std::cout<<"end of vector of Symbol in while"<<std::endl; return false;}
		symbol=m_vectChar[idxTest];
	  }
	m_ui32IndexCurrentChar=idxTest;
	//identification de la paire associée
	uint32 symbolIdx=m_mapCharTab[symbol];
	if(m_mapIndexTab.find(symbolIdx)==m_mapIndexTab.end()) {std::cout<<"symbol without row/col : "<<symbolIdx<<std::endl; return false;}
	//remplissage des marqueurs de cible
	m_ui32Row=m_mapIndexTab[symbolIdx].first;
	m_ui32Column=m_mapIndexTab[symbolIdx].second;
	
	std::cout<<"next symbol = "<<symbol.c_str()<<" <- "<<m_ui32Row<<","<<m_ui32Column<<std::endl;
	
	return true;
}


