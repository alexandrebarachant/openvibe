#if defined TARGET_HAS_ThirdPartyGUSBampCAPI

#include "ovasCDriverGTecGUSBamp.h"
#include "ovasCConfigurationGTecGUSBamp.h"

#include <openvibe-toolkit/ovtk_all.h>

#include <system/Time.h>

#include <cmath>

#include <iostream>

#include <windows.h>
#include <gUSBamp.h>
#define boolean OpenViBE::boolean

using namespace OpenViBEAcquisitionServer;
using namespace OpenViBE;
using namespace std;

static const uint32 g_ui32AcquiredChannelCount=16;

//___________________________________________________________________//
//                                                                   //

CDriverGTecGUSBamp::CDriverGTecGUSBamp(IDriverContext& rDriverContext)
	:IDriver(rDriverContext)
	,m_pCallback(NULL)
	,m_bInitialized(false)
	,m_bStarted(false)
	,m_ui32SampleCountPerSentBlock(0)
	,m_ui32DeviceIndex(uint32(-1))
	,m_ui32BufferSize(0)
	,m_pBuffer(NULL)
	,m_pSample(NULL)
	,m_pDevice(NULL)
	,m_pEvent(NULL)
	,m_pOverlapped(NULL)
{
	m_oHeader.setSamplingFrequency(512);
	m_oHeader.setChannelCount(16);
}

void CDriverGTecGUSBamp::release(void)
{
	delete this;
}

const char* CDriverGTecGUSBamp::getName(void)
{
	return "g.Tec gUSBamp";
}

//___________________________________________________________________//
//                                                                   //

