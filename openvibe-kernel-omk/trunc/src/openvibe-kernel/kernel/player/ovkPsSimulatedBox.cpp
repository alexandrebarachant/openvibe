#include "ovkPsSimulatedBox.h"
#include "ovkCBoxAlgorithmContext.h"
#include "ovkCMessageClock.h"
#include "ovkCMessageEvent.h"
#include "ovkCMessageSignal.h"

#include <iostream>

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
#define boolean OpenViBE::boolean

// #define _ScopeTester_
#define _MaxCrash_ 5

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

PsSimulatedBox::PsSimulatedBox(
	PsController& rController,
	const PsObjectDescriptor& rObjectDescriptor,
	const IKernelContext& rKernelContext,
	const IScenario& rScenario)
	:TKernelObject<IBoxIO>(rKernelContext)
	,PsSimulatedBoxBase(rController, rObjectDescriptor)
	,m_ui32CrashCount(0)
	,m_bReadyToProcess(false)
	,m_bActive(true)
	,m_pBoxAlgorithm(NULL)
	,m_pScenario(&rScenario)
	,m_pBox(NULL)
	,m_ui64LastClockActivationDate(0)
	,m_ui64ClockFrequency(0)
	,m_ui64ClockActivationStep(0)
	,m_bAlreadyClockActivatedOnce(false)
{
	log() << LogLevel_Debug << "PsSimulatedBox::PsSimulatedBox\n";
}

PsSimulatedBox::~PsSimulatedBox(void)
{
	log() << LogLevel_Debug << "PsSimulatedBox::~PsSimulatedBox\n";
}

void PsSimulatedBox::init(void)
{
	::PsSimulatedBoxBase::init();

	CIdentifier l_oIdentifier;
	::PsName l_oNameIdentifier=getName();
	::PsString l_oNameString=l_oNameIdentifier.getPsString();
	l_oIdentifier.fromString(l_oNameIdentifier.getCString());

	m_pBox=m_pScenario->getBoxDetails(l_oIdentifier);

	m_vInput.resize(m_pBox->getInputCount());
	m_vOutput.resize(m_pBox->getOutputCount());
	m_vCurrentOutput.resize(m_pBox->getOutputCount());

	log() << LogLevel_Debug << "PsSimulatedBox::init(" << CString(getName().getCString()) << "|" << m_pBox->getName() << ")\n";

	m_oBenchmarkChronoProcessClock.reset(1024);
	m_oBenchmarkChronoProcessInput.reset(1024);
	m_oBenchmarkChronoProcess.reset(1024);

	m_bAlreadyClockActivatedOnce=false;
}

void PsSimulatedBox::computeParameters(void)
{
	log() << LogLevel_Debug << "computeParameters::init(" << CString(getName().getCString()) << "|" << m_pBox->getName() << "|" << getSimulatedDate() << ")\n";

	if(!m_bActive) return;

	uint64 l_ui64CurrentDate=(((OpenViBE::uint64)this->getSimulatedDate())<<32);
	boolean l_bShouldProcessClock=(m_ui64ClockFrequency!=0) && (!m_bAlreadyClockActivatedOnce || (l_ui64CurrentDate-m_ui64LastClockActivationDate>=m_ui64ClockActivationStep));

	if(l_bShouldProcessClock)
	{
		CBoxAlgorithmContext l_oBoxAlgorithmContext(getKernelContext(), this, m_pBox);
#if defined _ScopeTester_
		Tools::CScopeTester l_oScopeTester("User code IBoxAlgorithm::processClock");
#endif
		try
		{
			m_oBenchmarkChronoProcessClock.stepIn();
			Kernel::CMessageClock l_oClockMessage(this->getKernelContext());
			m_ui64LastClockActivationDate+=(m_bAlreadyClockActivatedOnce?m_ui64ClockActivationStep:0);
			l_oClockMessage.setTime(m_ui64LastClockActivationDate/1000);
			m_pBoxAlgorithm->processClock(l_oBoxAlgorithmContext, l_oClockMessage);
			m_oBenchmarkChronoProcessClock.stepOut();
		}
		catch (...)
		{
			handleCrash("clock processing callback");
		}
		m_bReadyToProcess|=l_oBoxAlgorithmContext.isAlgorithmReadyToProcess();
		m_bAlreadyClockActivatedOnce=true;
	}

	if(l_bShouldProcessClock || m_ui64ClockFrequency==0)
	{
		CBoxAlgorithmContext l_oBoxAlgorithmContext(getKernelContext(), this, m_pBox);
#if defined _ScopeTester_
		Tools::CScopeTester l_oScopeTester("User code IBoxAlgorithm::getClockFrequency");
#endif
		try
		{
			// note: 1LL should be left shifted 64 bits but this
			//       would result in an integer over shift (the one
			//       would exit). Thus the left shift of 63 bits
			//       with a factor 2 multiplication after the division
			m_ui64ClockFrequency=m_pBoxAlgorithm->getClockFrequency(l_oBoxAlgorithmContext);
			m_ui64ClockActivationStep=1000*2*(m_ui64ClockFrequency?((1LL<<63)/m_ui64ClockFrequency):0);
		}
		catch (...)
		{
			handleCrash("clock frequency request callback");
		}
	}

	if(m_bReadyToProcess)
	{
		doProcess();
		m_bReadyToProcess=false;
	}

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
}

