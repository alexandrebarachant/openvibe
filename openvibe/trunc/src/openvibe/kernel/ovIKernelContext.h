#ifndef __OpenViBE_Kernel_IKernelContext_H__
#define __OpenViBE_Kernel_IKernelContext_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IKernelObjectFactory;
		class IPlayerManager;
		class IPluginManager;
		class IScenarioManager;
		class ITypeManager;
		class ILogManager;
		class IVisualisationManager;

		class OV_API IKernelContext : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::Kernel::IPlayerManager& getPlayerManager(void) const=0;
			virtual OpenViBE::Kernel::IPluginManager& getPluginManager(void) const=0;
			virtual OpenViBE::Kernel::IKernelObjectFactory& getKernelObjectFactory(void) const=0;
			virtual OpenViBE::Kernel::IScenarioManager& getScenarioManager(void) const=0;
			virtual OpenViBE::Kernel::ITypeManager& getTypeManager(void) const=0;
			virtual OpenViBE::Kernel::ILogManager& getLogManager(void) const=0;
			virtual OpenViBE::Kernel::IVisualisationManager& getVisualisationManager(void) const=0;

			virtual OpenViBE::Kernel::IKernelObjectFactory& getObjectFactory(void) const { return getKernelObjectFactory(); }

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_KernelContext)
		};
	};
};

#endif // __OpenViBE_Kernel_IKernel_H__
