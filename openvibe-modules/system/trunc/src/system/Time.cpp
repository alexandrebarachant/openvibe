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

#if 0
#include <stdio.h>
static void display(uint64 v)
{
	printf(" 0x%08x%08x ", (uint32)(v>>32), (uint32)(v));
}
#endif

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
	static uint64 l_ui64Frequency;
	static uint64 l_ui64CounterStart;
	static uint64 l_ui64Counter;
	//static uint32 l_ui32FrequencyOrder;

	if(!l_bInitialized)
	{
		LARGE_INTEGER l_oPerformanceFrequency;
		QueryPerformanceFrequency(&l_oPerformanceFrequency);
		l_ui64Frequency=l_oPerformanceFrequency.QuadPart;

		LARGE_INTEGER l_oPerformanceCounterStart;
		QueryPerformanceCounter(&l_oPerformanceCounterStart);
		l_ui64CounterStart=l_oPerformanceCounterStart.QuadPart;

		//l_ui32FrequencyOrder=1+(uint32)(log(1.+(double)l_ui64Frequency)/log(2.));

		l_bInitialized=true;
	}

	LARGE_INTEGER l_oPerformanceCounter;
	QueryPerformanceCounter(&l_oPerformanceCounter);
	l_ui64Counter=l_oPerformanceCounter.QuadPart-l_ui64CounterStart;

	// l_ui64Result=((l_ui64Counter<<(32-l_ui32FrequencyOrder))/l_ui64Frequency)<<l_ui32FrequencyOrder;
	l_ui64Result=((l_ui64Counter/l_ui64Frequency)<<32)+(((l_ui64Counter%l_ui64Frequency)<<32)/l_ui64Frequency);
#if 0
	display(l_ui32FrequencyOrder);
	display(l_ui64Frequency);
	display(l_ui64Counter);
	display(l_ui64Counter<<(32-l_ui32FrequencyOrder));
	display(l_ui64Result);
	display(r);
	display(r-l_ui64Result);
	printf("\n");
#endif
#else
#endif
	return l_ui64Result;
}