bool PsSimulatedBox::processOpenViBEDataUpdateEvent(::PsValuedEvent< ::PsTypeChunk >* pEvent)
{
	log() << LogLevel_Debug << "computeParameters::processOpenViBEDataUpdateEvent(" << CString(getName().getCString()) << "|" << m_pBox->getName() << "|" << pEvent->date << ")\n";

	if(!m_bActive) return false;

	m_vInput[pEvent->value.getIoConnectorIndex()].push_back(pEvent->value);

	CBoxAlgorithmContext l_oBoxAlgorithmContext(getKernelContext(), this, m_pBox);
	{
#if defined _ScopeTester_
		Tools::CScopeTester l_oScopeTester("User code IBoxAlgorithm::processInput");
#endif
		try
		{
			m_oBenchmarkChronoProcessInput.stepIn();
			m_pBoxAlgorithm->processInput(l_oBoxAlgorithmContext, pEvent->value.getIoConnectorIndex());
			m_oBenchmarkChronoProcessInput.stepOut();
		}
		catch (...)
		{
			handleCrash("input processing callback");
		}
	}
	m_bReadyToProcess|=l_oBoxAlgorithmContext.isAlgorithmReadyToProcess();

	return true ;
}

bool PsSimulatedBox::processMaskStartEvent(::PsEvent* pEvent)
{
	log() << LogLevel_Debug << "computeParameters::processMaskStartEvent(" << CString(getName().getCString()) << "|" << m_pBox->getName() << "|" << pEvent->date << ")\n";

	if(!m_bActive) return false;

	m_pBoxAlgorithm=dynamic_cast<IBoxAlgorithm*>(getKernelContext().getPluginManager().createPluginObject(m_pBox->getAlgorithmClassIdentifier()));
	CBoxAlgorithmContext l_oBoxAlgorithmContext(getKernelContext(), this, m_pBox);
	{
#if defined _ScopeTester_
		Tools::CScopeTester l_oScopeTester("User code IBoxAlgorithm::initialize");
#endif
		try
		{
			m_pBoxAlgorithm->initialize(l_oBoxAlgorithmContext);
		}
		catch (...)
		{
			handleCrash("initialization callback");
		}

#if defined _ScopeTester_
		Tools::CScopeTester l_oScopeTester("User code IBoxAlgorithm::getClockFrequency");
#endif
		try
		{
			// note: 1LL should be left shifted 64 bits but this
			//       would result in an integer over shift (the one
			//       would exit). Thus the left shift of 63 bits
			//       with a factor 2 multiplication after the division
			m_ui64ClockFrequency=m_pBoxAlgorithm->getClockFrequency(l_oBoxAlgorithmContext);
			m_ui64ClockActivationStep=1000*2*(m_ui64ClockFrequency?((1LL<<63)/m_ui64ClockFrequency):0);
		}
		catch (...)
		{
			handleCrash("clock frequency request callback");
		}
	}

	return true ;
}

