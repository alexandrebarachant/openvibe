#ifndef __OpenViBEKernel_Kernel_Player_CPlayer_H__
#define __OpenViBEKernel_Kernel_Player_CPlayer_H__

#include "../../ovk_base.h"
#include "../ovkTKernelObject.h"

#include <openvibe/ov_all.h>

// OpenMASK
#include <PsController.h>
#include <PsPvmController.h>

namespace OpenViBE
{
	namespace Kernel
	{
		namespace Player
		{
			class CPlayer : virtual public OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::Player::IPlayer>
			{
			public:

				CPlayer(OpenViBE::Kernel::IKernel& rKernel);
				virtual ~CPlayer(void);

				virtual OpenViBE::boolean reset(
					const OpenViBE::Kernel::IScenario& rScenario,
					OpenViBE::Kernel::IPluginManager& rPluginManager);

				virtual OpenViBE::boolean loop(void);

				_IsDerivedFromClass_Final_(OpenViBE::Kernel::Player::IPlayer, OVK_ClassId_Kernel_Player_Player);

			protected:

				::PsController* m_pController;
				::PsnReferenceObjectHandle* m_pControllerHandle;
				::PsObjectDescriptor* m_pSimulation;
				OpenViBE::Kernel::IScenario* m_pScenario;
			};
		};
	};
};

#endif // __OpenViBEKernel_Kernel_Player_CPlayer_H__
