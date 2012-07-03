#ifndef __OpenViBE_AcquisitionServer_CDriverGTecGUSBamp_H__
#define __OpenViBE_AcquisitionServer_CDriverGTecGUSBamp_H__

#if defined TARGET_HAS_ThirdPartyGUSBampCAPI

#include "../ovasIDriver.h"
#include "../ovasCHeader.h"

#include <gtk/gtk.h>
#include <vector>

#include "..\ovasTCustomConcurrentQueue.h"
#include <boost\thread\thread.hpp>
#include <boost\bind\bind.hpp>
#include <boost\scoped_ptr.hpp>

namespace OpenViBEAcquisitionServer
{
	class CDriverGTecGUSBamp : public OpenViBEAcquisitionServer::IDriver
	{
	public:

		CDriverGTecGUSBamp(OpenViBEAcquisitionServer::IDriverContext& rDriverContext);
		virtual void release(void);
		virtual const char* getName(void);

		virtual OpenViBE::boolean initialize(
		const OpenViBE::uint32 ui32SampleCountPerSentBlock, OpenViBEAcquisitionServer::IDriverCallback& rCallback);
		virtual OpenViBE::boolean uninitialize(void);

		virtual OpenViBE::boolean start(void);
		virtual OpenViBE::boolean stop(void);
		virtual OpenViBE::boolean loop(void);

		virtual OpenViBE::boolean isConfigurable(void);
		virtual OpenViBE::boolean configure(void);
		virtual const OpenViBEAcquisitionServer::IHeader* getHeader(void) { return &m_oHeader; }

	protected:

		OpenViBEAcquisitionServer::IDriverCallback* m_pCallback;
		OpenViBEAcquisitionServer::CHeader m_oHeader;

		OpenViBE::uint32 m_ui32SampleCountPerSentBlock;
		OpenViBE::uint32 m_ui32DeviceIndex;
		OpenViBE::uint32 m_ui32ActualDeviceIndex;
		OpenViBE::uint32 m_ui32BufferSize;
		OpenViBE::uint8* m_pBuffer;
		OpenViBE::float32* m_pSampleTranspose;
		OpenViBE::float32* m_pSample;
		void* m_pDevice;
		void* m_pEvent;
		void* m_pOverlapped;

		/**
		* Threading of GT_GetData : contribution by Anton Andreev (Gipsa-lab)
		**/
		TCustomConcurrentQueue < OpenViBE::float32* > m_qBufferQueue;
		boost::scoped_ptr<boost::thread> m_pThreadPtr;
		boost::mutex m_oMutex;
		OpenViBE::boolean m_bIsThreadRunning;
		/********************************************************************/

		void acquire(void);

		OpenViBE::uint32 m_ui32ActualImpedanceIndex;

		OpenViBE::uint8 m_ui8CommonGndAndRefBitmap;

		OpenViBE::int32 m_i32NotchFilterIndex;
		OpenViBE::int32 m_i32BandPassFilterIndex;

		/**
		* Event Channel implementation : contribution by Anton Andreev (Gipsa-lab)
		**/
		OpenViBE::boolean m_bTriggerInputEnabled;
		OpenViBE::uint32 m_ui32LastStimulation;

		typedef enum
		{
			STIMULATION_0	= 0,
			STIMULATION_64	= 64,
			STIMULATION_128	= 128,
			STIMULATION_192	= 192
		} gtec_triggers_t;
		/********************************************************************/

		OpenViBE::uint32 m_ui32TotalHardwareStimulations; //since start button clicked
		OpenViBE::uint32 m_ui32TotalDriverChunksLost; //since start button clicked
		OpenViBE::uint32 m_ui32AcquiredChannelCount; //number of channels 1..16 specified by user

	};
};

#endif // TARGET_HAS_ThirdPartyGUSBampCAPI

#endif // __OpenViBE_AcquisitionServer_CDriverGTecGUSBamp_H__