bool PsSimulatedBox::processMaskStopEvent(::PsEvent* pEvent)
{
	log() << LogLevel_Debug << "computeParameters::processMaskStopEvent(" << CString(getName().getCString()) << "|" << m_pBox->getName() << "|" << pEvent->date << ")\n";

	if(!m_bActive) return false;

	CBoxAlgorithmContext l_oBoxAlgorithmContext(getKernelContext(), this, m_pBox);
	{
#if defined _ScopeTester_
		Tools::CScopeTester l_oScopeTester("User code IBoxAlgorithm::uninitialize");
#endif
		try
		{
			m_pBoxAlgorithm->uninitialize(l_oBoxAlgorithmContext);
		}
		catch (...)
		{
			handleCrash("uninitialization callback");
		}

		m_pBoxAlgorithm->release();
		m_pBoxAlgorithm=NULL;
	}

	return true ;
}

void PsSimulatedBox::handleCrash(const char* sHintName)
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

void PsSimulatedBox::doProcess(void)
{
	log() << LogLevel_Debug << "computeParameters::doProcess(" << CString(getName().getCString()) << "|" << m_pBox->getName() << ")\n";

	if(!m_bActive) return;

	CBoxAlgorithmContext l_oBoxAlgorithmContext(getKernelContext(), this, m_pBox);
	{
#if defined _ScopeTester_
		Tools::CScopeTester l_oScopeTester("User code IBoxAlgorithm::process");
#endif
		try
		{
			m_oBenchmarkChronoProcess.stepIn();
			m_pBoxAlgorithm->process(l_oBoxAlgorithmContext);
			m_oBenchmarkChronoProcess.stepOut();
		}
		catch (...)
		{
			handleCrash("processing callback");
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
			::PsName target(l_oTargetBoxIdentifier.toString());

			uint32 l_ui32SourceOutputIndex=l_pLink->getSourceBoxOutputIndex();
			list< ::PsTypeChunk >::iterator i=m_vOutput[l_ui32SourceOutputIndex].begin();
			while(i!=m_vOutput[l_ui32SourceOutputIndex].end())
			{
				i->setIoConnectorIndex(l_ui32TargetBoxInputIndex);
				sendOpenViBEDataUpdateEvent(target, (*i));
				++i;
			}
		}
		l_oLinkIdentifier=m_pScenario->getNextLinkIdentifierFromBox(l_oLinkIdentifier, m_pBox->getIdentifier());
	}

	// iterators for input and output chunks
	vector<list< ::PsTypeChunk > >::iterator i;
	list< ::PsTypeChunk >::iterator j;

	// perform input cleaning
	i=m_vInput.begin();
	while(i!=m_vInput.end())
	{
		j=i->begin();
		while(j!=i->end())
		{
			if(j->m_bDeprecated)
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
	j=m_vCurrentOutput.begin();
	while(j!=m_vCurrentOutput.end())
	{
		// checks buffer size
		if(j->getBuffer().getSize())
		{
			// the buffer has been (partially ?) filled but not sent
			CBoxAlgorithmContext l_oBoxAlgorithmContext(getKernelContext(), this, m_pBox);
			l_oBoxAlgorithmContext.getPlayerContext()->getLogManager() << LogLevel_Warning << "Output buffer filled but not marked as ready to send\n"; // $$$ should use log
			j->getBuffer().setSize(0, true);
		}

		++j;
	}
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// - --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

uint64 PsSimulatedBox::getCurrentTime(void) const
{
	if(m_ui64ClockFrequency==0)
	{
		uint64 l_ui64CurrentDate=(((OpenViBE::uint64)this->getSimulatedDate())<<32);
		return l_ui64CurrentDate;
	}
	return m_ui64LastClockActivationDate;
}

CString PsSimulatedBox::getOVName(void) const
{
	return m_pBox->getName();
}

const IScenario& PsSimulatedBox::getScenario(void) const
{
	return *m_pScenario;
}

uint32 PsSimulatedBox::getInputChunkCount(
	const uint32 ui32InputIndex) const
{
	if(ui32InputIndex>=m_vInput.size())
	{
		return false;
	}
	return m_vInput[ui32InputIndex].size();
}

boolean PsSimulatedBox::getInputChunk(
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

	const ::PsTypeChunk& l_rChunk=_my_get_(m_vInput[ui32InputIndex], ui32ChunkIndex);
	rStartTime=l_rChunk.getStartTime();
	rEndTime=l_rChunk.getEndTime();
	rChunkSize=l_rChunk.getBuffer().getSize();
	rpChunkBuffer=l_rChunk.getBuffer().getDirectPointer();
	return true;
}

const IMemoryBuffer* PsSimulatedBox::getInputChunk(
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

	return &_my_get_(m_vInput[ui32InputIndex], ui32ChunkIndex).getBuffer();
}

uint64 PsSimulatedBox::getInputChunkStartTime(
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

	const ::PsTypeChunk& l_rChunk=_my_get_(m_vInput[ui32InputIndex], ui32ChunkIndex);
	return l_rChunk.getStartTime();
}

uint64 PsSimulatedBox::getInputChunkEndTime(
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

	const ::PsTypeChunk& l_rChunk=_my_get_(m_vInput[ui32InputIndex], ui32ChunkIndex);
	return l_rChunk.getEndTime();
}

boolean PsSimulatedBox::markInputAsDeprecated(
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
	_my_get_(m_vInput[ui32InputIndex], ui32ChunkIndex).m_bDeprecated=true;
	return true;
}

uint64 PsSimulatedBox::getOutputChunkSize(
	const uint32 ui32OutputIndex) const
{
	if(ui32OutputIndex>=m_vCurrentOutput.size())
	{
		return false;
	}
	return _my_get_(m_vCurrentOutput, ui32OutputIndex).getBuffer().getSize();
}

boolean PsSimulatedBox::setOutputChunkSize(
	const uint32 ui32OutputIndex,
	const uint64 ui64Size,
	const boolean bDiscard)
{
	if(ui32OutputIndex>=m_vOutput.size())
	{
		return false;
	}
	return _my_get_(m_vCurrentOutput, ui32OutputIndex).getBuffer().setSize(ui64Size, bDiscard);
}

uint8* PsSimulatedBox::getOutputChunkBuffer(
	const uint32 ui32OutputIndex)
{
	if(ui32OutputIndex>=m_vOutput.size())
	{
		return false;
	}
	return _my_get_(m_vCurrentOutput, ui32OutputIndex).getBuffer().getDirectPointer();
}

boolean PsSimulatedBox::appendOutputChunkData(
	const OpenViBE::uint32 ui32OutputIndex,
	const OpenViBE::uint8* pBuffer,
	const OpenViBE::uint64 ui64BufferSize)
{
	if(ui32OutputIndex>=m_vOutput.size())
	{
		return false;
	}
	return _my_get_(m_vCurrentOutput, ui32OutputIndex).getBuffer().appendOutputChunkData(pBuffer, ui64BufferSize);
}

IMemoryBuffer* PsSimulatedBox::getOutputChunk(
	const uint32 ui32OutputIndex)
{
	if(ui32OutputIndex>=m_vOutput.size())
	{
		return false;
	}
	return &_my_get_(m_vCurrentOutput, ui32OutputIndex).getBuffer();
}

boolean PsSimulatedBox::markOutputAsReadyToSend(
	const uint32 ui32OutputIndex,
	const uint64 ui64StartTime,
	const uint64 ui64EndTime)
{
	if(ui32OutputIndex>=m_vOutput.size())
	{
		return false;
	}

	// sets start and end time
	_my_get_(m_vCurrentOutput, ui32OutputIndex).setStartTime(ui64StartTime);
	_my_get_(m_vCurrentOutput, ui32OutputIndex).setEndTime(ui64EndTime);

	// copies chunk
	m_vOutput[ui32OutputIndex].push_back(_my_get_(m_vCurrentOutput, ui32OutputIndex));

	// resets chunk size
	_my_get_(m_vCurrentOutput, ui32OutputIndex).getBuffer().setSize(0, true);

	return true;
}
