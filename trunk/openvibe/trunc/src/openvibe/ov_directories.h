#ifndef __OpenViBE_Directories_H__
#define __OpenViBE_Directories_H__

#include <cstdlib>	 // For getenv()

#include "ovCString.h"

namespace OpenViBE
{
	class Directories 
	{
	// The functions in this file should be only used in bootstrapping circumstances where Configuration Manager (Kernel Context) is not available. 
	// With access to Configuration Manager, the paths should be fetched using tokens such as ${Path_UserData}. Note that in such a case changing the environment variable later may no longer be affected in the token value.

	// @NOTE These functions may not be thread-safe 
	public:
		static OpenViBE::CString getDistRootDir(void)
		{
			return pathFromEnv("OV_PATH_ROOT", OV_CMAKE_PATH_ROOT);
		}
		static OpenViBE::CString getBinDir(void)
		{
			return pathFromEnv("OV_PATH_BIN", OV_CMAKE_PATH_BIN);
		}
		static OpenViBE::CString getDataDir(void)
		{
			return pathFromEnv("OV_PATH_DATA", OV_CMAKE_PATH_DATA);
		}
		static OpenViBE::CString getLibDir(void)
		{
			return pathFromEnv("OV_PATH_LIB", OV_CMAKE_PATH_LIB);
		}
		static OpenViBE::CString getUserHomeDir(void)
		{
#if defined TARGET_OS_Windows
			return pathFromEnv("APPDATA", "openvibe-user");
#elif defined TARGET_OS_Linux
			return pathFromEnv("HOME", "openvibe-user");
#endif
		}
		static OpenViBE::CString getUserDataDir(void)
		{
#if defined TARGET_OS_Windows
			return getUserHomeDir() + "/openvibe";
#elif defined TARGET_OS_Linux
			return getUserHomeDir() + "/.config/openvibe";
#endif
		}
		static OpenViBE::CString getLogDir(void)
		{
			return getUserDataDir() + "/log";
		}

		// Used to convert \ in paths to /, we need this because \ is a special character for .conf token parsing
		static OpenViBE::CString convertPath(const OpenViBE::CString &strIn) 
		{
			OpenViBE::CString l_sOut(strIn); 
			unsigned int l_sLen = strIn.length();
			for(unsigned int i=0; i<l_sLen; i++) 
			{ 
				if(strIn[i]=='\\') 
				{
					l_sOut[i]='/'; 
				}
			}
			return l_sOut;
		}

	private:
		// Static class, don't allow instances
		Directories() { };

		// Returns ENV variable value or sDefaultPath if the variable doesn't exist. The path is converted with each \ to /.
		static OpenViBE::CString pathFromEnv(const char *sEnvVar, const char *sDefaultPath)
		{
			const char *l_sPathPtr = std::getenv(sEnvVar);
			OpenViBE::CString l_sPath = (l_sPathPtr ? l_sPathPtr : sDefaultPath);
			return convertPath(l_sPath);
		}
	};
};

#endif // __OpenViBE_Directories_H__
