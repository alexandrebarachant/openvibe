#include "ovpCBoxAlgorithmCVKserver.h"
#include <cstdlib>
#include <iostream>
#include <string>
// #include <strstream>
#include <sstream>
#include <math.h>
#include <cstdio>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Acquisition;

#define Debug 1
#define MAXSIZE 65536 //4096
char reception_buffer_RoBIKCVK[MAXSIZE] = "";

uint64 CBoxAlgorithmCVKServer::getClockFrequency(void)
{
	return 64LL<<32;
}

OpenViBE::boolean CBoxAlgorithmCVKServer::initialize(void)
{
	m_pStimulationDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
	m_pStimulationDecoder->initialize();
	ip_pStimulationMemoryBuffer.initialize(m_pStimulationDecoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
	op_pStimulationSet.initialize(m_pStimulationDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));

	//ip_ui64BufferDuration.initialize(m_pAcquisitionStreamEncoder->getInputParameter(OVP_GD_Algorithm_AcquisitionStreamEncoder_InputParameterId_BufferDuration));
	//ip_pExperimentInformationMemoryBuffer.initialize(m_pAcquisitionStreamEncoder->getInputParameter(OVP_GD_Algorithm_AcquisitionStreamEncoder_InputParameterId_ExperimentInformationStream));
	//ip_pSignalMemoryBuffer.initialize(m_pAcquisitionStreamEncoder->getInputParameter(OVP_GD_Algorithm_AcquisitionStreamEncoder_InputParameterId_SignalStream));
	//ip_pChannelLocalisationMemoryBuffer.initialize(m_pAcquisitionStreamEncoder->getInputParameter(OVP_GD_Algorithm_AcquisitionStreamEncoder_InputParameterId_ChannelLocalisationStream));
	//op_pAcquisitionMemoryBuffer.initialize(m_pAcquisitionStreamEncoder->getOutputParameter(OVP_GD_Algorithm_AcquisitionStreamEncoder_OutputParameterId_EncodedMemoryBuffer));

	m_ui64LastChunkStartTime=0;
	m_ui64LastChunkEndTime=0;
	m_pConnectionServer=NULL;

	initStimulationMap();
	m_vect64.reserve(5000);
	
	return true;
}

OpenViBE::boolean CBoxAlgorithmCVKServer::uninitialize(void)
{
	m_pConnectionServer->close();
	m_pConnectionServer->release();
	m_pConnectionServer=NULL;

	//ip_pChannelLocalisationMemoryBuffer.uninitialize();
	//ip_pSignalMemoryBuffer.uninitialize();
	//ip_pExperimentInformationMemoryBuffer.uninitialize();
	//ip_ui64BufferDuration.uninitialize();
	//op_pAcquisitionMemoryBuffer.uninitialize();
	
	op_pStimulationSet.uninitialize();
	ip_pStimulationMemoryBuffer.uninitialize();
	m_pStimulationDecoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationDecoder);

	applyOnFile();
	
	return true;
}

OpenViBE::boolean CBoxAlgorithmCVKServer::processClock(IMessageClock& rMessageClock)
{
	
    //récupération des informations
	CString l_sSettingValue;
	getStaticBoxContext().getSettingValue(0, l_sSettingValue);
	uint32 l_ui32ServerPort=::atoi(l_sSettingValue.toASCIIString());
	
    //creation du socket d'écoute
	if(!m_pConnectionServer)
		{
		 std::cout<< "create server" << std::endl;
		 m_pConnectionServer=Socket::createConnectionServer();
		}
	//connection à un serveur 
	if(m_pConnectionServer )//&& !m_pConnectionServer->isConnected())
		{
		 //this->getLogManager() << LogLevel_ImportantWarning << "Socket OK" << "\n";
		 if(!m_pConnectionServer->listen(l_ui32ServerPort))
			{
			 //this->getLogManager() << LogLevel_ImportantWarning << "Could not listen " << "\n";
			 //return true;
			}
		 else
			{
			 //this->getLogManager() << LogLevel_ImportantWarning << "listen OK" << "\n";
			 m_pConnectionClient=m_pConnectionServer->accept();
			 std::cout<<"connection accepted"<<std::endl;
			}
		 //getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
		}

	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}

