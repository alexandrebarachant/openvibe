#include "Time.h"
#include <iostream>

using namespace System;
using namespace std;

#if defined System_OS_Linux

#include <time.h>
#include <sys/time.h>

boolean Time::sleep(const uint32 ui32MilliSeconds)
{
	usleep(ui32MilliSeconds*1000);
}

uint32 Time::getTime(void)
{
	uint32 l_ui32Result=0;
	struct timeval l_oTimeValue;
	gettimeofday(&l_oTimeValue, NULL);
	l_ui32Result=(l_oTimeValue.tv_sec*1000)+(l_oTimeValue.tv_usec/1000);
	return l_ui32Result;
}

#elif defines System_OS_Windows

#include <windows.h>

boolean Time::sleep(const uint32 ui32MilliSeconds)
{
	Sleep(ui32MilliSeconds);
}

uint32 Time::getTime(void)
{
	return timeGetTime();
}

#else

#endif
