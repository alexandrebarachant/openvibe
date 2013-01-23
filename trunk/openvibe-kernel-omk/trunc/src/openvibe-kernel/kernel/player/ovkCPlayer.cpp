#include "ovkCPlayer.h"
#include "ovkCSimulatedBox.h"
#include "ovkCScheduler.h"

#include "../configuration/ovkCConfigurationManager.h"

#include <system/Time.h>

#include <xml/IReader.h>

#include <string>
#include <iostream>
#include <fstream>

//___________________________________________________________________//
//                                                                   //

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

#define _Scheduler_Default_Frequency_ 128
#define _Scheduler_Maximum_Loops_Duration_ (100LL << 22) /* 100/1024 second = approx 100ms */

//___________________________________________________________________//
//                                                                   //

CPlayer::CPlayer(const IKernelContext& rKernelContext)
	:TKernelObject<IPlayer>(rKernelContext)
	,m_oKernelContextBridge(rKernelContext)
	,m_oScheduler(m_oKernelContextBridge, *this)
	,m_ui64CurrentTimeToReach(0)
	,m_ui64Lateness(0)
	,m_eStatus(PlayerStatus_Uninitialized)
{
	uint64 l_ui64SchedulerFrequency=this->getConfigurationManager().expandAsUInteger("${Kernel_PlayerFrequency}");
	if(l_ui64SchedulerFrequency==0)
	{
		getLogManager() << LogLevel_ImportantWarning << "Invalid frequency configuration " << CString("Kernel_PlayerFrequency") << "=" << this->getConfigurationManager().expand("${Kernel_PlayerFrequency}") << " restored to default " << _Scheduler_Default_Frequency_ << "\n";
		l_ui64SchedulerFrequency=_Scheduler_Default_Frequency_;
	}
	else
	{
		getLogManager() << LogLevel_Trace << "Player frequency set to " << l_ui64SchedulerFrequency << "\n";
	}
	m_oScheduler.setFrequency(l_ui64SchedulerFrequency);
}

CPlayer::~CPlayer(void)
{
	if(m_eStatus!=PlayerStatus_Uninitialized)
	{
		this->uninitialize();
	}
}

//___________________________________________________________________//
//                                                                   //

boolean CPlayer::setScenario(
	const CIdentifier& rScenarioIdentifier)
{
	if(m_eStatus!=PlayerStatus_Uninitialized)
	{
		this->getLogManager() << LogLevel_Warning << "Trying to configure an initialized player !\n";
		return false;
	}

	this->getLogManager() << LogLevel_Trace << "Player setScenario\n";
	m_oScenarioIdentifier = rScenarioIdentifier;
	return m_oScheduler.setScenario(rScenarioIdentifier);;
}

boolean CPlayer::initialize(void)
{
	if(m_eStatus!=PlayerStatus_Uninitialized)
	{
		this->getLogManager() << LogLevel_Warning << "Trying to initialize an initialized player !\n";
		return false;
	}

	this->getLogManager() << LogLevel_Trace << "Player initialize\n";

	m_pLocalConfigurationManager=new CConfigurationManager(m_oKernelContextBridge, &this->getKernelContext().getConfigurationManager());

	// At this point we've inserted the bridge as a stand-in for Kernel context to the local CConfigurationManager, but the manager in the bridge is still the
	// 'global' one. Now lets change the config manager in the bridge to point to the local manager in order to load configurations into the local manager.
	m_oKernelContextBridge.setConfigurationManager(m_pLocalConfigurationManager);

	m_pLocalConfigurationManager->addConfigurationFromFile(this->getKernelContext().getConfigurationManager().expand("${Kernel_DelayedConfiguration}"));

	//get scenario path from the global configuration manager and insert it to the local manager.
	//after having the path, try to see if there's a scenario specific config file
	OpenViBE::CString l_sGlobalPathToken = "__volatile_Scenario" + m_oScenarioIdentifier.toString() + "Dir";
	OpenViBE::CString l_sWorkingDir = this->getKernelContext().getConfigurationManager().lookUpConfigurationTokenValue(l_sGlobalPathToken);
	if(l_sWorkingDir == CString(""))
	{
		this->getLogManager() << LogLevel_Warning << "Warning: Token " << l_sGlobalPathToken << " did not exist in the configuration manager. The token should have been set when the scenario was loaded or saved. If you're working on a new scenario that has never been saved, ignore this warning.\n";
		this->getLogManager() << LogLevel_Warning << "Warning: Will not attempt to load any scenario specific configuration file.\n";
	} 
	else
	{
		//create an easily named local token that scenarios can use to read their own current directory. Note that the value of this token will often be overwritten by OpenViBE.
		const CString l_sLocalPathToken("__volatile_ScenarioDir");
		const CString l_sOldPath = m_pLocalConfigurationManager->lookUpConfigurationTokenValue(l_sLocalPathToken);
		if(l_sOldPath == CString(""))
		{
			m_pLocalConfigurationManager->createConfigurationToken(l_sLocalPathToken,l_sWorkingDir);
		}
		else 
		{
			m_pLocalConfigurationManager->setConfigurationTokenValue( m_pLocalConfigurationManager->lookUpConfigurationTokenIdentifier(l_sLocalPathToken), l_sWorkingDir);
		}

		//load local, scenario-specific configuration file
		CString l_sConfigPath = l_sWorkingDir + "/scenario.conf";
		this->getLogManager() << LogLevel_Trace << "Requesting add of local scenario config from " << l_sConfigPath << "\n";
		m_pLocalConfigurationManager->addConfigurationFromFile( l_sConfigPath ); 
	}

	m_oScheduler.initialize();
	m_oBenchmarkChrono.reset(static_cast<uint32>(m_oScheduler.getFrequency()));

	m_ui64CurrentTimeToReach=0;
	m_ui64Lateness=0;
	m_eStatus=PlayerStatus_Stop;

	return true;

}

