#ifndef __OpenViBEKernel_CKernelContext_H__
#define __OpenViBEKernel_CKernelContext_H__

#include "ovkTKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class CKernelContext : virtual public OpenViBE::Kernel::IKernelContext
		{
		public:

			CKernelContext(OpenViBE::Kernel::IKernel& rKernel);
			virtual ~CKernelContext(void);

			virtual OpenViBE::Kernel::IObjectFactory& getObjectFactory(void) const;
			virtual OpenViBE::Kernel::IPluginManager& getPluginManager(void) const;
			virtual OpenViBE::Kernel::IScenarioManager& getScenarioManager(void) const;
			virtual OpenViBE::Kernel::ILogManager& getLogManager(void) const;

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::IKernelContext, OVK_ClassId_Kernel_KernelContext)

		protected:

			OpenViBE::Kernel::IKernel& m_rKernel;
			mutable OpenViBE::Kernel::IObjectFactory* m_pObjectFactory;
			mutable OpenViBE::Kernel::IPluginManager* m_pPluginManager;
			mutable OpenViBE::Kernel::IScenarioManager* m_pScenarioManager;
			mutable OpenViBE::Kernel::ILogManager* m_pLogManager;

		private:

			CKernelContext(void);

		};
	};
};

#endif // __OpenViBEKernel_CKernelContext_H__
