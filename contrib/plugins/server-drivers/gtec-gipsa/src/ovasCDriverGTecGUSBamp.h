#ifndef __OpenViBE_AcquisitionServer_CDriverGTecGUSBamp_H__
#define __OpenViBE_AcquisitionServer_CDriverGTecGUSBamp_H__

#if defined TARGET_HAS_ThirdPartyGUSBampCAPI

#include "ovasIDriver.h"
#include "../ovasCHeader.h"

#include <Windows.h>

#include "ringbuffer.h"

#include <gtk/gtk.h>
#include <vector>

//threading
#include <boost\thread\thread.hpp>
#include <boost\bind\bind.hpp>
#include <boost\scoped_ptr.hpp>
#include "boost/thread/condition.hpp"
#include <deque>
using namespace std;

namespace OpenViBEAcquisitionServer
{
	/**
	 * \class CDriverGTecGUSBamp
	 * \author Anton Andreev
	 * \date 19/07/2012
	 * \brief GTEC driver
	 *
	 * This driver was rewritten to match the code provided by Guger as much as possible. There are several things
	 * that all must work together so that higher frequencies are supported and no hardware triggers are lost.
	 *
	 * This driver supports several buffers so that the more than one GT_GetData can be executed in the beginning (QUEUE_SIZE)
	 * and then calls to GT_GetData are queued. This allows data to be processed by OpenVibe while waiting for the next result of
	 * a previously issued GT_GetData. The extra thread is added to support this and to allow for async IO. 
	 *
	 * Hardware triggers on the parallel port are supported.
	 *
	 * This driver also sets the current process to higher priority when started and reverts to normal when stopped. 
	 *
	 * It can also set the GTEC thread (which reading from the amplifier) to higher priority, but you should enable this according
	 * to your needs.
	 *
	 * Currently one limitation: only one device is supported even if most of the code for supporting more is already there
	 */

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

		OpenViBE::boolean CDriverGTecGUSBamp::acquire(void);

		//sets priority - could be used for higher frequencies
		//15 gives you realtime priority
		void CDriverGTecGUSBamp::applyPriority(boost::thread* thread, int priority);

	protected:

		static const int BUFFER_SIZE_SECONDS = 2;		         //the size of the GTEC ring buffer in seconds
		static const int GTEC_NUM_CHANNELS = 16;
		static const int QUEUE_SIZE = 8;//4 default		 //the number of GT_GetData calls that will be queued during acquisition to avoid loss of data
		static const int NUMBER_OF_SCANS = 32;           //the number of scans that should be received simultaneously (depending on the _sampleRate; see C-API documentation for this value!)
		
		OpenViBE::uint32 numDevices;                 //currently this driver supports 1 device 

		static const OpenViBE::uint32 nPoints = NUMBER_OF_SCANS * (GTEC_NUM_CHANNELS + 1);
		int validPoints;
		static const DWORD bufferSizeBytes;

		OpenViBEAcquisitionServer::IDriverCallback* m_pCallback;
		OpenViBEAcquisitionServer::CHeader m_oHeader;

		OpenViBE::uint32 m_ui32SampleCountPerSentBlock;

		OpenViBE::uint32 m_ui32DeviceIndex;
		//OpenViBE::uint32 m_ui32ActualDeviceIndex;
		
		OpenViBE::float32* m_pSample;

		OpenViBE::uint32 m_ui32ActualImpedanceIndex;

		OpenViBE::uint8 m_ui8CommonGndAndRefBitmap;

		OpenViBE::int32 m_i32NotchFilterIndex;
		OpenViBE::int32 m_i32BandPassFilterIndex;

		OpenViBE::boolean m_bTriggerInputEnabled;

		OpenViBE::uint32 m_ui32AcquiredChannelCount;      //number of channels 1..16 specified bu user

		OpenViBE::uint32 m_ui32TotalHardwareStimulations; //since start button clicked
		OpenViBE::uint32 m_ui32TotalDriverChunksLost;     //since start button clicked
		OpenViBE::uint32 m_ui32TotalRingBufferOverruns;
		OpenViBE::uint32 m_ui32TotalDataUnavailable; 

		//contains buffer per device and then QUEUE_SIZE buffers so that several calls to GT_GetData can be supported
		BYTE*** m_buffers;
	    OVERLAPPED** m_overlapped;

		OpenViBE::boolean m_flagIsFirstLoop;
		OpenViBE::boolean m_bufferOverrun;

		//ring buffer provided by Guger
		CRingBuffer<OpenViBE::float32> m_RingBuffer;	

		OpenViBE::uint32 m_ui32CurrentQueueIndex;

		boost::scoped_ptr<boost::thread> m_ThreadPtr;
		OpenViBE::boolean m_bIsThreadRunning;

		boost::mutex m_io_mutex;

		OpenViBE::float32 *m_bufferReceivedData;
		boost::condition  m_itemAvailable;

		deque<HANDLE> m_callSequenceHandles; //stores the handles to the currently opened devices

		OpenViBE::boolean ConfigureDevice(OpenViBE::uint32 deviceNumber);

		OpenViBE::boolean verifySyncMode();//Checks if devices are configured correctly when acquiring data from multiple devices

		//Selects which device to become the new master, used only when more than 1 device is available
		OpenViBE::boolean setMasterDevice(string targetMasterSerial); //0 first device
		void detectDevices();

	    OpenViBE::uint32 m_mastersCnt;
	    OpenViBE::uint32 m_slavesCnt;
	    string m_masterSerial;

		vector<string> m_vDevicesSerials;

	};
};

#endif // TARGET_HAS_ThirdPartyGUSBampCAPI

#endif // __OpenViBE_AcquisitionServer_CDriverGTecGUSBamp_H__
