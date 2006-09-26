#include "IEntryEnumerator.h"
#include "IEntry.h"
#include "IAttributes.h"

#include <string>

#ifdef FS_OS_Linux
 #include <glob.h>
 #include <sys/stat.h>
#elif defined FS_OS_Windows
 #include <windows.h>
#else
#endif

using namespace std;
using namespace FS;

//  * 2006-08-30 YRD - Portability note : using namespace FS confuses
//                     windows platform SDK because it defines itself
//                     a 'boolean' type. Thus the complete naming of
//                     this type in this sourcefile width 'FSBoolean'
typedef FS::boolean FSBoolean;

// ________________________________________________________________________________________________________________
//

namespace FS
{
	class CEntry : virtual public IEntry
	{
	public:

		CEntry(const string& sName);

		virtual const char* getName(void);

	public:

		string m_sName;
	};
};

// ________________________________________________________________________________________________________________
//

CEntry::CEntry(const string& sName)
	:m_sName(sName)
{
}

const char* CEntry::getName(void)
{
	return m_sName.c_str();
}

// ________________________________________________________________________________________________________________
//

namespace FS
{
	class CAttributes : virtual public IAttributes
	{
	public:

		CAttributes(void);
		virtual ~CAttributes(void);

		virtual boolean isFile(void);
		virtual boolean isDirectory(void);
		virtual boolean isSymbolicLink(void);

		virtual boolean isArchive(void);
		virtual boolean isReadOnly(void);
		virtual boolean isHidden(void);
		virtual boolean isSystem(void);
		virtual boolean isExecutable(void);

		virtual uint64 getSize(void);

	public:

		boolean m_bIsFile;
		boolean m_bIsDirectory;
		boolean m_bIsSymbolicLink;
		boolean m_bIsArchive;
		boolean m_bIsReadOnly;
		boolean m_bIsHidden;
		boolean m_bIsSystem;
		boolean m_bIsExecutable;
		uint64 m_ui64Size;
	};
};

// ________________________________________________________________________________________________________________
//

CAttributes::CAttributes(void)
	:m_bIsFile(false)
	,m_bIsDirectory(false)
	,m_bIsSymbolicLink(false)
	,m_bIsArchive(false)
	,m_bIsReadOnly(false)
	,m_bIsHidden(false)
	,m_bIsSystem(false)
	,m_bIsExecutable(false)
	,m_ui64Size(0)
{
}

CAttributes::~CAttributes(void)
{
}

// ________________________________________________________________________________________________________________
//

FSBoolean CAttributes::isFile(void)
{
	return m_bIsFile;
}

FSBoolean CAttributes::isDirectory(void)
{
	return m_bIsDirectory;
}

FSBoolean CAttributes::isSymbolicLink(void)
{
	return m_bIsSymbolicLink;
}

FSBoolean CAttributes::isArchive(void)
{
	return m_bIsArchive;
}

FSBoolean CAttributes::isReadOnly(void)
{
	return m_bIsReadOnly;
}

FSBoolean CAttributes::isHidden(void)
{
	return m_bIsHidden;
}

FSBoolean CAttributes::isSystem(void)
{
	return m_bIsSystem;
}

FSBoolean CAttributes::isExecutable(void)
{
	return m_bIsExecutable;
}

// ________________________________________________________________________________________________________________
//

uint64 CAttributes::getSize(void)
{
	return m_ui64Size;
}

// ________________________________________________________________________________________________________________
//

IEntryEnumerator::~IEntryEnumerator(void)
{
}

// ________________________________________________________________________________________________________________
//

namespace FS
{
	class CEntryEnumerator : virtual public IEntryEnumerator
	{
	public:
		CEntryEnumerator(IEntryEnumeratorCallBack& rEntryEnumeratorCallBack);
		virtual void release(void);
	protected:
		IEntryEnumeratorCallBack& m_rEntryEnumeratorCallBack;
	};
};

// ________________________________________________________________________________________________________________
//

CEntryEnumerator::CEntryEnumerator(IEntryEnumeratorCallBack& rEntryEnumeratorCallBack)
	:m_rEntryEnumeratorCallBack(rEntryEnumeratorCallBack)
{
}

void CEntryEnumerator::release(void)
{
	delete this;
}

// ________________________________________________________________________________________________________________
//

#ifdef FS_OS_Linux

namespace FS
{
	class CEntryEnumeratorLinux : virtual public CEntryEnumerator
	{
	public:
		CEntryEnumeratorLinux(IEntryEnumeratorCallBack& rEntryEnumeratorCallBack);
		virtual boolean enumerate(const char* sWildCard);
	};
};

#elif defined FS_OS_Windows

namespace FS
{
	class CEntryEnumeratorWindows : virtual public CEntryEnumerator
	{
	public:
		CEntryEnumeratorWindows(IEntryEnumeratorCallBack& rEntryEnumeratorCallBack);
		virtual boolean enumerate(const char* sWildCard);
	};
};

#else

namespace FS
{
	class CEntryEnumeratorDummy : virtual public CEntryEnumerator
	{
	public:
		CEntryEnumeratorDummy(IEntryEnumeratorCallBack& rEntryEnumeratorCallBack);
		virtual boolean enumerate(const char* sWildCard);
	};
};

#endif

// ________________________________________________________________________________________________________________
//

#ifdef FS_OS_Linux

