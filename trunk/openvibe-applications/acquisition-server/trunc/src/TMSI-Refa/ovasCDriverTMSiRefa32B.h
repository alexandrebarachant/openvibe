#ifndef __OpenViBE_AcquisitionServer_CDriverTMSiRefa32B_H__
#define __OpenViBE_AcquisitionServer_CDriverTMSiRefa32B_H__

#include "../ovasIDriver.h"
#include "../ovasCHeader.h"
#include "../ovas_base.h"

#if defined OVAS_OS_Windows

#include "ovasCConfigurationTMSIRefa32B.h"

#define RTLOADER "\\RTINST.Dll"

// Get Signal info

#define SIGNAL_NAME 40
#define MAX_BUFFER_SIZE 0xFFFFFFFF

namespace OpenViBEAcquisitionServer
{
	class CDriverTMSiRefa32B : virtual public OpenViBEAcquisitionServer::IDriver
	{
	public:

		CDriverTMSiRefa32B(OpenViBEAcquisitionServer::IDriverContext& rDriverContext);
		~CDriverTMSiRefa32B(void);
		virtual void release(void);
		virtual const char* getName(void);

		virtual OpenViBE::boolean isFlagSet(
			const OpenViBEAcquisitionServer::EDriverFlag eFlag) const
		{
			return eFlag==DriverFlag_IsUnstable;
		}

		virtual OpenViBE::boolean initialize(
			const OpenViBE::uint32 ui32SampleCountPerSentBlock,
			OpenViBEAcquisitionServer::IDriverCallback& rCallback);
		virtual OpenViBE::boolean uninitialize(void);

		virtual OpenViBE::boolean start(void);
		virtual OpenViBE::boolean stop(void);
		virtual OpenViBE::boolean loop(void);

		virtual OpenViBE::boolean isConfigurable(void);
		virtual OpenViBE::boolean configure(void);
		virtual const OpenViBEAcquisitionServer::IHeader* getHeader(void) { return &m_oHeader; }

	protected:
		OpenViBE::boolean reset(void);
		OpenViBEAcquisitionServer::IDriverCallback* m_pCallback;
		OpenViBEAcquisitionServer::CHeader m_oHeader;

		OpenViBE::boolean m_bInitialized;
		OpenViBE::boolean m_bStarted;
		OpenViBE::uint32 m_ui32SampleCountPerSentBlock;
		OpenViBE::float32 *m_pSample;

		OpenViBE::uint32 m_ui32SampleIndex;

		OpenViBE::uint32 m_ui32StartTime;
		OpenViBE::uint64 m_ui64SampleCountTotal;
		OpenViBE::uint64 m_ui64AutoAddedSampleCount;
		OpenViBE::uint64 m_ui64AutoRemovedSampleCount;
		OpenViBE::boolean refreshDevicePath(void);
	};
};

#endif // OVAS_OS_Windows

#endif // __OpenViBE_AcquisitionServer_CDriverTMSiRefa32B_H__
