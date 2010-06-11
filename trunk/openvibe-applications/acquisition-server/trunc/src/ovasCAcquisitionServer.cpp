#include "ovasCAcquisitionServer.h"

#include <openvibe-toolkit/ovtk_all.h>

#include <system/Memory.h>
#include <system/Time.h>

#include <fstream>
#include <sstream>

#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <cstring>

#include <cassert>

#define boolean OpenViBE::boolean

namespace
{
	// because std::tolower has multiple signatures,
	// it can not be easily used in std::transform
	// this workaround is taken from http://www.gcek.net/ref/books/sw/cpp/ticppv2/
	template <class charT>
	charT to_lower(charT c)
	{
		return std::tolower(c);
	}
};

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBEAcquisitionServer;
using namespace std;

namespace OpenViBEAcquisitionServer
{
	class CDriverContext : public OpenViBEAcquisitionServer::IDriverContext
	{
	public:

		CDriverContext(const OpenViBE::Kernel::IKernelContext& rKernelContext, OpenViBEAcquisitionServer::CAcquisitionServer& rAcquisitionServer)
			:m_rKernelContext(rKernelContext)
			,m_rAcquisitionServer(rAcquisitionServer)
		{
		}

		virtual ILogManager& getLogManager(void) const
		{
			return m_rKernelContext.getLogManager();
		}

		virtual IConfigurationManager& getConfigurationManager(void) const
		{
			return m_rKernelContext.getConfigurationManager();
		}

		virtual boolean isConnected(void) const
		{
			return m_rAcquisitionServer.isConnected();
		}

		virtual boolean isStarted(void) const
		{
			return m_rAcquisitionServer.isStarted();
		}

		virtual int64 getJitterSampleCount(void) const
		{
			return m_rAcquisitionServer.getJitterSampleCount();
		}

		virtual boolean correctJitterSampleCount(int64 i64SampleCount)
		{
			return m_rAcquisitionServer.correctJitterSampleCount(i64SampleCount);
		}

		virtual int64 getJitterToleranceSampleCount(void) const
		{
			return m_rAcquisitionServer.getJitterToleranceSampleCount();
		}

		virtual int64 getSuggestedJitterCorrectionSampleCount(void) const
		{
			return m_rAcquisitionServer.getSuggestedJitterCorrectionSampleCount();
		}

		virtual boolean updateImpedance(const uint32 ui32ChannelIndex, const float64 f64Impedance)
		{
			return m_rAcquisitionServer.updateImpedance(ui32ChannelIndex, f64Impedance);
		}

	protected:

		const IKernelContext& m_rKernelContext;
		CAcquisitionServer& m_rAcquisitionServer;
	};
}

//___________________________________________________________________//
//                                                                   //

