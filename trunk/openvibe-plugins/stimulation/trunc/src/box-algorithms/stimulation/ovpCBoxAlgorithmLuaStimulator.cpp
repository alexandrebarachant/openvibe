#include "ovpCBoxAlgorithmLuaStimulator.h"

#if defined TARGET_HAS_ThirdPartyLua

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Stimulation;

namespace
{
	class CLuaThread
	{
	public:

		CLuaThread(CBoxAlgorithmLuaStimulator* pBox)
			:m_pBox(pBox)
		{
		}

		CLuaThread(const CLuaThread& rLuaThread)
			:m_pBox(rLuaThread.m_pBox)
		{
		}

		void operator() (void)
		{
			m_pBox->doThread();
		}

	protected:

		CBoxAlgorithmLuaStimulator* m_pBox;
	};
}

static void lua_setcallback(lua_State* pState, const char* sName, int (*fpCallback)(lua_State* pState), void* pUserData)
{
	lua_pushlightuserdata(pState, pUserData);
	lua_pushcclosure(pState, fpCallback, 1);
	lua_setglobal(pState, sName);
}

static void lua_report(ILogManager& rLogManager, lua_State* pLuaState, int iCode)
{
	if(iCode)
	{
		rLogManager << LogLevel_ImportantWarning << "Lua error: " << lua_tostring(pLuaState, -1) << "\n";
		lua_pop(pLuaState, 1);
	}
}

static int lua_get_current_time_cb(lua_State* pState)
{
	int l_iArguments=lua_gettop(pState);
	if(l_iArguments!=0)
	{
		lua_pushstring(pState, "get_current_time takes 0 parameter");
		lua_error(pState);
		return 0;
	}

	CBoxAlgorithmLuaStimulator* l_pThis=static_cast < CBoxAlgorithmLuaStimulator* >(lua_touserdata(pState, lua_upvalueindex(1)));
	if(l_pThis)
	{
		if(l_pThis->m_ui32State != CBoxAlgorithmLuaStimulator::State_Processing)
		{
			lua_pushstring(pState, "You should only call openvibe functions in process");
			lua_error(pState);
			return 0;
		}

		uint64 l_ui64Time=0;
		if(!l_pThis->getCurrentTimeCB(l_ui64Time))
		{
			lua_pushstring(pState, "get_current_time failed for unknown reason");
			lua_error(pState);
			return 0;
		}
		else
		{
			lua_pushnumber(pState, (float64(l_ui64Time))/(1LL<<32));
			return 1;
		}
	}
	return 0;
}

static int lua_sleep_cb(lua_State* pState)
{
	int l_iArguments=lua_gettop(pState);
	if(l_iArguments!=0)
	{
		lua_pushstring(pState, "sleep takes 0 parameter");
		lua_error(pState);
		return 0;
	}

	CBoxAlgorithmLuaStimulator* l_pThis=static_cast < CBoxAlgorithmLuaStimulator* >(lua_touserdata(pState, lua_upvalueindex(1)));
	if(l_pThis)
	{
		if(l_pThis->m_ui32State != CBoxAlgorithmLuaStimulator::State_Processing)
		{
			lua_pushstring(pState, "You should only call openvibe functions in process");
			lua_error(pState);
			return 0;
		}

		if(!l_pThis->sleepCB())
		{
			lua_pushstring(pState, "sleep failed for unknown reason");
			lua_error(pState);
			return 0;
		}
	}
	return 0;
}

static int lua_get_stimulation_count_cb(lua_State* pState)
{
	int l_iArguments=lua_gettop(pState);
	if(l_iArguments!=1)
	{
		lua_pushstring(pState, "get_stimulation_count takes 1 parameter");
		lua_error(pState);
		return 0;
	}

	CBoxAlgorithmLuaStimulator* l_pThis=static_cast < CBoxAlgorithmLuaStimulator* >(lua_touserdata(pState, lua_upvalueindex(1)));
	if(l_pThis)
	{
		if(l_pThis->m_ui32State != CBoxAlgorithmLuaStimulator::State_Processing)
		{
			lua_pushstring(pState, "You should only call openvibe functions in process");
			lua_error(pState);
			return 0;
		}

		uint32 l_ui32Count=0;
		if(!l_pThis->getStimulationCountCB(lua_tointeger(pState, 1)-1, l_ui32Count))
		{
			lua_pushstring(pState, "get_stimulation_count failed for unknown reason");
			lua_error(pState);
			return 0;
		}
		else
		{
			lua_pushinteger(pState, l_ui32Count);
			return 1;
		}
	}
	return 0;
}

