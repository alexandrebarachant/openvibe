#ifndef __OpenViBE_AcquisitionServer_CDriverGTecGUSBamp_H__
#define __OpenViBE_AcquisitionServer_CDriverGTecGUSBamp_H__

#if defined TARGET_HAS_ThirdPartyGUSBampCAPI

#include "../ovasIDriver.h"
#include "../ovasCHeader.h"

#include <gtk/gtk.h>
#include <vector>

namespace OpenViBEAcquisitionServer
{
	class CDriverGTecGUSBamp : public OpenViBEAcquisitionServer::IDriver
	{
	public:

		CDriverGTecGUSBamp(OpenViBEAcquisitionServer::IDriverContext& rDriverContext);
		virtual void release(void);
		virtual const char* getName(void);

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

		OpenViBE::uint32 m_ui32ActualImpedanceIndex;

		OpenViBE::uint8 m_ui8CommonGndAndRefBitmap;

		OpenViBE::int32 m_i32NotchFilterIndex;
		OpenViBE::int32 m_i32BandPassFilterIndex;

		OpenViBE::boolean m_bTriggerInputEnabled;
		OpenViBE::uint32 m_ui32LastStimulation;

		typedef enum
		{
			//CHANNEL_NB      = 33,
			//SAMPLING_RATE	= 500,
			//SAMPLES_NB		= 32,
			STIMULATION_0	= 0,
			STIMULATION_128	= 128,
			STIMULATION_64	= 64,
			STIMULATION_192	= (STIMULATION_128 + STIMULATION_64)
		} gtec_stimulations_t;

		OpenViBE::uint32 m_ui32TotalHardwareStimulations;
		//OpenViBE::uint32 m_ui32TotalDriverChunksLost;
	};
};

#endif // TARGET_HAS_ThirdPartyGUSBampCAPI

#endif // __OpenViBE_AcquisitionServer_CDriverGTecGUSBamp_H__
