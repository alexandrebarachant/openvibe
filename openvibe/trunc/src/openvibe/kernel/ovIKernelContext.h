#ifndef __OpenViBE_Kernel_IKernelContext_H__
#define __OpenViBE_Kernel_IKernelContext_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IObjectFactory;
		class IPluginManager;
		class IScenarioManager;
		class ILogManager;

		class OV_API IKernelContext : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::Kernel::IPluginManager& getPluginManager(void) const=0;
			virtual OpenViBE::Kernel::IObjectFactory& getObjectFactory(void) const=0;
			virtual OpenViBE::Kernel::IScenarioManager& getScenarioManager(void) const=0;
			virtual OpenViBE::Kernel::ILogManager& getLogManager(void) const=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_KernelContext)
		};
	};
};

#endif // __OpenViBE_Kernel_IKernel_H__