static int lua_get_stimulation_cb(lua_State* pState)
{
	int l_iArguments=lua_gettop(pState);
	if(l_iArguments!=2)
	{
		lua_pushstring(pState, "get_stimulation takes 2 parameter");
		lua_error(pState);
		return 0;
	}

	CBoxAlgorithmLuaStimulator* l_pThis=static_cast < CBoxAlgorithmLuaStimulator* >(lua_touserdata(pState, lua_upvalueindex(1)));
	if(l_pThis)
	{
		if(l_pThis->m_ui32State != CBoxAlgorithmLuaStimulator::State_Processing)
		{
			lua_pushstring(pState, "You should only call openvibe functions in process");
			lua_error(pState);
			return 0;
		}

		uint64 l_ui64Identifier;
		uint64 l_ui64Time;
		uint64 l_ui64Duration;
		if(!l_pThis->getStimulationCB(lua_tointeger(pState, 1)-1, lua_tointeger(pState, 2)-1, l_ui64Identifier, l_ui64Time, l_ui64Duration))
		{
			lua_pushstring(pState, "get_stimulation failed for unknown reason");
			lua_error(pState);
			return 0;
		}
		else
		{
			lua_pushinteger(pState, l_ui64Identifier);
			lua_pushnumber(pState, l_ui64Time/float64(1LL<<32));
			lua_pushnumber(pState, l_ui64Duration/float64(1LL<<32));
			return 3;
		}
	}
	return 0;
}

static int lua_remove_stimulation_cb(lua_State* pState)
{
	int l_iArguments=lua_gettop(pState);
	if(l_iArguments!=2)
	{
		lua_pushstring(pState, "remove_stimulation takes 2 parameter");
		lua_error(pState);
		return 0;
	}

	CBoxAlgorithmLuaStimulator* l_pThis=static_cast < CBoxAlgorithmLuaStimulator* >(lua_touserdata(pState, lua_upvalueindex(1)));
	if(l_pThis)
	{
		if(l_pThis->m_ui32State != CBoxAlgorithmLuaStimulator::State_Processing)
		{
			lua_pushstring(pState, "You should only call openvibe functions in process");
			lua_error(pState);
			return 0;
		}

		if(!l_pThis->removeStimulationCB(lua_tointeger(pState, 1)-1, lua_tointeger(pState, 2)-1))
		{
			lua_pushstring(pState, "remove_stimulation failed for unknown reason");
			lua_error(pState);
			return 0;
		}
	}
	return 0;
}

static int lua_send_stimulation_cb(lua_State* pState)
{
	int l_iArguments=lua_gettop(pState);
	if(l_iArguments!=4 && l_iArguments!=3)
	{
		lua_pushstring(pState, "send_stimulation takes either 3 or 4 parameter");
		lua_error(pState);
		return 0;
	}

	CBoxAlgorithmLuaStimulator* l_pThis=static_cast < CBoxAlgorithmLuaStimulator* >(lua_touserdata(pState, lua_upvalueindex(1)));
	if(l_pThis)
	{
		if(l_pThis->m_ui32State != CBoxAlgorithmLuaStimulator::State_Processing)
		{
			lua_pushstring(pState, "You should only call openvibe functions in process");
			lua_error(pState);
			return 0;
		}

		if(!l_pThis->sendStimulationCB(
			lua_tointeger(pState, 1)-1,
			lua_tointeger(pState, 2),
			uint64(lua_tonumber(pState, 3)*(1LL<<32)),
			l_iArguments==4?uint64(lua_tonumber(pState, 4)*(1LL<<32)):0))
		{
			lua_pushstring(pState, "send_stimulation failed for unknown reason");
			lua_error(pState);
			return 0;
		}
	}

	return 0;
}

CBoxAlgorithmLuaStimulator::CBoxAlgorithmLuaStimulator(void)
#if BOOST_VERSION >= 103500
	:m_oInnerLock(m_oMutex, boost::defer_lock)
	,m_oOuterLock(m_oMutex, boost::defer_lock)
#else
	:m_oInnerLock(m_oMutex, false)
	,m_oOuterLock(m_oMutex, false)
