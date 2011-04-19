#include "ovpCBoxAlgorithmBrutEEGSignalServer.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Acquisition;

uint64 CBoxAlgorithmBrutEEGSignalServer::getClockFrequency(void)
{
	return 64LL<<32;
}

OpenViBE::boolean CBoxAlgorithmBrutEEGSignalServer::initialize(void)
{
	m_pSignalDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamDecoder));
	m_pSignalDecoder->initialize();
	ip_pSignalMemoryBuffer.initialize(m_pSignalDecoder->getInputParameter(OVP_GD_Algorithm_SignalStreamDecoder_InputParameterId_MemoryBufferToDecode));
    op_pDecodedMatrix.initialize(m_pSignalDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_Matrix));
	op_ui64SamplingRate.initialize(m_pSignalDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_SamplingRate));

	m_pStimulationDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
	m_pStimulationDecoder->initialize();
	ip_pStimulationMemoryBuffer.initialize(m_pStimulationDecoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
	op_pStimulationSet.initialize(m_pStimulationDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
	
	m_ui64LastChunkStartTime=0;
	m_ui64LastChunkEndTime=0;
	m_pConnectionServer=NULL;
	
	m_bStarted=false;

	return true;
}

OpenViBE::boolean CBoxAlgorithmBrutEEGSignalServer::uninitialize(void)
{
	m_pConnectionServer->close();
	m_pConnectionServer->release();
	m_pConnectionServer=NULL;

	op_pStimulationSet.uninitialize();
	ip_pStimulationMemoryBuffer.uninitialize();
	m_pStimulationDecoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationDecoder);
	
	op_ui64SamplingRate.uninitialize();
	op_pDecodedMatrix.uninitialize();
	ip_pSignalMemoryBuffer.uninitialize();
	m_pSignalDecoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pSignalDecoder);

	return true;
}

OpenViBE::boolean CBoxAlgorithmBrutEEGSignalServer::processClock(IMessageClock& rMessageClock)
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
			  std::cout<<"connection established"<<std::endl;
			}
		 //getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
		}

	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}

OpenViBE::boolean CBoxAlgorithmBrutEEGSignalServer::process(void)
{
	if(!m_pConnectionClient || !m_pConnectionClient->isConnected())
	{
		getLogManager() << LogLevel_ImportantWarning << "connection lost\n";
		return false;
	}

	// IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	// std::cout<<"number of stim chunk : "<<l_rDynamicBoxContext.getInputChunkCount(2)<<std::endl;
	
	for(uint32 k=0; k<l_rDynamicBoxContext.getInputChunkCount(2); k++)
	  {
		//std::cout<<"will get some data from IO"<<std::endl;
		ip_pStimulationMemoryBuffer=l_rDynamicBoxContext.getInputChunk(2,k);
		//std::cout<<"will decode"<<std::endl;
		m_pStimulationDecoder->process();

		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedHeader))
		  {
		  }
		//
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
		  {
			if(!m_bStarted && op_pStimulationSet->getStimulationCount()!=0)
			  {
				m_bStarted=true;
				std::cout<<"enable sending operation" << std::endl;
				//break;
			  }
		  }
		//
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedEnd))
		  {
		  }

		l_rDynamicBoxContext.markInputAsDeprecated(2, k);
	  }

	// std::cout<<"number of signal chunk : "<<l_rDynamicBoxContext.getInputChunkCount(1)<<std::endl;
	if(l_rDynamicBoxContext.getInputChunkCount(1)>1)
	  {std::cout<<"Time is not fluent, this server got "<<l_rDynamicBoxContext.getInputChunkCount(1)<<" chunks."<<std::endl;}
	
	std::vector<float32> l_vf32Cmd;
	for(uint32 k=0; k<l_rDynamicBoxContext.getInputChunkCount(1); k++)
	  {
		// std::cout<<"will get some data from IO"<<std::endl;
		ip_pSignalMemoryBuffer=l_rDynamicBoxContext.getInputChunk(1,k);
		// std::cout<<"will decode"<<std::endl;
		m_pSignalDecoder->process();
		//
		IMatrix* l_pInputMatrix=op_pDecodedMatrix;
		uint64 freq=op_ui64SamplingRate;
		
		
		OpenViBE::uint64 l_uilastTime=0;
		//
		if(m_pSignalDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedHeader))
		  {
		  }
		//
		if(m_pSignalDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedBuffer))
		  {
		  
		  	if(!m_bStarted)
			  {
				// std::cout<<"flush input"<<std::endl;
				l_rDynamicBoxContext.markInputAsDeprecated(1, k);
				continue;
			  }
			else
			  {
				// std::cout<<"flush stopped"<<std::endl;
			  }
	  
	  
			// std::cout<<"some data arrived"<<std::endl;
			uint32 maxSize=0;
			for(uint32 i=0; i<l_pInputMatrix->getDimensionCount(); i++)
			  {
				i==0 ? 	maxSize+=l_pInputMatrix->getDimensionSize(0) :
						maxSize*=l_pInputMatrix->getDimensionSize(i) ;
			  }
			
			// std::cout<<"size : "<<maxSize<<std::endl;
			for(uint32 i=0; i<maxSize; i++)
			  {
				l_vf32Cmd.push_back(l_pInputMatrix->getBuffer()[i]);
			  }
				  
		  }
		//
		if(m_pSignalDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedEnd))
		  {
		  }

		l_rDynamicBoxContext.markInputAsDeprecated(1, k);
	  }
	
	if(m_pConnectionClient && m_pConnectionClient->isConnected() && m_pConnectionClient->isReadyToSend())
	{
		//char* buf="hello World";
		// std::stringstream cmdTest;
		// cmdTest<<"toto"<<10<<'10'<<"#"<<std::hex<<10<<std::dec<<10<<std::endl;
		// std::cout<<"test "<<cmdTest.str()<<std::endl;
		// float32 tab[]={420001.1,2.2,3.3,4.4,0};
		// std::cout<<"test tab "<<(reinterpret_cast<char *>(tab))<<std::endl;
		
		uint32 l_uiCmdSize=l_vf32Cmd.size()*4; //tab.size()*8->32
		// std::cout<<"send for a size of "<<l_uiCmdSize<<std::endl;
		if(l_uiCmdSize>0)
		  {
			uint32 l_uireallySend=m_pConnectionClient->sendBuffer((reinterpret_cast<char *>(&l_vf32Cmd[0])), l_uiCmdSize);
			std::cout<<"At time : "<<timeConvertion(this->getPlayerContext().getCurrentTime())<<std::endl;
			std::cout<<"really send = "<<l_uireallySend<<" octet."<<std::endl;
			if(!l_uireallySend)
			  {
				getLogManager() << LogLevel_ImportantWarning << "Could not send memory buffer content of size " << l_uiCmdSize << "\n";
				return false;
			  }
			// else {std::cout<<"transmission OK"<<std::endl;}
			// else
			  // {WriteTime(this->getPlayerContext().getCurrentTime());}
		  }

	}
	else
	{
		//getLogManager() << LogLevel_Info << "Skipped...\n";
	}

	return true;
}

float64 CBoxAlgorithmBrutEEGSignalServer::timeConvertion(OpenViBE::uint64 time)
{
 float64 timesms=float64(time>>32);
  timesms+=float64(((((time<<32)>>32)+65536)*1000)>>32)/1000;
 // std::cout<<"time : "<<time<<". ui_ms : "<<((time<<32)>>32)<<". s : "<<timesms<<std::endl;
 return timesms;
}
