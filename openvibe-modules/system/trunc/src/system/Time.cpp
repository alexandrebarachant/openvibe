#include "Time.h"

#if defined System_OS_Linux
 #include <time.h>
 #include <sys/time.h>
#elif defined System_OS_Windows
 #include <windows.h>
#else
#endif

using namespace System;
#define boolean System::boolean

#if defined System_OS_Linux

boolean Time::sleep(const uint32 ui32MilliSeconds)
{
	usleep(ui32MilliSeconds*1000);
	return true;
}

uint32 Time::getTime(void)
{
	uint32 l_ui32Result=0;
	struct timeval l_oTimeValue;
	gettimeofday(&l_oTimeValue, NULL);
	l_ui32Result=(l_oTimeValue.tv_sec*1000)+(l_oTimeValue.tv_usec/1000);
	return l_ui32Result;
}

#elif defined System_OS_Windows

boolean Time::sleep(const uint32 ui32MilliSeconds)
{
	Sleep(ui32MilliSeconds);
	return true;
}

uint32 Time::getTime(void)
{
	return timeGetTime();
}

#else

#endif