#endif
{
}

uint64 CBoxAlgorithmLuaStimulator::getClockFrequency(void)
{
	return 128LL<<32; // the box clock frequency
}

boolean CBoxAlgorithmLuaStimulator::processClock(CMessageClock& rMessageClock)
{
	this->getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CBoxAlgorithmLuaStimulator::initialize(void)
{
	uint32 i;
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();

	CString l_sLuaScriptFilename=FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 0);
	l_sLuaScriptFilename=this->getConfigurationManager().expand(l_sLuaScriptFilename);

	m_ui32State=State_Unstarted;
	m_pLuaState=lua_open();

	luaL_openlibs(m_pLuaState);

	lua_setcallback(m_pLuaState, "get_current_time", ::lua_get_current_time_cb, this);
	lua_setcallback(m_pLuaState, "sleep", ::lua_sleep_cb, this);
	lua_setcallback(m_pLuaState, "get_stimulation_count", ::lua_get_stimulation_count_cb, this);
	lua_setcallback(m_pLuaState, "get_stimulation", ::lua_get_stimulation_cb, this);
	lua_setcallback(m_pLuaState, "remove_stimulation", ::lua_remove_stimulation_cb, this);
	lua_setcallback(m_pLuaState, "send_stimulation", ::lua_send_stimulation_cb, this);

	lua_report(this->getLogManager(), m_pLuaState, luaL_dostring(m_pLuaState, "function initialize() end"));
	lua_report(this->getLogManager(), m_pLuaState, luaL_dostring(m_pLuaState, "function uninitialize() end"));
	lua_report(this->getLogManager(), m_pLuaState, luaL_dostring(m_pLuaState, "function process() end"));
	lua_report(this->getLogManager(), m_pLuaState, luaL_dofile(m_pLuaState, l_sLuaScriptFilename.toASCIIString()));

	lua_report(this->getLogManager(), m_pLuaState, luaL_dostring(m_pLuaState, "initialize()"));

	m_vInputStimulation.resize(l_rStaticBoxContext.getInputCount());
	m_vOutputStimulation.resize(l_rStaticBoxContext.getOutputCount());

	for(i=0; i<l_rStaticBoxContext.getInputCount(); i++)
	{
		IAlgorithmProxy* l_pSteamDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
		l_pSteamDecoder->initialize();
		m_vStreamDecoder.push_back(l_pSteamDecoder);
	}

	for(i=0; i<l_rStaticBoxContext.getOutputCount(); i++)
	{
		IAlgorithmProxy* l_pStreamEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamEncoder));
		l_pStreamEncoder->initialize();
		m_vStreamEncoder.push_back(l_pStreamEncoder);
	}

	m_ui64LastTime=0;
	m_pLuaThread=NULL;

	return true;
}

boolean CBoxAlgorithmLuaStimulator::uninitialize(void)
{
	uint32 i;
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();

	for(i=0; i<l_rStaticBoxContext.getInputCount(); i++)
	{
		m_vStreamDecoder[i]->uninitialize();
		this->getAlgorithmManager().releaseAlgorithm(*m_vStreamDecoder[i]);
	}
	m_vStreamDecoder.clear();

	for(i=0; i<l_rStaticBoxContext.getOutputCount(); i++)
	{
		m_vStreamEncoder[i]->uninitialize();
		this->getAlgorithmManager().releaseAlgorithm(*m_vStreamEncoder[i]);
	}
	m_vStreamEncoder.clear();

	delete m_pLuaThread;

	lua_report(this->getLogManager(), m_pLuaState, luaL_dostring(m_pLuaState, "uninitialize()"));
	lua_close(m_pLuaState);

	return true;
}

