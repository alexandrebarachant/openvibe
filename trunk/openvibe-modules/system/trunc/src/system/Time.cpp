#include "Time.h"

#include <cmath>

#if defined System_OS_Linux
 #include <unistd.h>
 #include <ctime>
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
	static boolean l_bInitialized=false;
	static struct timeval l_oTimeValueStart;
	struct timeval l_oTimeValue;
	uint64 l_i64TimeMicroSecond=0;

	if(!l_bInitialized)
	{
		gettimeofday(&l_oTimeValueStart, NULL);

		l_bInitialized=true;
	}

	gettimeofday(&l_oTimeValue, NULL);
	int64 l_i64SecDiff=(int64)(l_oTimeValue.tv_sec-l_oTimeValueStart.tv_sec);
	int64 l_i64USecDiff=(int64)(l_oTimeValue.tv_usec-l_oTimeValueStart.tv_usec);

	l_i64TimeMicroSecond+=l_i64SecDiff*1000000;
	l_i64TimeMicroSecond+=l_i64USecDiff;

	l_ui64Result=((l_i64TimeMicroSecond/1000000)<<32)+(((l_i64TimeMicroSecond%1000000)<<32)/1000000);
#elif defined System_OS_Windows

#if 0
	static boolean l_bInitialized=false;
	static uint64 l_ui64Frequency;
	static uint64 l_ui64CounterStart;
	uint64 l_ui64Counter;

	if(!l_bInitialized)
	{
		LARGE_INTEGER l_oPerformanceFrequency;
		QueryPerformanceFrequency(&l_oPerformanceFrequency);
		l_ui64Frequency=l_oPerformanceFrequency.QuadPart;

		LARGE_INTEGER l_oPerformanceCounterStart;
		QueryPerformanceCounter(&l_oPerformanceCounterStart);
		l_ui64CounterStart=l_oPerformanceCounterStart.QuadPart;

		l_bInitialized=true;
	}

	LARGE_INTEGER l_oPerformanceCounter;
	QueryPerformanceCounter(&l_oPerformanceCounter);
	l_ui64Counter=l_oPerformanceCounter.QuadPart-l_ui64CounterStart;

	l_ui64Result=((l_ui64Counter/l_ui64Frequency)<<32)+(((l_ui64Counter%l_ui64Frequency)<<32)/l_ui64Frequency);

#else

	static boolean l_bInitialized=false;
	static uint64 l_ui64CounterStart;
	uint64 l_ui64Counter;

	l_ui64Counter=uint64(timeGetTime());
	l_ui64Counter=((l_ui64Counter/1000)<<32)+(((l_ui64Counter%1000)<<32)/1000);

	if(!l_bInitialized)
	{
		l_ui64CounterStart=l_ui64Counter;
		l_bInitialized=true;
	}

	l_ui64Result=l_ui64Counter-l_ui64CounterStart;

#endif

#else
#endif
	return l_ui64Result;
}
