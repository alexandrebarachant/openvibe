#include "ovkCPluginModule.h"

#include <map>
#include <vector>

#if defined OVK_OS_Linux
	#include <dlfcn.h>
#elif defined OVK_OS_Windows
	#include <windows.h>
#else
#endif

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

namespace OpenViBE
{
	namespace Kernel
	{
		class CPluginModuleBase : virtual public TKernelObject<IPluginModule>
		{
		public:
			CPluginModuleBase(const IKernelContext& rKernelContext);

			virtual boolean initialize(void);
			virtual boolean getPluginObjectDescription(
				uint32 ui32Index,
				IPluginObjectDesc*& rpPluginObjectDescription);
			virtual boolean uninitialize(void);
			virtual boolean getFileName(CString& rFileName) const;

			_IsDerivedFromClass_Final_(IPluginModule, OV_UndefinedIdentifier);

		protected:

			virtual boolean isOpen(void) const=0;

			vector<IPluginObjectDesc*> m_vPluginObjectDescriptor;
			CString m_sFileName;
			boolean m_bGotDescriptions;

			boolean (*onInitializeCB)(void);
			boolean (*onGetPluginObjectDescriptionCB)(uint32, Plugins::IPluginObjectDesc*&);
			boolean (*onUninitializeCB)(void);
		};
	};
};

#define boolean OpenViBE::boolean

//___________________________________________________________________//
//                                                                   //

CPluginModuleBase::CPluginModuleBase(const IKernelContext& rKernelContext)
	:TKernelObject<IPluginModule>(rKernelContext)
	,m_bGotDescriptions(false)
	,onInitializeCB(NULL)
	,onGetPluginObjectDescriptionCB(NULL)
	,onUninitializeCB(NULL)
{
}

boolean CPluginModuleBase::initialize(void)
{
	if(!isOpen())
	{
		return false;
	}
	if(!onInitializeCB)
	{
		return true;
	}
	return onInitializeCB();
}

boolean CPluginModuleBase::getPluginObjectDescription(
	uint32 ui32Index,
	IPluginObjectDesc*& rpPluginObjectDescription)
{
	if(!m_bGotDescriptions)
	{
		if(!isOpen())
		{
			return false;
		}
		if(!onGetPluginObjectDescriptionCB)
		{
			return false;
		}

		uint32 l_ui32Index=0;
		IPluginObjectDesc* l_pPluginObjectDescriptor=NULL;
		while(onGetPluginObjectDescriptionCB(l_ui32Index, l_pPluginObjectDescriptor))
		{
			if(l_pPluginObjectDescriptor)
			{
				m_vPluginObjectDescriptor.push_back(l_pPluginObjectDescriptor);
			}
			l_ui32Index++;
		}

		m_bGotDescriptions=true;
	}

	if(ui32Index>=m_vPluginObjectDescriptor.size())
	{
		rpPluginObjectDescription=NULL;
		return false;
	}

	rpPluginObjectDescription=m_vPluginObjectDescriptor[ui32Index];
	return true;
}

boolean CPluginModuleBase::uninitialize(void)
{
	if(!isOpen())
	{
		return false;
	}
	if(!onUninitializeCB)
	{
		return true;
	}
	return onUninitializeCB();
}

boolean CPluginModuleBase::getFileName(
	CString& rFileName) const
{
	if(!isOpen())
	{
		return false;
	}
	rFileName=m_sFileName;
	return true;
}

//___________________________________________________________________//
//                                                                   //

#if defined OVK_OS_Linux

namespace OpenViBE
{
	namespace Kernel
	{
		class CPluginModuleLinux : virtual public CPluginModuleBase
		{
		public:

			CPluginModuleLinux(const IKernelContext& rKernelContext);

			virtual boolean load(
				const CString& sFileName,
				CString* pError);
			virtual boolean unload(
				CString* pError);
			virtual boolean isOpen(void) const;

		protected:

			void* m_pFileHandle;
		};
	};
};

#elif defined OVK_OS_Windows

namespace OpenViBE
{
	namespace Kernel
	{
		class CPluginModuleWindows : virtual public CPluginModuleBase
		{
		public:

			CPluginModuleWindows(const IKernelContext& rKernelContext);

			virtual boolean load(
				const CString& sFileName,
				CString* pError);
			virtual boolean unload(
				CString* pError);

		protected:

			virtual boolean isOpen(void) const;

			HMODULE m_pFileHandle;
		};
	};
};

#else

namespace OpenViBE
{
	namespace Kernel
	{
		class CPluginModuleDummy : virtual public CPluginModuleBase
		{
		public:

			CPluginModuleDummy(const IKernelContext& rKernelContext);

			virtual boolean load(
				const CString& sFileName,
				CString* pError);
			virtual boolean unload(
				CString* pError);

		protected:

			virtual boolean isOpen(void) const;
		};
	};
};

#endif

//___________________________________________________________________//
//                                                                   //

#if defined OVK_OS_Linux

CPluginModuleLinux::CPluginModuleLinux(const IKernelContext& rKernelContext)
	:TKernelObject<IPluginModule>(rKernelContext)
	,CPluginModuleBase(rKernelContext)
	,m_pFileHandle(NULL)
{
}

