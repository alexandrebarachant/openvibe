#include "ovpCBoxAlgorithmExperimentStarter.h"

#if defined OVP_OS_Windows

#include <cstdlib>
#include <iostream>
#include <sstream>

#include <stdio.h>
#define boolean OpenViBE::boolean

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Acquisition;

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

uint64 CBoxAlgorithmExperimentStarter::getClockFrequency(void)
{
	return 64LL<<32;
}

boolean CBoxAlgorithmExperimentStarter::initialize(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();

	m_pStimulationEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamEncoder));
	m_pStimulationEncoder->initialize();

	ip_pStimulationSet.initialize(m_pStimulationEncoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_InputParameterId_StimulationSet));

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------

	CString strSeq	=_AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 0);

	//m_fileSequence.open (strSeq.toASCIIString(), std::fstream::in);
	//if (!m_fileSequence.is_open())
	//  {
	//   std::cout << "Error opening sequence file "<<strSeq<<std::endl;
	//   return false;
	//  }

	m_pConnectionClient=NULL;
	m_pConnectionServer=NULL;

	HeaderToSend=true;
	SomethingToSend=false;
	SomethingToSendInOV=false;
	mc_StartClock=clock();
	return true;
}

boolean CBoxAlgorithmExperimentStarter::uninitialize(void)
{
	ip_pStimulationSet.uninitialize();

	m_pStimulationEncoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationEncoder);

	//if(m_fileSequence.is_open()) {m_fileSequence.close();}
	return true;
}

boolean CBoxAlgorithmExperimentStarter::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CBoxAlgorithmExperimentStarter::processClock(IMessageClock& rMessageClock)
{
    //récupération des informations
	CString l_sSettingValue;
	getStaticBoxContext().getSettingValue(1, l_sSettingValue);
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

			 mc_StartClock=clock();
			 std::cout<<"At"<<mc_StartClock<<std::endl;
			 //
			 GetSystemTime(&mo_st);
			 addTimeSTsecond(mo_st,15);
			 std::cout<<"starting time : YY/MM/DD : HH/MM/SS/MS = "<<mo_st.wYear<<"/"<<mo_st.wMonth<<"/"<<mo_st.wDay<<" : "<<
						mo_st.wHour<<"/"<<mo_st.wMinute<<"/"<<mo_st.wSecond<<"/"<<mo_st.wMilliseconds<<std::endl;
			 //
			 timeStartConnection=this->getPlayerContext().getCurrentTime();
			 std::cout<<"At time : "<<timeStartConnection<<std::endl;

			 SomethingToSend=true;
			 SomethingToSendInOV=true;
			}
		 //getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
		}

	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}

