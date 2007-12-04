#ifndef __OpenViBE_Kernel_Visualisation_IVisualisationTree_H__
#define __OpenViBE_Kernel_Visualisation_IVisualisationTree_H__

#include "../ovIKernelObject.h"

#include "ovIVisualisationWidget.h"

typedef struct _GtkWidget   GtkWidget;
typedef struct _GtkTreeView GtkTreeView;
typedef struct _GtkTreeIter GtkTreeIter;
typedef struct _GtkTreePath GtkTreePath;

namespace OpenViBE
{
	namespace Kernel
	{
		//Drag locations around a widget
		enum EDragDataLocation
		{
			EDragData_Left,
			EDragData_Right,
			EDragData_Top,
			EDragData_Bottom
		};

		//Column types in a visualisation tree
		enum EVisualisationTreeColumn
		{
			EVisualisationTreeColumn_StringName,       // IBox name
			EVisualisationTreeColumn_StringStockIcon,  // Gtk stock icon
			EVisualisationTreeColumn_ULongNodeType,    // EVisualisationTreeNode type
			EVisualisationTreeColumn_StringIdentifier, // IVisualisationWidget identifier
			EVisualisationTreeColumn_PointerWidget     // GtkWidget pointer
		};

		//Node types in a visualisation tree
		enum EVisualisationTreeNode
		{
			EVisualisationTreeNode_Undefined,
			EVisualisationTreeNode_VPU,
			EVisualisationTreeNode_VisualisationWindow,
			EVisualisationTreeNode_VisualisationPanel,
			EVisualisationTreeNode_VisualisationBox,
			EVisualisationTreeNode_VerticalSplit,
			EVisualisationTreeNode_HorizontalSplit,
			EVisualisationTreeNode_Unaffected
		};

		class IVisualisationWidget;
		class IVisualisationTree;

		struct OV_API ITreeViewCB
		{
			virtual ~ITreeViewCB(void) { }
			virtual void              createTreeWidget      (OpenViBE::Kernel::IVisualisationWidget* pVisualisationWidget)                            { };
			virtual ::GtkWidget*      loadTreeWidget        (OpenViBE::Kernel::IVisualisationWidget* pVisualisationWidget)                            { return NULL; }
			virtual void              endLoadTreeWidget     (::GtkWidget* pTreeWidget)                                                                { }
			virtual ::GtkWidget*      getVisualisationWidget(::GtkWidget* pTreeWidget)                                                                { return pTreeWidget; }
			virtual ::GtkWidget*      getTreeWidget         (::GtkWidget* pVisualisationWidget)                                                       { return pVisualisationWidget; }
			virtual const char*       getTreeWidgetIcon     (OpenViBE::Kernel::EVisualisationTreeNode eVisualisationTreeNode)                         { return ""; }
			virtual void              resizePanedWidget     (OpenViBE::Kernel::IVisualisationWidget* pVisualisationWidget, ::GtkWidget* pPanedWidget) { }
			virtual OpenViBE::boolean setWidgets            (const OpenViBE::CString& rName, ::GtkWidget* pWidget, ::GtkWidget* pToolbarWidget)       { return false; }
		};

		class OV_API IVisualisationTree : virtual public OpenViBE::Kernel::IAttributable
		{
		public:
			virtual void init(
				const OpenViBE::Kernel::IScenario* pScenario)=0;

			//VisualisationWidget management
			virtual OpenViBE::boolean getNextVisualisationWidgetIdentifier(
				OpenViBE::CIdentifier& rIdentifier) const=0;
			virtual OpenViBE::boolean getNextVisualisationWidgetIdentifier(
				OpenViBE::CIdentifier& rIdentifier,
				OpenViBE::Kernel::EVisualisationWidgetType oType) const=0;
			virtual OpenViBE::boolean	isVisualisationWidget(
				const OpenViBE::CIdentifier& rIdentifier) const=0;
			virtual OpenViBE::Kernel::IVisualisationWidget* getVisualisationWidget(
				const OpenViBE::CIdentifier& rIdentifier) const=0;
			virtual OpenViBE::Kernel::IVisualisationWidget* getVisualisationWidgetFromBoxIdentifier(
				const OpenViBE::CIdentifier& rBoxIdentifier) const=0;
			virtual OpenViBE::boolean	addVisualisationWidget(
				OpenViBE::CIdentifier& rIdentifier,
				const OpenViBE::CString& rName,
				OpenViBE::Kernel::EVisualisationWidgetType oType,
				const OpenViBE::CIdentifier& rParentIdentifier,
				OpenViBE::uint32 ui32ParentIndex,
				const OpenViBE::CIdentifier& rBoxIdentifier,
				OpenViBE::uint32 ui32NbChildren)=0;
			virtual OpenViBE::boolean	getVisualisationWidgetIndex(
				const OpenViBE::CIdentifier& rIdentifier,
				OpenViBE::uint32& ui32Index) const=0;
			virtual OpenViBE::boolean	unparentVisualisationWidget(
				const OpenViBE::CIdentifier& rIdentifier,
				OpenViBE::uint32& ui32Index)=0;
			virtual OpenViBE::boolean	parentVisualisationWidget(
				const OpenViBE::CIdentifier& rIdentifier,
				const OpenViBE::CIdentifier& rParentIdentifier,
				OpenViBE::uint32 ui32Index)=0;
			//if boolean set to false, VisBoxes are unaffected only (as opposed as destroyed)
			virtual OpenViBE::boolean	destroyHierarchy(
				const OpenViBE::CIdentifier& rIdentifier,
				OpenViBE::boolean bDestroyVisualisationBoxes=true)=0;