OpenViBE::boolean CBoxAlgorithmCVKServer::process(void)
{
	if(!m_pConnectionClient || !m_pConnectionClient->isConnected())
	{
		getLogManager() << LogLevel_ImportantWarning << "connection lost\n";
		return false;
	}

	// IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	
	///extracted from another code file : 
	//uint32 nombreMorceau=l_pDynamicBoxContext->getInputChunkCount(0);
	//pour chacun de ces morceaux faire
	//
	//ip_pExperimentInformationMemoryBuffer=l_rDynamicBoxContext.getInputChunk(0,0);
	//ip_pSignalMemoryBuffer=l_rDynamicBoxContext.getInputChunk(1,0);
	//ip_pChannelLocalisationMemoryBuffer=l_rDynamicBoxContext.getInputChunk(3,0);

	std::stringstream l_sCmdLine;
	for(uint32 k=0; k<l_rDynamicBoxContext.getInputChunkCount(2); k++)
	  {
		//std::cout<<"will get some data from IO"<<std::endl;
		ip_pStimulationMemoryBuffer=l_rDynamicBoxContext.getInputChunk(2,k);
		//std::cout<<"will decode"<<std::endl;
		m_pStimulationDecoder->process();
		
		OpenViBE::uint64 l_uilastTime=0;
		//
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedHeader))
		  {
		  }
		//
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
		  {
			//std::cout<<"some data arrived"<<std::endl;
			
			for(uint32 j=0; j<op_pStimulationSet->getStimulationCount(); j++)
			{
				uint64 l_Stimulation_id=op_pStimulationSet->getStimulationIdentifier(j);
				if(l_uilastTime!=op_pStimulationSet->getStimulationDate(j))
				  {
					l_uilastTime=op_pStimulationSet->getStimulationDate(j);
					l_sCmdLine<<"Time"<<l_uilastTime<<"#";
				  }
				//
				if(l_Stimulation_id>=OVTK_StimulationId_Label_01 && l_Stimulation_id<=OVTK_StimulationId_Label_19)
				  {
					l_sCmdLine<<"Lab"<<l_Stimulation_id<<"#";
				  }
				else
				  {
					l_sCmdLine<<"Life"<<timeConvertion(op_pStimulationSet->getStimulationDuration(j))<<"#";
					l_sCmdLine<<"Stim"<<l_Stimulation_id<<"#";
				  }
				UseStimulationForMap(op_pStimulationSet->getStimulationIdentifier(j));
			}
		  }
		//
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedEnd))
		  {
		  }

		l_rDynamicBoxContext.markInputAsDeprecated(2, k);
	  }
	//
	l_sCmdLine <<std::ends;
	
	
	if(m_pConnectionClient && m_pConnectionClient->isConnected() && m_pConnectionClient->isReadyToSend())
	{
		std::string l_sStringToSend=l_sCmdLine.str();
		uint32 l_uiStrSize=l_sStringToSend.size();
		if(l_uiStrSize>1) {if(Debug==1) {std::cout<<"send "<<l_sStringToSend<<". for a size of "<<l_uiStrSize<<std::endl;}}
		if(l_uiStrSize>1)
		  {
			if(!m_pConnectionClient->sendBuffer(l_sStringToSend.c_str(), l_uiStrSize))
			  {
				getLogManager() << LogLevel_ImportantWarning << "Could not send memory buffer content of size " << l_uiStrSize << "\n";
				return false;
			  }
			//else {std::cout<<"emit hello world"<<std::endl;}
			else
			  {WriteTime(this->getPlayerContext().getCurrentTime());}
		  }

	}
	else
	{
		//getLogManager() << LogLevel_Info << "nothing to send\n";
	}

	if(m_pConnectionClient && m_pConnectionClient->isConnected() && m_pConnectionClient->isReadyToReceive())
	{
		uint32 oct=m_pConnectionClient->receiveBuffer(reception_buffer_RoBIKCVK, MAXSIZE);
		
		if(oct<=0)
		  {
			getLogManager() << LogLevel_ImportantWarning << "error of reception\n";
		  }
		else
		  {
			if(Debug==1) {std::cout<<oct<<" data received : "<<reception_buffer_RoBIKCVK<<"."<<std::endl;}
			parseCVKincome(oct);
		  }
	}
	else
	{
		// getLogManager() << LogLevel_Info << "nothing to receive\n";
	}
	
	return true;
}

