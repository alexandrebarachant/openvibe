#include "ovpCEBMLBaseEncoder.h"

#include <system/Memory.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::StreamCodecs;

CEBMLBaseEncoder::CEBMLBaseEncoder(void)
	:m_pEBMLWriterHelper(NULL)
	,m_pEBMLWriter(NULL)
	,m_oEBMLWriterCallbackProxy(
		*this,
		&CEBMLBaseEncoder::write)
{
}

// ________________________________________________________________________________________________________________
//

boolean CEBMLBaseEncoder::initialize(void)
{
	m_oMemoryBufferHandle.initialize(getOutputParameter(OVP_EBMLEncoder_MemoryBuffer_OutParameterId));

	m_pEBMLWriter=EBML::createWriter(m_oEBMLWriterCallbackProxy);
	m_pEBMLWriterHelper=EBML::createWriterHelper();
	m_pEBMLWriterHelper->connect(m_pEBMLWriter);

	return true;
}

boolean CEBMLBaseEncoder::uninitialize(void)
{
	m_pEBMLWriterHelper->disconnect();
	m_pEBMLWriterHelper->release();
	m_pEBMLWriterHelper=NULL;

	m_pEBMLWriter->release();
	m_pEBMLWriter=NULL;

	m_oMemoryBufferHandle.uninitialize();

	return true;
}

// ________________________________________________________________________________________________________________
//

boolean CEBMLBaseEncoder::process(void)
{
	if(isInputTriggerActive(OVP_EBMLEncoder_WriteHeader_TriggerId))
	{
		m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header);
		 m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_StreamType);
		  m_pEBMLWriterHelper->setUIntegerAsChildData(0);
		 m_pEBMLWriterHelper->closeChild();
		 m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_StreamVersion);
		  m_pEBMLWriterHelper->setUIntegerAsChildData(0);
		 m_pEBMLWriterHelper->closeChild();
		 this->processHeader();
		m_pEBMLWriterHelper->closeChild();
		activateOutputTrigger(OVP_EBMLEncoder_MemoryBufferUpdated_TriggerId, true);
	}

	if(isInputTriggerActive(OVP_EBMLEncoder_WriteBuffer_TriggerId))
	{
		m_pEBMLWriterHelper->openChild(OVTK_NodeId_Buffer);
		 this->processBuffer();
		m_pEBMLWriterHelper->closeChild();
		activateOutputTrigger(OVP_EBMLEncoder_MemoryBufferUpdated_TriggerId, true);
	}

	if(isInputTriggerActive(OVP_EBMLEncoder_WriteEnd_TriggerId))
	{
		m_pEBMLWriterHelper->openChild(OVTK_NodeId_End);
		 this->processEnd();
		m_pEBMLWriterHelper->closeChild();
		activateOutputTrigger(OVP_EBMLEncoder_MemoryBufferUpdated_TriggerId, true);
	}

	return true;
}

// ________________________________________________________________________________________________________________
//

void CEBMLBaseEncoder::write(const void* pBuffer, const EBML::uint64 ui64BufferSize)
{
	uint64 l_ui64CurrentBufferSize=m_oMemoryBufferHandle->getSize();
	m_oMemoryBufferHandle->setSize(l_ui64CurrentBufferSize+ui64BufferSize, false);
	System::Memory::copy(m_oMemoryBufferHandle->getDirectPointer()+l_ui64CurrentBufferSize, pBuffer, ui64BufferSize);
}
