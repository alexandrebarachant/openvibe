// #if defined __MY_COMPILE_ALL

#include "ovkCSimulatedBox.h"
#include "ovkCPlayer.h"
#include "ovkCBoxAlgorithmContext.h"
#include "ovkCMessageClock.h"
#include "ovkCMessageEvent.h"
#include "ovkCMessageSignal.h"

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

#define _ScopeTester_
#define _MaxCrash_ 5

#define __OV_FUNC__ CString("unknown_function_name")
#define __OV_LINE__ uint32(__LINE__)
#define __OV_FILE__ CString(__FILE__)

// ________________________________________________________________________________________________________________
//

namespace
{
	template <typename T>
	T& _my_get_(list<T>& rList, uint32 ui32Index)
	{
		uint32 i;
		typename list<T>::iterator it=rList.begin();
		for(i=0; i<ui32Index; i++)
		{
			++it;
		}
		return *it;
	}

	template <typename T>
	const T& _my_get_(const list<T>& rList, uint32 ui32Index)
	{
		uint32 i;
		typename list<T>::const_iterator it=rList.begin();
		for(i=0; i<ui32Index; i++)
		{
			++it;
		}
		return *it;
	}
}

// ________________________________________________________________________________________________________________
//

#define _Bad_Time_ 0xffffffffffffffffll

CSimulatedBox::CSimulatedBox(const IKernelContext& rKernelContext, CScheduler& rScheduler)
	:TKernelObject<IBoxIO>(rKernelContext)
	,m_ui32CrashCount(0)
	,m_bReadyToProcess(false)
	,m_bActive(true)
	,m_pBoxAlgorithm(NULL)
	,m_pScenario(NULL)
	,m_pBox(NULL)
	,m_rScheduler(rScheduler)
	,m_ui64LastClockActivationDate(_Bad_Time_)
	,m_ui64ClockFrequency(0)
	,m_ui64ClockActivationStep(0)
{
	log() << LogLevel_Debug << __OV_FUNC__ << " - " << __OV_FILE__ << ":" << __OV_LINE__ << "\n";
}

CSimulatedBox::~CSimulatedBox(void)
{
	log() << LogLevel_Debug << __OV_FUNC__ << " - " << __OV_FILE__ << ":" << __OV_LINE__ << "\n";
}

boolean CSimulatedBox::setScenarioIdentifier(const CIdentifier& rScenarioIdentifier)
{
	// FIXME test if rScenario is a scenario identifier
	m_pScenario=&getScenarioManager().getScenario(rScenarioIdentifier);
	return m_pScenario!=NULL;
}

boolean CSimulatedBox::setBoxIdentifier(const CIdentifier& rBoxIdentifier)
{
	if(!m_pScenario)
	{
		return false;
	}

	m_pBox=m_pScenario->getBoxDetails(rBoxIdentifier);
	return m_pBox!=NULL;
}

boolean CSimulatedBox::initialize(void)
{
	log() << LogLevel_Debug << __OV_FUNC__ << " - " << __OV_FILE__ << ":" << __OV_LINE__ << "\n";

	if(!m_bActive) return false;

	// FIXME test for already initialized boxes etc
	if(!m_pBox) return false;
	if(!m_pScenario) return false;

	m_vInput.resize(m_pBox->getInputCount());
	m_vOutput.resize(m_pBox->getOutputCount());
	m_vCurrentOutput.resize(m_pBox->getOutputCount());

	m_oBenchmarkChronoProcessClock.reset(1024);
	m_oBenchmarkChronoProcessInput.reset(1024);
	m_oBenchmarkChronoProcess.reset(1024);

	m_ui64LastClockActivationDate=_Bad_Time_;
	m_ui64ClockFrequency=0;
	m_ui64ClockActivationStep=0;

	m_pBoxAlgorithm=dynamic_cast<IBoxAlgorithm*>(getKernelContext().getPluginManager().createPluginObject(m_pBox->getAlgorithmClassIdentifier()));
	if(!m_pBoxAlgorithm)
	{
		getLogManager() << LogLevel_Error << "Could not create box algorithm with class id " << m_pBox->getAlgorithmClassIdentifier() << "... This box will be deactivated but the whole scenario behavior will probably suffer !\n";
		m_bActive=false;
		return false;
	}

	{
		CBoxAlgorithmContext l_oBoxAlgorithmContext(getKernelContext(), this, m_pBox);
		{
#if defined _ScopeTester_
			Tools::CScopeTester l_oScopeTester(getKernelContext(), m_pBox->getName() + CString(" (IBoxAlgorithm::initialize)"));
#endif
			try
			{
				m_pBoxAlgorithm->initialize(l_oBoxAlgorithmContext);
			}
			catch (...)
			{
				this->handleCrash("initialization callback");
			}
		}
	}

	return true ;
}

