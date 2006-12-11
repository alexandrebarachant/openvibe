#ifndef __OpenViBE_Kernel_IObjectFactory_H__
#define __OpenViBE_Kernel_IObjectFactory_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class OV_API IObjectFactory : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::IObject* createObject(
				const OpenViBE::CIdentifier& rClassIdentifier)=0;
			virtual OpenViBE::IObject* createObject(
				const OpenViBE::CIdentifier& rClassIdentifier,
				OpenViBE::IObject& rUserData)=0;
			virtual OpenViBE::IObject* createObject(
				const OpenViBE::CIdentifier& rClassIdentifier,
				const OpenViBE::IObject& rUserData)=0;
			virtual OpenViBE::boolean releaseObject(
				OpenViBE::IObject* pObject)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_ObjectFactory)
		};
	};
};

#endif // __OpenViBE_Kernel_IObjectFactory_H__