CAcquisitionServer::CAcquisitionServer(const IKernelContext& rKernelContext)
	:m_rKernelContext(rKernelContext)
	,m_pDriverContext(NULL)
	,m_pDriver(NULL)
	,m_pAcquisitionStreamEncoder(NULL)
	,m_pExperimentInformationStreamEncoder(NULL)
	,m_pSignalStreamEncoder(NULL)
	,m_pStimulationStreamEncoder(NULL)
	,m_pChannelLocalisationStreamEncoder(NULL)
	,m_pConnectionServer(NULL)
	,m_bInitialized(false)
	,m_bStarted(false)
{
	m_pDriverContext=new CDriverContext(rKernelContext, *this);
	m_pAcquisitionStreamEncoder=&m_rKernelContext.getAlgorithmManager().getAlgorithm(m_rKernelContext.getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_AcquisitionStreamEncoder));
	m_pExperimentInformationStreamEncoder=&m_rKernelContext.getAlgorithmManager().getAlgorithm(m_rKernelContext.getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_ExperimentInformationStreamEncoder));
	m_pSignalStreamEncoder=&m_rKernelContext.getAlgorithmManager().getAlgorithm(m_rKernelContext.getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamEncoder));
	m_pStimulationStreamEncoder=&m_rKernelContext.getAlgorithmManager().getAlgorithm(m_rKernelContext.getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamEncoder));
	// m_pChannelLocalisationStreamEncoder=&m_rKernelContext.getAlgorithmManager().getAlgorithm(m_rKernelContext.getAlgorithmManager().createAlgorithm(/*TODO*/));

	m_pAcquisitionStreamEncoder->initialize();
	m_pExperimentInformationStreamEncoder->initialize();
	m_pSignalStreamEncoder->initialize();
	m_pStimulationStreamEncoder->initialize();
	// m_pChannelLocalisationStreamEncoder->initialize();

	op_pAcquisitionMemoryBuffer.initialize(m_pAcquisitionStreamEncoder->getOutputParameter(OVP_GD_Algorithm_AcquisitionStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
	op_pExperimentInformationMemoryBuffer.initialize(m_pExperimentInformationStreamEncoder->getOutputParameter(OVP_GD_Algorithm_ExperimentInformationStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
	op_pSignalMemoryBuffer.initialize(m_pSignalStreamEncoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
	op_pStimulationMemoryBuffer.initialize(m_pStimulationStreamEncoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
	// op_pChannelLocalisationMemoryBuffer.initialize(m_pChannelLocalisationStreamEncoder->getOutputParameter(OVP_GD_Algorithm_ChannelLocalisationStreamEncoder_OutputParameterId_EncodedMemoryBuffer));

	TParameterHandler < IMemoryBuffer* > ip_pAcquisitionExperimentInformationMemoryBuffer(m_pAcquisitionStreamEncoder->getInputParameter(OVP_GD_Algorithm_AcquisitionStreamEncoder_InputParameterId_ExperimentInformationStream));
	TParameterHandler < IMemoryBuffer* > ip_pAcquisitionSignalMemoryBuffer(m_pAcquisitionStreamEncoder->getInputParameter(OVP_GD_Algorithm_AcquisitionStreamEncoder_InputParameterId_SignalStream));
	TParameterHandler < IMemoryBuffer* > ip_pAcquisitionStimulationMemoryBuffer(m_pAcquisitionStreamEncoder->getInputParameter(OVP_GD_Algorithm_AcquisitionStreamEncoder_InputParameterId_StimulationStream));
	// TParameterHandler < IMemoryBuffer* > ip_pAcquisitionChannelLocalisationMemoryBuffer(m_pAcquisitionStreamEncoder->getInputParameter(OVP_GD_Algorithm_AcquisitionStreamEncoder_InputParameterId_ChannelLocalisationStream));

	ip_pAcquisitionExperimentInformationMemoryBuffer.setReferenceTarget(op_pExperimentInformationMemoryBuffer);
	ip_pAcquisitionSignalMemoryBuffer.setReferenceTarget(op_pSignalMemoryBuffer);
	ip_pAcquisitionStimulationMemoryBuffer.setReferenceTarget(op_pStimulationMemoryBuffer);
	// ip_pAcquisitionChannelLocalisationMemoryBuffer.setReferenceTarget(op_pStimulationMemoryBuffer);
}

CAcquisitionServer::~CAcquisitionServer(void)
{
	if(m_bStarted)
	{
		m_pDriver->stop();
		// m_pDriverContext->onStop(*m_pDriver->getHeader());
		m_bStarted=false;
	}

	if(m_bInitialized)
	{
		m_pDriver->uninitialize();
		// m_pDriverContext->onUninitialize(*m_pDriver->getHeader());
		m_bInitialized=false;
	}

	if(m_pConnectionServer)
	{
		m_pConnectionServer->release();
		m_pConnectionServer=NULL;
	}

	list < pair < Socket::IConnection*, uint64 > >::iterator itConnection=m_vConnection.begin();
	while(itConnection!=m_vConnection.end())
	{
		itConnection->first->release();
		itConnection=m_vConnection.erase(itConnection);
	}

	// op_pChannelLocalisationMemoryBuffer.uninitialize();
	op_pStimulationMemoryBuffer.uninitialize();
	op_pSignalMemoryBuffer.uninitialize();
	op_pExperimentInformationMemoryBuffer.uninitialize();
	op_pAcquisitionMemoryBuffer.uninitialize();

	// m_pChannelLocalisationStreamEncoder->uninitialize();
	m_pStimulationStreamEncoder->uninitialize();
	m_pSignalStreamEncoder->uninitialize();
	m_pExperimentInformationStreamEncoder->uninitialize();
	m_pAcquisitionStreamEncoder->uninitialize();

	// m_rKernelContext.getAlgorithmManager().releaseAlgorithm(*m_pChannelLocalisationStreamEncoder);
	m_rKernelContext.getAlgorithmManager().releaseAlgorithm(*m_pStimulationStreamEncoder);
	m_rKernelContext.getAlgorithmManager().releaseAlgorithm(*m_pSignalStreamEncoder);
	m_rKernelContext.getAlgorithmManager().releaseAlgorithm(*m_pExperimentInformationStreamEncoder);
	m_rKernelContext.getAlgorithmManager().releaseAlgorithm(*m_pAcquisitionStreamEncoder);

	delete m_pDriverContext;
	m_pDriverContext=NULL;
}

IDriverContext& CAcquisitionServer::getDriverContext(void)
{
	return *m_pDriverContext;
}

uint32 CAcquisitionServer::getClientCount(void)
{
	return uint32(m_vConnection.size());
}

float64 CAcquisitionServer::getImpedance(const uint32 ui32ChannelIndex)
{
	if(ui32ChannelIndex < m_vImpedance.size())
	{
		return m_vImpedance[ui32ChannelIndex];
	}
	return OVAS_Impedance_Unknown;
}

//___________________________________________________________________//
//                                                                   //

boolean CAcquisitionServer::loop(void)
{
	// m_rKernelContext.getLogManager() << LogLevel_Debug << "idleCB\n";

	CStimulationSet l_oStimulationSet;
	boolean l_bBufferReady=false;

	// Searches for new connection(s)
	if(m_pConnectionServer)
	{
		if(m_pConnectionServer->isReadyToReceive())
		{
			// Accespts new client
			Socket::IConnection* l_pConnection=m_pConnectionServer->accept();
			if(l_pConnection!=NULL)
			{
				m_rKernelContext.getLogManager() << LogLevel_Trace << "Received new connection\n";

				m_vConnection.push_back(pair < Socket::IConnection*, uint64 >(l_pConnection, ((m_ui64SampleCount-m_vPendingBuffer.size())<<32)/m_ui32SamplingFrequency));

				m_rKernelContext.getLogManager() << LogLevel_Debug << "Creating header\n";

				// op_pChannelLocalisationMemoryBuffer->setSize(0, true);
				op_pStimulationMemoryBuffer->setSize(0, true);
				op_pSignalMemoryBuffer->setSize(0, true);
				op_pExperimentInformationMemoryBuffer->setSize(0, true);
				op_pAcquisitionMemoryBuffer->setSize(0, true);

				// m_pChannelLocalisationStreamEncoder->process(OVP_GD_Algorithm_ChannelLocalisationStreamEncoder_InputTriggerId_EncodeHeader);
				m_pStimulationStreamEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeHeader);
				m_pSignalStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeHeader);
				m_pExperimentInformationStreamEncoder->process(OVP_GD_Algorithm_ExperimentInformationStreamEncoder_InputTriggerId_EncodeHeader);
				m_pAcquisitionStreamEncoder->process(OVP_GD_Algorithm_AcquisitionStreamEncoder_InputTriggerId_EncodeHeader);

				uint64 l_ui64MemoryBufferSize=op_pAcquisitionMemoryBuffer->getSize();
				l_pConnection->sendBufferBlocking(&l_ui64MemoryBufferSize, sizeof(l_ui64MemoryBufferSize));
				l_pConnection->sendBufferBlocking(op_pAcquisitionMemoryBuffer->getDirectPointer(), (uint32)op_pAcquisitionMemoryBuffer->getSize());
			}
		}
	}

	// Eventually builds up buffer
	if(m_vPendingBuffer.size() >= m_ui32SampleCountPerSentBlock)
	{
		m_rKernelContext.getLogManager() << LogLevel_Debug << "Creating buffer\n";

		TParameterHandler < IMatrix* > ip_pMatrix(m_pSignalStreamEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_Matrix));
		for(uint32 j=0; j<m_ui32ChannelCount; j++)
		{
			for(uint32 i=0; i<m_ui32SampleCountPerSentBlock; i++)
			{
				ip_pMatrix->getBuffer()[j*m_ui32SampleCountPerSentBlock+i]=m_vPendingBuffer[i][j];
			}
		}
		m_vPendingBuffer.erase(m_vPendingBuffer.begin(), m_vPendingBuffer.begin()+m_ui32SampleCountPerSentBlock);

		OpenViBEToolkit::Tools::StimulationSet::appendRange(l_oStimulationSet, m_oPendingStimulationSet, 0, ((m_ui64SampleCount-m_vPendingBuffer.size())<<32)/m_ui32SamplingFrequency, 0);
		OpenViBEToolkit::Tools::StimulationSet::removeRange(                   m_oPendingStimulationSet, 0, ((m_ui64SampleCount-m_vPendingBuffer.size())<<32)/m_ui32SamplingFrequency);

		l_bBufferReady=true;
	}
	else
	{
		// Calls driver's loop
		if(!m_pDriver->loop())
		{
			m_rKernelContext.getLogManager() << LogLevel_ImportantWarning << "Something bad happened in the loop callback, stoping the acquisition\n";
			return false;
		}
	}

	// Sends data to connected client(s)
	// and clean disconnected client(s)
	list < pair < Socket::IConnection*, uint64 > >::iterator itConnection=m_vConnection.begin();
	while(itConnection!=m_vConnection.end())
	{
		Socket::IConnection* l_pConnection=itConnection->first;

		if(!l_pConnection->isConnected())
		{
			l_pConnection->release();
			itConnection=m_vConnection.erase(itConnection);
		}
		else
		{
			// Sends buffer
			if(l_bBufferReady)
			{
				m_rKernelContext.getLogManager() << LogLevel_Debug << "Creating buffer\n";

				// op_pChannelLocalisationMemoryBuffer->setSize(0, true);
				op_pStimulationMemoryBuffer->setSize(0, true);
				op_pSignalMemoryBuffer->setSize(0, true);
				op_pExperimentInformationMemoryBuffer->setSize(0, true);
				op_pAcquisitionMemoryBuffer->setSize(0, true);

				// uint64 l_ui64TimeOffset=((itConnection->second<<32)/m_ui32SamplingFrequency);
				TParameterHandler < IStimulationSet* > ip_pStimulationSet(m_pStimulationStreamEncoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_InputParameterId_StimulationSet));
				// OpenViBEToolkit::Tools::StimulationSet::copy(*ip_pStimulationSet, m_oStimulationSet, l_ui64TimeOffset);
				OpenViBEToolkit::Tools::StimulationSet::copy(*ip_pStimulationSet, l_oStimulationSet, -int64(itConnection->second));

				// m_pChannelLocalisationStreamEncoder->process(OVP_GD_Algorithm_ChannelLocalisationStreamEncoder_InputTriggerId_EncodeBuffer);
				m_pStimulationStreamEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeBuffer);
				m_pSignalStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeBuffer);
				m_pExperimentInformationStreamEncoder->process(OVP_GD_Algorithm_ExperimentInformationStreamEncoder_InputTriggerId_EncodeBuffer);
				m_pAcquisitionStreamEncoder->process(OVP_GD_Algorithm_AcquisitionStreamEncoder_InputTriggerId_EncodeBuffer);

				uint64 l_ui64MemoryBufferSize=op_pAcquisitionMemoryBuffer->getSize();
				l_pConnection->sendBufferBlocking(&l_ui64MemoryBufferSize, sizeof(l_ui64MemoryBufferSize));
				l_pConnection->sendBufferBlocking(op_pAcquisitionMemoryBuffer->getDirectPointer(), (uint32)op_pAcquisitionMemoryBuffer->getSize());

				// itConnection->second+=m_ui32SampleCountPerSentBlock;
			}
			itConnection++;
		}
	}

	return true;
}

