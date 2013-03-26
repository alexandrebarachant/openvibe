#ifndef __Time_H__
#define __Time_H__

#include "defines.h"

namespace System
{
	class System_API Time
	{
	public:

		static System::boolean sleep(const System::uint32 ui32MilliSeconds);
		static System::boolean zsleep(const System::uint64 ui64Seconds); // fixed point 32:32 seconds
		static System::uint32 getTime(void);

		static System::uint64 zgetTime(void); // fixed point 32:32 seconds

		// Converts fixed point 32:32 representation to seconds
		// @TODO move to time arithmetic class once its up
		static System::float32 convertToSeconds(uint64 ui64FixedPointTime) {
			return ((ui64FixedPointTime * 1000) >> 32) * .001f;
		}

	private:

		Time(void);
	};
};

#endif // __Time_H__