boolean CDriverGTecGUSBamp::initialize(
	const uint32 ui32SampleCountPerSentBlock,
	IDriverCallback& rCallback)
{
	if(m_bInitialized)
	{
		return false;
	}

	if(!m_oHeader.isChannelCountSet()
	 ||!m_oHeader.isSamplingFrequencySet())
	{
		return false;
	}

	// If device has not been selected, try to find a device
	uint32 i=0, l_ui32DeviceIndex=m_ui32DeviceIndex;
	while(i < 11 && l_ui32DeviceIndex==uint32(-1))
	{
		::HANDLE l_pHandle=::GT_OpenDevice(i);
		if(l_pHandle)
		{
			::GT_CloseDevice(&l_pHandle);
			l_ui32DeviceIndex=i;
		}
		i++;
	}

	if(l_ui32DeviceIndex==uint32(-1))
	{
		return false;
	}

	m_pEvent=::CreateEvent(NULL, FALSE, FALSE, NULL);
	if(!m_pEvent)
	{
		return false;
	}

	m_ui32BufferSize=(g_ui32AcquiredChannelCount+1)*ui32SampleCountPerSentBlock*sizeof(float)+HEADER_SIZE;
	m_pBuffer=new uint8[m_ui32BufferSize];
	m_pSample=new float32[m_oHeader.getChannelCount()*ui32SampleCountPerSentBlock];
	if(!m_pBuffer || !m_pSample)
	{
		delete [] m_pBuffer;
		delete [] m_pSample;
		::CloseHandle(m_pEvent);
		return false;
	}
	::memset(m_pBuffer, 0, m_ui32BufferSize);
	m_pSampleTranspose=reinterpret_cast<float32*>(m_pBuffer+HEADER_SIZE);
	m_pOverlapped=new ::OVERLAPPED;
	if(!m_pOverlapped)
	{
		delete [] m_pBuffer;
		delete [] m_pSample;
		::CloseHandle(m_pEvent);
		return false;
	}
#define m_pOverlapped ((::OVERLAPPED*)m_pOverlapped)
	::memset(m_pOverlapped, 0, sizeof(::OVERLAPPED));
	m_pOverlapped->hEvent=m_pEvent;

	m_pDevice=::GT_OpenDevice(l_ui32DeviceIndex);
	if(!m_pDevice)
	{
		delete m_pOverlapped;
		delete [] m_pBuffer;
		delete [] m_pSample;
		::CloseHandle(m_pEvent);
		return false;
	}

	::UCHAR l_oChannel[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

	::REF l_oReference;
	l_oReference.ref1=FALSE;
	l_oReference.ref2=FALSE;
	l_oReference.ref3=FALSE;
	l_oReference.ref4=FALSE;

	::GND l_oGround;
	l_oGround.GND1=FALSE;
	l_oGround.GND2=FALSE;
	l_oGround.GND3=FALSE;
	l_oGround.GND4=FALSE;

	if(!::GT_SetMode(m_pDevice, M_NORMAL)) std::cout << "err GT_SetMode\n";
	if(!::GT_SetBufferSize(m_pDevice, ui32SampleCountPerSentBlock)) std::cout << "err GT_SetBufferSize\n";
	if(!::GT_SetChannels(m_pDevice, l_oChannel, sizeof(l_oChannel)/sizeof(::UCHAR))) std::cout << "err GT_SetChannels\n";
	if(!::GT_SetSlave(m_pDevice, FALSE)) std::cout << "err GT_SetSlave\n";
	if(!::GT_EnableTriggerLine(m_pDevice, TRUE)) std::cout << "err GT_EnableTriggerLine\n";
// GT_EnableSC
// GT_SetBipolar
/* */
	for(uint32 i=0; i<g_ui32AcquiredChannelCount; i++)
	{
		if(!::GT_SetBandPass(m_pDevice, i+1, -1)) std::cout << "err GT_SetBandPass for channel " << i << "\n";
		if(!::GT_SetNotch(m_pDevice, i+1, -1)) std::cout << "err GT_SetNotch for channel " << i << "\n";
	}
/* */
	if(!::GT_SetSampleRate(m_pDevice, m_oHeader.getSamplingFrequency())) std::cout << "err GT_SetSampleRate\n";
#if 1 // most probably not necessary with g.GAMMAbox
	if(!::GT_SetReference(m_pDevice, l_oReference)) std::cout << "err GT_SetReference\n";
	if(!::GT_SetGround(m_pDevice, l_oGround)) std::cout << "err GT_SetGround\n";
#endif
// GT_SetDAC

	m_ui32SampleCountPerSentBlock=ui32SampleCountPerSentBlock;
	m_pCallback=&rCallback;
	m_bInitialized=true;

	return true;
}

boolean CDriverGTecGUSBamp::start(void)
{
	if(!m_bInitialized)
	{
		return false;
	}

	if(m_bStarted)
	{
		return false;
	}

	::GT_Start(m_pDevice);

	m_bStarted=true;

	return true;
}

boolean CDriverGTecGUSBamp::loop(void)
{
	if(!m_bInitialized)
	{
		return false;
	}

	if(!m_bStarted)
	{
		return false;
	}

	if(::GT_GetData(m_pDevice, m_pBuffer, m_ui32BufferSize, m_pOverlapped))
	{
		if(::WaitForSingleObject(m_pOverlapped->hEvent, 2000)==WAIT_OBJECT_0)
		{
			DWORD l_dwByteCount=0;
			::GetOverlappedResult(m_pDevice, m_pOverlapped, &l_dwByteCount, FALSE);
			if(l_dwByteCount==m_ui32BufferSize)
			{
				for(uint32 i=0; i<m_oHeader.getChannelCount() && i<g_ui32AcquiredChannelCount; i++)
				{
					for(uint32 j=0; j<m_ui32SampleCountPerSentBlock; j++)
					{
						m_pSample[i*m_ui32SampleCountPerSentBlock+j]=m_pSampleTranspose[j*(g_ui32AcquiredChannelCount+1)+i];
					}
				}
				m_pCallback->setSamples(m_pSample);

				// TODO manage stims
			}
			else
			{
				// std::cout << "l_dwByteCount and m_ui32BufferSize differs : " << l_dwByteCount << "/" << m_ui32BufferSize << "(header size is " << HEADER_SIZE << ")\n";
			}
		}
		else
		{
			// TIMEOUT
			// std::cout << "timeout 1\n";
		}
	}
	else
	{
		// TIMEOUT
		// std::cout << "timeout 2\n";
	}

	return true;
}

boolean CDriverGTecGUSBamp::stop(void)
{
	if(!m_bInitialized)
	{
		return false;
	}

	if(!m_bStarted)
	{
		return false;
	}

	::GT_Stop(m_pDevice);
	::GT_ResetTransfer(m_pDevice);

	m_bStarted=false;

	return !m_bStarted;
}

boolean CDriverGTecGUSBamp::uninitialize(void)
{
	if(!m_bInitialized)
	{
		return false;
	}

	if(m_bStarted)
	{
		return false;
	}

	::GT_CloseDevice(&m_pDevice);
	::CloseHandle(m_pEvent);
	delete [] m_pBuffer;
	delete [] m_pSample;
	delete m_pOverlapped;
	m_pDevice=NULL;
	m_pEvent=NULL;
	m_pBuffer=NULL;
	m_pSample=NULL;
	m_pCallback=NULL;

	m_bInitialized=false;

	return true;
}

//___________________________________________________________________//
//                                                                   //
boolean CDriverGTecGUSBamp::isConfigurable(void)
{
	return true;
}

boolean CDriverGTecGUSBamp::configure(void)
{
	CConfigurationGTecGUSBamp m_oConfiguration("../share/openvibe-applications/acquisition-server/interface-GTec-GUSBamp.glade", m_ui32DeviceIndex);
	if(!m_oConfiguration.configure(m_oHeader))
	{
		return false;
	}
	return true;
}

#endif // TARGET_HAS_ThirdPartyGUSBampCAPI
