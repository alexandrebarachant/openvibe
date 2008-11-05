#ifndef __OpenViBEKernel_Kernel_Configuration_CConfigurationManager_H__
#define __OpenViBEKernel_Kernel_Configuration_CConfigurationManager_H__

#include "../ovkTKernelObject.h"

#include <map>
#include <string>

namespace OpenViBE
{
	namespace Kernel
	{
		typedef struct
		{
			OpenViBE::CString m_sConfigurationName;
			OpenViBE::CString m_sConfigurationValue;
		} SConfigurationToken;

		class CConfigurationManager : public OpenViBE::Kernel::TKernelObject < OpenViBE::Kernel::IConfigurationManager >
		{
		public:

			CConfigurationManager(const OpenViBE::Kernel::IKernelContext& rKernelContext);

			virtual OpenViBE::boolean addConfigurationFromFile(
				const OpenViBE::CString& rFileNameWildCard);

			virtual OpenViBE::CIdentifier createConfigurationToken(
				const OpenViBE::CString& rConfigurationTokenName,
				const OpenViBE::CString& rConfigurationTokenValue);
			virtual OpenViBE::boolean releaseConfigurationToken(
				const OpenViBE::CIdentifier& rConfigurationTokenIdentifier);
			virtual OpenViBE::CIdentifier getNextConfigurationTokenIdentifier(
				const OpenViBE::CIdentifier& rPreviousConfigurationTokenIdentifier) const;

			virtual OpenViBE::CString getConfigurationTokenName(
				const OpenViBE::CIdentifier& rConfigurationTokenIdentifier) const;
			virtual OpenViBE::CString getConfigurationTokenValue(
				const OpenViBE::CIdentifier& rConfigurationTokenIdentifier) const;

			virtual OpenViBE::boolean setConfigurationTokenName(
				const OpenViBE::CIdentifier& rConfigurationTokenIdentifier,
				const OpenViBE::CString& rConfigurationTokenName);
			virtual OpenViBE::boolean setConfigurationTokenValue(
				const OpenViBE::CIdentifier& rConfigurationTokenIdentifier,
				const OpenViBE::CString& rConfigurationTokenValue);

			virtual OpenViBE::CIdentifier lookUpConfigurationTokenIdentifier(
				const OpenViBE::CString& rConfigurationTokenName) const;

			virtual OpenViBE::CString expandExpression(
				const OpenViBE::CString& rExpression) const;

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TKernelObject < OpenViBE::Kernel::IConfigurationManager >, OVK_ClassId_Kernel_Configuration_ConfigurationManager);

		protected:

			virtual OpenViBE::CIdentifier getUnusedIdentifier(void) const;

			OpenViBE::boolean expand(const std::string& sValue, std::string& sResult) const;

		protected:

			std::map < OpenViBE::CIdentifier, OpenViBE::Kernel::SConfigurationToken > m_vConfigurationToken;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_Configuration_CConfigurationManager_H__
