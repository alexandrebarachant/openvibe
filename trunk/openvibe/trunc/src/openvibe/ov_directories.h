#ifndef __OpenViBE_Directories_H__
#define __OpenViBE_Directories_H__

#include <stdlib.h>	 // For getenv()

#include "ovCString.h"

namespace OpenViBE
{
	class Directories 
	{
	// @NOTE These functions may not be thread-safe 
	public:
		static OpenViBE::CString getDistRootDir(void)
		{
			return pathFromEnv("OV_DISTROOT", "..");
		}
		static OpenViBE::CString getBinDir(void)
		{
			return pathFromEnv("OV_BINDIR", "../bin");
		}
		static OpenViBE::CString getDataDir(void)
		{
			return pathFromEnv("OV_DATADIR", "../share");
		}
		static OpenViBE::CString getLibDir(void)
		{
			return pathFromEnv("OV_LIBDIR", "../lib");
		}
		static OpenViBE::CString getLogDir(void)
		{
#if defined TARGET_OS_Windows
			OpenViBE::CString l_sPath = pathFromEnv("APPDATA", "../log");
			l_sPath = l_sPath + "/openvibe";
#elif defined TARGET_OS_Linux
			OpenViBE::CString l_sPath = pathFromEnv("HOME", "../log");
			l_sPath = l_sPath + "/.config/openvibe";
#endif
			return l_sPath;
		}

		// Used to convert \ in paths given by getenv() to /, we need this because \ is a special character for .conf token parsing
		static OpenViBE::CString convertPath(const OpenViBE::CString &strIn) 
		{
			OpenViBE::CString l_sOut(strIn); 
			unsigned int l_sLen = strIn.length();
			for(unsigned int i=0;i<l_sLen;i++) { if(strIn[i]=='\\') l_sOut[i]='/'; };
			return l_sOut;
		}

	private:

		static OpenViBE::CString pathFromEnv(const char *sEnvVar, const char *sDefaultPath)
		{
			const char *l_sPathPtr = getenv(sEnvVar);
			OpenViBE::CString l_sPath = (l_sPathPtr ? l_sPathPtr : sDefaultPath);
			return convertPath(l_sPath);
		}
	};
};

#endif // __OpenViBE_Directories_H__
