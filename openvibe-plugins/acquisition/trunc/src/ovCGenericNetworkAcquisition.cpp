#include "ovCGenericNetworkAcquisition.h"

#include <system/Memory.h>

#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Acquisition;

using namespace std;

namespace OpenViBEPlugins
{
	namespace Acquisition
	{
		class CGenericNetworkAcquisition_ReaderCallBack : virtual public EBML::IReaderCallBack
		{
		public:

			CGenericNetworkAcquisition_ReaderCallBack(CGenericNetworkAcquisition& rPlugin)
				:m_rPlugin(rPlugin)
				,m_pReaderHelper(NULL)
			{
				m_pReaderHelper=EBML::createReaderHelper();
			}

			virtual ~CGenericNetworkAcquisition_ReaderCallBack()
			{
				m_pReaderHelper->release();
			}

			virtual EBML::boolean isMasterChild(const EBML::CIdentifier& rIdentifier)
			{
				return false;
			}

			virtual void openChild(const EBML::CIdentifier& rIdentifier)
			{
				m_oCurrentIdentifier=rIdentifier;
			}

			virtual void processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize)
			{
				m_rPlugin.m_pWriter->openChild(m_oCurrentIdentifier);
				m_rPlugin.m_pWriter->setChildData(pBuffer, ui64BufferSize);
				m_rPlugin.m_pWriter->closeChild();
			}

			virtual void closeChild(void)
			{
			}

		public:

			CGenericNetworkAcquisition& m_rPlugin;
			EBML::CIdentifier m_oCurrentIdentifier;
			EBML::IReaderHelper* m_pReaderHelper;
		};

		class CGenericNetworkAcquisition_WriterCallBack : virtual public EBML::IWriterCallBack
		{
		public:

			CGenericNetworkAcquisition_WriterCallBack(CGenericNetworkAcquisition& rPlugin)
				:m_rPlugin(rPlugin)
			{
			}

			virtual void write(const void* pBuffer, const EBML::uint64 ui64BufferSize)
			{
				m_rPlugin.m_pOutput->setChunkSize(m_rPlugin.m_ui64CurrentBufferSize+ui64BufferSize, false);
				System::Memory::copy(m_rPlugin.m_pOutput->getChunkBuffer()+m_rPlugin.m_ui64CurrentBufferSize, pBuffer, ui64BufferSize);
				m_rPlugin.m_ui64CurrentBufferSize+=ui64BufferSize;
				m_rPlugin.m_pOutput->markAsReadyToSend(0,0); // $$$
			}

			CGenericNetworkAcquisition& m_rPlugin;
		};
	};
};

boolean CGenericNetworkAcquisitionDesc::getBoxPrototype(IBoxProto& rPrototype) const
{
	// Adds box inputs

	// Adds box outputs
	rPrototype.addOutput("EEG stream",           OV_TypeId_EEGStream);

	// Adds box settings
	rPrototype.addSetting("EEG server hostname", OV_TypeId_String,  "localhost");
	rPrototype.addSetting("EEG server port",     OV_TypeId_Integer, "9999");

	return true;
}

uint32 CGenericNetworkAcquisitionDesc::getClockFrequency(void) const
{
	return 1; // $$$
}

CGenericNetworkAcquisition::CGenericNetworkAcquisition(void)
	:m_pConnectionClient(NULL)
	,m_ui32ServerHostPort(0)
	,m_pReaderCallBack(NULL)
	,m_pReader(NULL)
	,m_pWriterCallBack(NULL)
	,m_pWriter(NULL)
	,m_pOutput(NULL)
{
}

boolean CGenericNetworkAcquisition::initialize(
	const IPluginObjectContext& rContext)
{
	const IBoxContext& rBoxContext=dynamic_cast<const IBoxContext&>(rContext);

	// Builds up client connection
	m_pConnectionClient=Socket::createConnectionClient();

	// Prepares EBML reader
	m_pReaderCallBack=new CGenericNetworkAcquisition_ReaderCallBack(*this);
	m_pReader=EBML::createReader(*m_pReaderCallBack);

	// Prepares EBML writer
	m_pWriterCallBack=new CGenericNetworkAcquisition_WriterCallBack(*this);
	m_pWriter=EBML::createWriter(*m_pWriterCallBack);

	// Parses box settings to try connecting to server
	CString l_sServerHostPort;
	rBoxContext.getSettingValue(0, m_sServerHostName);
	rBoxContext.getSettingValue(1, l_sServerHostPort);
	m_ui32ServerHostPort=atoi(l_sServerHostPort);

	// Tries to connect to server
	m_pConnectionClient->connect(m_sServerHostName, m_ui32ServerHostPort);

	return true;
}

boolean CGenericNetworkAcquisition::uninitialize(
	const IPluginObjectContext& rContext)
{
	const IBoxContext& rBoxContext=dynamic_cast<const IBoxContext&>(rContext);

	// Ceans up EBML writer
	m_pWriter->release();
	m_pWriter=NULL;
	delete m_pWriterCallBack;
	m_pWriterCallBack=NULL;

	// Cleans up EBML reader
	m_pReader->release();
	m_pReader=NULL;
	delete m_pReaderCallBack;
	m_pReaderCallBack=NULL;

	// Cleans up client connection
	m_pConnectionClient->close();
	m_pConnectionClient->release();
	m_pConnectionClient=NULL;

	return true;
}

boolean CGenericNetworkAcquisition::processClock(
	IBoxAlgorithmContext& rBoxAlgorithmContext,
	CMessageClock& rMessageClock)
{
	// Checks if connection is correctly established
	if(!m_pConnectionClient->isConnected())
	{
		// In case it is not, try to reconnect
		m_pConnectionClient->connect(m_sServerHostName, m_ui32ServerHostPort);
	}
	else
	{
		// Checks if connection has pending data
		if(m_pConnectionClient->isReadyToReceive())
		{
			// Data are waiting, mark box algorithm as ready to go :)
			rBoxAlgorithmContext.markAlgorithmAsReadyToProcess();
		}
	}
	return true;
}

boolean CGenericNetworkAcquisition::process(IBoxAlgorithmContext& rBoxAlgorithmContext)
{
	uint8 l_pBuffer[1024];
	uint32 l_ui32Received;
	m_ui64CurrentBufferSize=0;

	m_pOutput=rBoxAlgorithmContext.getBoxContext().getOutput(0);
	do
	{
		// Receives data from the connection
		l_ui32Received=m_pConnectionClient->receiveBuffer(l_pBuffer, sizeof(l_pBuffer));

		// Sends them to the EBML processor
		m_pReader->processData(l_pBuffer, l_ui32Received);
	}
	while(m_pConnectionClient->isReadyToReceive());
	m_pOutput=NULL;

	return true;
}