			//tree view creation/registration
			virtual ::GtkTreeView* createTreeViewWithModel(void)=0;
			virtual void setTreeViewCB(
				OpenViBE::Kernel::ITreeViewCB* pTreeViewCB)=0;

			//called whenever model is changed
			virtual void reloadTree(void)=0;

			//called upon tree reloading (for each visualisation panel)
			//and whenever dialog is resized (for active visualisation panel only)
			virtual void resizeVisualisationPanel(
				::GtkTreeIter* pVisualisationPanelIter)=0;

			//helper functions
			virtual OpenViBE::boolean getTreeSelection(
				::GtkTreeView* pTreeView,
				::GtkTreeIter* pIter)=0;
			virtual GtkTreePath* getTreePath(
				::GtkTreeIter* pTreeIter) const=0;
			virtual unsigned long getULongValueFromTreeIter(
				::GtkTreeIter* pTreeIter,
				OpenViBE::Kernel::EVisualisationTreeColumn eVisualisationTreeColumn) const=0;
			virtual const char* getStringValueFromTreeIter(
				::GtkTreeIter* pTreeIter,
				char** ppString,
				OpenViBE::Kernel::EVisualisationTreeColumn eVisualisationTreeColumn) const=0;
			virtual void*	getPointerValueFromTreeIter(
				::GtkTreeIter* pTreeIter,
				void** pPointer,
				OpenViBE::Kernel::EVisualisationTreeColumn eVisualisationTreeColumn) const=0;
			virtual void getIdentifierFromTreeIter(
				::GtkTreeIter* pTreeIter,
				OpenViBE::CIdentifier& rIdentifier,
				OpenViBE::Kernel::EVisualisationTreeColumn eVisualisationTreeColumn) const=0;

			//find a node by its label and type
			virtual OpenViBE::boolean findChildNodeFromRoot(
				::GtkTreeIter* pIter,
				const char* label,
				OpenViBE::Kernel::EVisualisationTreeNode eType)=0;
			virtual OpenViBE::boolean findChildNodeFromParent(
				::GtkTreeIter* pIter,
				const char* label,
				OpenViBE::Kernel::EVisualisationTreeNode eType)=0;

			//find a node by its pointer to a Gtk widget
			virtual OpenViBE::boolean findChildNodeFromRoot(
				::GtkTreeIter* pIter,
				void* pWidget)=0;
			virtual OpenViBE::boolean findChildNodeFromParent(
				::GtkTreeIter* pIter,
				void* pWidget)=0;

			//find a node by its identifier
			virtual OpenViBE::boolean findChildNodeFromRoot(
				::GtkTreeIter* pIter,
				OpenViBE::CIdentifier oIdentifier)=0;
			virtual OpenViBE::boolean	findChildNodeFromParent(
				::GtkTreeIter* pIter,
				OpenViBE::CIdentifier oIdentifier)=0;

			//find first parent node of a given type
			virtual OpenViBE::boolean	findParentNode(
				::GtkTreeIter* pIter,
				OpenViBE::Kernel::EVisualisationTreeNode eType)=0;

			//drag n drop
			virtual void dragDataReceivedInWidgetCB(
				const OpenViBE::CIdentifier& rSrcIdentifier,
				::GtkWidget* pDstWidget)=0;
			virtual void dragDataReceivedOutsideWidgetCB(
				const OpenViBE::CIdentifier& rSrcIdentifier,
				::GtkWidget* pDstWidget,
				OpenViBE::Kernel::EDragDataLocation eLocation)=0;

			//dynamic widget parenting
			virtual OpenViBE::boolean setWidgets(
				const OpenViBE::CString& rVisualisationBoxName,
				::GtkWidget* pWidget,
				::GtkWidget* pToolbarWidget)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IAttributable, OV_ClassId_Kernel_Visualisation_VisualisationTree)
		};
	};
};

#endif // __OpenViBE_Kernel_Visualisation_IVisualisationTree_H__
