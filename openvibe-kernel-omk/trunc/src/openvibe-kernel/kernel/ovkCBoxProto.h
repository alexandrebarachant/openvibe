#ifndef __OpenViBEKernel_Kernel_CBoxProto_H__
#define __OpenViBEKernel_Kernel_CBoxProto_H__

#include "ovkTKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class CBoxProto : virtual public OpenViBE::Kernel::TKernelObject<OpenViBE::Plugins::IBoxProto>
		{
		public:

			CBoxProto(const OpenViBE::Kernel::IKernelContext& rKernelContext, OpenViBE::Kernel::IBox& rBox);

			virtual OpenViBE::uint32 addInput(
				const OpenViBE::CString& sName,
				const OpenViBE::CIdentifier& rTypeIdentifier);
			virtual OpenViBE::uint32 addOutput(
				const OpenViBE::CString& sName,
				const OpenViBE::CIdentifier& rTypeIdentifier);
			virtual OpenViBE::uint32 addSetting(
				const OpenViBE::CString& sName,
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& sDefaultValue);

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxProto, OVK_ClassId_Kernel_BoxProto)

		protected:

			OpenViBE::Kernel::IBox& m_rBox;

		private:

			CBoxProto(void);
		};
	};
};

#endif // __OpenViBEKernel_Kernel_CBoxProto_H__
