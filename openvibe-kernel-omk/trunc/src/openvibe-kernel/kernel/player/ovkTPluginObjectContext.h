#ifndef __OpenViBEKernel_Kernel_TPluginObjectContext_H__
#define __OpenViBEKernel_Kernel_TPluginObjectContext_H__

#include "../../ovk_base.h"
#include "../ovkTKernelObject.h"

#include <openvibe/ov_all.h>

namespace OpenViBE
{
	namespace Kernel
	{
		template <class T> class TPluginObjectContext : virtual public OpenViBE::Kernel::TKernelObject<T>
		{
		public:

			TPluginObjectContext(const OpenViBE::Kernel::IKernelContext& rKernelContext)
				:TKernelObject<T>(rKernelContext)
			{
			}

			_IsDerivedFromClass_(OpenViBE::Kernel::TKernelObject<T>, OVK_ClassId_Kernel_PluginObjectContext)
		};
	};
};

#endif // __OpenViBEKernel_Kernel_TPluginObjectContext_H__
