#ifndef __FS_IEntryEnumerator_H__
#define __FS_IEntryEnumerator_H__

#include "defines.h"

namespace FS
{
	class IEntry;
	class IAttributes;

	class FS_API IEntryEnumeratorCallBack
	{
	public:
		virtual FS::boolean callback(FS::IEntry& rEntry, FS::IAttributes& rAttributes)=0;
		virtual ~IEntryEnumeratorCallBack(void) { }
	};

	class FS_API IEntryEnumerator
	{
	public:
		virtual FS::boolean enumerate(const char* sWildcard)=0;
		virtual void release(void)=0;
	protected:
		virtual ~IEntryEnumerator(void);
	};

	extern FS_API FS::IEntryEnumerator* createEntryEnumerator(FS::IEntryEnumeratorCallBack& rCallBack);
};

#endif // __FS_IEntryEnumerator_H__
