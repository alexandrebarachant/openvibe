#ifndef __OpenViBE_AcquisitionServer_CDriverMicromedIntraEEG_H__
#define __OpenViBE_AcquisitionServer_CDriverMicromedIntraEEG_H__

#include "../ovasIDriver.h"
#include "../ovasCHeader.h"

#if defined OVAS_OS_Windows
#include <openvibe/ov_all.h>
#include <iostream>
#include <socket/IConnectionClient.h>
#include <socket/IConnectionServer.h>

namespace OpenViBEAcquisitionServer
{
	class CDriverMicromedIntraEEG : public OpenViBEAcquisitionServer::IDriver
	{
	public:

		CDriverMicromedIntraEEG(OpenViBEAcquisitionServer::IDriverContext& rDriverContext);
		virtual ~CDriverMicromedIntraEEG(void);
		virtual const char* getName(void);

/*
		virtual OpenViBE::boolean isFlagSet(
			const OpenViBEAcquisitionServer::EDriverFlag eFlag) const
		{
			return eFlag==DriverFlag_IsUnstable;
		}
*/

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

		Socket::IConnectionServer* m_pConnectionServer;
		OpenViBE::uint32 m_ui32ServerHostPort;
		Socket::IConnection* m_pConnection;
		short MyReceive(char* buf, long dataLen);

	protected:

		virtual OpenViBE::boolean dropData();
		OpenViBE::boolean m_bValid;

		OpenViBEAcquisitionServer::IDriverCallback* m_pCallback;
		OpenViBEAcquisitionServer::CHeader m_oHeader;

		OpenViBE::uint32 m_ui32SampleCountPerSentBlock;
		OpenViBE::float32* m_pSample;

		OpenViBE::uint32 m_ui32IndexIn;
		OpenViBE::uint32 m_ui32IndexOut;
		OpenViBE::uint32 m_ui32BuffDataIndex;

		char* m_pStructHeader;
		char* m_pStructHeaderInfo;
		unsigned short int *m_pStructBuffData;
	};
};

#endif
#endif // __OpenViBE_AcquisitionServer_CDriverMicromedIntraEEG_H__