boolean CPlayer::uninitialize(void)
{
	if(m_eStatus==PlayerStatus_Uninitialized)
	{
		this->getLogManager() << LogLevel_Warning << "Trying to uninitialize an uninitialized player !\n";
		return false;
	}

	this->getLogManager() << LogLevel_Trace << "Player uninitialize\n";

	m_oScheduler.uninitialize();

	m_oKernelContextBridge.setConfigurationManager(NULL);
	delete m_pLocalConfigurationManager;
	m_pLocalConfigurationManager=NULL;

	m_eStatus=PlayerStatus_Uninitialized;
	return true;
}

boolean CPlayer::stop(void)
{
	if(m_eStatus==PlayerStatus_Uninitialized)
	{
		this->getLogManager() << LogLevel_Warning << "Player has to be initialized before to use it !\n";
		return false;
	}

	this->getLogManager() << LogLevel_Trace << "Player stop\n";

	m_eStatus=PlayerStatus_Stop;

	return true;
}

boolean CPlayer::pause(void)
{
	if(m_eStatus==PlayerStatus_Uninitialized)
	{
		this->getLogManager() << LogLevel_Warning << "Player has to be initialized before to use it !\n";
		return false;
	}

	this->getLogManager() << LogLevel_Trace << "Player pause\n";

	m_eStatus=PlayerStatus_Pause;

	return true;
}

boolean CPlayer::step(void)
{
	if(m_eStatus==PlayerStatus_Uninitialized)
	{
		this->getLogManager() << LogLevel_Warning << "Player has to be initialized before to use it !\n";
		return false;
	}

	this->getLogManager() << LogLevel_Trace << "Player step\n";

	m_eStatus=PlayerStatus_Step;

	return true;
}

boolean CPlayer::play(void)
{
	if(m_eStatus==PlayerStatus_Uninitialized)
	{
		this->getLogManager() << LogLevel_Warning << "Player has to be initialized before to use it !\n";
		return false;
	}

	this->getLogManager() << LogLevel_Trace << "Player play\n";

	m_eStatus=PlayerStatus_Play;

	return true;
}

boolean CPlayer::forward(void)
{
	if(m_eStatus==PlayerStatus_Uninitialized)
	{
		this->getLogManager() << LogLevel_Warning << "Player has to be initialized before to use it !\n";
		return false;
	}

	this->getLogManager() << LogLevel_Trace << "Player forward\n";

	m_eStatus=PlayerStatus_Forward;

	return true;
}

EPlayerStatus CPlayer::getStatus(void) const
{
	return m_eStatus;
}

float64 CPlayer::getCPUUsage(const CIdentifier& rProcessingUnitIdentifier) const
{
	return m_oScheduler.getCPUUsage();
}

boolean CPlayer::loop(
	const uint64 ui64ElapsedTime)
{
	if(m_eStatus==PlayerStatus_Uninitialized)
	{
		this->getLogManager() << LogLevel_Warning << "Player has to be initialized before to use it !\n";
		return false;
	}

	switch(m_eStatus)
	{
		// Calls a single controller loop and goes back to pause state
		case PlayerStatus_Step:
			m_ui64CurrentTimeToReach+=(1LL<<32)/m_oScheduler.getFrequency();
			m_eStatus=PlayerStatus_Pause;
			break;

		// Calls multiple controller loops
		case PlayerStatus_Forward:
			// We can't know what m_ui64CurrentTimeToReach should be in advance
			// We will try to do as many scheduler loops as possible until
			// _Scheduler_Maximum_Loops_Duration_ seconds elapsed
			break;

		// Simply updates time according to delta time
		case PlayerStatus_Play:
			m_ui64CurrentTimeToReach+=ui64ElapsedTime;
			break;

		default:
			return true;
			break;
	}

	uint64 l_ui64StartTime=System::Time::zgetTime();
	boolean l_bFinished=false;
	while(!l_bFinished)
	{
		if(m_eStatus!=PlayerStatus_Forward && m_oScheduler.getCurrentTime() > m_ui64CurrentTimeToReach)
		{
			l_bFinished=true;
		}
		else
		{
			m_oScheduler.loop();
		}
		if(System::Time::zgetTime() > l_ui64StartTime+_Scheduler_Maximum_Loops_Duration_)
		{
			l_bFinished=true;
		}
	}

	if(m_eStatus==PlayerStatus_Forward)
	{
		m_ui64CurrentTimeToReach=m_oScheduler.getCurrentTime();
	}

	uint64 l_ui64Lateness;
	if(m_ui64CurrentTimeToReach>m_oScheduler.getCurrentTime())
	{
		l_ui64Lateness=(m_ui64CurrentTimeToReach-m_oScheduler.getCurrentTime())>>32;
	}
	else
	{
		l_ui64Lateness=0;
	}

	if(l_ui64Lateness!=m_ui64Lateness)
	{
		this->getLogManager() << (l_ui64Lateness==0?LogLevel_Info:(l_ui64Lateness>=10?LogLevel_ImportantWarning:LogLevel_Warning))
			<< "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Player" << LogColor_PopStateBit
			<< "::" << LogColor_PushStateBit << LogColor_ForegroundBlue << "can not reach realtime" << LogColor_PopStateBit << "> "
			<< l_ui64Lateness << " second(s) late...\n";
		m_ui64Lateness=l_ui64Lateness;
	}

	return true;
}

uint64 CPlayer::getCurrentSimulatedTime(void) const
{
	return m_oScheduler.getCurrentTime();
}