boolean CSimulatedBox::uninitialize(void)
{
	log() << LogLevel_Debug << __OV_FUNC__ << " - " << __OV_FILE__ << ":" << __OV_LINE__ << "\n";

	if(!m_bActive) return false;

	{
		CBoxAlgorithmContext l_oBoxAlgorithmContext(getKernelContext(), this, m_pBox);
		{
#if defined _ScopeTester_
			Tools::CScopeTester l_oScopeTester(getKernelContext(), m_pBox->getName() + CString(" (IBoxAlgorithm::uninitialize)"));
#endif
			{
				try
				{
					m_pBoxAlgorithm->uninitialize(l_oBoxAlgorithmContext);
				}
				catch (...)
				{
					this->handleCrash("uninitialization callback");
				}
			}
		}
	}

	m_pBoxAlgorithm->release();
	m_pBoxAlgorithm=NULL;

	return true ;
}

boolean CSimulatedBox::processClock(void)
{
	log() << LogLevel_Debug << __OV_FUNC__ << " - " << __OV_FILE__ << ":" << __OV_LINE__ << "\n";

	if(!m_bActive) return false;

	{
		CBoxAlgorithmContext l_oBoxAlgorithmContext(getKernelContext(), this, m_pBox);
		{
#if defined _ScopeTester_
			Tools::CScopeTester l_oScopeTester(getKernelContext(), m_pBox->getName() + CString(" (IBoxAlgorithm::getClockFrequency)"));
#endif
			try
			{
				uint64 l_ui64NewClockFrequency=m_pBoxAlgorithm->getClockFrequency(l_oBoxAlgorithmContext);
				if(l_ui64NewClockFrequency==0)
				{
					m_ui64ClockActivationStep=_Bad_Time_;
					m_ui64LastClockActivationDate=_Bad_Time_;
				}
				else
				{
					// note: 1LL should be left shifted 64 bits but this
					//       would result in an integer over shift (the one
					//       would exit). Thus the left shift of 63 bits
					//       and the left shift of 1 bit after the division
					m_ui64ClockActivationStep=((1LL<<63)/l_ui64NewClockFrequency)<<1;
				}
				m_ui64ClockFrequency=l_ui64NewClockFrequency;
			}
			catch (...)
			{
				this->handleCrash("clock frequency request callback");
			}
		}
	}

	if((m_ui64ClockFrequency!=0) && (m_ui64LastClockActivationDate==_Bad_Time_ || m_rScheduler.getCurrentTime()-m_ui64LastClockActivationDate>=m_ui64ClockActivationStep))
	{
		CBoxAlgorithmContext l_oBoxAlgorithmContext(getKernelContext(), this, m_pBox);
		{
#if defined _ScopeTester_
			Tools::CScopeTester l_oScopeTester(getKernelContext(), m_pBox->getName() + CString(" (IBoxAlgorithm::processClock)"));
#endif
			try
			{
				m_oBenchmarkChronoProcessClock.stepIn();

				if(m_ui64LastClockActivationDate==_Bad_Time_)
				{
					m_ui64LastClockActivationDate=m_rScheduler.getCurrentTime();
				}
				else
				{
					m_ui64LastClockActivationDate=m_ui64LastClockActivationDate+m_ui64ClockActivationStep;
				}

				Kernel::CMessageClock l_oClockMessage(this->getKernelContext());
				l_oClockMessage.setTime(m_ui64LastClockActivationDate);
				m_pBoxAlgorithm->processClock(l_oBoxAlgorithmContext, l_oClockMessage);
				m_oBenchmarkChronoProcessClock.stepOut();
			}
			catch (...)
			{
				this->handleCrash("clock processing callback");
			}
			m_bReadyToProcess|=l_oBoxAlgorithmContext.isAlgorithmReadyToProcess();
		}
	}

	return true;
}

