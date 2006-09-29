#ifndef __OpenViBE_Kernel_IPluginObject_H__
#define __OpenViBE_Kernel_IPluginObject_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class OV_API IPluginObject : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_PluginObject)
		};
	};
};

#endif // __OpenViBE_Kernel_IPluginObject_H__
