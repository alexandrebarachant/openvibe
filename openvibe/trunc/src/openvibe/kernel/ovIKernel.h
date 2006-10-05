#ifndef __OpenViBE_Kernel_IKernel_H__
#define __OpenViBE_Kernel_IKernel_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IObjectFactory;
		class IPluginManager;
		class IScenarioManager;
		class IKernelLog;

		class OV_API IKernel : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual void release(void)=0;

			virtual OpenViBE::Kernel::IPluginManager& getPluginManager(void)=0;
			virtual const OpenViBE::Kernel::IPluginManager& getPluginManager(void) const=0;
			virtual OpenViBE::Kernel::IObjectFactory& getObjectFactory(void)=0;
			virtual const OpenViBE::Kernel::IObjectFactory& getObjectFactory(void) const=0;
			virtual OpenViBE::Kernel::IScenarioManager& getScenarioManager(void)=0;
			virtual const OpenViBE::Kernel::IScenarioManager& getScenarioManager(void) const=0;
			virtual OpenViBE::Kernel::IKernelLog& getKernelLog(
				const OpenViBE::uint32 ui32Level=0) const=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Kernel)
		};
	};
};

#endif // __OpenViBE_Kernel_IKernel_H__