//___________________________________________________________________//
//                                                                   //

boolean CAcquisitionServer::connect(IDriver& rDriver, IHeader& rHeaderCopy, uint32 ui32SamplingCountPerSentBlock, uint32 ui32ConnectionPort)
{
	m_rKernelContext.getLogManager() << LogLevel_Debug << "connect\n";

	m_pDriver=&rDriver;
	m_ui32SampleCountPerSentBlock=ui32SamplingCountPerSentBlock;

	// Initializes driver
	if(!m_pDriver->initialize(m_ui32SampleCountPerSentBlock, *this))
	{
		m_rKernelContext.getLogManager() << LogLevel_Error << "Connection failed...\n";
		return false;
	}

	// m_pDriverContext->onInitialize(*m_pDriver->getHeader());

	m_rKernelContext.getLogManager() << LogLevel_Info << "Connection succeeded !\n";

	const IHeader& l_rHeader=*rDriver.getHeader();

	m_ui32ChannelCount=l_rHeader.getChannelCount();
	m_ui32SamplingFrequency=l_rHeader.getSamplingFrequency();

	m_vImpedance.resize(m_ui32ChannelCount, OVAS_Impedance_NotAvailable);
	m_vSwapBuffer.resize(m_ui32ChannelCount);

	m_rKernelContext.getLogManager() << LogLevel_Info << "Connecting to device...\n";

	m_pConnectionServer=Socket::createConnectionServer();
	if(m_pConnectionServer->listen(ui32ConnectionPort))
	{
		m_bInitialized=true;

		uint64 l_ui64ToleranceDurationBeforeWarning=m_rKernelContext.getConfigurationManager().expandAsInteger("${AcquisitionServer_ToleranceDuration}", 100);

		m_i64JitterSampleCount=0;
		m_i64JitterToleranceSampleCount=(l_ui64ToleranceDurationBeforeWarning * m_ui32SamplingFrequency) / 1000;
		m_i64JitterCorrectionSampleCountAdded=0;
		m_i64JitterCorrectionSampleCountRemoved=0;

		m_rKernelContext.getLogManager() << LogLevel_Trace << "Driver monitoring tolerance set to " << l_ui64ToleranceDurationBeforeWarning << " milliseconds - eq " << m_i64JitterToleranceSampleCount << " samples\n";

		TParameterHandler < uint64 > ip_ui64BufferDuration(m_pAcquisitionStreamEncoder->getInputParameter(OVP_GD_Algorithm_AcquisitionStreamEncoder_InputParameterId_BufferDuration));

		TParameterHandler < uint64 > ip_ui64ExperimentIdentifier(m_pExperimentInformationStreamEncoder->getInputParameter(OVP_GD_Algorithm_ExperimentInformationStreamEncoder_InputParameterId_ExperimentIdentifier));
		TParameterHandler < CString* > ip_pExperimentDate(m_pExperimentInformationStreamEncoder->getInputParameter(OVP_GD_Algorithm_ExperimentInformationStreamEncoder_InputParameterId_ExperimentDate));
		TParameterHandler < uint64 > ip_ui64SubjectIdentifier(m_pExperimentInformationStreamEncoder->getInputParameter(OVP_GD_Algorithm_ExperimentInformationStreamEncoder_InputParameterId_SubjectIdentifier));
		TParameterHandler < CString* > ip_pSubjectName(m_pExperimentInformationStreamEncoder->getInputParameter(OVP_GD_Algorithm_ExperimentInformationStreamEncoder_InputParameterId_SubjectName));
		TParameterHandler < uint64 > ip_ui64SubjectAge(m_pExperimentInformationStreamEncoder->getInputParameter(OVP_GD_Algorithm_ExperimentInformationStreamEncoder_InputParameterId_SubjectAge));
		TParameterHandler < uint64 > ip_ui64SubjectGender(m_pExperimentInformationStreamEncoder->getInputParameter(OVP_GD_Algorithm_ExperimentInformationStreamEncoder_InputParameterId_SubjectGender));
		TParameterHandler < uint64 > ip_ui64LaboratoryIdentifier(m_pExperimentInformationStreamEncoder->getInputParameter(OVP_GD_Algorithm_ExperimentInformationStreamEncoder_InputParameterId_LaboratoryIdentifier));
		TParameterHandler < CString* > ip_pLaboratoryName(m_pExperimentInformationStreamEncoder->getInputParameter(OVP_GD_Algorithm_ExperimentInformationStreamEncoder_InputParameterId_LaboratoryName));
		TParameterHandler < uint64 > ip_ui64TechnicianIdentifier(m_pExperimentInformationStreamEncoder->getInputParameter(OVP_GD_Algorithm_ExperimentInformationStreamEncoder_InputParameterId_TechnicianIdentifier));
		TParameterHandler < CString* > ip_pTechnicianName(m_pExperimentInformationStreamEncoder->getInputParameter(OVP_GD_Algorithm_ExperimentInformationStreamEncoder_InputParameterId_TechnicianName));

		TParameterHandler < IMatrix* > ip_pMatrix(m_pSignalStreamEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_Matrix));
		TParameterHandler < uint64 > ip_ui64SamplingRate(m_pSignalStreamEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_SamplingRate));

		TParameterHandler < IStimulationSet* > ip_pStimulationSet(m_pStimulationStreamEncoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_InputParameterId_StimulationSet));

		ip_ui64BufferDuration=(((uint64)m_ui32SampleCountPerSentBlock)<<32)/m_ui32SamplingFrequency;

		ip_ui64ExperimentIdentifier=l_rHeader.getExperimentIdentifier();
		ip_ui64SubjectIdentifier=l_rHeader.getSubjectAge();
		ip_ui64SubjectGender=l_rHeader.getSubjectGender();

		ip_ui64SamplingRate=m_ui32SamplingFrequency;
		ip_pMatrix->setDimensionCount(2);
		ip_pMatrix->setDimensionSize(0, m_ui32ChannelCount);
		ip_pMatrix->setDimensionSize(1, m_ui32SampleCountPerSentBlock);
		for(uint32 i=0; i<m_ui32ChannelCount; i++)
		{
			string l_sChannelName=l_rHeader.getChannelName(i);
			if(l_sChannelName!="")
			{
				ip_pMatrix->setDimensionLabel(0, i, l_sChannelName.c_str());
			}
			else
			{
				std::stringstream ss;
				ss << "Channel " << i+1;
				ip_pMatrix->setDimensionLabel(0, i, ss.str().c_str());
			}
		}

		// TODO Gain is ignored
	}
	else
	{
		m_rKernelContext.getLogManager() << LogLevel_Error << "Could not listen on TCP port (firewall problem ?)\n";
		return false;
	}

	IHeader::copy(rHeaderCopy, l_rHeader);
	return true;
}

