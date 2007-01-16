#include "ovkPsSimulatedBox.h"
#include "ovkPsDuplicatedContext.h"

#include "../ovkCBoxAlgorithmContext.h"

#include <iostream>

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
#define boolean OpenViBE::boolean

// #define _ScopeTester_
// #define _OMK_BehaviorTester_

class OpenViBE::CMessageClock
{
public:
	int i;
};

PsSimulatedBox::PsSimulatedBox(
	PsController& rController,
	const PsObjectDescriptor& rObjectDescriptor)
	:PsSimulatedBoxBase(rController, rObjectDescriptor)
	,m_oKernelContextHandle(*this)
	,m_oScenarioHandle(*this)
	,m_oPluginManagerHandle(*this)
	,m_bReadyToProcess(false)
	,m_pBoxAlgorithmDesc(NULL)
	,m_pBoxAlgorithm(NULL)
	,m_pBox(NULL)
{
}

PsSimulatedBox::~PsSimulatedBox(void)
{
}

void PsSimulatedBox::init(void)
{
	::PsSimulatedBoxBase::init();

	CIdentifier l_oIdentifier;
	::PsName l_oNameIdentifier=getName();
	::PsString l_oNameString=l_oNameIdentifier.getPsString();
	l_oIdentifier.fromString(l_oNameIdentifier.getCString());

	const IScenario& l_rScenario=m_oScenarioHandle.getConcreteScenario();
	m_pBox=l_rScenario.getBoxDetails(l_oIdentifier);

	m_vInput.resize(m_pBox->getInputCount());
	m_vOutput.resize(m_pBox->getOutputCount());

	IPluginManager& l_rPluginManager=m_oPluginManagerHandle.getConcretePluginManager();
	m_pBoxAlgorithmDesc=dynamic_cast<const IBoxAlgorithmDesc*>(l_rPluginManager.getPluginObjectDescCreating(m_pBox->getAlgorithmClassIdentifier()));
	m_pBoxAlgorithm=dynamic_cast<IBoxAlgorithm*>(l_rPluginManager.createPluginObject(m_pBox->getAlgorithmClassIdentifier()));
	CBoxAlgorithmContext l_oBoxAlgorithmContext(m_oKernelContextHandle.getKernelContext(), this, m_pBox);
	{
#if defined _ScopeTester_
		Tools::CScopeTester l_oScopeTester("User code IBoxAlgorithm::initialize");
#endif
		m_pBoxAlgorithm->initialize(l_oBoxAlgorithmContext);
	}
#ifdef _OMK_BehaviorTester_
	cout<<"PsSimulatedBox::init("<<getName()<<"|"<<m_pBox->getName()<<")"<<endl;
#endif
}

