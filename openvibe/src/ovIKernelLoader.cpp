#include "ovIKernelLoader.h"
#include "kernel/ovIKernelDesc.h"
#include "kernel/ovIKernel.h"

#include <iostream>

#if defined(OV_OS_Linux)
	#include <dlfcn.h>
#elif defined(OV_OS_Windows)
#else
#endif

namespace OpenViBE
{

//___________________________________________________________________//
//                                                                   //

class CKernelLoaderBase : virtual public OpenViBE::IKernelLoader
{
public:

	CKernelLoaderBase(const std::string& sFileName)
		:m_sFileName(sFileName)
	{
	}

	virtual OpenViBE::boolean load(void)
	{
		if(!onLoadCB)
		{
			return true;
		}
		return onLoadCB();
	}

	virtual OpenViBE::boolean getKernelDesc(OpenViBE::Kernel::IKernelDesc*& rpKernelDesc)
	{
		if(!onGetKernelDescCB)
		{
			return false;
		}
		return onGetKernelDescCB(rpKernelDesc);
	}

	virtual OpenViBE::boolean unload(void)
	{
		if(!onUnloadCB)
		{
			return true;
		}
		return onUnloadCB();
	}

	_IsDerivedFromClass_Final_(OpenViBE::IKernelLoader, OV_ClassIdentifier_KernelLoaderFinal)

protected:

	CKernelLoaderBase(void);

	std::string m_sFileName;
	OpenViBE::boolean (*onLoadCB)(void);
	OpenViBE::boolean (*onUnloadCB)(void);
	OpenViBE::boolean (*onGetKernelDescCB)(OpenViBE::Kernel::IKernelDesc*&);

};

//___________________________________________________________________//
//                                                                   //

#if defined(OV_OS_Linux)

class CKernelLoaderLinux : virtual public CKernelLoaderBase
{
public:

	CKernelLoaderLinux(const std::string sFileName)
		:CKernelLoaderBase(sFileName)
		,m_pFileHandle(NULL)
	{
		m_pFileHandle=dlopen(sFileName.c_str(), RTLD_LAZY);
		if(m_pFileHandle)
		{
			onLoadCB=(OpenViBE::boolean (*)(void))dlsym(m_pFileHandle, "onLoad");
			onUnloadCB=(OpenViBE::boolean (*)(void))dlsym(m_pFileHandle, "onUnload");
			onGetKernelDescCB=(OpenViBE::boolean (*)(OpenViBE::Kernel::IKernelDesc*&))dlsym(m_pFileHandle, "onGetKernelDesc");
		}
	}

	virtual ~CKernelLoaderLinux(void)
	{
		if(m_pFileHandle)
		{
			dlclose(m_pFileHandle);
		}
	}

protected:

	void* m_pFileHandle;
};

#endif

//___________________________________________________________________//
//                                                                   //

OpenViBE::boolean IKernelLoader::create(OpenViBE::IKernelLoader*& rpKernelLoader, const std::string sFileName)
{
	rpKernelLoader=NULL;
#if defined (OV_OS_Linux)
	rpKernelLoader=new CKernelLoaderLinux(sFileName);
#else
#endif
	return (rpKernelLoader!=NULL);
}

OpenViBE::boolean IKernelLoader::release(OpenViBE::IKernelLoader*& rpKernelLoader)
{
	if(!rpKernelLoader)
	{
		return false;
	}
	delete rpKernelLoader;
	rpKernelLoader=NULL;
	return true;
}

//___________________________________________________________________//
//                                                                   //

};
