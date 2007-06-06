#ifndef __OpenViBE_IKernelLoader_H__
#define __OpenViBE_IKernelLoader_H__

#include "ovIObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IKernelDesc;
	};

	class OV_API IKernelLoader : virtual public OpenViBE::IObject
	{
	public:

		virtual OpenViBE::boolean load(
			const OpenViBE::CString& sFileName,
			OpenViBE::CString* pError=NULL)=0;
		virtual OpenViBE::boolean unload(
			CString* pError=NULL)=0;

		virtual OpenViBE::boolean initialize(void)=0;
		virtual OpenViBE::boolean getKernelDesc(
			OpenViBE::Kernel::IKernelDesc*& rpKernelDesc)=0;
		virtual OpenViBE::boolean uninitialize(void)=0;

		virtual void release(void)=0;

		_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassId_KernelLoader)
	};

	extern OV_API OpenViBE::IKernelLoader* createKernelLoader(void);
};

#endif // __OpenViBE_IKernelLoader_H__
