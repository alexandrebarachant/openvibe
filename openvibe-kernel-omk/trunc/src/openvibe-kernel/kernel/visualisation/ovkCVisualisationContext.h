#ifndef __OpenViBE_Kernel_Visualisation_CVisualisationContext_H__
#define __OpenViBE_Kernel_Visualisation_CVisualisationContext_H__

#include "../ovkTKernelObject.h"

class PsSimulatedBox;

namespace OpenViBE
{
	namespace Kernel
	{
		class CVisualisationContext : virtual public OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IVisualisationContext>
		{
		public:

			CVisualisationContext(const OpenViBE::Kernel::IKernelContext& rKernelContext, ::PsSimulatedBox* pSimulatedBox);

			virtual ~CVisualisationContext(void);

			virtual OpenViBE::boolean setWidgets(
				::GtkWidget* pWidget,
				::GtkWidget* pToolbarWidget);

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IVisualisationContext>, OVK_ClassId_Kernel_Visualisation_VisualisationContext)

		protected:

			::PsSimulatedBox* m_pSimulatedBox;
			OpenViBE::Kernel::IVisualisationManager* m_pVisualisationManagerBridge;
		};
	};
};

#endif // __OpenViBE_Kernel_Visualisation_CVisualisationContext_H__
