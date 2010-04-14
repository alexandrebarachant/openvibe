#if defined TARGET_HAS_ThirdPartyUSBFirstAmpAPI

#include "ovasCDriverBrainProductsVAmp.h"
#include "ovasCConfigurationBrainProductsVAmp.h"
#include "ovasCHeaderBrainProductsVAmp.h"

#include <system/Time.h>
#include <windows.h>

#include <cstdlib>
#include <cstring>

#include <iostream>
#include <vector>

using namespace OpenViBEAcquisitionServer;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace std;

#define boolean OpenViBE::boolean

//___________________________________________________________________//
//                                                                   //

CDriverBrainProductsVAmp::CDriverBrainProductsVAmp(IDriverContext& rDriverContext)
	:IDriver(rDriverContext)
	,m_pCallback(NULL)
	,m_ui32SampleCountPerSentBlock(0)
	,m_ui32TotalSampleCount(0)
	,m_pSample(NULL)
	,m_bFirstStart(false)
{
	m_oHeader.setChannelCount(16);
	m_oHeader.setSamplingFrequency(2000);

	t_faDataModeSettings l_tFastModeSettings;
	l_tFastModeSettings.Mode20kHz4Channels.ChannelsPos[0] = 7;
	l_tFastModeSettings.Mode20kHz4Channels.ChannelsNeg[0] = -1;
	l_tFastModeSettings.Mode20kHz4Channels.ChannelsPos[1] = 8;
	l_tFastModeSettings.Mode20kHz4Channels.ChannelsNeg[1] = -1;
	l_tFastModeSettings.Mode20kHz4Channels.ChannelsPos[2] = 9;
	l_tFastModeSettings.Mode20kHz4Channels.ChannelsNeg[2] = -1;
	l_tFastModeSettings.Mode20kHz4Channels.ChannelsPos[3] = 10;
	l_tFastModeSettings.Mode20kHz4Channels.ChannelsNeg[3] = -1;

	m_oHeader.setFastModeSettings(l_tFastModeSettings);
	m_oHeader.setDataMode(dmNormal);

	m_oHeader.setDeviceId(FA_ID_INVALID);
}

CDriverBrainProductsVAmp::~CDriverBrainProductsVAmp(void)
{
}

const char* CDriverBrainProductsVAmp::getName(void)
{
	return "Brain Products V-Amp";
}

//___________________________________________________________________//
//                                                                   //

boolean CDriverBrainProductsVAmp::initialize(
	const uint32 ui32SampleCountPerSentBlock,
	IDriverCallback& rCallback)
{
	m_rDriverContext.getLogManager() << LogLevel_Trace << "INIT called.\n";
	if(m_rDriverContext.isConnected())
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "[INIT] VAmp Driver: Driver already initialized.\n";
		return false;
	}

	if(!m_oHeader.isChannelCountSet()
	 ||!m_oHeader.isSamplingFrequencySet())
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "[INIT] VAmp Driver: Channel count or frequency not set.\n";
		return false;
	}

	// Builds up a buffer to store acquired samples. This buffer will be sent to the acquisition server later.
	m_pSample=new float32[m_oHeader.getChannelCount()*ui32SampleCountPerSentBlock];
	if(!m_pSample)
	{
		delete [] m_pSample;
		m_pSample=NULL;
		m_rDriverContext.getLogManager() << LogLevel_Error << "[INIT] VAmp Driver: Samples allocation failed.\n";
		return false;
	}

	int32 l_i32DeviceId = m_oHeader.getDeviceId();
	//__________________________________
	// Hardware initialization

	// if no device selected with the properties dialog
	// we take the last device connected
	if(l_i32DeviceId == FA_ID_INVALID)
	{
		// We try to get the last opened device,
		uint32 l_uint32LastOpenedDeviceID = faGetCount(); // Get the last opened Device id.

		if (l_uint32LastOpenedDeviceID == FA_ID_INVALID) // failed
		{
			m_rDriverContext.getLogManager() << LogLevel_Error << "[INIT] VAmp Driver: faGetCount failed to get last opened device.\n";
			return false;
		}

		l_i32DeviceId = faGetId(l_uint32LastOpenedDeviceID -1);
		m_oHeader.setDeviceId(l_i32DeviceId);
	}

	if (l_i32DeviceId != FA_ID_INVALID)
	{
		m_rDriverContext.getLogManager() << LogLevel_Trace << "[INIT] VAmp Driver: Active device ID(" << m_oHeader.getDeviceId() << ").\n";
	}
	else
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "[INIT] VAmp Driver: No device connected !\n";
		return false;
	}

	// Open the device.
	uint32 l_uint32OpenReturn = faOpen(l_i32DeviceId);
	if (l_uint32OpenReturn != FA_ERR_OK)
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "[INIT] VAmp Driver: faOpen(" << l_i32DeviceId << ") FAILED(" << l_uint32OpenReturn << ").\n";
		return false;
	}

    if(m_oHeader.getDataMode() == dm20kHz4Channels)
    {
        faSetDataMode(l_i32DeviceId, dm20kHz4Channels, &(m_oHeader.getFastModeSettings()));
    }
    else
    {
        faSetDataMode(l_i32DeviceId, dmNormal, NULL);
    }

    uint32 l_uint32ErrorCode = faStart(l_i32DeviceId);

    if (l_uint32ErrorCode != FA_ERR_OK)
    {
        m_rDriverContext.getLogManager() << LogLevel_Error << "[START] VAmp Driver: faStart FAILED(" << l_uint32ErrorCode << "). Closing device.\n";
        faClose(l_i32DeviceId);
        return false;
    }

	//__________________________________
	// Saves parameters
	m_pCallback=&rCallback;
	m_ui32SampleCountPerSentBlock=ui32SampleCountPerSentBlock;

	return true;
}

