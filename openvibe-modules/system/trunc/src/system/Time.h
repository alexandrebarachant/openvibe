#ifndef __Time_H__
#define __Time_H__

#include "defines.h"

namespace System
{
	class System_API Time
	{
	public:

		static System::boolean sleep(const System::uint32 ui32MilliSeconds);
		static System::uint32 getTime(void);

	private:

		Time(void);
	};
};

#endif // __Time_H__
