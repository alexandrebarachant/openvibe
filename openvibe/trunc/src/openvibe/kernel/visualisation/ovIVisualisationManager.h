#ifndef __OpenViBE_Kernel_Visualisation_IVisualisationManager_H__
#define __OpenViBE_Kernel_Visualisation_IVisualisationManager_H__

#include "../ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class OV_API IVisualisationManager : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			class OV_API IVisualisationTreeEnum
			{
			public:
				virtual ~IVisualisationTreeEnum(void) { }
				virtual OpenViBE::boolean callback(
					const OpenViBE::CIdentifier& rVisualisationTreeIdentifier,
					const OpenViBE::Kernel::IVisualisationTree& rVisualisationTree)=0;
			};

			virtual OpenViBE::boolean createVisualisationTree(
				OpenViBE::CIdentifier& rVisualisationTreeIdentifier)=0;
			virtual OpenViBE::boolean releaseVisualisationTree(
				const OpenViBE::CIdentifier& rVisualisationTreeIdentifier)=0;
			virtual OpenViBE::Kernel::IVisualisationTree& getVisualisationTree(
				const OpenViBE::CIdentifier& rVisualisationTreeIdentifier)=0;
			virtual OpenViBE::boolean enumerateVisualisationTrees(
				OpenViBE::Kernel::IVisualisationManager::IVisualisationTreeEnum& rCallBack) const=0;
			virtual OpenViBE::boolean setWidgets(
				const OpenViBE::CIdentifier& rVisualisationTreeIdentifer,
				const OpenViBE::CString& rVisualisationBoxName,
				::GtkWidget* pWidget,
				::GtkWidget* pToolbarWidget)=0;
#ifdef OV_3D
			virtual void init3DContext(void)=0;
			virtual OpenViBE::boolean create3DVisualisationWidget(
				void* pParentWidget,
				OpenViBE::uint32& ui32Index)=0;
			virtual OpenViBE::boolean realize3DVisualisationWidgets(void)=0;
			virtual void release3DContext(void)=0;
#endif
			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Visualisation_VisualisationManager)
		};
	};
};

#endif // __OpenViBE_Kernel_Visualisation_IVisualisationManager_H__
