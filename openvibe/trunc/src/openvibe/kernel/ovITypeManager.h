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

			virtual OpenViBE::boolean registerType(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& sTypeName)=0;
			virtual OpenViBE::boolean registerStreamType(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& sTypeName,
				const OpenViBE::CIdentifier& rParentTypeIdentifier)=0;
			virtual OpenViBE::boolean registerEnumerationType(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& sTypeName,
				const OpenViBE::CString& sEnumerationString)=0;
			virtual OpenViBE::boolean registerBitMaskType(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& sTypeName,
				const OpenViBE::CString& sBitMaskString)=0;

			virtual OpenViBE::boolean isRegistered(
				const OpenViBE::CIdentifier& rTypeIdentifier)=0;
			virtual OpenViBE::boolean isStream(
				const OpenViBE::CIdentifier& rTypeIdentifier)=0;
			virtual OpenViBE::boolean isEnumeration(
				const OpenViBE::CIdentifier& rTypeIdentifier)=0;
			virtual OpenViBE::boolean isBitMask(
				const OpenViBE::CIdentifier& rTypeIdentifier)=0;

			virtual OpenViBE::CString getTypeName(
				const OpenViBE::CIdentifier& rTypeIdentifier)=0;
			virtual OpenViBE::CIdentifier getStreamParentType(
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
