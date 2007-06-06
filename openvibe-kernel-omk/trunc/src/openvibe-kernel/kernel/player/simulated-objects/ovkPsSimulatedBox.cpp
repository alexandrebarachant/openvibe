#include "ovkPsSimulatedBox.h"

#include "../ovkCBoxAlgorithmContext.h"
#include "../ovkCMessageClock.h"
#include "../ovkCMessageEvent.h"
#include "../ovkCMessageSignal.h"

#include <iostream>

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
#define boolean OpenViBE::boolean

// #define _ScopeTester_
// #define _OMK_BehaviorTester_
#define _MaxCrash_ 5

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
	,m_ui64Time(0)
{
#ifdef _OMK_BehaviorTester_
	cout<<"PsSimulatedBox::PsSimulatedBox"<<endl;
#endif
}

PsSimulatedBox::~PsSimulatedBox(void)
{
#ifdef _OMK_BehaviorTester_
	cout<<"PsSimulatedBox::~PsSimulatedBox"<<endl;
#endif
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

#ifdef _OMK_BehaviorTester_
	cout<<"PsSimulatedBox::init("<<getName()<<"|"<<m_pBox->getName()<<")"<<endl;
#endif
}

void PsSimulatedBox::computeParameters(void)
{
#ifdef _OMK_BehaviorTester_
	cout<<"PsSimulatedBox::computeParameters("<<getName()<<"|"<<m_pBox->getName()<<")"<<endl;
#endif

	if(!m_bActive) return;

	CBoxAlgorithmContext l_oBoxAlgorithmContext(getKernelContext(), this, m_pBox);
	{
#if defined _ScopeTester_
		Tools::CScopeTester l_oScopeTester("User code IBoxAlgorithm::processClock");
#endif
		try
		{
			// $$$$$$$$$$
			IKernelContext* l_pContext=NULL;
			Kernel::CMessageClock m(*l_pContext);
			m.setTime(m_ui64Time);
			m_pBoxAlgorithm->processClock(l_oBoxAlgorithmContext, m);
			m_ui64Time+=0x00100000;
		}
		catch (...)
		{
			handleCrash("clock processing callback");
		}
	}
	m_bReadyToProcess|=l_oBoxAlgorithmContext.isAlgorithmReadyToProcess();

	if(m_bReadyToProcess)
	{
		doProcess();
		m_bReadyToProcess=false;
	}
}

bool PsSimulatedBox::processOpenViBEDataUpdateEvent(::PsValuedEvent< ::PsTypeChunk >* pEvent)
{
#ifdef _OMK_BehaviorTester_
	cout<<"PsSimulatedBox::processOpenViBEDataUpdateEvent("<<getName()<<"|"<<m_pBox->getName()<<"|"<<pEvent->date<<")"<<endl;
#endif

	if(!m_bActive) return false;

	m_vInput[pEvent->value.getIoConnectorIndex()].push_back(pEvent->value);

	CBoxAlgorithmContext l_oBoxAlgorithmContext(getKernelContext(), this, m_pBox);
	{
#if defined _ScopeTester_
		Tools::CScopeTester l_oScopeTester("User code IBoxAlgorithm::processInput");
#endif
		try
		{
			m_pBoxAlgorithm->processInput(l_oBoxAlgorithmContext, pEvent->value.getIoConnectorIndex());
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
#ifdef _OMK_BehaviorTester_
	cout<<"PsSimulatedBox::processMaskStartEvent("<<getName()<<"|"<<m_pBox->getName()<<"|"<<pEvent->date<<")"<<endl;
#endif

	if(!m_bActive) return false;

//	m_pBoxAlgorithmDesc=dynamic_cast<const IBoxAlgorithmDesc*>(getKernelContext().getPluginManager().getPluginObjectDescCreating(m_pBox->getAlgorithmClassIdentifier()));
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
	}

	return true ;
}

bool PsSimulatedBox::processMaskStopEvent(::PsEvent* pEvent)
{
#ifdef _OMK_BehaviorTester_
	cout<<"PsSimulatedBox::processMaskStopEvent("<<getName()<<"|"<<m_pBox->getName()<<"|"<<pEvent->date<<")"<<endl;
#endif

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
		log () << LogLevel_Error << "  This box has been disabled !\n";
		m_bActive=false;
	}
}