boolean CBoxAlgorithmLuaStimulator::process(void)
{
	uint32 i, j, k;
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	uint64 l_ui64CurrentTime=this->getPlayerContext().getCurrentTime();

	for(i=0; i<l_rStaticBoxContext.getInputCount(); i++)
	{
		for(j=0; j<l_rDynamicBoxContext.getInputChunkCount(i); j++)
		{
			TParameterHandler < const IMemoryBuffer* > ip_pMemoryBuffer(m_vStreamDecoder[i]->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
			TParameterHandler < const IStimulationSet* > op_pStimulationSet(m_vStreamDecoder[i]->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));;
			ip_pMemoryBuffer=l_rDynamicBoxContext.getInputChunk(i, j);
			m_vStreamDecoder[i]->process();
			if(m_vStreamDecoder[i]->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedHeader))
			{
			}
			if(m_vStreamDecoder[i]->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
			{
				for(k=0; k<op_pStimulationSet->getStimulationCount(); k++)
				{
					m_vInputStimulation[i].insert(std::make_pair(op_pStimulationSet->getStimulationDate(k), std::make_pair(op_pStimulationSet->getStimulationIdentifier(k), op_pStimulationSet->getStimulationDuration(k))));
				}
			}
			if(m_vStreamDecoder[i]->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedEnd))
			{
			}
			l_rDynamicBoxContext.markInputAsDeprecated(i, j);
		}
	}

	m_oOuterLock.lock();

	// Executes one step of the thread
	switch(m_ui32State)
	{
		case State_Unstarted:
			m_ui32State=State_Processing;
			m_pLuaThread=new boost::thread(CLuaThread(this));
			m_oCondition.wait(m_oOuterLock);
			break;

		case State_Processing:
			m_oCondition.notify_one();
			m_oCondition.wait(m_oOuterLock);
			break;

		case State_Finished:
			break;

		default:
			break;
	}

	// Executes one step of the box once the thread has finished
	if(m_pLuaThread)
	{
		switch(m_ui32State)
		{
			case State_Unstarted:
				break; // This should never happen

			case State_Processing:
				break;

			case State_Finished:
				if(m_pLuaThread)
				{
					m_pLuaThread->join();
					delete m_pLuaThread;
					m_pLuaThread=NULL;
					this->getLogManager() << LogLevel_Info << "Lua script terminated\n";
				}
				break;

			default:
				break;
		}
	}

	m_oOuterLock.unlock();

	for(i=0; i<l_rStaticBoxContext.getOutputCount(); i++)
	{
		CStimulationSet l_oStimulationSet;
		TParameterHandler < const IStimulationSet* > ip_pStimulationSet(m_vStreamEncoder[i]->getInputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_InputParameterId_StimulationSet));
		TParameterHandler < IMemoryBuffer* > op_pMemoryBuffer(m_vStreamEncoder[i]->getOutputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
		ip_pStimulationSet=&l_oStimulationSet;
		op_pMemoryBuffer=l_rDynamicBoxContext.getOutputChunk(i);

		std::multimap < uint64, std::pair < uint64, uint64 > >::iterator itStimulation = m_vOutputStimulation[i].begin();
		while(itStimulation!=m_vOutputStimulation[i].end() && itStimulation->first <= l_ui64CurrentTime)
		{
			std::multimap < uint64, std::pair < uint64, uint64 > >::iterator l_itStimulation = itStimulation;
			itStimulation++;

			l_oStimulationSet.appendStimulation(
				l_itStimulation->second.first,
				l_itStimulation->first,
				l_itStimulation->second.second);
			this->getLogManager() << LogLevel_Debug << "On output " << i << " - should send stimulation " << l_itStimulation->second.first << " at date " << l_itStimulation->first << " with duration " << l_itStimulation->second.second << "\n";

			m_vOutputStimulation[i].erase(l_itStimulation);
		}

		if(l_ui64CurrentTime==m_ui64LastTime)
		{
			m_vStreamEncoder[i]->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeHeader);
		}
		else
		{
			m_vStreamEncoder[i]->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeBuffer);
		}
		l_rDynamicBoxContext.markOutputAsReadyToSend(i, m_ui64LastTime, l_ui64CurrentTime);
	}

	m_ui64LastTime=l_ui64CurrentTime;

	return true;
}

boolean CBoxAlgorithmLuaStimulator::_waitForStimulation(uint32 ui32InputIndex, uint32 ui32StimulationIndex)
{
	if(ui32InputIndex >= m_vInputStimulation.size())
	{
		this->getLogManager() << LogLevel_ImportantWarning << "Input " << ui32InputIndex << " does not exist\n";
		return false;
	}

	while(m_vInputStimulation[ui32InputIndex].size() <= ui32StimulationIndex)
	{
		switch(m_ui32State)
		{
			case State_Unstarted:
				return false; // this should never happen

			case State_Finished:
				return false;

			case State_Processing:
				this->sleepCB();
				break;
		}
	}

	return true;
}

