#ifndef __OpenViBE_IKernelLoader_H__
#define __OpenViBE_IKernelLoader_H__

#include "ov_base.h"
#include "ovIObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IKernelDesc;
	};

	class IKernelLoader : virtual public OpenViBE::IObject
	{
	public:

		static OpenViBE::boolean create(OpenViBE::IKernelLoader*& rpKernelLoader, const std::string sFileName);
		static OpenViBE::boolean release(OpenViBE::IKernelLoader*& rpKernelLoader);

		virtual OpenViBE::boolean load(void)=0;
		virtual OpenViBE::boolean getKernelDesc(
			OpenViBE::Kernel::IKernelDesc*& rpKernelDesc)=0;
		virtual OpenViBE::boolean unload(void)=0;

		_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassIdentifier_KernelLoader)
	};
};

#endif // __OpenViBE_IKernelLoader_H__
