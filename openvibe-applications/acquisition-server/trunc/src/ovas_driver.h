#ifndef __OpenViBE_AcquisitionServer_Driver_H__
#define __OpenViBE_AcquisitionServer_Driver_H__

#include "ovas_base.h"

namespace OpenViBEAcquisitionServer
{
	typedef void (*ovasDriverProcessDataCB)(const OpenViBEAcquisitionServer::float32* pSample, void* pUserData);
	typedef OpenViBEAcquisitionServer::boolean (*ovasDriverInitialize)(const OpenViBEAcquisitionServer::uint32 ui32SamplingRate, const OpenViBEAcquisitionServer::uint32 ui32ChannelCount, const OpenViBEAcquisitionServer::uint32 ui32SampleCountPerChannel, ovasDriverProcessDataCB fpProcessData, void* pUserData);
	typedef OpenViBEAcquisitionServer::boolean (*ovasDriverStart)(void);
	typedef OpenViBEAcquisitionServer::boolean (*ovasDriverLoop)(void);
	typedef OpenViBEAcquisitionServer::boolean (*ovasDriverStop)(void);
	typedef OpenViBEAcquisitionServer::boolean (*ovasDriverUninitialize)(void);

	typedef struct
	{
		OpenViBEAcquisitionServer::ovasDriverInitialize m_fpInitialize;
		OpenViBEAcquisitionServer::ovasDriverStart m_fpStart;
		OpenViBEAcquisitionServer::ovasDriverLoop m_fpLoop;
		OpenViBEAcquisitionServer::ovasDriverStop m_fpStop;
		OpenViBEAcquisitionServer::ovasDriverUninitialize m_fpUninitialize;
	} SDriver;
};

#endif // __OpenViBE_AcquisitionServer_Driver_H__