CEntryEnumeratorLinux::CEntryEnumeratorLinux(IEntryEnumeratorCallBack& rEntryEnumeratorCallBack)
	:CEntryEnumerator(rEntryEnumeratorCallBack)
{
}

FSBoolean CEntryEnumeratorLinux::enumerate(const char* sWildCard)
{
	glob_t l_oGlobStruc;
	memset(&l_oGlobStruc, GLOB_NOSORT, sizeof(l_oGlobStruc));
	if(glob(sWildCard, 0, NULL, &l_oGlobStruc))
	{
		return false;
	}

	size_t i=0;
	FSBoolean l_bFinished=false;
	while(!l_bFinished)
	{
		if(i<l_oGlobStruc.gl_pathc)
		{
			char* l_sName=l_oGlobStruc.gl_pathv[i];
			CEntry l_oEntry(l_sName);
			CAttributes l_oAttributes;

			struct stat l_oLStat;
			struct stat l_oStat;
			if(!lstat(l_sName, &l_oLStat) && !stat(l_sName, &l_oStat))
			{
				l_oAttributes.m_bIsDirectory=S_ISDIR(l_oStat.st_mode)?true:false;
				l_oAttributes.m_bIsFile=S_ISREG(l_oStat.st_mode)?true:false;
				l_oAttributes.m_bIsSymbolicLink=S_ISLNK(l_oLStat.st_mode)?true:false;

				l_oAttributes.m_bIsArchive=false;
				l_oAttributes.m_bIsReadOnly=l_oStat.st_mode&S_IWUSR?false:true;
				l_oAttributes.m_bIsHidden=false;
				l_oAttributes.m_bIsSystem=S_ISBLK(l_oStat.st_mode)|S_ISFIFO(l_oStat.st_mode)|S_ISSOCK(l_oStat.st_mode)|S_ISCHR(l_oStat.st_mode)?true:false;
				l_oAttributes.m_bIsExecutable=l_oStat.st_mode&S_IXUSR?true:false;

				l_oAttributes.m_ui64Size=l_oStat.st_size;

				// Sends to callback
				if(!m_rEntryEnumeratorCallBack.callback(l_oEntry, l_oAttributes))
				{
					l_bFinished=true;
				}
			}
			i++;
		}
		else
		{
			l_bFinished=true;
		}
	}

	return true;
}

#elif defined FS_OS_Windows

CEntryEnumeratorWindows::CEntryEnumeratorWindows(IEntryEnumeratorCallBack& rEntryEnumeratorCallBack)
	:CEntryEnumerator(rEntryEnumeratorCallBack)
{
}

FSBoolean CEntryEnumeratorWindows::enumerate(const char* sWildCard)
{
	WIN32_FIND_DATA l_oFindData;
	HANDLE l_pFileHandle=FindFirstFile(sWildCard, &l_oFindData);
	if(l_pFileHandle!=INVALID_HANDLE_VALUE)
	{
		FSBoolean l_bFinished=false;
		while(!l_bFinished)
		{
			CEntry l_oEntry(l_oFindData.cFileName);
			CAttributes l_oAttributes;

			l_oAttributes.m_bIsDirectory=(l_oFindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)?true:false;
			l_oAttributes.m_bIsFile=(l_oFindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)?false:true;
			l_oAttributes.m_bIsSymbolicLink=false;

			l_oAttributes.m_bIsArchive=(l_oFindData.dwFileAttributes&FILE_ATTRIBUTE_ARCHIVE)?true:false;
			l_oAttributes.m_bIsReadOnly=(l_oFindData.dwFileAttributes&FILE_ATTRIBUTE_READONLY)?true:false;
			l_oAttributes.m_bIsHidden=(l_oFindData.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN)?true:false;
			l_oAttributes.m_bIsSystem=(l_oFindData.dwFileAttributes&FILE_ATTRIBUTE_SYSTEM)?true:false;
			l_oAttributes.m_bIsExecutable=false; // TODO

			l_oAttributes.m_ui64Size=(l_oFindData.nFileSizeHigh<<16)+l_oFindData.nFileSizeLow;

			// Sends to callback
			if(!m_rEntryEnumeratorCallBack.callback(l_oEntry, l_oAttributes))
			{
				l_bFinished=true;
			}

			if(!FindNextFile(l_pFileHandle, &l_oFindData))
			{
				l_bFinished=true;
			}
		}
	}
	return true;
}

#else

CEntryEnumeratorDummy::CEntryEnumeratorDummy(IEntryEnumeratorCallBack& rEntryEnumeratorCallBack)
	:CEntryEnumerator(rEntryEnumeratorCallBack)
{
}

FSBoolean CEntryEnumeratorDummy::enumerate(const char* sWildCard)
{
	return true;
}

#endif

FS_API IEntryEnumerator* FS::createEntryEnumerator(IEntryEnumeratorCallBack& rEntryEnumeratorCallBack)
{
	IEntryEnumerator* l_pResult=NULL;
#ifdef FS_OS_Linux
	l_pResult=new CEntryEnumeratorLinux(rEntryEnumeratorCallBack);
#elif defined FS_OS_Windows
	l_pResult=new CEntryEnumeratorWindows(rEntryEnumeratorCallBack);
#else
	l_pResult=new CEntryEnumeratorDummy(rEntryEnumeratorCallBack);
#endif
	return l_pResult;
}