void CBoxAlgorithmCVKServer::initStimulationMap()
{
 m_mapId[OVTK_StimulationId_ExperimentStart]="OVTK_StimulationId_ExperimentStart";
 m_mapId[OVTK_StimulationId_ExperimentStop]="OVTK_StimulationId_ExperimentStop";
 m_mapId[OVTK_StimulationId_SegmentStart]="OVTK_StimulationId_SegmentStart";
 m_mapId[OVTK_StimulationId_SegmentStop]="OVTK_StimulationId_SegmentStop";
 m_mapId[OVTK_StimulationId_TrialStart]="OVTK_StimulationId_TrialStart";
 m_mapId[OVTK_StimulationId_TrialStop]="OVTK_StimulationId_TrialStop";
 m_mapId[OVTK_StimulationId_BaselineStart]="OVTK_StimulationId_BaselineStart";
 m_mapId[OVTK_StimulationId_BaselineStop]="OVTK_StimulationId_BaselineStop";
 m_mapId[OVTK_StimulationId_RestStart]="OVTK_StimulationId_RestStart";
 m_mapId[OVTK_StimulationId_RestStop]="OVTK_StimulationId_RestStop";
 m_mapId[OVTK_StimulationId_VisualStimulationStart]="OVTK_StimulationId_VisualStimulationStart";
 m_mapId[OVTK_StimulationId_VisualStimulationStop]="OVTK_StimulationId_VisualStimulationStop";
 m_mapId[OVTK_StimulationId_VisualSteadyStateStimulationStart]="OVTK_StimulationId_VisualSteadyStateStimulationStart";
 m_mapId[OVTK_StimulationId_VisualSteadyStateStimulationStop]="OVTK_StimulationId_VisualSteadyStateStimulationStop";
 
 m_mapId[OVTK_StimulationId_LabelStart]="OVTK_StimulationId_LabelStart";
 m_mapId[OVTK_StimulationId_Label_00]="OVTK_StimulationId_Label_00";
 m_mapId[OVTK_StimulationId_Label_01]="OVTK_StimulationId_Label_01";
 m_mapId[OVTK_StimulationId_Label_02]="OVTK_StimulationId_Label_02";
 m_mapId[OVTK_StimulationId_Label_03]="OVTK_StimulationId_Label_03";
 m_mapId[OVTK_StimulationId_Label_04]="OVTK_StimulationId_Label_04";
 m_mapId[OVTK_StimulationId_Label_05]="OVTK_StimulationId_Label_05";
 m_mapId[OVTK_StimulationId_Label_06]="OVTK_StimulationId_Label_06";
 m_mapId[OVTK_StimulationId_Label_07]="OVTK_StimulationId_Label_07";
 m_mapId[OVTK_StimulationId_Label_08]="OVTK_StimulationId_Label_08";
 m_mapId[OVTK_StimulationId_Label_09]="OVTK_StimulationId_Label_09";
 m_mapId[OVTK_StimulationId_Label_0A]="OVTK_StimulationId_Label_0A";
 m_mapId[OVTK_StimulationId_Label_0B]="OVTK_StimulationId_Label_0B";
 m_mapId[OVTK_StimulationId_Label_0C]="OVTK_StimulationId_Label_0C";
 m_mapId[OVTK_StimulationId_Label_0D]="OVTK_StimulationId_Label_0D";
 m_mapId[OVTK_StimulationId_Label_0E]="OVTK_StimulationId_Label_0E";
 m_mapId[OVTK_StimulationId_Label_0F]="OVTK_StimulationId_Label_0F";

 m_mapId[OVTK_StimulationId_Label_10]="OVTK_StimulationId_Label_10";
 m_mapId[OVTK_StimulationId_Label_11]="OVTK_StimulationId_Label_11";
 m_mapId[OVTK_StimulationId_Label_12]="OVTK_StimulationId_Label_12";
 m_mapId[OVTK_StimulationId_Label_13]="OVTK_StimulationId_Label_13";
 m_mapId[OVTK_StimulationId_Label_14]="OVTK_StimulationId_Label_14";
 m_mapId[OVTK_StimulationId_Label_15]="OVTK_StimulationId_Label_15";
 m_mapId[OVTK_StimulationId_Label_16]="OVTK_StimulationId_Label_16";
 m_mapId[OVTK_StimulationId_Label_17]="OVTK_StimulationId_Label_17";
 m_mapId[OVTK_StimulationId_Label_18]="OVTK_StimulationId_Label_18";
 m_mapId[OVTK_StimulationId_Label_19]="OVTK_StimulationId_Label_19";
 m_mapId[OVTK_StimulationId_Label_1A]="OVTK_StimulationId_Label_1A";
 m_mapId[OVTK_StimulationId_Label_1B]="OVTK_StimulationId_Label_1B";
 m_mapId[OVTK_StimulationId_Label_1C]="OVTK_StimulationId_Label_1C";
 m_mapId[OVTK_StimulationId_Label_1D]="OVTK_StimulationId_Label_1D";
 m_mapId[OVTK_StimulationId_Label_1E]="OVTK_StimulationId_Label_1E";
 m_mapId[OVTK_StimulationId_Label_1F]="OVTK_StimulationId_Label_1F";
 m_mapId[OVTK_StimulationId_LabelEnd]="OVTK_StimulationId_LabelEnd";
}

