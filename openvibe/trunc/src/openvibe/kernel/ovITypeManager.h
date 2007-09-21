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
				const OpenViBE::CString& sTypeName)=0;
			virtual OpenViBE::boolean registerEnumerationEntry(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& sEntryName,
				const OpenViBE::uint64 ui64EntryValue)=0;

			virtual OpenViBE::boolean registerBitMaskType(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& sTypeName)=0;
			virtual OpenViBE::boolean registerBitMaskEntry(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& sEntryName,
				const OpenViBE::uint64 ui64EntryValue)=0;

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

			virtual OpenViBE::uint64 getEnumerationEntryCount(
				const OpenViBE::CIdentifier& rTypeIdentifier)=0;
			virtual OpenViBE::boolean getEnumerationEntry(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::uint64 ui64EntryIndex,
				OpenViBE::CString& sEntryName,
				OpenViBE::uint64& rEntryValue)=0;
			virtual OpenViBE::CString getEnumerationEntryNameFromValue(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::uint64 ui64EntryValue)=0;
			virtual OpenViBE::uint64 getEnumerationEntryValueFromName(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& rEntryName)=0;

			virtual OpenViBE::uint64 getBitMaskEntryCount(
				const OpenViBE::CIdentifier& rTypeIdentifier)=0;
			virtual OpenViBE::boolean getBitMaskEntry(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::uint64 ui64EntryIndex,
				OpenViBE::CString& sEntryName,
				OpenViBE::uint64& rEntryValue)=0;
			virtual OpenViBE::CString getBitMaskEntryNameFromValue(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::uint64 ui64EntryValue)=0;
			virtual OpenViBE::uint64 getBitMaskEntryValueFromName(
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& rEntryName)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_TypeManager);
		};
	};
};

#endif // __OpenViBE_Kernel_ITypeManager_H__