boolean CBoxAlgorithmExperimentStarter::process(void)
{
	if(!m_pConnectionClient || !m_pConnectionClient->isConnected())
	{
		getLogManager() << LogLevel_ImportantWarning << "connection lost\n";
		return false;
	}

	// std::cout<<"start Process"<<std::endl;

	// IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	TParameterHandler < IMemoryBuffer* > op_pMemoryBuffer(m_pStimulationEncoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
	op_pMemoryBuffer=l_rDynamicBoxContext.getOutputChunk(0);

	LARGE_INTEGER start;
	::QueryPerformanceCounter(&start);
	//std::cout<<"query time : "<<start.QuadPart<<" : "<<start.LowPart<<"--"<<start.HighPart<<std::endl;

	GetSystemTime(&mo_st_Current);
	//std::cout<<"in process, YY/MM/DD : HH/MM/SS/MS ="<<mo_st_Current.wYear<<"/"<<mo_st_Current.wMonth<<"/"<<mo_st_Current.wDay<<" : "<<mo_st_Current.wHour<<"/"<<mo_st_Current.wMinute<<"/"<<mo_st_Current.wSecond<<"/"<<mo_st_Current.wMilliseconds<<std::endl;

	//std::cout<<"At time : "<<this->getPlayerContext().getCurrentTime()<<std::endl;
	uint64 l_ui64StartTime=this->getPlayerContext().getCurrentTime();
	uint64 l_ui64EndTime=l_ui64StartTime;

	if(HeaderToSend)
	  {
		std::cout<<"send header"<<std::endl;
		m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeHeader);
		l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_ui64StartTime, l_ui64EndTime);
		HeaderToSend=false;
	  }

	if(SomethingToSend)
	  {
		std::cout<<"somethingTosend"<<std::endl;
	  	if(m_pConnectionClient && m_pConnectionClient->isConnected() && m_pConnectionClient->isReadyToSend())
		  {
			std::stringstream cmdText;
			cmdText<<"STARTEXPERIMENT AT YY/MM/DD : HH/MM/SS/MS = "<<mo_st.wYear<<"/"<<mo_st.wMonth<<"/"<<mo_st.wDay<<" : "<<mo_st.wHour<<"/"<<mo_st.wMinute<<"/"<<mo_st.wSecond<<"/"<<mo_st.wMilliseconds<<"."<<std::endl;
			std::cout<<"cmd = "<<cmdText.str()<<std::endl;

			uint32 l_uiCmdSize=cmdText.str().size();
			//std::cout<<"send for a size of "<<l_uiCmdSize<<std::endl;
			if(l_uiCmdSize>0)
			  {
				uint32 l_uireallySend=m_pConnectionClient->sendBuffer(cmdText.str().c_str(), l_uiCmdSize);
				std::cout<<"really send = "<<l_uireallySend<<" octet."<<std::endl;
				if(!l_uireallySend)
				  {
					getLogManager() << LogLevel_ImportantWarning << "Could not send memory buffer content of size " << l_uiCmdSize << "\n";
					return false;
				  }
			  }

			SomethingToSend=false;
		  }
	  }

	if(SomethingToSendInOV && TimeSTover(mo_st,mo_st_Current))
	  {
		std::cout<<std::endl<<std::endl<<std::endl<<std::endl<<std::endl;
		std::cout<<"send buffer at "<<std::endl;
		std::cout<<"in process, YY/MM/DD : HH/MM/SS/MS ="<<mo_st_Current.wYear<<"/"<<mo_st_Current.wMonth<<"/"<<mo_st_Current.wDay<<" : "<<mo_st_Current.wHour<<"/"<<mo_st_Current.wMinute<<"/"<<mo_st_Current.wSecond<<"/"<<mo_st_Current.wMilliseconds<<std::endl;
		std::cout<<std::endl<<std::endl<<std::endl<<std::endl<<std::endl;
		///diffuser dans openVIBE
		ip_pStimulationSet->appendStimulation (OVTK_StimulationId_RestStart, l_ui64StartTime, 0);

		//émettre
		m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeBuffer);
		l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_ui64StartTime, l_ui64EndTime);

		SomethingToSendInOV=false;
	  }

	return true;
}

boolean CBoxAlgorithmExperimentStarter::addTimeSTsecond(SYSTEMTIME& st, uint32 ts)
{
 std::cout<<"addTime : "<<ts<<std::endl;
 uint32 Second=st.wSecond+ts;
 uint32 SecondRatio=Second%60;
 uint32 SecondRetenu=Second/60;
 st.wSecond=SecondRatio;
 std::cout<<"second : "<<Second<<"%"<<SecondRatio<<"//"<<SecondRetenu<<std::endl;
 if(SecondRetenu==0) {return true;}

 uint32 Minute=st.wMinute+SecondRetenu;
 uint32 MinuteRatio=Minute%60;
 uint32 MinuteRetenu=Minute/60;
 st.wMinute=MinuteRatio;
 std::cout<<"Minute : "<<Minute<<"%"<<MinuteRatio<<"//"<<MinuteRetenu<<std::endl;
 if(MinuteRetenu==0) {return true;}

 uint32 Hour=st.wHour+MinuteRetenu;
 uint32 HourRatio=Hour%24;
 uint32 HourRetenu=Hour/24;
 st.wHour=HourRatio;
 std::cout<<"Hour : "<<Hour<<"%"<<HourRatio<<"//"<<HourRetenu<<std::endl;
 if(HourRetenu==0) {return true;}

 return false;
}

boolean CBoxAlgorithmExperimentStarter::TimeSTover(SYSTEMTIME& mostref, SYSTEMTIME& most)
{
 if(most.wYear>mostref.wYear) {return true;}
 if(most.wYear<mostref.wYear) {return false;}
 //
 if(most.wMonth>mostref.wMonth) {return true;}
 if(most.wMonth<mostref.wMonth) {return false;}
 //
 if(most.wDay>mostref.wDay) {return true;}
 if(most.wDay<mostref.wDay) {return false;}
 //
 if(most.wHour>mostref.wHour) {return true;}
 if(most.wHour<mostref.wHour) {return false;}
 //
 if(most.wMinute>mostref.wMinute) {return true;}
 if(most.wMinute<mostref.wMinute) {return false;}
 //
 if(most.wSecond>mostref.wSecond) {return true;}
 if(most.wSecond<mostref.wSecond) {return false;}
 //
 if(most.wMilliseconds>=mostref.wMilliseconds) {return true;}
 if(most.wMilliseconds<mostref.wMilliseconds) {return false;}
 //
 return false;
}

#endif
