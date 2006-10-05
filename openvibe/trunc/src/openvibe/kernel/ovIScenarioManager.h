#ifndef __OpenViBE_Kernel_IScenarioManager_H__
#define __OpenViBE_Kernel_IScenarioManager_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IScenarioManager : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			class OV_API IScenarioEnum
			{
			public:
				virtual ~IScenarioEnum(void) { }
				virtual OpenViBE::boolean callback(
					const OpenViBE::CIdentifier& rScenarioIdentifier,
					const OpenViBE::Kernel::IScenario& rScenario)=0;
			};

			virtual OpenViBE::boolean createScenario(
				OpenViBE::CIdentifier& rScenarioIdentifier)=0;
			virtual OpenViBE::boolean releaseScenario(
				const OpenViBE::CIdentifier& rScenarioIdentifier)=0;
			virtual OpenViBE::Kernel::IScenario& getScenario(
				const OpenViBE::CIdentifier& rScenarioIdentifier)=0;
			virtual OpenViBE::boolean enumerateScenarios(
				OpenViBE::Kernel::IScenarioManager::IScenarioEnum& rCallBack) const=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_ScenarioManager);
		};
	};
};

#endif // __OpenViBE_Kernel_IScenarioManager_H__