boolean CSimulatedBox::processInput(const uint32 ui32InputIndex, const CChunk& rChunk)
{
	log() << LogLevel_Debug << __OV_FUNC__ << " - " << __OV_FILE__ << ":" << __OV_LINE__ << "\n";

	if(!m_bActive) return false;

	m_vInput[ui32InputIndex].push_back(rChunk);

	{
		CBoxAlgorithmContext l_oBoxAlgorithmContext(getKernelContext(), this, m_pBox);
		{
#if defined _ScopeTester_
			Tools::CScopeTester l_oScopeTester(getKernelContext(), m_pBox->getName() + CString(" (IBoxAlgorithm::processInput)"));
#endif
			try
			{
				m_oBenchmarkChronoProcessInput.stepIn();
				m_pBoxAlgorithm->processInput(l_oBoxAlgorithmContext, ui32InputIndex);
				m_oBenchmarkChronoProcessInput.stepOut();
			}
			catch (...)
			{
				this->handleCrash("input processing callback");
			}
		}
		m_bReadyToProcess|=l_oBoxAlgorithmContext.isAlgorithmReadyToProcess();
	}

	return true;
}

boolean CSimulatedBox::process(void)
{
	log() << LogLevel_Debug << __OV_FUNC__ << " - " << __OV_FILE__ << ":" << __OV_LINE__ << "\n";

	if(!m_bActive) return false;
	if(!m_bReadyToProcess) return true;

	{
		CBoxAlgorithmContext l_oBoxAlgorithmContext(getKernelContext(), this, m_pBox);
		{
#if defined _ScopeTester_
			Tools::CScopeTester l_oScopeTester(getKernelContext(), m_pBox->getName() + CString(" (IBoxAlgorithm::process)"));
#endif
			try
			{
				m_oBenchmarkChronoProcess.stepIn();
				m_pBoxAlgorithm->process(l_oBoxAlgorithmContext);
				m_oBenchmarkChronoProcess.stepOut();
			}
			catch (...)
			{
				this->handleCrash("processing callback");
			}
		}
	}

	// perform output sending
	CIdentifier l_oLinkIdentifier=m_pScenario->getNextLinkIdentifierFromBox(OV_UndefinedIdentifier, m_pBox->getIdentifier());
	while(l_oLinkIdentifier!=OV_UndefinedIdentifier)
	{
		const ILink* l_pLink=m_pScenario->getLinkDetails(l_oLinkIdentifier);
		if(l_pLink)
		{
			CIdentifier l_oTargetBoxIdentifier=l_pLink->getTargetBoxIdentifier();
			uint32 l_ui32TargetBoxInputIndex=l_pLink->getTargetBoxInputIndex();

			uint32 l_ui32SourceOutputIndex=l_pLink->getSourceBoxOutputIndex();
			list < CChunk >::iterator i=m_vOutput[l_ui32SourceOutputIndex].begin();
			while(i!=m_vOutput[l_ui32SourceOutputIndex].end())
			{
				m_rScheduler.sendInput(*i, l_oTargetBoxIdentifier, l_ui32TargetBoxInputIndex);
				++i;
			}
		}
		l_oLinkIdentifier=m_pScenario->getNextLinkIdentifierFromBox(l_oLinkIdentifier, m_pBox->getIdentifier());
	}

	// iterators for input and output chunks
	vector < list< CChunk > >::iterator i;
	list < CChunk >::iterator j;
	vector < CChunk >::iterator k;

	// perform input cleaning
	i=m_vInput.begin();
	while(i!=m_vInput.end())
	{
		j=i->begin();
		while(j!=i->end())
		{
			if(j->isDeprecated())
			{
				j=i->erase(j);
			}
			else
			{
				++j;
			}
		}
		++i;
	}

	// flushes sent output chunks
	i=m_vOutput.begin();
	while(i!=m_vOutput.end())
	{
		i->resize(0);
		++i;
	}

	// discards waiting output chunks
	k=m_vCurrentOutput.begin();
	while(k!=m_vCurrentOutput.end())
	{
		// checks buffer size
		if(k->getBuffer().getSize())
		{
			// the buffer has been (partially ?) filled but not sent
			CBoxAlgorithmContext l_oBoxAlgorithmContext(getKernelContext(), this, m_pBox);
			l_oBoxAlgorithmContext.getPlayerContext()->getLogManager() << LogLevel_Warning << "Output buffer filled but not marked as ready to send\n"; // $$$ should use log
			k->getBuffer().setSize(0, true);
		}

		++k;
	}

	m_bReadyToProcess=false;

#if 0
/*-----------------------------------------------*/
/* TODO send this messages with better frequency */
	if(m_oBenchmarkChronoProcessClock.hasNewEstimation())
	{
		log() << LogLevel_Benchmark
			<< "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Player" << LogColor_PopStateBit
			<< "::" << LogColor_PushStateBit << LogColor_ForegroundBlue << "process clock" << LogColor_PopStateBit
			<< "::" << m_pBox->getName() << "> "
			<< "Average computing time is " << ((m_oBenchmarkChronoProcessClock.getAverageStepInDuration()*1000000)>>32) << "us\n";
	}
	if(m_oBenchmarkChronoProcessInput.hasNewEstimation())
	{
		log() << LogLevel_Benchmark
			<< "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Player" << LogColor_PopStateBit
			<< "::" << LogColor_PushStateBit << LogColor_ForegroundBlue << "process input" << LogColor_PopStateBit
			<< "::" << m_pBox->getName() << "> "
			<< "Average computing time is " << ((m_oBenchmarkChronoProcessInput.getAverageStepInDuration()*1000000)>>32) << "us\n";
	}
	if(m_oBenchmarkChronoProcess.hasNewEstimation())
	{
		log() << LogLevel_Benchmark
			<< "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Player" << LogColor_PopStateBit
			<< "::" << LogColor_PushStateBit << LogColor_ForegroundBlue << "process      " << LogColor_PopStateBit
			<< "::" << m_pBox->getName() << "> "
			<< "Average computing time is " << ((m_oBenchmarkChronoProcess.getAverageStepInDuration()*1000000)>>32) << "us\n";
	}
/* TODO Thank you for reading :)                 */
/*-----------------------------------------------*/
#endif

	return true;
}

