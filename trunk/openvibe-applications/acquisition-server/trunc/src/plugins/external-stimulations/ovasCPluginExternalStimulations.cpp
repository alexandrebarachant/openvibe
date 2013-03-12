#include "ovasCPluginExternalStimulations.h"
#include "../../ovasCAcquisitionServer.h"


#include <boost/interprocess/ipc/message_queue.hpp>

#include <vector>
#include <ctime>

#include <iostream>

#define boolean OpenViBE::boolean

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBEAcquisitionServer;
using namespace OpenViBEAcquisitionServerPlugins;
using namespace std;

CPluginExternalStimulations::CPluginExternalStimulations(CAcquisitionServer& acquisitionServer) :
	IAcquisitionServerPlugin(acquisitionServer),
	m_rKernelContext(acquisitionServer.m_rKernelContext),
	m_bIsExternalStimulationsEnabled(false)
{
	m_rKernelContext.getLogManager() << LogLevel_Info << "Loading plugin: Software Tagging\n";

	m_oProperties.name = "Software Tagging";

	addSetting<boolean>("Enable External Stimulations", true);
	addSetting<OpenViBE::CString>("External Stimulation Queue Name", "openvibeExternalStimulations");

}

CPluginExternalStimulations::~CPluginExternalStimulations()
{
}

// Hooks

void CPluginExternalStimulations::startHook()
{
	m_bIsExternalStimulationsEnabled = getSetting<boolean>("Enable External Stimulations");

	if (m_bIsExternalStimulationsEnabled)
	{
		m_sExternalStimulationsQueueName = getSetting<OpenViBE::CString>("External Stimulation Queue Name");
		ftime(&m_CTStartTime);
		m_bIsESThreadRunning = true;
		m_ESthreadPtr.reset(new boost::thread( boost::bind(&CPluginExternalStimulations::readExternalStimulations , this )));
		m_rKernelContext.getLogManager() << LogLevel_Info << "Software tagging activated...\n";
	}
	m_vExternalStimulations.clear();

	m_iDebugExternalStimulationsSent=0;
	m_iDebugCurrentReadIPCStimulations = 0;
	m_iDebugStimulationsLost = 0;
	m_iDebugStimulationsReceivedEarlier = 0;
	m_iDebugStimulationsReceivedLate = 0;
	m_iDebugStimulationsReceivedWrongSize = 0;
	m_iDebugStimulationsBuffered = 0;

}

void CPluginExternalStimulations::loopHook(CStimulationSet &stimulationSet, uint64 start, uint64 end)
{
	if (m_bIsExternalStimulationsEnabled)
	{
		//m_rKernelContext.getLogManager() << LogLevel_Error << "Checking for external stimulations:" << p << "\n";
		addExternalStimulations(&stimulationSet,m_rKernelContext.getLogManager(),start,end);
	}

}

void CPluginExternalStimulations::stopHook()
{
	if (m_bIsExternalStimulationsEnabled)
	{
		m_bIsESThreadRunning = false;
		m_ESthreadPtr->join();
	}


	//software tagging diagnosting
	m_rKernelContext.getLogManager() << LogLevel_Debug << "  Total external ones received through IPC: " << m_iDebugCurrentReadIPCStimulations << "\n";
	m_rKernelContext.getLogManager() << LogLevel_Debug << "  Sent to Designer: " << m_iDebugExternalStimulationsSent << "\n";
	m_rKernelContext.getLogManager() << LogLevel_Debug << "  Lost because of invalid timestamp: " << m_iDebugStimulationsLost << "\n";
	m_rKernelContext.getLogManager() << LogLevel_Debug << "  Stimulations that came earlier: " << m_iDebugStimulationsReceivedEarlier << "\n";
	m_rKernelContext.getLogManager() << LogLevel_Debug << "  Stimulations that came later: " << 	m_iDebugStimulationsReceivedLate << "\n";
	m_rKernelContext.getLogManager() << LogLevel_Debug << "  Stimulations that had wrong size: " << 	m_iDebugStimulationsReceivedWrongSize << "\n";
	m_rKernelContext.getLogManager() << LogLevel_Debug << "  Buffered: " << 	m_iDebugStimulationsBuffered << "\n";

	int processed=0;
	vector<SExternalStimulation>::iterator cii;
	for(cii=m_vExternalStimulations.begin(); cii!=m_vExternalStimulations.end(); cii++)
	{
		if (cii->isProcessed)
			processed++;
	}
	m_rKernelContext.getLogManager() << LogLevel_Debug << "  processed: " << processed << "\n";
	//end software tagging diagnosting
}

void CPluginExternalStimulations::acceptNewConnectionHook()
{
	m_vExternalStimulations.clear();
}

// Plugin specific methods

