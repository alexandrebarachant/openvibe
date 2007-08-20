#ifndef __OpenViBEKernel_Kernel_Player_CStaticBoxContext_H__
#define __OpenViBEKernel_Kernel_Player_CStaticBoxContext_H__

#include "../ovkTKernelObject.h"

#include "../../tools/ovkTBoxProxy.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class CStaticBoxContext : virtual public OpenViBE::Tools::TBoxProxy<OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IBox> >
		{
		public:

			CStaticBoxContext(const OpenViBE::Kernel::IKernelContext& rKernelContext, const OpenViBE::Kernel::IBox* pBox);

			_IsDerivedFromClass_Final_(OpenViBE::Tools::TBoxProxy<OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IBox> >, OVK_ClassId_Kernel_Player_StaticBoxContext)
		};
	};
};

#endif // __OpenViBEKernel_Kernel_Player_CStaticBoxContext_H__