boolean CDriverBrainProductsVAmp::start(void)
{
	m_rDriverContext.getLogManager() << LogLevel_Trace << "START called.\n";
	if(!m_rDriverContext.isConnected())
	{
		return false;
	}

	if(m_rDriverContext.isStarted())
	{
		return false;
	}

	m_bFirstStart = true;
	//The bonus...
	HBITMAP l_bitmap = (HBITMAP) LoadImage(NULL, "../share/openvibe-applications/acquisition-server/vamp.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if(l_bitmap == NULL || faSetBitmap(m_oHeader.getDeviceId(),l_bitmap ) != FA_ERR_OK)
	{
		m_rDriverContext.getLogManager() << LogLevel_Warning << "[START] VAmp Driver: BMP load failed.\n";
	}

	return true;

}

boolean CDriverBrainProductsVAmp::loop(void)
{
	if(!m_rDriverContext.isConnected())
	{
		return false;
	}

    t_faDataModel16 l_DataBufferNormalMode; // buffer for the next block in normal mode
    uint32 l_uint32ReadLengthNormalMode = sizeof(t_faDataModel16);

    t_faDataFormatMode20kHz l_DataBufferFastMode; // buffer for fast mode acquisition
    uint32 l_uint32ReadLengthFastMode = sizeof(t_faDataFormatMode20kHz);

    int32 l_i32DeviceId = m_oHeader.getDeviceId();

	if(m_rDriverContext.isStarted())
	{
		//uint32 l_i32Timeout = 1000; // 1 second timeout
		uint32 l_i32ReceivedSamples=0;
		//uint32 l_ui32StartTime=System::Time::getTime();

#if DEBUG
		uint32 l_uint32ReadErrorCount = 0;
		uint32 l_uint32ReadSuccessCount = 0;
		uint32 l_uint32ReadZeroCount = 0;
#endif
		//------------------------------------------
 		if(m_bFirstStart)
 		{
 			int32 l_i32ReturnValue = 1;
			int32 l_i32BufferCount = 0;
 			while(l_i32ReturnValue > 0)
 			{
 				if(m_oHeader.getDataMode() == dmNormal)
 				{
 					l_i32ReturnValue = faGetData(l_i32DeviceId, &l_DataBufferNormalMode, l_uint32ReadLengthNormalMode);
 				}

 				if(m_oHeader.getDataMode() == dm20kHz4Channels)
 				{
 					l_i32ReturnValue = faGetData(l_i32DeviceId, &l_DataBufferFastMode, l_uint32ReadLengthFastMode);
 				}
				if(l_i32ReturnValue>0) l_i32BufferCount+=l_i32ReturnValue;
 			}
			m_rDriverContext.getLogManager() << LogLevel_Trace << "[LOOP] cleaning buffer ("<<l_i32BufferCount<<" bytes). \n";
 			m_bFirstStart = false;
 		}

		while(l_i32ReceivedSamples < m_ui32SampleCountPerSentBlock /*&& System::Time::getTime()-l_ui32StartTime < l_i32Timeout*/)
		{
			// we need to "getData" with the right output structure according to acquisition mode

			int32 l_i32ReturnLength = 0;
			if(m_oHeader.getDataMode() == dmNormal)
			{
				l_i32ReturnLength = faGetData(l_i32DeviceId, &l_DataBufferNormalMode, l_uint32ReadLengthNormalMode);
			}

			if(m_oHeader.getDataMode() == dm20kHz4Channels)
			{
				l_i32ReturnLength = faGetData(l_i32DeviceId, &l_DataBufferFastMode, l_uint32ReadLengthFastMode);
			}

			if(l_i32ReturnLength > 0)
			{
#if DEBUG
				l_uint32ReadSuccessCount++;
#endif
				//we just received one set of samples from device, one sample per channel
				if(m_oHeader.getDataMode() == dmNormal)
				{
					for (uint32 i=0; i < m_oHeader.getChannelCount(); i++)
					{
						m_pSample[i*m_ui32SampleCountPerSentBlock+l_i32ReceivedSamples] = (float32)(l_DataBufferNormalMode.Main[i]*m_oHeader.getChannelGain(i));
					}
				}
				// 4 pairs, not related to channel count
				if(m_oHeader.getDataMode() == dm20kHz4Channels)
				{
					for (uint32 i=0; i < m_oHeader.getChannelCount(); i++) // channel count returns the pair count is that case
					{
						m_pSample[i*m_ui32SampleCountPerSentBlock+l_i32ReceivedSamples] = (float32)(l_DataBufferFastMode.Main[i]*m_oHeader.getChannelGain(i));
					}
				}

				l_i32ReceivedSamples++;
			}
#if DEBUG
			if(l_i32ReturnLength < 0)
			{

				l_uint32ReadErrorCount++;

			}
			if(l_i32ReturnLength == 0)
			{
				l_uint32ReadZeroCount++;

			}
#endif
		}// while received < m_ui32SampleCountPerSentBlock
#if DEBUG
        m_rDriverContext.getLogManager() << LogLevel_Debug << "[LOOP] VAmp Driver: stats for the current block : Success="<<l_uint32ReadSuccessCount<<" Error="<<l_uint32ReadErrorCount<<" Zero="<<l_uint32ReadZeroCount<<"\n";
#endif
		//____________________________

		// no stimulations received from hardware, the set is empty
		CStimulationSet l_oStimulationSet;

		m_pCallback->setSamples(m_pSample);

		m_pCallback->setStimulationSet(l_oStimulationSet);
	}

	return true;
}

boolean CDriverBrainProductsVAmp::stop(void)
{

	m_rDriverContext.getLogManager() << LogLevel_Trace << "STOP called.\n";
	if(!m_rDriverContext.isConnected())
	{
		return false;
	}

	if(!m_rDriverContext.isStarted())
	{
		return false;
	}

	m_bFirstStart = false;

	return true;
}

boolean CDriverBrainProductsVAmp::uninitialize(void)
{
	if(!m_rDriverContext.isConnected())
	{
		return false;
	}

	if(m_rDriverContext.isStarted())
	{
		return false;
	}

	uint32 l_uint32ErrorCode = faStop(m_oHeader.getDeviceId());
	if (l_uint32ErrorCode != FA_ERR_OK)
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "[STOP] VAmp Driver: faStop FAILED(" << l_uint32ErrorCode << ").\n";
		faClose(m_oHeader.getDeviceId());
		return false;
	}

	m_rDriverContext.getLogManager() << LogLevel_Trace << "Uninitialize called. Closing the device.\n";

	l_uint32ErrorCode = faClose(m_oHeader.getDeviceId());
	if (l_uint32ErrorCode != FA_ERR_OK)
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "[UINIT] VAmp Driver: faClose FAILED(" << l_uint32ErrorCode << ").\n";
		faClose(m_oHeader.getDeviceId());
		return false;
	}
	delete [] m_pSample;
	m_pSample=NULL;
	m_pCallback=NULL;

	return true;
}

//___________________________________________________________________//
//                                                                   //

boolean CDriverBrainProductsVAmp::isConfigurable(void)
{
	return true;
}

boolean CDriverBrainProductsVAmp::configure(void)
{
	CConfigurationBrainProductsVAmp m_oConfiguration(m_rDriverContext, "../share/openvibe-applications/acquisition-server/interface-BrainProducts-VAmp.glade", &m_oHeader); // the specific header is passed into the specific configuration

	if(!m_oConfiguration.configure(*(m_oHeader.getBasicHeader()))) // the basic configure will use the basic header
	{
		return false;
	}

	if(m_oHeader.getDataMode() == dm20kHz4Channels)
	{
		m_rDriverContext.getLogManager() << LogLevel_Trace << "Pair names :\n";
		for(uint32 i = 0; i < m_oHeader.getPairCount();i++)
		{
			m_rDriverContext.getLogManager() << LogLevel_Trace << "  Pair " << i << " > " << m_oHeader.getPairName(i) << "\n";
		}
	}

	return true;
}

#endif // TARGET_HAS_ThirdPartyUSBFirstAmpAPI
