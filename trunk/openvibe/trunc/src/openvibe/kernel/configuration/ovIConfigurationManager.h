#ifndef __OpenViBE_Kernel_Configuration_IConfigurationManager_H__
#define __OpenViBE_Kernel_Configuration_IConfigurationManager_H__

#include "../ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class OV_API IConfigurationManager : public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual void clear(void)=0;

			virtual OpenViBE::boolean addConfigurationFromFile(
				const OpenViBE::CString& rFileNameWildCard)=0;

			virtual OpenViBE::CIdentifier createConfigurationToken(
				const OpenViBE::CString& rConfigurationTokenName,
				const OpenViBE::CString& rConfigurationTokenValue)=0;
			virtual OpenViBE::boolean releaseConfigurationToken(
				const OpenViBE::CIdentifier& rConfigurationTokenIdentifier)=0;
			virtual OpenViBE::CIdentifier getNextConfigurationTokenIdentifier(
				const OpenViBE::CIdentifier& rPreviousConfigurationTokenIdentifier) const=0;

			virtual OpenViBE::CString getConfigurationTokenName(
				const OpenViBE::CIdentifier& rConfigurationTokenIdentifier) const=0;
			virtual OpenViBE::CString getConfigurationTokenValue(
				const OpenViBE::CIdentifier& rConfigurationTokenIdentifier) const=0;

			virtual OpenViBE::boolean setConfigurationTokenName(
				const OpenViBE::CIdentifier& rConfigurationTokenIdentifier,
				const OpenViBE::CString& rConfigurationTokenName)=0;
			virtual OpenViBE::boolean setConfigurationTokenValue(
				const OpenViBE::CIdentifier& rConfigurationTokenIdentifier,
				const OpenViBE::CString& rConfigurationTokenValue)=0;

			virtual OpenViBE::CIdentifier lookUpConfigurationTokenIdentifier(
				const OpenViBE::CString& rConfigurationTokenName) const=0;

			virtual OpenViBE::CString expand(
				const OpenViBE::CString& rExpression) const=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Configuration_ConfigurationManager);

			virtual OpenViBE::float64 expandAsFloat(
				const OpenViBE::CString& rExpression,
				const OpenViBE::float64 f64FallbackValue=0) const=0;
			virtual OpenViBE::int64 expandAsInteger(
				const OpenViBE::CString& rExpression,
				const OpenViBE::int64 i64FallbackValue=0) const=0;
			virtual OpenViBE::uint64 expandAsUInteger(
				const OpenViBE::CString& rExpression,
				const OpenViBE::uint64 ui64FallbackValue=0) const=0;
			virtual OpenViBE::boolean expandAsBoolean(
				const OpenViBE::CString& rExpression,
				const OpenViBE::boolean bFallbackValue=true) const=0;
			virtual OpenViBE::uint64 expandAsEnumerationEntryValue(
				const OpenViBE::CString& rExpression,
				const OpenViBE::CIdentifier& rEnumerationTypeIdentifier,
				const OpenViBE::uint64 ui64FallbackValue=0) const=0;
		};
	};
};

#endif // __OpenViBE_Kernel_Configuration_IConfigurationManager_H__
