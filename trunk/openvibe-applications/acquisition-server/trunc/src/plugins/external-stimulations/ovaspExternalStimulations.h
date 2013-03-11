#ifndef __OpenViBE_AcquisitionServer_ExternalStimulations_H__
#define __OpenViBE_AcquisitionServer_ExternalStimulations_H__

/**
  * \brief Acquisition Server plugin adding the capability to receive stimulations from external sources
  *
  * \author Anton Andreev
  * \author Jozef Legeny
  */

#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/version.hpp>

#include <sys/timeb.h>

#include "../../ovasIAcquisitionServerPlugin.h"

namespace OpenViBEAcquisitionServer
{
	class CAcquisitionServer;

	namespace OpenViBEAcquisitionServerPlugins
	{
		class CExternalStimulations : public IAcquisitionServerPlugin
		{
				// Plugin interface
			public:
				CExternalStimulations(CAcquisitionServer& acquisitionServer);
				virtual ~CExternalStimulations();

				virtual void startHook();
				virtual void stopHook();
				virtual void loopHook(OpenViBE::CStimulationSet& stimulationSet, OpenViBE::uint64 start, OpenViBE::uint64 end);
				virtual void acceptNewConnectionHook();


				// Plugin implementation

				const OpenViBE::Kernel::IKernelContext& m_rKernelContext;

				struct SExternalStimulation
				{
						OpenViBE::uint64 timestamp;
						OpenViBE::uint64 identifier;
						OpenViBE::boolean isProcessed;
						OpenViBE::boolean alreadyCountedAsEarlier;
				};

				void addExternalStimulations(OpenViBE::CStimulationSet*, OpenViBE::Kernel::ILogManager& logm,OpenViBE::uint64 start,OpenViBE::uint64 end);
				void readExternalStimulations();

				void acquireExternalStimulationsVRPN(OpenViBE::CStimulationSet* ss, OpenViBE::Kernel::ILogManager& logm,OpenViBE::uint64 start,OpenViBE::uint64 end);

				struct timeb m_CTStartTime; //time when the acquisition process started in local computer time

				std::vector < SExternalStimulation > m_vExternalStimulations;

				OpenViBE::boolean m_bIsExternalStimulationsEnabled;
				OpenViBE::CString m_sExternalStimulationsQueueName;

				OpenViBE::boolean setExternalStimulationsEnabled(OpenViBE::boolean bActive);
				OpenViBE::boolean isExternalStimulationsEnabled(void);

				// Debugging of external stimulations
				int m_iDebugStimulationsLost;
				int m_iDebugExternalStimulationsSent;
				int m_iDebugCurrentReadIPCStimulations;
				int m_iDebugStimulationsReceivedEarlier;
				int m_iDebugStimulationsReceivedLate;
				int m_iDebugStimulationsReceivedWrongSize;
				int m_iDebugStimulationsBuffered;

				//added for acquiring external stimulations
				boost::scoped_ptr<boost::thread> m_ESthreadPtr;
				OpenViBE::boolean m_bIsESThreadRunning;
				boost::mutex m_es_mutex;
				boost::condition  m_esAvailable;
		};


	}
}

#endif // __OpenViBE_AcquisitionServer_ExternalStimulations_H__