boolean CBoxAlgorithmLuaStimulator::getCurrentTimeCB(uint64& rui64Time)
{
	rui64Time=this->getPlayerContext().getCurrentTime();
	return true;
}

boolean CBoxAlgorithmLuaStimulator::sleepCB(void)
{
	m_oCondition.notify_one();
	m_oCondition.wait(m_oInnerLock);
	return true;
}

boolean CBoxAlgorithmLuaStimulator::getStimulationCountCB(uint32 ui32InputIndex, uint32& rui32Count)
{
	if(ui32InputIndex >= m_vInputStimulation.size())
	{
		this->getLogManager() << LogLevel_Warning << "Input " << ui32InputIndex+1 << " does not exist\n";
		rui32Count = 0;
		return true;
	}

	rui32Count=m_vInputStimulation[ui32InputIndex].size();
	return true;
}

boolean CBoxAlgorithmLuaStimulator::getStimulationCB(uint32 ui32InputIndex, uint32 ui32StimulationIndex, uint64& rui64Identifier, uint64& rui64Time, uint64& rui64Duration)
{
	if(!this->_waitForStimulation(ui32InputIndex, ui32StimulationIndex))
	{
		return false;
	}

	std::multimap < uint64, std::pair < uint64, uint64 > >::iterator l_itStimulation=m_vInputStimulation[ui32InputIndex].begin();

	for(uint32 i=0; i<ui32StimulationIndex; i++, l_itStimulation++);

	rui64Identifier=l_itStimulation->second.first;
	rui64Time=l_itStimulation->first;
	rui64Duration=l_itStimulation->second.second;

	this->getLogManager() << LogLevel_Debug << "getStimulationCB " << ui32InputIndex+1 << " " << ui32StimulationIndex+1 << " " << rui64Identifier << " " << rui64Time << " " << rui64Duration << "\n";

	return true;
}

boolean CBoxAlgorithmLuaStimulator::removeStimulationCB(uint32 ui32InputIndex, uint32 ui32StimulationIndex)
{
	if(!this->_waitForStimulation(ui32InputIndex, ui32StimulationIndex))
	{
		return false;
	}

	std::multimap < uint64, std::pair < uint64, uint64 > >::iterator l_itStimulation=m_vInputStimulation[ui32InputIndex].begin();

	for(uint32 i=0; i<ui32StimulationIndex; i++, l_itStimulation++);

	m_vInputStimulation[ui32InputIndex].erase(l_itStimulation);

	this->getLogManager() << LogLevel_Debug << "removeStimulationCB " << ui32InputIndex+1 << " " << ui32StimulationIndex+1 << "\n";

	return true;
}

boolean CBoxAlgorithmLuaStimulator::sendStimulationCB(uint32 ui32OutputIndex, uint64 ui64Identifier, uint64 ui64Time, uint64 ui64Duration)
{
	if(ui32OutputIndex < m_vOutputStimulation.size())
	{
		if(ui64Time >= this->getPlayerContext().getCurrentTime())
		{
			this->getLogManager() << LogLevel_Debug << "sendStimulationCB " << ui32OutputIndex+1 << " " << ui64Identifier << " " << ui64Time << " " << ui64Duration << "\n";
			m_vOutputStimulation[ui32OutputIndex].insert(std::make_pair(ui64Time, std::make_pair(ui64Identifier, ui64Duration)));
		}
		else
		{
			this->getLogManager() << LogLevel_ImportantWarning << "Ignored outdated stimulation " << ui64Identifier << " " << ui64Time << " " << ui64Duration << " sent on output " << ui32OutputIndex+1 << " - current time is " << this->getPlayerContext().getCurrentTime() << "\n";
		}
	}
	else
	{
		this->getLogManager() << LogLevel_ImportantWarning << "Ignored stimulation " << ui64Identifier << " " << ui64Time << " " << ui64Duration << " sent on unexistant output " << ui32OutputIndex+1 << "\n";
	}
	return true;
}

void CBoxAlgorithmLuaStimulator::doThread(void)
{
	m_oInnerLock.lock();
	lua_report(this->getLogManager(), m_pLuaState, luaL_dostring(m_pLuaState, "process()"));
	m_ui32State=State_Finished;
	m_oInnerLock.unlock();

	m_oCondition.notify_one();
}

#endif // TARGET_HAS_ThirdPartyLua
