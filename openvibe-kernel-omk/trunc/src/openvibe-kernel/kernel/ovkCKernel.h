#ifndef __OpenViBEKernel_CKernel_H__
#define __OpenViBEKernel_CKernel_H__

#include "ovkTKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class CKernel : virtual public OpenViBE::Kernel::IKernel
		{
		public:

			CKernel(void);

			virtual void release(void);

			virtual OpenViBE::Kernel::IObjectFactory& getObjectFactory(void);
			virtual const OpenViBE::Kernel::IObjectFactory& getObjectFactory(void) const;
			virtual OpenViBE::Kernel::IPluginManager& getPluginManager(void);
			virtual const OpenViBE::Kernel::IPluginManager& getPluginManager(void) const;
			virtual OpenViBE::Kernel::IScenarioManager& getScenarioManager(void);
			virtual const OpenViBE::Kernel::IScenarioManager& getScenarioManager(void) const;
			virtual OpenViBE::Kernel::IKernelLog& getKernelLog(
				const OpenViBE::uint32 ui32Level=0) const;

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::IKernel, OVK_ClassId_Kernel_Kernel)

		protected:

			OpenViBE::Kernel::IObjectFactory* m_pObjectFactory;
			OpenViBE::Kernel::IPluginManager* m_pPluginManager;
			OpenViBE::Kernel::IScenarioManager* m_pScenarioManager;
			mutable OpenViBE::Kernel::IKernelLog* m_pKernelLog;

		};
	};
};

#endif // __OpenViBEKernel_CKernel_H__