boolean CAcquisitionServer::start(void)
{
	m_rKernelContext.getLogManager() << LogLevel_Debug << "buttonStartPressedCB\n";

	m_rKernelContext.getLogManager() << LogLevel_Info << "Starting the acquisition...\n";

	// Starts driver
	if(!m_pDriver->start())
	{
		m_rKernelContext.getLogManager() << LogLevel_Error << "Starting failed !\n";
		return false;
	}
	// m_pDriverContext->onStart(*m_pDriver->getHeader());

	m_rKernelContext.getLogManager() << LogLevel_Info << "Now acquiring...\n";

	m_vPendingBuffer.clear();
	m_oPendingStimulationSet.clear();

	m_ui64SampleCount=0;
	m_ui64LastSampleCount=0;
	m_i64JitterSampleCount=0;
	m_i64JitterCorrectionSampleCountAdded=0;
	m_i64JitterCorrectionSampleCountRemoved=0;
	m_ui64StartTime=System::Time::zgetTime();

	m_bStarted=true;
	return true;
}

boolean CAcquisitionServer::stop(void)
{
	m_rKernelContext.getLogManager() << LogLevel_Debug << "buttonStopPressedCB\n";

	m_rKernelContext.getLogManager() << LogLevel_Info << "Stoping the acquisition.\n";

	if(-m_i64JitterToleranceSampleCount * 5 < m_i64JitterSampleCount && m_i64JitterSampleCount < m_i64JitterToleranceSampleCount * 5)
	{
	}
	else
	{
		uint64 l_ui64TheoricalSampleCount=m_ui64SampleCount-m_i64JitterSampleCount;
		m_rKernelContext.getLogManager() << LogLevel_Warning << "After " << (((System::Time::zgetTime()-m_ui64StartTime) * 1000) >> 32) * .001f << " seconds, theorical samples per second does not match real samples per second.\n";
		m_rKernelContext.getLogManager() << LogLevel_Warning << "  Received : " << m_ui64SampleCount << " samples.\n";
		m_rKernelContext.getLogManager() << LogLevel_Warning << "  Should have received : " << l_ui64TheoricalSampleCount << " samples.\n";
		m_rKernelContext.getLogManager() << LogLevel_Warning << "  Difference was : " << m_i64JitterSampleCount << " samples.\n";
		if(m_i64JitterCorrectionSampleCountAdded==0 && m_i64JitterCorrectionSampleCountRemoved==0)
		{
			m_rKernelContext.getLogManager() << LogLevel_Warning << "  The driver did not try to correct this difference.\n";
		}
		else
		{
			m_rKernelContext.getLogManager() << LogLevel_Warning << "  The driver however tried to correct this difference and added/removed " << m_i64JitterCorrectionSampleCountAdded << "/" << m_i64JitterCorrectionSampleCountRemoved << " samples .\n";
		}
		m_rKernelContext.getLogManager() << LogLevel_Warning << "  Please submit a bug report (including the acquisition server log file or at least this complete message) for the driver you are using.\n";
	}

	// Stops driver
	m_pDriver->stop();
	// m_pDriverContext->onStop(*m_pDriver->getHeader());

	m_bStarted=false;
	return true;
}

