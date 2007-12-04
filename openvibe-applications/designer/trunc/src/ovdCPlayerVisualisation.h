#ifndef __OpenViBEDesigner_CPlayerVisualisation_H__
#define __OpenViBEDesigner_CPlayerVisualisation_H__

#include <string>
#include <vector>
#include <map>
#include <gtk/gtk.h>

//#define SAVE_PANED_POSITIONS

namespace OpenViBEDesigner
{	
	class CPlayerVisualisation : public OpenViBE::Kernel::ITreeViewCB
	{
	public:		
		CPlayerVisualisation(
			const OpenViBE::Kernel::IKernelContext& rKernelContext, 
			const OpenViBE::Kernel::IScenario& rScenario,
			OpenViBE::Kernel::IVisualisationTree& rVisualisationTree);

		virtual ~CPlayerVisualisation();
	
		void init();
		void release();
	
		//ITreeViewCB callbacks overloading
		GtkWidget* loadTreeWidget(
			OpenViBE::Kernel::IVisualisationWidget* pWidget);
		void endLoadTreeWidget(
			GtkWidget* treeWidget);
		OpenViBE::boolean	setWidgets(
			const OpenViBE::CString& rVisualisationBoxName, 
			GtkWidget* pWidget,
			GtkWidget* pToolbarWidget);

		void showTopLevelWindows();
		void hideTopLevelWindows();

		void realize3DWidgets();
				
	protected:										
#ifdef SAVE_PANED_POSITIONS		
		//callback for paned handle position changes
		static gboolean	notify_position_paned_cb(
			GtkWidget* widget, 
			GParamSpec* spec, 
			void* user_data);
		virtual void enablePanedSignals(
			GtkWidget* pPaned, 
			OpenViBE::boolean b);		
		void notifyPositionPanedCB(
			GtkWidget* widget);

		static gboolean window_state_event_cb(
			GtkWidget* widget,
			GdkEventWindowState* event,
      gpointer user_data);
		static void window_size_allocate_cb(
			GtkWidget* widget,
      GtkAllocation* allocation,
      gpointer user_data);		
#endif

		static gboolean configure_event_cb(
			GtkWidget* widget, 
			GdkEventConfigure* event,
			gpointer user_data);
		static gboolean widget_expose_event_cb(
			GtkWidget* widget,
			GdkEventExpose* event,
			gpointer user_data);			
		void resizeCB(
			GtkContainer* container);		
		
		//callbacks for DND
		static void	drag_data_get_from_widget_cb(
			GtkWidget* pSrcWidget, 
			GdkDragContext* pDC, 
			GtkSelectionData* pSelectionData,
			guint uiInfo,
			guint uiTime,
			gpointer pData);
		static void	drag_data_received_in_widget_cb(
			GtkWidget* pDstWidget, 
			GdkDragContext*,
			gint,
			gint,
			GtkSelectionData* pSelectionData,
			guint,
			guint,
			gpointer pData);

		//callback for toolbar
		static void button_clicked_cb(
			GtkButton* pButton, 
			gpointer user_data);
		void showToolbarCB(GtkWidget* pButton);

		const OpenViBE::Kernel::IKernelContext&	m_rKernelContext;
		const OpenViBE::Kernel::IScenario& m_rScenario;
		OpenViBE::Kernel::IVisualisationTree& m_rVisualisationTree;				
		std::map<OpenViBE::CIdentifier, GtkWindow*> m_mVisualisationWindow;
		std::map<GtkWidget*, OpenViBE::CIdentifier> m_mVisualisationWidget;
		std::map<GtkWidget*, GtkWidget*> m_mVisualisationWidgetToolbar;
		GtkWidget* m_pCurrentToolbar;
	};
};

#endif // __OpenViBEDesigner_CPlayerVisualisation_H__
