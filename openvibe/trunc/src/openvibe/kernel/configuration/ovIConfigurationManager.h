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

			virtual OpenViBE::CString expandExpression(
				const OpenViBE::CString& rExpression) const=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Configuration_ConfigurationManager);
		};
	};
};

#endif // __OpenViBE_Kernel_Configuration_IConfigurationManager_H__
