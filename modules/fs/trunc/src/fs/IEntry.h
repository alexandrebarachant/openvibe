#ifndef __FS_IEntry_H__
#define __FS_IEntry_H__

#include "defines.h"

namespace FS
{
	class FS_API IEntry
	{
	public:
		virtual const char* getName(void)=0;
	protected:
		virtual ~IEntry(void);
	};
};

#endif // __FS_IEntry_H__
