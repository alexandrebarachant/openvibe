#ifndef __OpenViBEKernel_Kernel_Scenario_IScenarioManager_H__
#define __OpenViBEKernel_Kernel_Scenario_IScenarioManager_H__

#include "../ovkTKernelObject.h"

#include <map>

namespace OpenViBE
{
	namespace Kernel
	{
		class CScenarioManager : virtual public OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IScenarioManager>
		{
		public:

			CScenarioManager(const OpenViBE::Kernel::IKernelContext& rKernelContext);

			virtual OpenViBE::boolean createScenario(
				OpenViBE::CIdentifier& rScenarioIdentifier);
			virtual OpenViBE::boolean releaseScenario(
				const OpenViBE::CIdentifier& rScenarioIdentifier);
			virtual OpenViBE::Kernel::IScenario& getScenario(
				const OpenViBE::CIdentifier& rScenarioIdentifier);
			virtual OpenViBE::boolean enumerateScenarios(
				OpenViBE::Kernel::IScenarioManager::IScenarioEnum& rCallBack) const;

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IScenarioManager>, OVK_ClassId_Kernel_Scenario_ScenarioManager);

		protected:

			virtual OpenViBE::CIdentifier getUnusedIdentifier(void) const;

		protected:

			std::map<OpenViBE::CIdentifier, OpenViBE::Kernel::IScenario*> m_vScenario;

		};
	};
};

#endif // __OpenViBEKernel_Kernel_Scenario_IScenarioManager_H__
