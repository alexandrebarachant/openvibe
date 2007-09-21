#ifndef __OpenViBEKernel_Kernel_Player_CPlayerContext_H__
#define __OpenViBEKernel_Kernel_Player_CPlayerContext_H__

#include "../ovkTKernelObject.h"

class PsSimulatedBox;

namespace OpenViBE
{
	namespace Kernel
	{
		class CPlayerContext : virtual public OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IPlayerContext>
		{
		public:

			CPlayerContext(const OpenViBE::Kernel::IKernelContext& rKernelContext, ::PsSimulatedBox* pSimulatedBox);
			virtual ~CPlayerContext();

			virtual OpenViBE::boolean sendSignal(
				const OpenViBE::CMessageSignal& rMessageSignal);
			virtual OpenViBE::boolean sendMessage(
				const OpenViBE::CMessageEvent& rMessageEvent,
				const OpenViBE::CIdentifier& rTargetIdentifier);
			virtual OpenViBE::boolean sendMessage(
				const OpenViBE::CMessageEvent& rMessageEvent,
				const OpenViBE::CIdentifier* pTargetIdentifier,
				const OpenViBE::uint32 ui32TargetIdentifierCount);

			virtual OpenViBE::Kernel::ILogManager& getLogManager(void);
			virtual OpenViBE::Kernel::IScenarioManager& getScenarioManager(void);
			virtual OpenViBE::Kernel::ITypeManager& getTypeManager(void);

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IPlayerContext>, OVK_ClassId_Kernel_Player_PlayerContext)

		protected:

			::PsSimulatedBox* m_pSimulatedBox;
			OpenViBE::Kernel::ILogManager* m_pLogManagerBridge;
			OpenViBE::Kernel::IScenarioManager* m_pScenarioManagerBridge;
			OpenViBE::Kernel::ITypeManager* m_pTypeManagerBridge;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_Player_CPlayerContext_H__
