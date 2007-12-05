#include "ovpCEBMLBaseDecoder.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::StreamCodecs;

CEBMLBaseDecoder::CEBMLBaseDecoder(void)
	:m_pEBMLReaderHelper(NULL)
	,m_pEBMLReader(NULL)
	,m_oEBMLReaderCallbackProxy(
		*this,
		&CEBMLBaseDecoder::isMasterChild,
		&CEBMLBaseDecoder::openChild,
		&CEBMLBaseDecoder::processChildData,
		&CEBMLBaseDecoder::closeChild)
{
}

// ________________________________________________________________________________________________________________
//

boolean CEBMLBaseDecoder::initialize(void)
{
	m_oEBMLMemoryBufferHandle.initialize(getInputParameter(OVP_EBMLDecoder_EBMLMemoryBuffer_InParameterId));

	m_pEBMLReaderHelper=EBML::createReaderHelper();

	m_pEBMLReader=EBML::createReader(m_oEBMLReaderCallbackProxy);

	return true;
}

boolean CEBMLBaseDecoder::uninitialize(void)
{
	m_pEBMLReader->release();
	m_pEBMLReader=NULL;

	m_pEBMLReaderHelper->release();
	m_pEBMLReaderHelper=NULL;

	m_oEBMLMemoryBufferHandle.uninitialize();

	return true;
}

// ________________________________________________________________________________________________________________
//

boolean CEBMLBaseDecoder::process(void)
{
	m_pEBMLReader->processData(m_oEBMLMemoryBufferHandle->getDirectPointer(), m_oEBMLMemoryBufferHandle->getSize());

	return true;
}

// ________________________________________________________________________________________________________________
//

EBML::boolean CEBMLBaseDecoder::isMasterChild(const EBML::CIdentifier& rIdentifier)
{
	if(rIdentifier==OVTK_NodeId_Header)                    { return true; }
	else if(rIdentifier==OVTK_NodeId_Header_StreamVersion) { return false; }
	else if(rIdentifier==OVTK_NodeId_Header_StreamType)    { return false; }
	else if(rIdentifier==OVTK_NodeId_Header)               { return true; }
	else if(rIdentifier==OVTK_NodeId_Buffer)               { return true; }
	else if(rIdentifier==OVTK_NodeId_End)                  { return true; }
	return false;
}

void CEBMLBaseDecoder::openChild(const EBML::CIdentifier& rIdentifier)
{
	if(rIdentifier==OVTK_NodeId_Header)
	{
		activateOutputTrigger(OVP_EBMLDecoder_GotHeader_TriggerId, true);
	}
	if(rIdentifier==OVTK_NodeId_Buffer)
	{
		activateOutputTrigger(OVP_EBMLDecoder_GotBuffer_TriggerId, true);
	}
	if(rIdentifier==OVTK_NodeId_End)
	{
		activateOutputTrigger(OVP_EBMLDecoder_GotEnd_TriggerId, true);
	}
}

void CEBMLBaseDecoder::processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize)
{
}

void CEBMLBaseDecoder::closeChild(void)
{
}
