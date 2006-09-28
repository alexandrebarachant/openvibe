#ifndef __OpenViBE_Kernel_IKernelObject_H__
#define __OpenViBE_Kernel_IKernelObject_H__

#include "../ovIObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class OV_API IKernelObject : virtual public OpenViBE::IObject
		{
		public:

			_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassId_Kernel_KernelObject)

		};
	};
};

#endif // __OpenViBE_Kernel_IKernelObject_H__
