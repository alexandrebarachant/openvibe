#ifndef __OpenViBEKernel_Kernel_Player_CPlayer_H__
#define __OpenViBEKernel_Kernel_Player_CPlayer_H__

#include "../ovkTKernelObject.h"

#include <openvibe/ov_all.h>

// OpenMASK
#include <PsController.h>

namespace OpenViBE
{
	namespace Kernel
	{
		class CPlayer : virtual public OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IPlayer>
		{
		public:

			CPlayer(const OpenViBE::Kernel::IKernelContext& rKernelContext);
			virtual ~CPlayer(void);

			virtual OpenViBE::boolean reset(
				const OpenViBE::Kernel::IScenario& rScenario,
				OpenViBE::Kernel::IPluginManager& rPluginManager);

			virtual OpenViBE::boolean loop(void);

			// IBoxEnum callback
			OpenViBE::boolean callback(
				const OpenViBE::Kernel::IScenario& rScenario,
				OpenViBE::Kernel::IBox& rBox);

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::IPlayer, OVK_ClassId_Kernel_Player);

		protected:

			::PsController* m_pController;
			::PsnReferenceObjectHandle* m_pControllerHandle;
			::PsObjectDescriptor* m_pSimulation;
			OpenViBE::Kernel::IScenario* m_pScenario;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_Player_CPlayer_H__
