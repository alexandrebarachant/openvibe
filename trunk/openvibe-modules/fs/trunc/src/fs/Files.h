#ifndef __FS_Files_H__
#define __FS_Files_H__

#include "defines.h"

namespace FS
{
	class FS_API Files
	{
	public:

		static FS::boolean equals(const char* pFile1, const char* pFile2);
		static FS::boolean fileExists(const char* pathToCheck);
		static FS::boolean directoryExists(const char* pathToCheck);
	private:

		Files(void);
	};
};

#endif // __FS_Files_H__
