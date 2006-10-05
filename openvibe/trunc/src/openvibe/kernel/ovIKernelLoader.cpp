#include "ovIKernelLoader.h"
#include "ovIKernelDesc.h"
#include "ovIKernel.h"

#if defined OV_OS_Linux
 #include <dlfcn.h>
#elif defined OV_OS_Windows
 #include <windows.h>
#else
#endif

#include <iostream>
#include <string>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;

typedef OpenViBE::boolean ovBoolean;

#define OV_ClassId_Kernel_KernelLoaderFinal OpenViBE::CIdentifier(0x624A6E5B, 0x52228CEA)

//___________________________________________________________________//
//                                                                   //

namespace OpenViBE
{
	class CKernelLoaderBase : virtual public IKernelLoader
	{
	public:

		CKernelLoaderBase(void);

		virtual ovBoolean initialize(void);
		virtual ovBoolean getKernelDesc(IKernelDesc*& rpKernelDesc);
		virtual ovBoolean uninitialize(void);
		virtual void release(void);

		_IsDerivedFromClass_Final_(IKernelLoader, OV_ClassId_Kernel_KernelLoaderFinal)

	protected:

		virtual ovBoolean isOpen(void)=0;

		CString m_sFileName;
		ovBoolean (*onInitializeCB)(void);
		ovBoolean (*onGetKernelDescCB)(IKernelDesc*&);
		ovBoolean (*onUninitializeCB)(void);
	};
};

//___________________________________________________________________//
//                                                                   //

CKernelLoaderBase::CKernelLoaderBase(void)
	:onInitializeCB(NULL)
	,onGetKernelDescCB(NULL)
	,onUninitializeCB(NULL)
{
}

ovBoolean CKernelLoaderBase::initialize(void)
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

ovBoolean CKernelLoaderBase::getKernelDesc(
	IKernelDesc*& rpKernelDesc)
{
	if(!isOpen())
	{
		return false;
	}
	if(!onGetKernelDescCB)
	{
		return false;
	}
	return onGetKernelDescCB(rpKernelDesc);
}

ovBoolean CKernelLoaderBase::uninitialize(void)
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

void CKernelLoaderBase::release(void)
{
	delete this;
}

//___________________________________________________________________//
//                                                                   //

#if defined OV_OS_Linux

namespace OpenViBE
{
	class CKernelLoaderLinux : virtual public CKernelLoaderBase
	{
	public:

		CKernelLoaderLinux(void);

		virtual ovBoolean load(const CString& sFileName, CString* pError);
		virtual ovBoolean unload(CString* pError);

	protected:

		virtual ovBoolean isOpen(void);

		void* m_pFileHandle;
	};
};

#elif defined OV_OS_Windows

namespace OpenViBE
{
	class CKernelLoaderWindows : virtual public CKernelLoaderBase
	{
	public:
		CKernelLoaderWindows(void);

		virtual ovBoolean load(const CString& sFileName, CString* pError);
		virtual ovBoolean unload(CString* pError);

	protected:

		virtual ovBoolean isOpen(void);

		HMODULE m_pFileHandle;
	};
};

#else

namespace OpenViBE
{
	class CKernelLoaderDummy : virtual public CKernelLoaderBase
	{
	public:
		virtual ovBoolean load(const CString& sFileName, CString* pError);
		virtual ovBoolean unload(CString* pError);

	protected:

		virtual isOpen(void);
	};
};

#endif

//___________________________________________________________________//
//                                                                   //

#if defined OV_OS_Linux

CKernelLoaderLinux::CKernelLoaderLinux(void)
	:m_pFileHandle(NULL)
{
}

