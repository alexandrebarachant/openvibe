#ifndef __OpenViBEKernel_Kernel_TKernelObject_H__
#define __OpenViBEKernel_Kernel_TKernelObject_H__

#include "../ovk_base.h"

#include <openvibe/ov_all.h>

namespace OpenViBE
{
	namespace Kernel
	{
		template <class T> class OV_API TKernelObject : virtual public T
		{
		public:

			TKernelObject(OpenViBE::Kernel::IKernel& rKernel)
				:m_rKernel(rKernel)
			{
			}

			OpenViBE::Kernel::IKernel& getKernel(void)
			{
				return m_rKernel;
			}

			const OpenViBE::Kernel::IKernel& getKernel(void) const
			{
				return m_rKernel;
			}

			_IsDerivedFromClass_(T, OVK_ClassId_Kernel_KernelObject)

		private:

			OpenViBE::Kernel::IKernel& m_rKernel;

			TKernelObject(void);
		};
	};
};

#endif // __OpenViBEKernel_Kernel_IKernelObject_H__
