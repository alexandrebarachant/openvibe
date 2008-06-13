#ifndef __OpenViBEKernel_Kernel_Visualisation_CVisualisationManager_H__
#define __OpenViBEKernel_Kernel_Visualisation_CVisualisationManager_H__

#include "../ovkTKernelObject.h"

#include <map>

namespace OpenViBE
{
	namespace Kernel
	{
		//class COgreContext;

		class CVisualisationManager : public OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IVisualisationManager>
		{
		public:

			CVisualisationManager(
				const OpenViBE::Kernel::IKernelContext& rKernelContext);

			virtual OpenViBE::boolean createVisualisationTree(
				OpenViBE::CIdentifier& rVisualisationTreeIdentifier);
			virtual OpenViBE::boolean releaseVisualisationTree(
				const OpenViBE::CIdentifier& rVisualisationTreeIdentifier);
			virtual OpenViBE::Kernel::IVisualisationTree& getVisualisationTree(
				const OpenViBE::CIdentifier& rVisualisationTreeIdentifier);
			virtual OpenViBE::boolean enumerateVisualisationTrees(
				OpenViBE::Kernel::IVisualisationManager::IVisualisationTreeEnum& rCallBack) const;
			virtual OpenViBE::boolean setWidgets(
				const CIdentifier& rVisualisationTreeIdentifier,
				const CString& rVisualisationBoxName,
				::GtkWidget* pWidget,
				::GtkWidget* pToolbarWidget);
/*
			virtual void init3DContext();
			virtual OpenViBE::boolean create3DVisualisationWidget(
				void* pParentWidget,
				OpenViBE::uint32& ui32Index);
			virtual OpenViBE::boolean realize3DVisualisationWidgets();
			virtual void release3DContext();
*/
			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IVisualisationManager>, OVK_ClassId_Kernel_Visualisation_VisualisationManager);

		protected:

			virtual OpenViBE::CIdentifier getUnusedIdentifier(void) const;

		protected:

			std::map<OpenViBE::CIdentifier, OpenViBE::Kernel::IVisualisationTree*> m_vVisualisationTree;
			//OpenViBE::Kernel::COgreContext* m_pOgreContext;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_Visualisation_CVisualisationManager_H__
