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

			virtual OpenViBE::boolean registerType(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& sTypeName);
			virtual OpenViBE::boolean registerStreamType(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& sTypeName,
				const OpenViBE::CIdentifier& rParentTypeIdentifier);
			virtual OpenViBE::boolean registerEnumerationType(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& sTypeName,
				const OpenViBE::CString& sEnumerationString);
			virtual OpenViBE::boolean registerBitMaskType(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& sTypeName,
				const OpenViBE::CString& sBitMaskString);

			virtual OpenViBE::boolean isRegistered(
				const OpenViBE::CIdentifier& rTypeIdentifier);
			virtual OpenViBE::boolean isStream(
				const OpenViBE::CIdentifier& rTypeIdentifier);
			virtual OpenViBE::boolean isEnumeration(
				const OpenViBE::CIdentifier& rTypeIdentifier);
			virtual OpenViBE::boolean isBitMask(
				const OpenViBE::CIdentifier& rTypeIdentifier);

			virtual OpenViBE::CString getTypeName(
				const OpenViBE::CIdentifier& rTypeIdentifier);
			virtual OpenViBE::CIdentifier getStreamParentType(
				const OpenViBE::CIdentifier& rTypeIdentifier);
			virtual OpenViBE::CString getEnumerationString(
				const OpenViBE::CIdentifier& rTypeIdentifier);
			virtual OpenViBE::CString getBitMaskString(
				const OpenViBE::CIdentifier& rTypeIdentifier);

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::ITypeManager>, OVK_ClassId_Kernel_TypeManager);

		protected:

			std::map<OpenViBE::CIdentifier, OpenViBE::CString> m_vName;
			std::map<OpenViBE::CIdentifier, OpenViBE::CString> m_vEnumeration;
			std::map<OpenViBE::CIdentifier, OpenViBE::CString> m_vBitMask;
			std::map<OpenViBE::CIdentifier, OpenViBE::CIdentifier> m_vStream;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_ITypeManager_H__
