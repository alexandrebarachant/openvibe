#include "ovpCBoxAlgorithmSequenceTransducteur.h"

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

boolean CBoxAlgorithmSequenceTransducteur::initialize(void)
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

	getSequenceHeader();
	
	return true;
}

boolean CBoxAlgorithmSequenceTransducteur::uninitialize(void)
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

boolean CBoxAlgorithmSequenceTransducteur::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CBoxAlgorithmSequenceTransducteur::process(void)
{	
	// std::cout<<"start Process"<<std::endl;

	// IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

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
			// if(iop_pStimulationSet->getStimulationCount()!=0) {std::cout<<"receive something "<<iop_pStimulationSet->getStimulationCount()<<std::endl;}
			for(uint32 j=0; j<iop_pStimulationSet->getStimulationCount(); j++)
			  {
				if(	m_uiMode<=1 && iop_pStimulationSet->getStimulationIdentifier(j) == OVTK_StimulationId_SegmentStart ||
					m_uiMode==2 && iop_pStimulationSet->getStimulationIdentifier(j) == OVTK_StimulationId_TrialStart)
				  {
				  	// std::cout<<"get next line of sequence"<<std::endl;
					getNextLine();
				  }
				if(iop_pStimulationSet->getStimulationIdentifier(j) == OVTK_StimulationId_VisualStimulationStart)
					{
						// std::cout<<"it is a Visual Start"<<std::endl;
						uint32 value=(m_queValue.empty()) ? -1: m_queValue.front();
						// std::cout<<"start at : "<<value<<". for a size of : "<<m_queValue.size()<<std::endl;
						m_queValue.pop();
						uint64 	id=getStimulationId(value),
								date=iop_pStimulationSet->getStimulationDate(j),
								duration=iop_pStimulationSet->getStimulationDuration(j);
						/*uint64 res=(j+1<iop_pStimulationSet->getStimulationCount())?
									iop_pStimulationSet->insertStimulation(j+1, id, date, duration):
									iop_pStimulationSet->appendStimulation(id, date, duration);*/
						uint64 res=iop_pStimulationSet->insertStimulation(j, id, date, duration);
						j++;
						// std::cout<<"res= "<<res<<std::endl;
					}
			  }
			// if(iop_pStimulationSet->getStimulationCount()!=0) {std::cout<<"send something "<<iop_pStimulationSet->getStimulationCount()<<std::endl;}
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


void CBoxAlgorithmSequenceTransducteur::getSequenceHeader()
{
	std::string text; 
	uint32 nb=0;
	if(m_fileSequence.eof()) {std::cout<<"reach end of file"<<std::endl; return;}
	getline(m_fileSequence,text);
	std::cout<<"mode index : "<<text<<"."<<std::endl;
	m_uiMode=uint32(atoi(text.c_str()));
	
	if(m_fileSequence.eof()) {std::cout<<"reach end of file"<<std::endl; return;}
	getline(m_fileSequence,text);
	std::cout<<"number of row : "<<text<<"."<<std::endl;
	m_uiRow=uint32(atoi(text.c_str()));	
	if(m_fileSequence.eof()) {std::cout<<"reach end of file"<<std::endl; return;}
	getline(m_fileSequence,text);
	std::cout<<"number of column : "<<text<<"."<<std::endl;
	m_uiColumn=uint32(atoi(text.c_str()));
	
	if(m_uiMode==1)
	  {
		if(m_fileSequence.eof()) {std::cout<<"reach end of file"<<std::endl; return;}
		getline(m_fileSequence,text);
		std::cout<<"number of repetition : "<<text<<"."<<std::endl;
		m_uiRep=uint32(atoi(text.c_str()));
	  }
}

void CBoxAlgorithmSequenceTransducteur::getNextLine()
{
	if(m_uiMode==0) {generate_random_sequence(); return;}
	
	if(m_fileSequence.eof()) {std::cout<<"reach end of file"<<std::endl; return;}
	std::string text;
	getline(m_fileSequence,text);
	
	// m_vectValue.clear();
	while(!m_queValue.empty()) {m_queValue.pop();}
	int32 idx=0;
	do
	  {
		// std::cout<<"string piece = "<<(text.c_str()+idx)<<std::endl;
		// std::cout<<"number : "<<atoi(text.c_str()+idx)<<std::endl;
		if(!atoi(text.c_str()+idx)) {break;} //attention, ne lis pas les 0...
		// m_vectValue.push_back(atoi(text.c_str()+idx));
		m_queValue.push(atoi(text.c_str()+idx));
		idx=text.find(" ", idx)+1;
		// std::cout<<"idx : "<<idx<<std::endl;
	  }while(idx!=std::string::npos+1);
}

uint64 CBoxAlgorithmSequenceTransducteur::getStimulationId(uint32 idx)
{
	// if(idx<m_uiRow+1) {return OVTK_StimulationId_Label_01+idx-1;}
	if(idx<m_uiRow+m_uiColumn+1) {return OVTK_StimulationId_Label_01+idx-1;}
	return 0;
}

void CBoxAlgorithmSequenceTransducteur::generate_random_sequence(void)
{
	uint32 i,j;
	while(!m_queValue.empty()) {m_queValue.pop();}

	std::vector < uint32 > l_vRC;
	for(i=0; i<m_uiRow+m_uiColumn; i++)
	{
		l_vRC.push_back(i+1);
	}
	for(i=0; i<m_uiRow+m_uiColumn; i++)
	{
		j=rand()%l_vRC.size();
		m_queValue.push(l_vRC[j]);
		l_vRC.erase(l_vRC.begin()+j);
	}
	
	// std::cout<<"generated sequence size = "<<m_queValue.size()<<std::endl;
}