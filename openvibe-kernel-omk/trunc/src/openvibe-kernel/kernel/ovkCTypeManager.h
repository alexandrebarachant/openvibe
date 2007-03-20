#ifndef __OpenViBEKernel_Kernel_ITypeManager_H__
#define __OpenViBEKernel_Kernel_ITypeManager_H__

#include "ovkTKernelObject.h"

#include <map>

namespace OpenViBE
{
	namespace Kernel
	{
		class CTypeManager : virtual public OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::ITypeManager>
		{
		public:

			CTypeManager(const OpenViBE::Kernel::IKernelContext& rKernelContext);

			virtual OpenViBE::boolean registerEnumerationType(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& rEnumerationString);
			virtual OpenViBE::boolean registerBitMaskType(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& rBitMaskString);

			virtual OpenViBE::boolean isRegistered(
				const OpenViBE::CIdentifier& rTypeIdentifier);
			virtual OpenViBE::boolean isEnumeration(
				const OpenViBE::CIdentifier& rTypeIdentifier);
			virtual OpenViBE::boolean isBitMask(
				const OpenViBE::CIdentifier& rTypeIdentifier);

			virtual OpenViBE::CString getEnumerationString(
				const OpenViBE::CIdentifier& rTypeIdentifier);
			virtual OpenViBE::CString getBitMaskString(
				const OpenViBE::CIdentifier& rTypeIdentifier);

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::ITypeManager>, OVK_ClassId_Kernel_TypeManager);

		protected:

			std::map<OpenViBE::CIdentifier, OpenViBE::CString> m_vEnumeration;
			std::map<OpenViBE::CIdentifier, OpenViBE::CString> m_vBitMask;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_ITypeManager_H__
