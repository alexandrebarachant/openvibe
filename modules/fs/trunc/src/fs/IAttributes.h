#ifndef __FS_IAttributes_H__
#define __FS_IAttributes_H__

#include "defines.h"

namespace FS
{
	class FS_API IAttributes
	{
	public:
		virtual FS::boolean isFile(void)=0;
		virtual FS::boolean isDirectory(void)=0;
		virtual FS::boolean isSymbolicLink(void)=0;

		virtual FS::boolean isArchive(void)=0;
		virtual FS::boolean isReadOnly(void)=0;
		virtual FS::boolean isHidden(void)=0;
		virtual FS::boolean isSystem(void)=0;
		virtual FS::boolean isExecutable(void)=0;

		virtual FS::uint64 getSize(void)=0;
	protected:
		virtual ~IAttributes(void);
	};
};

#endif // __FS_IAttributes_H__
