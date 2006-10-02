#ifndef __System_Memory_H__
#define __System_Memory_H__

#include "defines.h"

namespace System
{
	class System_API Memory
	{
	public:

		static System::boolean copy(void* pTargetBuffer, const void* pSourceBuffer, const System::uint64 ui64BufferSize);

	private:

		Memory(void);
	};
};

#endif // __System_Memory_H__