void CBoxAlgorithmCVKServer::UseStimulationForMap(OpenViBE::uint64 identifiant)
{
 m_mapId[identifiant];
}

void CBoxAlgorithmCVKServer::applyOnFile()
{
   FILE * pFile;
   pFile = fopen ("Stimulation_Correspondance.txt" , "w");
   if (pFile == NULL) perror ("Error opening file");
   else
   {
    for(std::map<OpenViBE::uint64, std::string>::iterator ite=m_mapId.begin(); ite!=m_mapId .end(); ++ite)
	  {
		std::ostringstream ost;
		ost << (*ite).first <<"="<<(*ite).second<<"\n";
	    std::string str=ost.str();
		fwrite (str.c_str() , 1 , str.size() , pFile );
	  }
    fclose (pFile);
   }

   FILE * pFile2;
   pFile2 = fopen ("ServerSendCommandTime.txt" , "w");
   if (pFile2 == NULL) perror ("Error opening file 2");
   else
   {
    for(std::vector<OpenViBE::uint64>::iterator ite=m_vect64.begin(); ite!=m_vect64.end(); ++ite)
	  {
		std::ostringstream ost2;
		ost2 << (*ite)<<"\n";
	    std::string str=ost2.str();
		fwrite (str.c_str() , 1 , str.size() , pFile2 );
	  }
    fclose (pFile2);
   }
}

void CBoxAlgorithmCVKServer::WriteTime(OpenViBE::uint64 time)
{
 m_vect64.push_back(time);
}

float64 CBoxAlgorithmCVKServer::timeConvertion(OpenViBE::uint64 time)
{
 float64 timesms=float64(time>>32);
  timesms+=float64(((((time<<32)>>32)+65536)*1000)>>32)/1000;
 //std::cout<<"time : "<<time<<". ui_ms : "<<((time<<32)>>32)<<". s : "<<timesms<<std::endl;
 return timesms;
}