boolean CPluginModuleLinux::load(
	const CString& sFileName,
	CString* pError)
{
	if(m_pFileHandle)
	{
		if(pError) *pError="plugin module already loaded";
		return false;
	}

	// m_pFileHandle=dlopen(sFileName, RTLD_NOW|RTLD_LOCAL);
	m_pFileHandle=dlopen(sFileName, RTLD_LAZY|RTLD_GLOBAL);
	if(!m_pFileHandle)
	{
		if(pError) *pError=dlerror();
		return false;
	}

	onInitializeCB=(boolean (*)(void))dlsym(m_pFileHandle, "onInitialize");
	onUninitializeCB=(boolean (*)(void))dlsym(m_pFileHandle, "onUninitialize");
	onGetPluginObjectDescriptionCB=(boolean (*)(uint32, Plugins::IPluginObjectDesc*&))dlsym(m_pFileHandle, "onGetPluginObjectDescription");
	if(!onGetPluginObjectDescriptionCB)
	{
		if(pError) *pError=dlerror();

		dlclose(m_pFileHandle);
		m_pFileHandle=NULL;
		onInitializeCB=NULL;
		onGetPluginObjectDescriptionCB=NULL;
		onUninitializeCB=NULL;
		return false;
	}

	m_sFileName=sFileName;
	return true;
}

boolean CPluginModuleLinux::unload(
	CString* pError)
{
	if(!m_pFileHandle)
	{
		if(pError) *pError="no plugin module currently loaded";
		return false;
	}

	dlclose(m_pFileHandle);
	m_pFileHandle=NULL;
	onInitializeCB=NULL;
	onGetPluginObjectDescriptionCB=NULL;
	onUninitializeCB=NULL;
	return true;
}

boolean CPluginModuleLinux::isOpen(void) const
{
	return m_pFileHandle!=NULL;
}

#elif defined OVK_OS_Windows

CPluginModuleWindows::CPluginModuleWindows(const IKernelContext& rKernelContext)
	:TKernelObject<IPluginModule>(rKernelContext)
	,CPluginModuleBase(rKernelContext)
	,m_pFileHandle(NULL)
{
}

boolean CPluginModuleWindows::load(
	const CString& sFileName,
	CString* pError)
{
	if(m_pFileHandle)
	{
		if(pError) *pError="plugin module already loaded";
		return false;
	}

	m_pFileHandle=LoadLibrary(sFileName);
	if(!m_pFileHandle)
	{
		if(pError)
		{
			LPVOID l_pMessageBuffer=NULL;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER|
				FORMAT_MESSAGE_FROM_SYSTEM|
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				GetLastError(),
				0, // Default language
				(LPTSTR)&l_pMessageBuffer,
				0,
				NULL);
			*pError=(char*)l_pMessageBuffer;
			LocalFree(l_pMessageBuffer);
		}
		return false;
	}

	onInitializeCB=(boolean (*)(void))GetProcAddress(m_pFileHandle, "onInitialize");
	onUninitializeCB=(boolean (*)(void))GetProcAddress(m_pFileHandle, "onUninitialize");
	onGetPluginObjectDescriptionCB=(boolean (*)(uint32, Plugins::IPluginObjectDesc*&))GetProcAddress(m_pFileHandle, "onGetPluginObjectDescription");
	if(!onGetPluginObjectDescriptionCB)
	{
		if(pError)
		{
			LPVOID l_pMessageBuffer=NULL;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER|
				FORMAT_MESSAGE_FROM_SYSTEM|
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				GetLastError(),
				0, // Default language
				(LPTSTR)&l_pMessageBuffer,
				0,
				NULL);
			*pError=(char*)l_pMessageBuffer;
			LocalFree(l_pMessageBuffer);
		}

		FreeLibrary(m_pFileHandle);
		m_pFileHandle=NULL;
		onInitializeCB=NULL;
		onGetPluginObjectDescriptionCB=NULL;
		onUninitializeCB=NULL;
		return false;
	}

	m_sFileName=sFileName;
	return true;
}

boolean CPluginModuleWindows::unload(
	CString* pError)
{
	if(!m_pFileHandle)
	{
		if(pError) *pError="no plugin module currently loaded";
		return false;
	}

	FreeLibrary(m_pFileHandle);
	m_pFileHandle=NULL;
	onInitializeCB=NULL;
	onGetPluginObjectDescriptionCB=NULL;
	onUninitializeCB=NULL;
	return true;
}

boolean CPluginModuleWindows::isOpen(void) const
{
	return m_pFileHandle!=NULL;
}

#else

#endif

//___________________________________________________________________//
//                                                                   //

CPluginModule::CPluginModule(const IKernelContext& rKernelContext)
	:TKernelObject<IPluginModule>(rKernelContext)
	,m_pImplementation(NULL)
{
#if defined OVK_OS_Linux
	m_pImplementation=new CPluginModuleLinux(getKernelContext());
#elif defined OVK_OS_Windows
	m_pImplementation=new CPluginModuleWindows(getKernelContext());
#else
#endif
}

CPluginModule::~CPluginModule(void)
{
	delete m_pImplementation;
}

boolean CPluginModule::load(
	const CString& sFileName,
	CString* pError)
{
	return !m_pImplementation?false:m_pImplementation->load(sFileName, pError);
}

boolean CPluginModule::unload(
	CString* pError)
{
	return !m_pImplementation?false:m_pImplementation->unload(pError);
}

boolean CPluginModule::initialize(void)
{
	return !m_pImplementation?false:m_pImplementation->initialize();
}

boolean CPluginModule::getPluginObjectDescription(
	uint32 ui32Index,
	IPluginObjectDesc*& rpPluginObjectDescription)
{
	return !m_pImplementation?false:m_pImplementation->getPluginObjectDescription(ui32Index, rpPluginObjectDescription);
}

boolean CPluginModule::uninitialize(void)
{
	return !m_pImplementation?false:m_pImplementation->uninitialize();
}

boolean CPluginModule::getFileName(
	CString& rFileName) const
{
	return !m_pImplementation?false:m_pImplementation->getFileName(rFileName);
}
