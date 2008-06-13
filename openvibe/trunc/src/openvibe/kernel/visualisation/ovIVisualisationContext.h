#ifndef __OpenViBE_Kernel_Visualisation_IVisualisationContext_H__
#define __OpenViBE_Kernel_Visualisation_IVisualisationContext_H__

#include "../ovIKernelObject.h"

typedef struct _GtkWidget GtkWidget;

namespace OpenViBE
{
	namespace Kernel
	{
		/**
		 * \class IVisualisationContext
		 * \author Vincent Delannoy (INRIA/IRISA)
		 * \date 2007-11
		 * \brief Visualisation manager interface for plugin objects
		 */
		class OV_API IVisualisationContext : public OpenViBE::Kernel::IKernelObject
		{
		public:

			/**
			 * \brief Sets the GtkWidgets corresponding to a visualisation plugin and its toolbar
			 * \param pWidget [in] : main widget of the plugin
			 * \param pToolbarWidget [in] : toolbar widget
			 *
			 * \return \e true in case of success, \e false otherwise.
			 */
			virtual OpenViBE::boolean setWidgets(
				::GtkWidget* pWidget,
				::GtkWidget* pToolbarWidget)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Visualisation_VisualisationContext)
		};
	};
};

#endif // __OpenViBE_Kernel_Visualisation_IVisualisationContext_H__
