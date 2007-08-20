#ifndef __OpenViBE_Kernel_Player_IPlayer_H__
#define __OpenViBE_Kernel_Player_IPlayer_H__

#include "../ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IScenario;

		class OV_API IPlayer : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::boolean reset(
				const OpenViBE::Kernel::IScenario& rScenario,
				OpenViBE::Kernel::IPluginManager& rPluginManager)=0;

			virtual OpenViBE::boolean loop(void)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Player_Player);
		};
	};
};

#endif // __OpenViBE_Kernel_Player_IPlayer_H__
