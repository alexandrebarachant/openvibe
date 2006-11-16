#ifndef __OpenViBE_Kernel_IKernel_H__
#define __OpenViBE_Kernel_IKernel_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IKernelContext;

		class OV_API IKernel : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual void release(void)=0;

			// $$$$$$$$$$$
			virtual const OpenViBE::Kernel::IKernelContext* getContext(void) const { return NULL; }

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Kernel)
		};
	};
};

#endif // __OpenViBE_Kernel_IKernel_H__
