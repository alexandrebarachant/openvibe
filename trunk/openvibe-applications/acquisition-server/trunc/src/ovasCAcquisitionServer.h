#ifndef __OpenViBE_AcquisitionServer_CAcquisitionServer_H__
#define __OpenViBE_AcquisitionServer_CAcquisitionServer_H__

#include "ovas_base.h"
#include "ovasIDriver.h"
#include "ovasIHeader.h"

#include <socket/IConnectionServer.h>

#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/version.hpp>

#include <string>
#include <vector>
#include <list>

namespace OpenViBEAcquisitionServer
{
	class CDriverContext;
	class CAcquisitionServer : public OpenViBEAcquisitionServer::IDriverCallback
	{
	public:

		CAcquisitionServer(const OpenViBE::Kernel::IKernelContext& rKernelContext);
		virtual ~CAcquisitionServer(void);

		virtual OpenViBEAcquisitionServer::IDriverContext& getDriverContext();

		OpenViBE::uint32 getClientCount(void);
		OpenViBE::float64 getImpedance(const OpenViBE::uint32 ui32ChannelIndex);

		OpenViBE::boolean loop(void);

		OpenViBE::boolean connect(OpenViBEAcquisitionServer::IDriver& rDriver, OpenViBEAcquisitionServer::IHeader& rHeaderCopy, OpenViBE::uint32 ui32SamplingCountPerSentBlock, OpenViBE::uint32 ui32ConnectionPort);
		OpenViBE::boolean start(void);
		OpenViBE::boolean stop(void);
		OpenViBE::boolean disconnect(void);

		// Driver samples information callback
		virtual void setSamples(const OpenViBE::float32* pSample);
		virtual void setSamples(const OpenViBE::float32* pSample, const OpenViBE::uint32 ui32SampleCount);
		virtual void setStimulationSet(const OpenViBE::IStimulationSet& rStimulationSet);

		// Driver context callback
		virtual OpenViBE::boolean isConnected(void) const { return m_bInitialized; }
		virtual OpenViBE::boolean isStarted(void) const { return m_bStarted; }
		virtual OpenViBE::int64 getJitterSampleCount(void) const { return m_i64JitterSampleCount; }
		virtual OpenViBE::int64 getJitterToleranceSampleCount(void) const { return m_i64JitterToleranceSampleCount; }
		virtual OpenViBE::int64 getSuggestedJitterCorrectionSampleCount(void) const;
		virtual OpenViBE::boolean correctJitterSampleCount(OpenViBE::int64 i64SampleCount);
		virtual OpenViBE::boolean updateImpedance(const OpenViBE::uint32 ui32ChannelIndex, const OpenViBE::float64 f64Impedance);

	public:

		boost::mutex m_oExecutionMutex;
		boost::mutex m_oProtectionMutex;

	protected :

		const OpenViBE::Kernel::IKernelContext& m_rKernelContext;
		OpenViBEAcquisitionServer::CDriverContext* m_pDriverContext;
		OpenViBEAcquisitionServer::IDriver* m_pDriver;

		OpenViBE::Kernel::IAlgorithmProxy* m_pAcquisitionStreamEncoder;
		OpenViBE::Kernel::IAlgorithmProxy* m_pExperimentInformationStreamEncoder;
		OpenViBE::Kernel::IAlgorithmProxy* m_pSignalStreamEncoder;
		OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationStreamEncoder;
		OpenViBE::Kernel::IAlgorithmProxy* m_pChannelLocalisationStreamEncoder;

		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pAcquisitionMemoryBuffer;
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pExperimentInformationMemoryBuffer;
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pSignalMemoryBuffer;
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pStimulationMemoryBuffer;
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pChannelLocalisationMemoryBuffer;

		std::list < std::pair < Socket::IConnection*, OpenViBE::uint64 > > m_vConnection;
		std::vector < std::vector < OpenViBE::float32 > > m_vPendingBuffer;
		std::vector < OpenViBE::float32 > m_vSwapBuffer;
		std::vector < OpenViBE::float64 > m_vImpedance;
		Socket::IConnectionServer* m_pConnectionServer;

		OpenViBE::boolean m_bInitialized;
		OpenViBE::boolean m_bStarted;
		OpenViBE::boolean m_bGotData;
		OpenViBE::uint32 m_ui32ChannelCount;
		OpenViBE::uint32 m_ui32SamplingFrequency;
		OpenViBE::uint32 m_ui32SampleCountPerSentBlock;
		OpenViBE::uint64 m_ui64SampleCount;
		OpenViBE::uint64 m_ui64LastSampleCount;
		OpenViBE::uint64 m_ui64StartTime;

		OpenViBE::uint64 m_ui64JitterToleranceDuration;
		OpenViBE::int64 m_i64JitterSampleCount;
		OpenViBE::int64 m_i64JitterToleranceSampleCount;
		OpenViBE::int64 m_i64JitterCorrectionSampleCountAdded;
		OpenViBE::int64 m_i64JitterCorrectionSampleCountRemoved;

		OpenViBE::uint64 m_ui64DriverTimeoutDuration;
		OpenViBE::uint64 m_ui64StartedDriverSleepDuration;
		OpenViBE::uint64 m_ui64StoppedDriverSleepDuration;

		OpenViBE::uint8* m_pSampleBuffer;
		OpenViBE::CStimulationSet m_oPendingStimulationSet;
	};
};

#endif // __OpenViBE_AcquisitionServer_CAcquisitionServer_H__
