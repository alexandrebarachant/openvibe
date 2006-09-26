#ifndef __OpenViBE_Kernel_IObjectFactory_H__
#define __OpenViBE_Kernel_IObjectFactory_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IObjectFactory : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::IObject* createObject(
				const OpenViBE::CIdentifier& rClassIdentifier)=0;
			virtual OpenViBE::boolean releaseObject(
				OpenViBE::IObject* pObject)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassIdentifier_ObjectFactory)
		};
	};
};

#endif // __OpenViBE_Kernel_IObjectFactory_H__
