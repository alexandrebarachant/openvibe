#ifndef __OpenViBEKernel_Kernel_CStaticBoxContext_H__
#define __OpenViBEKernel_Kernel_CStaticBoxContext_H__

#include "ovkTPluginObjectContext.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class CStaticBoxContext : virtual public OpenViBE::Kernel::TPluginObjectContext<OpenViBE::Plugins::IStaticBoxContext>
		{
		public:

			CStaticBoxContext(const OpenViBE::Kernel::IKernelContext& rKernelContext, const OpenViBE::Kernel::IBox* pBox);

			virtual OpenViBE::CIdentifier getIdentifier(void) const;
			virtual OpenViBE::CString getName(void) const;

			virtual OpenViBE::uint32 getInputCount(void) const;
			virtual OpenViBE::boolean getInputType(
				const OpenViBE::uint32 ui32InputIndex,
				OpenViBE::CIdentifier& rTypeIdentifier) const;
			virtual OpenViBE::boolean getInputName(
				const OpenViBE::uint32 ui32InputIndex,
				OpenViBE::CString& rName) const;

			virtual OpenViBE::uint32 getOutputCount(void) const;
			virtual OpenViBE::boolean getOutputType(
				const OpenViBE::uint32 ui32OutputIndex,
				OpenViBE::CIdentifier& rTypeIdentifier) const;
			virtual OpenViBE::boolean getOutputName(
				const OpenViBE::uint32 ui32OutputIndex,
				OpenViBE::CString& rName) const;

			virtual OpenViBE::uint32 getSettingCount(void) const;
			virtual OpenViBE::boolean getSettingType(
				const OpenViBE::uint32 ui32SettingIndex,
				OpenViBE::CIdentifier& rTypeIdentifier) const;
			virtual OpenViBE::boolean getSettingValue(
				const OpenViBE::uint32 ui32SettingIndex,
				OpenViBE::CString& sValue) const;

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TPluginObjectContext<OpenViBE::Plugins::IStaticBoxContext>, OVK_ClassId_Kernel_StaticBoxContext)

		protected:

			const OpenViBE::Kernel::IBox* m_pBox;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_CStaticBoxContext_H__
