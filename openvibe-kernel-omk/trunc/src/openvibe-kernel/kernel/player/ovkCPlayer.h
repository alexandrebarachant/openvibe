#ifndef __OpenViBEKernel_Kernel_Player_CPlayer_H__
#define __OpenViBEKernel_Kernel_Player_CPlayer_H__

#include "../ovkTKernelObject.h"

#include <openvibe/ov_all.h>

#include <system/CChrono.h>

class PsController;
class PsnReferenceObjectHandle;
class PsObjectDescriptor;

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

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::IPlayer, OVK_ClassId_Kernel_Player_Player);

		protected:

			::PsController* m_pController;
			::PsnReferenceObjectHandle* m_pControllerHandle;
			::PsObjectDescriptor* m_pSimulation;
			OpenViBE::Kernel::IScenario* m_pScenario;

			OpenViBE::uint32 m_ui32ControllerSteps;
			OpenViBE::uint32 m_ui32StartTime;
			OpenViBE::uint32 m_ui32SecondsLate;

		private:

			System::CChrono m_oBenchmarkChrono;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_Player_CPlayer_H__