void CPluginExternalStimulations::readExternalStimulations()
{
	using namespace boost::interprocess;

	//std::cout << "Creating External Stimulations thread" << std::endl;
	//std::cout << "Queue Name : " << m_sExternalStimulationsQueueName << std::endl;
	//char mq_name[255];
	//std::strcpy(mq_name, m_sExternalStimulationsQueueName.toASCIIString());
	const int chunk_length=3;
	const int pause_time=5;

	unsigned int priority;
	size_t recvd_size;

	uint64 chunk[chunk_length];

	while (m_bIsESThreadRunning)
	{
		bool success = false;
		try
		{
			//Open a message queue.
			message_queue mq
					(open_only  //only open
					 ,m_sExternalStimulationsQueueName.toASCIIString()    //name
					 //,mq_name    //name
					 );

			success = mq.try_receive(&chunk, sizeof(chunk), recvd_size, priority);
		}
		catch(interprocess_exception & /* ex */)
		{
			//m_bIsESThreadRunning = false;
			//m_rKernelContext.getLogManager() << LogLevel_Error << "Problem with message queue in external stimulations:" << ex.what() << "\n";
			boost::this_thread::sleep(boost::posix_time::milliseconds(pause_time));
			continue;
		}

		if (!success)
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(pause_time));
			continue;
		}

		m_iDebugCurrentReadIPCStimulations++;

		if(recvd_size != sizeof(chunk))
		{
			//m_rKernelContext.getLogManager() << LogLevel_Error << "Problem with type of received data when reqding external stimulation!\n";
			m_iDebugStimulationsReceivedWrongSize++;
		}
		else
		{
			//m_rKernelContext.getLogManager() << LogLevel_Warning << "received\n";

			SExternalStimulation stim;

			stim.identifier = chunk[1];
			uint64 received_time = chunk[2];

			//1. calculate time
			uint64 ct_start_time_ms = (m_CTStartTime.time * 1000 + m_CTStartTime.millitm);

			int64 time_test = received_time - ct_start_time_ms;

			if (time_test<0)
			{
				m_iDebugStimulationsLost++;
				//m_rKernelContext.getLogManager() << LogLevel_Warning <<  "AS: external stimulation time is invalid, probably stimulation is before reference point, total invalid so far: " << m_i32FlashesLost << "\n";
				boost::this_thread::sleep(boost::posix_time::milliseconds(pause_time));
				continue; //we skip this stimulation
			}
			//2. Convert to OpenVibe time
			uint64 ct_event_time = received_time - ct_start_time_ms;

			float64 time = (float64)ct_event_time / (float64)1000;

			uint64 ov_time = (uint64)(time * 1024)<<22;
			stim.timestamp = ov_time;

			stim.alreadyCountedAsEarlier = false;

			//3. Store, the main thread will process it
			{
				//lock
				boost::mutex::scoped_lock lock(m_es_mutex);

				m_vExternalStimulations.push_back(stim);
				m_iDebugStimulationsBuffered++;
				m_esAvailable.notify_one();
				//unlock
			}

			boost::this_thread::sleep(boost::posix_time::milliseconds(pause_time));

		}
	}
}

void CPluginExternalStimulations::addExternalStimulations(OpenViBE::CStimulationSet* ss, OpenViBE::Kernel::ILogManager& logm,uint64 start,uint64 end)
{
	uint64 duration_ms = 40;
	{
		//lock
		boost::mutex::scoped_lock lock(m_es_mutex);

		vector<SExternalStimulation>::iterator cii;

		for(cii=m_vExternalStimulations.begin(); cii!=m_vExternalStimulations.end(); cii++)
		{
			if (cii->isProcessed==true) continue;

			// if time matches current chunk being processed - send it
			if (cii->timestamp >= start && cii->timestamp < end)
			{
				//flashes_in_this_time_chunk++;
				//logm << LogLevel_Error << "Stimulation added." << "\n";
				ss->appendStimulation(cii->identifier, cii->timestamp, duration_ms);
				m_iDebugExternalStimulationsSent++;
				//m_vExternalStimulations.erase(cii);
				cii->isProcessed = true;
			}
			else
				//the stimulation is coming too late - after the current block being processed
				//we correct the timestamp to the current block and we send it
				if (cii->timestamp < start)
				{
					m_iDebugStimulationsReceivedLate++;
					ss->appendStimulation(cii->identifier, start, duration_ms);
					m_iDebugExternalStimulationsSent++;
					//m_vExternalStimulations.erase(cii);
					cii->isProcessed = true;
				}
				else //stim.timestamp > end - coming before the currently processed block, so we still can put in the right place
				{
					//save the stimulation for later
					if (!cii->alreadyCountedAsEarlier)
					{
						m_iDebugStimulationsReceivedEarlier++;
						cii->alreadyCountedAsEarlier = true;
					}
					continue;
				}
		}

		m_esAvailable.notify_one();
		//unlock
	}
}


boolean CPluginExternalStimulations::setExternalStimulationsEnabled(boolean bActive)
{
	m_bIsExternalStimulationsEnabled=bActive;
	return true;
}

boolean CPluginExternalStimulations::isExternalStimulationsEnabled(void)
{
	return m_bIsExternalStimulationsEnabled;
}
