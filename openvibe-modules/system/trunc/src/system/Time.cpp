#include "Time.h"

#include <math.h>

#if defined System_OS_Linux
 #include <unistd.h>
 #include <time.h>
 #include <sys/time.h>
#elif defined System_OS_Windows
 #include <windows.h>
#else
#endif

using namespace System;
#define boolean System::boolean

boolean Time::sleep(const uint32 ui32MilliSeconds)
{
	return zsleep(((((uint64)ui32MilliSeconds)<<22)/1000)<<10);
}

boolean Time::zsleep(const uint64 ui64Seconds)
{
#if defined System_OS_Linux
	usleep((ui64Seconds*1000000)>>32);
#elif defined System_OS_Windows
	Sleep((uint32)(((ui64Seconds>>10)*1000)>>22));
#else
#endif
	return true;
}

uint32 Time::getTime(void)
{
	return (uint32)(((zgetTime()>>22)*1000)>>10);
}

uint64 Time::zgetTime(void)
{
	uint64 l_ui64Result=0;
#if defined System_OS_Linux
	struct timeval l_oTimeValue;
	gettimeofday(&l_oTimeValue, NULL);
	l_ui64Result+=(((uint64)l_oTimeValue.tv_sec)<<32);
	l_ui64Result+=(((uint64)l_oTimeValue.tv_usec)<<32)/1000000;
#elif defined System_OS_Windows
	static boolean l_bInitialized=false;
	static uint32 l_ui32FrequencyOrder=0;
	static LARGE_INTEGER l_oPerformanceCounter;
	static LARGE_INTEGER l_oPerformanceFrequency;
	if(!l_bInitialized)
	{
		QueryPerformanceFrequency(&l_oPerformanceFrequency);
		l_ui32FrequencyOrder=1+log((float)l_oPerformanceFrequency.QuadPart);
		l_bInitialized=true;
	}
	QueryPerformanceCounter(&l_oPerformanceCounter);
	l_ui64Result=(((l_oPerformanceCounter.QuadPart<<l_ui32FrequencyOrder)/l_oPerformanceFrequency.QuadPart)<<(32-l_ui32FrequencyOrder));
#else
#endif
	return l_ui64Result;
}
