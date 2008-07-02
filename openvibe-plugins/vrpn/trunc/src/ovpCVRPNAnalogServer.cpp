#if defined TARGET_HAS_ThirdPartyVRPN

#include "ovpCVRPNAnalogServer.h"
#include "ovpIVRPNServerManager.h"

using namespace OpenViBE;
using namespace OpenViBE::Plugins;
using namespace OpenViBE::Kernel;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::VRPN;
using namespace OpenViBEToolkit;
using namespace std;

CVRPNAnalogServer::CVRPNAnalogServer()
	:m_pReader(NULL)
	,m_pStreamedMatrixReaderCallBack(NULL)
	,m_ui32AnalogCount(0)
{
}

boolean CVRPNAnalogServer::initialize()
{
	const IBox * l_pBox=getBoxAlgorithmContext()->getStaticBoxContext();

	//initializes the ebml input
	m_pStreamedMatrixReaderCallBack = createBoxAlgorithmStreamedMatrixInputReaderCallback(*this);
	m_pReader=EBML::createReader(*m_pStreamedMatrixReaderCallBack);

	//get server name, and creates an analog server for this server
	CString l_oServerName;
	l_pBox->getSettingValue(0, l_oServerName);

	IVRPNServerManager::getInstance().initialize();
	IVRPNServerManager::getInstance().addServer(l_oServerName, m_oServerIdentifier);

	return true;
}

boolean CVRPNAnalogServer::uninitialize()
{
	//release the ebml reader
	releaseBoxAlgorithmStreamedMatrixInputReaderCallback(m_pStreamedMatrixReaderCallBack);

	m_pReader->release();
	m_pReader=NULL;

	IVRPNServerManager::getInstance().uninitialize();

	return true;
}

boolean CVRPNAnalogServer::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CVRPNAnalogServer::process()
{
	IBoxIO * l_pBoxIO=getBoxAlgorithmContext()->getDynamicBoxContext();

	for(uint32 i=0; i<l_pBoxIO->getInputChunkCount(0); i++)
	{
		uint64 l_ui64ChunkSize;
		const uint8* l_pChunkBuffer=NULL;
		if(l_pBoxIO->getInputChunk(0, i, m_ui64StartTime, m_ui64EndTime, l_ui64ChunkSize, l_pChunkBuffer))
		{
			m_pReader->processData(l_pChunkBuffer, l_ui64ChunkSize);
			l_pBoxIO->markInputAsDeprecated(0, i);
		}
	}

	if(m_bAnalogSet)
	{
	IVRPNServerManager::getInstance().process();
	}

	return true;
}

void CVRPNAnalogServer::setMatrixDimmensionCount(const uint32 ui32DimmensionCount)
{
	m_ui32AnalogCount=0;
	m_bAnalogSet=false;
}

void CVRPNAnalogServer::setMatrixDimmensionSize(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionSize)
{
	if(m_ui32AnalogCount==0)
	{
		m_ui32AnalogCount=1;
	}
	m_ui32AnalogCount*=ui32DimmensionSize;
}

void CVRPNAnalogServer::setMatrixDimmensionLabel(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel)
{
	// NOTHING TO DO
}

void CVRPNAnalogServer::setMatrixBuffer(const float64* pBuffer)
{
	if(!m_bAnalogSet)
	{
		IVRPNServerManager::getInstance().setAnalogCount(m_oServerIdentifier, m_ui32AnalogCount+1);
		getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Trace << "Created VRPN analog server for " << m_ui32AnalogCount << " channel(s)\n";
		m_bAnalogSet=true;
	}

	// updates the analog server channels
	for(uint32 i=0; i<m_ui32AnalogCount; i++)
	{
		if(!IVRPNServerManager::getInstance().setAnalogState(m_oServerIdentifier, i+1, pBuffer[i]))
		{
			getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Warning << "Could not set analog state !\n";
		}
	}
}

#endif // OVP_HAS_Vrpn