boolean CBoxAlgorithmCVKServer::parseCVKincome(uint32 oct)
{
 ///"[action:startAnimation|time:666585888489|zoneID:33033]"
	std::string baliseAction="action:";
	std::string baliseTime="time:";
	std::string baliseData="zoneID:";
	std::string delimiter="|";
	std::string ender="]";
	
	std::string str=reception_buffer_RoBIKCVK;
	//check str.size()/oct...
	if(Debug==2) {std::cout<<"string to parse : "<<str.c_str()<<"."<<std::endl;}
	
	std::string piece;
	size_t found= str.find(ender.c_str());
	if(Debug==2) {std::cout<<ender.c_str()<<" : new session end at "<<found<<std::endl;}
	while(found!=std::string::npos)
	  {
		if(Debug==2) {std::cout<<"parsing in progress"<<std::endl;}
		//
		piece.clear();
		piece=extractBalise(str,baliseAction,delimiter);
		if(!piece.empty()) 
		  {
			if(Debug==2) {std::cout<<"extracted action : "<<piece<<" => ";}
			if(!Action2value(piece)) {std::cout<<"echec of action convertion"<<std::endl;}
			else {if(Debug==2) {std::cout<<"action = "<<m_actionBack<<std::endl;}}
		  }
		//
		piece.clear();
		piece=extractBalise(str,baliseTime,delimiter);
		if(!piece.empty()) 
		  {
			if(Debug==2) {std::cout<<"extracted time : "<<piece<<" => ";}
			if(!Time2value(piece)) {std::cout<<"echec of time convertion"<<std::endl;}
			else {if(Debug==2) {std::cout<<"time = "<<m_timeBack<<std::endl;}}
		  }
		//
		piece.clear();
		piece=extractBalise(str,baliseData,ender);
		if(!piece.empty()) 
		  {
			if(Debug==2) {std::cout<<"extracted data : "<<piece<<" => ";}
			if(!Data2value(piece)) {std::cout<<"echec of data convertion"<<std::endl;}
			else {if(Debug==2) {std::cout<<"data = "<<m_DataBack<<std::endl;}}
		  }
		//
		found=str.find(ender.c_str(),found+1);
		if(found!=std::string::npos) {if(Debug==2) {std::cout<<ender.c_str()<<" : new session end at "<<found<<std::endl;}}
	  }
 
	return true;
}

std::string CBoxAlgorithmCVKServer::extractBalise(std::string& base, std::string& balise, std::string& ender)
{
	std::string piece;
	size_t found= base.find(balise.c_str());
	if(found==std::string::npos)
	  {std::cout<<"no '"<<balise<<"' balise"<<std::endl;}
	else
	  {
		size_t foundDelimit= base.find(ender.c_str(),found);
		if(foundDelimit==std::string::npos)
		  {std::cout<<"no end balise for '"<<balise<<"' from "<<found<<std::endl;}
		else
		  {
			piece=base.substr(found+balise.size(), foundDelimit-(found+balise.size()));
		  }
	  }
	return piece;
}

OpenViBE::boolean CBoxAlgorithmCVKServer::Action2value(std::string& str)
{
	m_actionBack=0;
	if(str=="startAnimation")
	  {m_actionBack=1;}
	return true;
}

OpenViBE::boolean CBoxAlgorithmCVKServer::Time2value(std::string& str)
{
	long double base=10;
	int32 rIdx=str.size();
	int32 rLength=9;
	uint32 count=0;
	uint64 Pow=1;
	m_timeBack=0;
	while(rIdx>rLength)
	  {
		// std::cout<<"idx : "<<rIdx<<" / "<<rLength<<std::endl;
		std::string piece=str.substr(rIdx-rLength,rLength);
		// std::cout<<"time piece = "<<piece<<std::endl;
		m_timeBack+=(::atoi(piece.c_str())*Pow);
		//
		rIdx-=rLength;
		count++;
		Pow=uint64(pow(base,int(count*rLength)));
	  }
	//dernier cas
	rLength=rIdx;
	rIdx=0;
	std::string piece=str.substr(rIdx,rLength);
	// std::cout<<"time piece = "<<piece<<std::endl;
	m_timeBack+=(::atoi(piece.c_str())*Pow);

	if(m_timeBack==0) {return false;}
	return true;
}

OpenViBE::boolean CBoxAlgorithmCVKServer::Data2value(std::string& str)
{
	m_DataBack=::atoi(str.c_str());
	if(m_DataBack==0) {return false;}
	return true;
}
