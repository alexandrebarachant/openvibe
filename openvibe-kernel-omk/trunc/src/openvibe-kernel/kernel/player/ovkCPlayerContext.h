#ifndef __OpenViBEKernel_Kernel_CPlayerContext_H__
#define __OpenViBEKernel_Kernel_CPlayerContext_H__

#include "ovkTPluginObjectContext.h"
#include "simulated-objects/ovkPsSimulatedBox.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class CPlayerContext : virtual public OpenViBE::Kernel::TPluginObjectContext<OpenViBE::Plugins::IPlayerContext>
		{
		public:

			CPlayerContext(const OpenViBE::Kernel::IKernelContext& rKernelContext, ::PsSimulatedBox* pSimulatedBox);

			virtual OpenViBE::boolean sendSignal(
				const OpenViBE::CMessageSignal& rMessageSignal);
			virtual OpenViBE::boolean sendMessage(
				const OpenViBE::CMessageEvent& rMessageEvent,
				const OpenViBE::CIdentifier& rTargetIdentifier);

			virtual OpenViBE::boolean sendMessage(
				const OpenViBE::CMessageEvent& rMessageEvent,
				const OpenViBE::CIdentifier* pTargetIdentifier,
				const OpenViBE::uint32 ui32TargetIdentifierCount);

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TPluginObjectContext<OpenViBE::Plugins::IPlayerContext>, OVK_ClassId_Kernel_PlayerContext)

		protected:

			::PsSimulatedBox* m_pSimulatedBox;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_CPlayerContext_H__