void PsSimulatedBox::doProcess(void)
{
#ifdef _OMK_BehaviorTester_
	cout<<"PsSimulatedBox::doProcess("<<getName()<<"|"<<m_pBox->getName()<<")"<<endl;
#endif

	if(!m_bActive) return;

	CBoxAlgorithmContext l_oBoxAlgorithmContext(getKernelContext(), this, m_pBox);
	{
#if defined _ScopeTester_
		Tools::CScopeTester l_oScopeTester("User code IBoxAlgorithm::process");
#endif
		try
		{
			m_pBoxAlgorithm->process(l_oBoxAlgorithmContext);
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
			vector< ::PsTypeChunk >::iterator i=m_vOutput[l_ui32SourceOutputIndex].begin();
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
	vector<vector< ::PsTypeChunk > >::iterator i;
	vector< ::PsTypeChunk >::iterator j;

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
			cout << "[WARNING] Output buffer filled but not marked as ready to send\n"; // $$$ should use log
			j->getBuffer().setSize(0, true);
		}

		++j;
	}
}

#if 0
boolean PsSimulatedBox::callback(const IScenario& rScenario, ILink& rLink)
{
	CIdentifier l_oTargetBoxIdentifier=rLink.getTargetBoxIdentifier();
	uint32 l_ui32TargetBoxInputIndex=rLink.getTargetBoxInputIndex();
	::PsName target(l_oTargetBoxIdentifier.toString());

	uint32 l_ui32SourceOutputIndex=rLink.getSourceBoxOutputIndex();
	vector< ::PsTypeChunk >::iterator i=m_vOutput[l_ui32SourceOutputIndex].begin();
	while(i!=m_vOutput[l_ui32SourceOutputIndex].end())
	{
		i->setIoConnectorIndex(l_ui32TargetBoxInputIndex);
		sendOpenViBEDataUpdateEvent(target, (*i));
		++i;
	}
	return true;
}
#endif

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// - --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

uint32 PsSimulatedBox::getInputChunkCount(
	const uint32 ui32InputIndex) const
{
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
	if(ui32ChunkIndex>=m_vInput[ui32InputIndex].size())
	{
		return false;
	}

	const ::PsTypeChunk& l_rChunk=m_vInput[ui32InputIndex][ui32ChunkIndex];
	rStartTime=l_rChunk.getStartTime();
	rEndTime=l_rChunk.getEndTime();
	rChunkSize=l_rChunk.getBuffer().getSize();
	rpChunkBuffer=l_rChunk.getBuffer().getDirectPointer();
	return true;
}

boolean PsSimulatedBox::markInputAsDeprecated(
	const uint32 ui32InputIndex,
	const uint32 ui32ChunkIndex)
{
	if(ui32ChunkIndex>=m_vInput[ui32InputIndex].size())
	{
		return false;
	}
	m_vInput[ui32InputIndex][ui32ChunkIndex].m_bDeprecated=true;
	return true;
}

uint64 PsSimulatedBox::getOutputChunkSize(
	const uint32 ui32OutputIndex) const
{
	return m_vCurrentOutput[ui32OutputIndex].getBuffer().getSize();
}

boolean PsSimulatedBox::setOutputChunkSize(
	const uint32 ui32OutputIndex,
	const uint64 ui64Size,
	const boolean bDiscard)
{
	return m_vCurrentOutput[ui32OutputIndex].getBuffer().setSize(ui64Size, bDiscard);
}

uint8* PsSimulatedBox::getOutputChunkBuffer(
	const uint32 ui32OutputIndex)
{
	return m_vCurrentOutput[ui32OutputIndex].getBuffer().getDirectPointer();
}

boolean PsSimulatedBox::appendOutputChunkData(
	const OpenViBE::uint32 ui32OutputIndex,
	const OpenViBE::uint8* pBuffer,
	const OpenViBE::uint64 ui64BufferSize)
{
	return m_vCurrentOutput[ui32OutputIndex].getBuffer().appendOutputChunkData(pBuffer, ui64BufferSize);
}

boolean PsSimulatedBox::markOutputAsReadyToSend(
	const uint32 ui32OutputIndex,
	const uint64 ui64StartTime,
	const uint64 ui64EndTime)
{
	// sets start and end time
	m_vCurrentOutput[ui32OutputIndex].setStartTime(ui64StartTime);
	m_vCurrentOutput[ui32OutputIndex].setEndTime(ui64EndTime);

	// copies chunk
	m_vOutput[ui32OutputIndex].push_back(m_vCurrentOutput[ui32OutputIndex]);

	// resets chunk size
	m_vCurrentOutput[ui32OutputIndex].getBuffer().setSize(0, true);

	return true;
}
