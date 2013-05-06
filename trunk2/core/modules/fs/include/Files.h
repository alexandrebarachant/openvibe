#ifndef __FS_Files_H__
#define __FS_Files_H__

#include "defines.h"

namespace FS
{
	class OV_API Files
	{
	public:

		static FS::boolean equals(const char* pFile1, const char* pFile2);
		static FS::boolean fileExists(const char* pathToCheck);
		static FS::boolean directoryExists(const char* pathToCheck);
		// Creates all components of a path to the filesystem
		static FS::boolean createPath(const char* sPath);
		// Creates all components of a path to the filesystem except the last part (i.e. for paths including a filename in the end)
		static FS::boolean createParentPath(const char* sPath);
	private:

		Files(void);
	};
};

#endif // __FS_Files_H__