boolean CSimulatedBox::isReadyToProcess(void) const
{
	return m_bReadyToProcess;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// - --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

uint64 CSimulatedBox::getCurrentTime(void) const
{
	return m_rScheduler.getCurrentTime();
}

CString CSimulatedBox::getName(void) const
{
	return m_pBox->getName();
}

const IScenario& CSimulatedBox::getScenario(void) const
{
	return *m_pScenario;
}

// ________________________________________________________________________________________________________________
//

uint32 CSimulatedBox::getInputChunkCount(
	const uint32 ui32InputIndex) const
{
	if(ui32InputIndex>=m_vInput.size())
	{
		return false;
	}
	return m_vInput[ui32InputIndex].size();
}

boolean CSimulatedBox::getInputChunk(
	const uint32 ui32InputIndex,
	const uint32 ui32ChunkIndex,
	uint64& rStartTime,
	uint64& rEndTime,
	uint64& rChunkSize,
	const uint8*& rpChunkBuffer) const
{
	if(ui32InputIndex>=m_vInput.size())
	{
		return false;
	}
	if(ui32ChunkIndex>=m_vInput[ui32InputIndex].size())
	{
		return false;
	}

	const CChunk& l_rChunk=_my_get_(m_vInput[ui32InputIndex], ui32ChunkIndex);
	rStartTime=l_rChunk.getStartTime();
	rEndTime=l_rChunk.getEndTime();
	rChunkSize=l_rChunk.getBuffer().getSize();
	rpChunkBuffer=l_rChunk.getBuffer().getDirectPointer();
	return true;
}

const IMemoryBuffer* CSimulatedBox::getInputChunk(
	const uint32 ui32InputIndex,
	const uint32 ui32ChunkIndex) const
{
	if(ui32InputIndex>=m_vInput.size())
	{
		return NULL;
	}
	if(ui32ChunkIndex>=m_vInput[ui32InputIndex].size())
	{
		return NULL;
	}
	return &_my_get_(m_vInput[ui32InputIndex], ui32ChunkIndex).getBuffer();
}

uint64 CSimulatedBox::getInputChunkStartTime(
	const uint32 ui32InputIndex,
	const uint32 ui32ChunkIndex) const
{
	if(ui32InputIndex>=m_vInput.size())
	{
		return false;
	}
	if(ui32ChunkIndex>=m_vInput[ui32InputIndex].size())
	{
		return false;
	}

	const CChunk& l_rChunk=_my_get_(m_vInput[ui32InputIndex], ui32ChunkIndex);
	return l_rChunk.getStartTime();
}

uint64 CSimulatedBox::getInputChunkEndTime(
	const uint32 ui32InputIndex,
	const uint32 ui32ChunkIndex) const
{
	if(ui32InputIndex>=m_vInput.size())
	{
		return false;
	}
	if(ui32ChunkIndex>=m_vInput[ui32InputIndex].size())
	{
		return false;
	}

	const CChunk& l_rChunk=_my_get_(m_vInput[ui32InputIndex], ui32ChunkIndex);
	return l_rChunk.getEndTime();
}

boolean CSimulatedBox::markInputAsDeprecated(
	const uint32 ui32InputIndex,
	const uint32 ui32ChunkIndex)
{
	if(ui32InputIndex>=m_vInput.size())
	{
		return false;
	}
	if(ui32ChunkIndex>=m_vInput[ui32InputIndex].size())
	{
		return false;
	}
	_my_get_(m_vInput[ui32InputIndex], ui32ChunkIndex).markAsDeprecated(true);
	return true;
}

// ________________________________________________________________________________________________________________
//

uint64 CSimulatedBox::getOutputChunkSize(
	const uint32 ui32OutputIndex) const
{
	if(ui32OutputIndex>=m_vCurrentOutput.size())
	{
		return 0;
	}
	return m_vCurrentOutput[ui32OutputIndex].getBuffer().getSize();
}

boolean CSimulatedBox::setOutputChunkSize(
	const uint32 ui32OutputIndex,
	const uint64 ui64Size,
	const boolean bDiscard)
{
	if(ui32OutputIndex>=m_vOutput.size())
	{
		return false;
	}
	return m_vCurrentOutput[ui32OutputIndex].getBuffer().setSize(ui64Size, bDiscard);
}

uint8* CSimulatedBox::getOutputChunkBuffer(
	const uint32 ui32OutputIndex)
{
	if(ui32OutputIndex>=m_vOutput.size())
	{
		return NULL;
	}
	return m_vCurrentOutput[ui32OutputIndex].getBuffer().getDirectPointer();
}

boolean CSimulatedBox::appendOutputChunkData(
	const uint32 ui32OutputIndex,
	const uint8* pBuffer,
	const uint64 ui64BufferSize)
{
	if(ui32OutputIndex>=m_vOutput.size())
	{
		return false;
	}
	return m_vCurrentOutput[ui32OutputIndex].getBuffer().appendOutputChunkData(pBuffer, ui64BufferSize);
}

IMemoryBuffer* CSimulatedBox::getOutputChunk(
	const uint32 ui32OutputIndex)
{
	if(ui32OutputIndex>=m_vOutput.size())
	{
		return NULL;
	}
	return &m_vCurrentOutput[ui32OutputIndex].getBuffer();
}

boolean CSimulatedBox::markOutputAsReadyToSend(
	const uint32 ui32OutputIndex,
	const uint64 ui64StartTime,
	const uint64 ui64EndTime)
{
	if(ui32OutputIndex>=m_vOutput.size())
	{
		return false;
	}

	// sets start and end time
	m_vCurrentOutput[ui32OutputIndex].setStartTime(ui64StartTime);
	m_vCurrentOutput[ui32OutputIndex].setEndTime(ui64EndTime);

	// copies chunk
	m_vOutput[ui32OutputIndex].push_back(m_vCurrentOutput[ui32OutputIndex]);

	// resets chunk size
	m_vCurrentOutput[ui32OutputIndex].getBuffer().setSize(0, true);

	return true;
}

// ________________________________________________________________________________________________________________
//

void CSimulatedBox::handleCrash(const char* sHintName)
{
	m_ui32CrashCount++;

	log() << LogLevel_Error << "Plugin code caused crash " << m_ui32CrashCount << " time(s)\n";
	log() << LogLevel_Error << "  [name:" << m_pBox->getName() << "]\n";
	log() << LogLevel_Error << "  [identifier:" << m_pBox->getIdentifier() << "]\n";
	log() << LogLevel_Error << "  [algorithm class identifier:" << m_pBox->getAlgorithmClassIdentifier() << "]\n";
	log() << LogLevel_Error << "  [place:" << sHintName << "]\n";

	if(m_ui32CrashCount>=_MaxCrash_)
	{
		log () << LogLevel_Fatal << "  This plugin has been disabled !\n";
		m_bActive=false;
	}
}

// #endif // __MY_COMPILE_ALL