ovBoolean CKernelLoaderLinux::load(
	const CString& sFileName,
	CString* pError)
{
	if(m_pFileHandle)
	{
		if(pError) *pError="kernel already loaded";
		return false;
	}

	// m_pFileHandle=dlopen(sFileName, RTLD_NOW|RTLD_LOCAL);
	m_pFileHandle=dlopen(sFileName, RTLD_LAZY|RTLD_GLOBAL);
	if(!m_pFileHandle)
	{
		if(pError) *pError=dlerror();
		return false;
	}

	onInitializeCB=(ovBoolean (*)(void))dlsym(m_pFileHandle, "onInitialize");
	onUninitializeCB=(ovBoolean (*)(void))dlsym(m_pFileHandle, "onUninitialize");
	onGetKernelDescCB=(ovBoolean (*)(IKernelDesc*&))dlsym(m_pFileHandle, "onGetKernelDesc");
	if(!onGetKernelDescCB)
	{
		if(pError) *pError=dlerror();

		dlclose(m_pFileHandle);
		m_pFileHandle=NULL;
		onInitializeCB=NULL;
		onGetKernelDescCB=NULL;
		onUninitializeCB=NULL;
		return false;
	}
	return true;
}

ovBoolean CKernelLoaderLinux::unload(
	CString* pError)
{
	if(!m_pFileHandle)
	{
		if(pError) *pError="no kernel currently loaded";
		return false;
	}
	dlclose(m_pFileHandle);
	m_pFileHandle=NULL;
	onInitializeCB=NULL;
	onGetKernelDescCB=NULL;
	onUninitializeCB=NULL;
	return true;
}

ovBoolean CKernelLoaderLinux::isOpen(void)
{
	return m_pFileHandle!=NULL;
}

#elif defined OV_OS_Windows

CKernelLoaderWindows::CKernelLoaderWindows(void)
	:m_pFileHandle(NULL)
{
}

ovBoolean CKernelLoaderWindows::load(
	const CString& sFileName,
	CString* pError)
{
	if(m_pFileHandle)
	{
		if(pError) *pError="kernel already loaded";
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

	onInitializeCB=(ovBoolean (*)(void))GetProcAddress(m_pFileHandle, "onInitialize");
	onUninitializeCB=(ovBoolean (*)(void))GetProcAddress(m_pFileHandle, "onUninitialize");
	onGetKernelDescCB=(ovBoolean (*)(IKernelDesc*&))GetProcAddress(m_pFileHandle, "onGetKernelDesc");
	if(!onGetKernelDescCB)
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
		onGetKernelDescCB=NULL;
		onUninitializeCB=NULL;
		return false;
	}
	return true;
}

ovBoolean CKernelLoaderWindows::unload(
	CString* pError)
{
	if(!m_pFileHandle)
	{
		if(pError) *pError="no kernel currently loaded";
		return false;
	}
	FreeLibrary(m_pFileHandle);
	m_pFileHandle=NULL;
	onInitializeCB=NULL;
	onGetKernelDescCB=NULL;
	onUninitializeCB=NULL;

	return true;
}

ovBoolean CKernelLoaderWindows::isOpen(void)
{
	return m_pFileHandle!=NULL;
}

#else

ovBoolean CKernelLoaderDummy::load(
	const CString& sFileName, 
	CString* pError)
{
	if(pError) *pError="Not implemented for this configuration";
	return false;
}

ovBoolean CKernelLoaderDummy::unload(
	CString* pError)
{
	if(pError) *pError="Not implemented for this configuration";
	return false;
}

ovBoolean CKernelLoaderDummy::isOpen(void)
{
	return false;
}

#endif

//___________________________________________________________________//
//                                                                   //

OV_API IKernelLoader* OpenViBE::Kernel::createKernelLoader(void)
{
	IKernelLoader* l_pResult=NULL;
#if defined OV_OS_Linux
	l_pResult=new CKernelLoaderLinux();
#elif defined OV_OS_Windows
	l_pResult=new CKernelLoaderWindows();
#else
	l_pResult=new CKernelLoaderDummy();
#endif
	return l_pResult;
}