boolean CAcquisitionServer::disconnect(void)
{
	m_rKernelContext.getLogManager() << LogLevel_Info << "Disconnecting.\n";

	if(m_bInitialized)
	{
		m_pDriver->uninitialize();
		// m_pDriverContext->onUninitialize(*m_pDriver->getHeader());
	}

	m_vImpedance.clear();

	list < pair < Socket::IConnection*, uint64 > >::iterator itConnection=m_vConnection.begin();
	while(itConnection!=m_vConnection.end())
	{
		itConnection->first->release();
		itConnection=m_vConnection.erase(itConnection);
	}

	if(m_pConnectionServer)
	{
		m_pConnectionServer->release();
		m_pConnectionServer=NULL;
	}

	m_bInitialized=false;
	return true;
}

//___________________________________________________________________//
//                                                                   //

void CAcquisitionServer::setSamples(const float32* pSample)
{
	if(m_bStarted)
	{
		for(uint32 i=0; i<m_ui32SampleCountPerSentBlock; i++)
		{
			for(uint32 j=0; j<m_ui32ChannelCount; j++)
			{
				m_vSwapBuffer[j]=pSample[j*m_ui32SampleCountPerSentBlock+i];
			}
			m_vPendingBuffer.push_back(m_vSwapBuffer);
		}
		m_ui64LastSampleCount=m_ui64SampleCount;
		m_ui64SampleCount+=m_ui32SampleCountPerSentBlock;

		{
			uint64 l_ui64TheoricalSampleCount=(m_ui32SamplingFrequency * (System::Time::zgetTime()-m_ui64StartTime))>>32;
			m_i64JitterSampleCount=int64(m_ui64SampleCount-l_ui64TheoricalSampleCount);

			m_rKernelContext.getLogManager() << LogLevel_Trace << "Sample count jitter is : " << m_i64JitterSampleCount << " samples.\n";
		}
	}
	else
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "The acquisition is not started\n";
	}
}

