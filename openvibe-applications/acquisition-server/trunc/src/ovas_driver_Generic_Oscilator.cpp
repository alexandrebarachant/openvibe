#include "ovas_driver_Generic_Oscilator.h"

#include <system/Time.h>

#include <math.h>

#include <iostream>

using namespace OpenViBEAcquisitionServer;

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

static uint32 g_ui32TotalSampleCount=0;
static uint32 g_ui32LastTime;

//___________________________________________________________________//
//                                                                   //

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

	g_pSample=new float32[ui32ChannelCount*ui32SampleCountPerChannel];
	if(!g_pSample)
	{
		delete [] g_pSample;
		g_pSample=NULL;
		return false;
	}

	g_fpDriverProcessData=fpProcessData;
	g_pUserData=pUserData;
	g_bInitialized=true;
	g_ui32SamplingRate=ui32SamplingRate;
	g_ui32ChannelCount=ui32ChannelCount;
	g_ui32SampleCountPerChannel=ui32SampleCountPerChannel;
	g_ui32TotalSampleCount=0;

	g_ui32LastTime=System::Time::getTime();
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

	g_bStarted=true;
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

	uint32 l_ui32CurrentTime=System::Time::getTime();
	if(l_ui32CurrentTime-g_ui32LastTime > g_ui32SamplingRate)
	{
		for(uint32 j=0; j<g_ui32ChannelCount; j++)
		{
			for(uint32 i=0; i<g_ui32SampleCountPerChannel; i++)
			{
/*
				g_pSample[j*g_ui32SampleCountPerChannel+i]=
					33.33f*sinf(((i+g_ui32TotalSampleCount)*2.0f*M_PI*(j*1.7f))/g_ui32SamplingRate)+
					33.33f*sinf(((i+g_ui32TotalSampleCount)*2.0f*M_PI*(j*0.7f))/g_ui32SamplingRate)+
					33.33f*sinf(((i+g_ui32TotalSampleCount)*2.0f*M_PI*(j*1.1f))/g_ui32SamplingRate);
*/
				g_pSample[j*g_ui32SampleCountPerChannel+i]=j;
			}
		}

		g_ui32TotalSampleCount+=g_ui32SampleCountPerChannel;
		g_fpDriverProcessData(g_pSample, g_pUserData);
		g_ui32LastTime=l_ui32CurrentTime;
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

	g_bStarted=false;
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

	delete [] g_pSample;
	g_pSample=NULL;
	return true;
}

SDriver OpenViBEAcquisitionServer::getDriver_Generic_Oscilator(void)
{
	SDriver l_oResult;
	l_oResult.m_fpInitialize=ovasDriverInitializeImpl;
	l_oResult.m_fpStart=ovasDriverStartImpl;
	l_oResult.m_fpLoop=ovasDriverLoopImpl;
	l_oResult.m_fpStop=ovasDriverStopImpl;
	l_oResult.m_fpUninitialize=ovasDriverUninitializeImpl;
	return l_oResult;
}
