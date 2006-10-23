#ifndef __OpenViBEKernel_Plugins_CBoxAlgorithmContext_H__
#define __OpenViBEKernel_Plugins_CBoxAlgorithmContext_H__

#include "ovkTPluginObjectContext.h"
#include "ovkCBoxContext.h"
#include "ovkCPlayerContext.h"
#include "simulated-objects/ovkPsSimulatedBox.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class CBoxAlgorithmContext : virtual public OpenViBE::Kernel::TPluginObjectContext<OpenViBE::Plugins::IBoxAlgorithmContext>
		{
		public:

			CBoxAlgorithmContext(::PsSimulatedBox& rSimulatedBox, const OpenViBE::Kernel::IBox& rBox);

			virtual OpenViBE::Plugins::IBoxContext& getBoxContext(void);
			virtual OpenViBE::Plugins::IPlayerContext& getPlayerContext(void);
			virtual OpenViBE::boolean markAlgorithmAsReadyToProcess(void);

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TPluginObjectContext<OpenViBE::Plugins::IBoxAlgorithmContext>, OVK_ClassId_Kernel_BoxAlgorithmContext)

			virtual OpenViBE::boolean isAlgorithmReadyToProcess(void);

		protected:

			OpenViBE::Kernel::CBoxContext m_oBoxContext;
			OpenViBE::Kernel::CPlayerContext m_oPlayerContext;
			OpenViBE::boolean m_bReadyToProcess;
		};
	};
};

#endif // __OpenViBEKernel_Plugins_CBoxAlgorithmContext_H__
