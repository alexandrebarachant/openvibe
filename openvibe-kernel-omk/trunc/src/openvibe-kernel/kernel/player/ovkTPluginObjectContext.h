#ifndef __OpenViBEKernel_Kernel_TPluginObjectContext_H__
#define __OpenViBEKernel_Kernel_TPluginObjectContext_H__

#include "../../ovk_base.h"

#include <openvibe/ov_all.h>

namespace OpenViBE
{
	namespace Kernel
	{
		template <class T>
		class TPluginObjectContext : virtual public T
		{
		public:

			_IsDerivedFromClass_(T, OVK_ClassId_Kernel_PluginObjectContext)
		};
	};
};

#endif // __OpenViBEKernel_Kernel_TPluginObjectContext_H__
