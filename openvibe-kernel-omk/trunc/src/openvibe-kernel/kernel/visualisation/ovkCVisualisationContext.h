#ifndef __OpenViBE_Kernel_Visualisation_CVisualisationContext_H__
#define __OpenViBE_Kernel_Visualisation_CVisualisationContext_H__

#include "../ovkTKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class CSimulatedBox;

		class CVisualisationContext : public OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IVisualisationContext>
		{
		public:

			CVisualisationContext(const OpenViBE::Kernel::IKernelContext& rKernelContext, OpenViBE::Kernel::CSimulatedBox* pSimulatedBox);

			virtual ~CVisualisationContext(void);

			virtual OpenViBE::boolean setWidgets(
				::GtkWidget* pWidget,
				::GtkWidget* pToolbarWidget);

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IVisualisationContext>, OVK_ClassId_Kernel_Visualisation_VisualisationContext)

		protected:

			OpenViBE::Kernel::CSimulatedBox* m_pSimulatedBox;
			OpenViBE::Kernel::IVisualisationManager* m_pVisualisationManagerBridge;
		};
	};
};

#endif // __OpenViBE_Kernel_Visualisation_CVisualisationContext_H__