void CAcquisitionServer::setStimulationSet(const IStimulationSet& rStimulationSet)
{
	if(m_bStarted)
	{
		OpenViBEToolkit::Tools::StimulationSet::append(m_oPendingStimulationSet, rStimulationSet, (m_ui64LastSampleCount<<32)/m_ui32SamplingFrequency);
	}
	else
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "The acquisition is not started\n";
	}
}

int64 CAcquisitionServer::getSuggestedJitterCorrectionSampleCount(void) const
{
	if(this->getJitterSampleCount() > this->getJitterToleranceSampleCount())
	{
		return -(this->getJitterSampleCount()-this->getJitterToleranceSampleCount()/2);
	}

	if(this->getJitterSampleCount() < -this->getJitterToleranceSampleCount())
	{
		return -(this->getJitterSampleCount()+this->getJitterToleranceSampleCount()/2);
	}

	return 0;
}

boolean CAcquisitionServer::correctJitterSampleCount(int64 i64SampleCount)
{
	if(!m_bStarted)
	{
		return false;
	}

	if(i64SampleCount == 0)
	{
		return true;
	}
	else
	{
		m_rKernelContext.getLogManager() << LogLevel_Trace << "Correcting jitter with " << i64SampleCount << " samples\n";
		if(i64SampleCount > 0)
		{
			for(int64 i=0; i<i64SampleCount; i++)
			{
				m_vPendingBuffer.push_back(m_vSwapBuffer);
			}

			m_oPendingStimulationSet.appendStimulation(OVTK_GDF_Incorrect, ((m_ui64SampleCount-1               ) << 32) / m_ui32SamplingFrequency, (i64SampleCount << 32) / m_ui32SamplingFrequency);
			m_oPendingStimulationSet.appendStimulation(OVTK_GDF_Correct,   ((m_ui64SampleCount-1+i64SampleCount) << 32) / m_ui32SamplingFrequency, 0);

			m_i64JitterSampleCount+=i64SampleCount;
			m_ui64LastSampleCount=m_ui64SampleCount;
			m_ui64SampleCount+=i64SampleCount;
			m_i64JitterCorrectionSampleCountAdded+=i64SampleCount;
		}
		else if(i64SampleCount < 0)
		{
			int64 l_i64SamplesToRemove=-i64SampleCount;
			if(l_i64SamplesToRemove>m_vPendingBuffer.size())
			{
				l_i64SamplesToRemove=m_vPendingBuffer.size();
			}

			m_vPendingBuffer.erase(m_vPendingBuffer.begin()+m_vPendingBuffer.size()-l_i64SamplesToRemove, m_vPendingBuffer.begin()+m_vPendingBuffer.size());
			OpenViBEToolkit::Tools::StimulationSet::removeRange(m_oPendingStimulationSet, ((m_ui64SampleCount-l_i64SamplesToRemove)<<32)/m_ui32SamplingFrequency, (m_ui64SampleCount<<32)/m_ui32SamplingFrequency);

			m_i64JitterSampleCount-=l_i64SamplesToRemove;
			m_ui64LastSampleCount=m_ui64SampleCount;
			m_ui64SampleCount-=l_i64SamplesToRemove;
			m_i64JitterCorrectionSampleCountRemoved+=l_i64SamplesToRemove;
		}
	}

	return true;
}

boolean CAcquisitionServer::updateImpedance(const uint32 ui32ChannelIndex, const float64 f64Impedance)
{
	if(ui32ChannelIndex >= m_vImpedance.size())
	{
		return false;
	}
	m_vImpedance[ui32ChannelIndex]=f64Impedance;
	return true;
}
