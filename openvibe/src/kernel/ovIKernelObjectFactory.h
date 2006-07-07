#ifndef __OpenViBE_Kernel_IKernelObjectFactory_H__
#define __OpenViBE_Kernel_IKernelObjectFactory_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IKernelObjectFactory : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::Kernel::IKernelObject* createKernelObject(
				const OpenViBE::CIdentifier& rClassIdentifier)=0;
			virtual OpenViBE::boolean releaseKernelObject(
				OpenViBE::Kernel::IKernelObject* pKernelObject)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassIdentifier_KernelObjectFactory)
		};
	};
};

#endif // __OpenViBE_Kernel_IKernelObjectFactory_H__
