#ifndef __OpenViBEKernel_Kernel_Player_CBoxAlgorithmContext_H__
#define __OpenViBEKernel_Kernel_Player_CBoxAlgorithmContext_H__

#include "../ovkTKernelObject.h"

#include "ovkCStaticBoxContext.h"
#include "ovkCDynamicBoxContext.h"
#include "ovkCPlayerContext.h"
#include "../visualisation/ovkCVisualisationContext.h"

class PsSimulatedBox;

namespace OpenViBE
{
	namespace Kernel
	{
		class CBoxAlgorithmContext : virtual public OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IBoxAlgorithmContext>
		{
		public:

			CBoxAlgorithmContext(const OpenViBE::Kernel::IKernelContext& rKernelContext, ::PsSimulatedBox* pSimulatedBox, const OpenViBE::Kernel::IBox* pBox);

			virtual OpenViBE::Kernel::IBox* getStaticBoxContext(void);
			virtual OpenViBE::Kernel::IBoxIO* getDynamicBoxContext(void);
			virtual OpenViBE::Kernel::IPlayerContext* getPlayerContext(void);
			virtual OpenViBE::Kernel::IVisualisationContext* getVisualisationContext(void);

			virtual OpenViBE::boolean markAlgorithmAsReadyToProcess(void);

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IBoxAlgorithmContext>, OVK_ClassId_Kernel_Player_BoxAlgorithmContext)

			virtual OpenViBE::boolean isAlgorithmReadyToProcess(void);

		protected:

			OpenViBE::Kernel::CStaticBoxContext m_oStaticBoxContext;
			OpenViBE::Kernel::CDynamicBoxContext m_oDynamicBoxContext;
			OpenViBE::Kernel::CPlayerContext m_oPlayerContext;
			OpenViBE::Kernel::CVisualisationContext m_oVisualisationContext;
			OpenViBE::boolean m_bReadyToProcess;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_Player_CBoxAlgorithmContext_H__
