#include "ovas_driver_MindMedia_NeXus32B.h"

#if defined OVAS_OS_Windows
 #include <windows.h>
 #define boolean OpenViBEAcquisitionServer::boolean
#endif

using namespace OpenViBEAcquisitionServer;

#if defined OVAS_OS_Windows

#define _MindMedia_NeXus32B_DLLFileName_ "NeXusDLL.dll"

//___________________________________________________________________//
//                                                                   //

typedef void (*NeXusDLL_ProcessData)(int iSampleCount, int iChannel, float* pSample);
typedef ::DWORD (*NeXusDLL_Init)(::NeXusDLL_ProcessData fpProcessData);
typedef ::DWORD (*NeXusDLL_Start)(::DWORD* dwSamplingRate);
typedef ::DWORD (*NeXusDLL_Stop)(void);

//___________________________________________________________________//
//                                                                   //

static ovasDriverProcessDataCB g_fpDriverProcessData=NULL;
static void* g_pUserData=NULL;
static boolean g_bInitialized=false;
static boolean g_bStarted=false;
static uint32 g_ui32SamplingRate=0;
static uint32 g_ui32ChannelCount=0;
static uint32 g_ui32SampleCountPerChannel=0;
static float32* g_pSample=NULL;

static uint32 g_ui32SampleIndex=0;

static HINSTANCE g_hNeXusDLLInstance=NULL;
static NeXusDLL_Init g_fpNeXusDLLInit=NULL;
static NeXusDLL_Start g_fpNeXusDLLStart=NULL;
static NeXusDLL_Stop g_fpNeXusDLLStop=NULL;

//___________________________________________________________________//
//                                                                   //

static void processData(int iSampleCount, int iChannel, float* pSample)
{
	for(int i=0; i<g_ui32ChannelCount; i++)
	{
		g_pSample[g_ui32SampleIndex+i*g_ui32SampleCountPerChannel]=pSample[i];
	}
	g_ui32SampleIndex++;
	g_ui32SampleIndex%=g_ui32SampleCountPerChannel;
	if(!g_ui32SampleIndex)
	{
		g_fpDriverProcessData(g_pSample, g_pUserData);
	}
}

static boolean ovasDriverInitializeImpl(
	const uint32 ui32SamplingRate,
	const uint32 ui32ChannelCount,
	const uint32 ui32SampleCountPerChannel,
	ovasDriverProcessDataCB fpProcessData,
	void* pUserData)
{
	if(g_bInitialized)
	{
		return false;
	}

	if(!fpProcessData)
	{
		return false;
	}

	g_hNeXusDLLInstance=::LoadLibrary(_MindMedia_NeXus32B_DLLFileName_);
	if(!g_hNeXusDLLInstance)
	{
		return false;
	}

	g_fpNeXusDLLInit=(NeXusDLL_Init)GetProcAddress(g_hNeXusDLLInstance, "InitNeXusDevice");
	g_fpNeXusDLLStart=(NeXusDLL_Start)GetProcAddress(g_hNeXusDLLInstance,"StartNeXusDevice");
	g_fpNeXusDLLStop=(NeXusDLL_Stop)GetProcAddress(g_hNeXusDLLInstance, "StopNeXusDevice");
	g_pSample=new float32[ui32ChannelCount*ui32SampleCountPerChannel];

	if(!g_fpNeXusDLLInit || !g_fpNeXusDLLStart || !g_fpNeXusDLLStop || !g_pSample)
	{
		::FreeLibrary(g_hNeXusDLLInstance);
		delete [] g_pSample;
		g_hNeXusDLLInstance=NULL;
		g_fpNeXusDLLInit=NULL;
		g_fpNeXusDLLStart=NULL;
		g_fpNeXusDLLStop=NULL;
		g_pSample=NULL;
		return false;
	}

	::DWORD l_dwError=g_fpNeXusDLLInit(processData);
	if(l_dwError)
	{
		::FreeLibrary(g_hNeXusDLLInstance);
		delete [] g_pSample;
		g_hNeXusDLLInstance=NULL;
		g_fpNeXusDLLInit=NULL;
		g_fpNeXusDLLStart=NULL;
		g_fpNeXusDLLStop=NULL;
		g_pSample=NULL;
		return false;
	}

	g_fpDriverProcessData=fpProcessData;
	g_pUserData=pUserData;
	g_bInitialized=true;
	g_ui32SamplingRate=ui32SamplingRate;
	g_ui32ChannelCount=ui32ChannelCount;
	g_ui32SampleCountPerChannel=ui32SampleCountPerChannel;
	g_ui32SampleIndex=0;

	return true;
}

static boolean ovasDriverStartImpl(void)
{
	if(!g_bInitialized)
	{
		return false;
	}

	if(g_bStarted)
	{
		return false;
	}

	::DWORD l_dwSamplingFrequency=::DWORD(g_ui32SamplingRate);
	::DWORD l_dwError=g_fpNeXusDLLStart(&l_dwSamplingFrequency);
	g_bStarted=(l_dwError?false:true);
	return g_bStarted;
}

static boolean ovasDriverLoopImpl(void)
{
	if(!g_bInitialized)
	{
		return false;
	}

	if(!g_bStarted)
	{
		return false;
	}

	return true;
}

static boolean ovasDriverStopImpl(void)
{
	if(!g_bInitialized)
	{
		return false;
	}

	if(!g_bStarted)
	{
		return false;
	}

	::DWORD l_dwError=g_fpNeXusDLLStop();
	g_bStarted=(l_dwError?true:false);
	return !g_bStarted;
}

static boolean ovasDriverUninitializeImpl(void)
{
	if(!g_bInitialized)
	{
		return false;
	}

	if(g_bStarted)
	{
		return false;
	}

	g_bInitialized=false;

	::FreeLibrary(g_hNeXusDLLInstance);
	delete [] g_pSample;
	g_hNeXusDLLInstance=NULL;
	g_fpNeXusDLLInit=NULL;
	g_fpNeXusDLLStart=NULL;
	g_fpNeXusDLLStop=NULL;
	g_pSample=NULL;

	return true;
}

#endif

SDriver OpenViBEAcquisitionServer::getDriver_MindMedia_NeXus32B(void)
{
	SDriver l_oResult;
#if defined OVAS_OS_Windows
	l_oResult.m_fpInitialize=ovasDriverInitializeImpl;
	l_oResult.m_fpStart=ovasDriverStartImpl;
	l_oResult.m_fpLoop=ovasDriverLoopImpl;
	l_oResult.m_fpStop=ovasDriverStopImpl;
	l_oResult.m_fpUninitialize=ovasDriverUninitializeImpl;
#else
	l_oResult.m_fpInitialize=NULL;
	l_oResult.m_fpStart=NULL;
	l_oResult.m_fpLoop=NULL;
	l_oResult.m_fpStop=NULL;
	l_oResult.m_fpUninitialize=NULL;
#endif
	return l_oResult;
}
