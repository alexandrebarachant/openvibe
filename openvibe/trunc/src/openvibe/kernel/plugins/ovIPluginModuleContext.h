#ifndef __OpenViBE_Kernel_Plugins_IPluginModuleContext_H__
#define __OpenViBE_Kernel_Plugins_IPluginModuleContext_H__

#include "../ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class ILogManager;
		class ITypeManager;
		class IScenarioManager;

		class OV_API IPluginModuleContext : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::Kernel::IScenarioManager& getScenarioManager(void) const=0;
			virtual OpenViBE::Kernel::ITypeManager& getTypeManager(void) const=0;
			virtual OpenViBE::Kernel::ILogManager& getLogManager(void) const=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Plugins_PluginModuleContext)
		};
	};
};

#endif // __OpenViBE_Kernel_Plugins_IPluginModuleContext_H__
