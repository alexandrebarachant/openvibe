#include "Files.h"

#if defined FS_OS_Linux
 #include <sys/stat.h>
#elif defined FS_OS_Windows
 #include <windows.h>
#else
#endif

#include <iostream>

using namespace FS;
using namespace std;

//  * 2006-08-30 YRD - Portability note : using namespace FS confuses
//                     windows platform SDK because it defines itself
//                     a 'boolean' type. Thus the complete naming of
//                     this type in this sourcefile width 'FSBoolean'
typedef FS::boolean FSBoolean;

#if defined FS_OS_Linux

FSBoolean Files::equals(const char* pFile1, const char* pFile2)
{
	bool l_bResult;
	if(pFile1 && pFile2)
	{
		struct stat l_oStat1;
		struct stat l_oStat2;
		FSBoolean l_bStat1=!stat(pFile1, &l_oStat1);
		FSBoolean l_bStat2=!stat(pFile2, &l_oStat2);

		if(!l_bStat1 && !l_bStat2)
		{
			l_bResult=true;
		}
		else if(l_bStat1 && l_bStat2)
		{
			l_bResult=
				(l_oStat1.st_dev==l_oStat2.st_dev)&&
				(l_oStat1.st_ino==l_oStat2.st_ino)&&
				(l_oStat1.st_size==l_oStat2.st_size)&&
				(l_oStat1.st_mtime==l_oStat2.st_mtime);
		}
		else
		{
			l_bResult=false;
		}
	}
	return l_bResult;
}

#elif defined FS_OS_Windows

#else

#endif
