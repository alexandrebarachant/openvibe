#ifndef __OpenViBE_Kernel_ITypeManager_H__
#define __OpenViBE_Kernel_ITypeManager_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class OV_API ITypeManager : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::boolean registerEnumerationType(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& rEnumerationString)=0;
			virtual OpenViBE::boolean registerBitMaskType(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& rBitMaskString)=0;

			virtual OpenViBE::boolean isRegistered(
				const OpenViBE::CIdentifier& rTypeIdentifier)=0;
			virtual OpenViBE::boolean isEnumeration(
				const OpenViBE::CIdentifier& rTypeIdentifier)=0;
			virtual OpenViBE::boolean isBitMask(
				const OpenViBE::CIdentifier& rTypeIdentifier)=0;

			virtual OpenViBE::CString getEnumerationString(
				const OpenViBE::CIdentifier& rTypeIdentifier)=0;
			virtual OpenViBE::CString getBitMaskString(
				const OpenViBE::CIdentifier& rTypeIdentifier)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_TypeManager);
		};
	};
};

#endif // __OpenViBE_Kernel_ITypeManager_H__