void PsSimulatedBox::computeParameters(void)
{
#ifdef _OMK_BehaviorTester_
	cout<<"PsSimulatedBox::computeParameters("<<getName()<<"|"<<m_pBox->getName()<<")"<<endl;
#endif

	CBoxAlgorithmContext l_oBoxAlgorithmContext(m_oKernelContextHandle.getKernelContext(), this, m_pBox);
	{
#if defined _ScopeTester_
		Tools::CScopeTester l_oScopeTester("User code IBoxAlgorithm::processClock");
#endif
		// $$$$$$$$$$
		CMessageClock m;
		m_pBoxAlgorithm->processClock(l_oBoxAlgorithmContext, m);
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
	cout<<"PsSimulatedBox::processOpenViBEDataUpdateEvent("<<getName()<<"|"<<m_pBox->getName()<<"|"<<pEvent->date<</*"|"<<*((const float*)(pEvent->value.getBuffer().getDirectPointer()+pEvent->value.getBuffer().getSize()-4))<<*/")"<<endl;
#endif

	m_vInput[pEvent->value.getIoConnectorIndex()].push_back(pEvent->value);

	CBoxAlgorithmContext l_oBoxAlgorithmContext(m_oKernelContextHandle.getKernelContext(), this, m_pBox);
	{
#if defined _ScopeTester_
		Tools::CScopeTester l_oScopeTester("User code IBoxAlgorithm::processInput");
#endif
		m_pBoxAlgorithm->processInput(l_oBoxAlgorithmContext, pEvent->value.getIoConnectorIndex());
	}
	m_bReadyToProcess|=l_oBoxAlgorithmContext.isAlgorithmReadyToProcess();

	return true ;
}

void PsSimulatedBox::doProcess(void)
{
	CBoxAlgorithmContext l_oBoxAlgorithmContext(m_oKernelContextHandle.getKernelContext(), this, m_pBox);
	{
#if defined _ScopeTester_
		Tools::CScopeTester l_oScopeTester("User code IBoxAlgorithm::process");
#endif
		m_pBoxAlgorithm->process(l_oBoxAlgorithmContext);
	}

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

	// perform output sending
	m_oScenarioHandle.getConcreteScenario().enumerateLinksFromBox(*this, m_pBox->getIdentifier());

	// mark output chunks as sent
	j=m_vOutput.begin();
	while(j!=m_vOutput.end())
	{
		j->m_bReadyToSend=false;
		++j;
	}
}

boolean PsSimulatedBox::callback(const IScenario& rScenario, ILink& rLink)
{
	uint32 l_ui32SourceOutputIndex=rLink.getSourceBoxOutputIndex();
	::PsTypeChunk& l_rChunk=m_vOutput[l_ui32SourceOutputIndex];
	if(l_rChunk.m_bReadyToSend)
	{
		CIdentifier l_oTargetBoxIdentifier=rLink.getTargetBoxIdentifier();
		uint32 l_ui32TargetBoxInputIndex=rLink.getTargetBoxInputIndex();
		::PsName target(l_oTargetBoxIdentifier.toString());

		l_rChunk.setIoConnectorIndex(l_ui32TargetBoxInputIndex);
		sendOpenViBEDataUpdateEvent(target, l_rChunk);
	}
	return true;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// - --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

uint32 PsSimulatedBox::getInputChunkCount(
	const uint32 ui32InputIndex)
{
	return m_vInput[ui32InputIndex].size();
}

boolean PsSimulatedBox::getInputChunk(
	const uint32 ui32InputIndex,
	const uint32 ui32ChunkIndex,
	uint64& rStartTime,
	uint64& rEndTime,
	uint64& rChunkSize,
	const uint8*& rpChunkBuffer)
{
	if(ui32ChunkIndex>=m_vInput[ui32InputIndex].size())
	{
		return false;
	}

	::PsTypeChunk& l_rChunk=m_vInput[ui32InputIndex][ui32ChunkIndex];
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
	const uint32 ui32OutputIndex)
{
	return m_vOutput[ui32OutputIndex].getBuffer().getSize();
}

boolean PsSimulatedBox::setOutputChunkSize(
	const uint32 ui32OutputIndex,
	const uint64 ui64Size,
	const boolean bDiscard)
{
	return m_vOutput[ui32OutputIndex].getBuffer().setSize(ui64Size, bDiscard);
}

uint8* PsSimulatedBox::getOutputChunkBuffer(
	const uint32 ui32OutputIndex)
{
	return m_vOutput[ui32OutputIndex].getBuffer().getDirectPointer();
}

boolean PsSimulatedBox::appendOutputChunkData(
	const OpenViBE::uint32 ui32OutputIndex,
	const OpenViBE::uint8* pBuffer,
	const OpenViBE::uint64 ui64BufferSize)
{
	return m_vOutput[ui32OutputIndex].getBuffer().appendOutputChunkData(pBuffer, ui64BufferSize);
}

boolean PsSimulatedBox::markOutputAsReadyToSend(
	const uint32 ui32OutputIndex,
	const uint64 ui64EndTime)
{
	if(m_vOutput[ui32OutputIndex].m_bReadyToSend)
	{
		return false;
	}

	m_vOutput[ui32OutputIndex].setStartTime(m_vOutput[ui32OutputIndex].getEndTime());
	m_vOutput[ui32OutputIndex].setEndTime(ui64EndTime);
	m_vOutput[ui32OutputIndex].m_bReadyToSend=true;
	return true;
}
