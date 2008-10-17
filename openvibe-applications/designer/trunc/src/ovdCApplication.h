#ifndef __OpenViBEDesigner_CApplication_H__
#define __OpenViBEDesigner_CApplication_H__

#include "ovd_base.h"

#include "ovdCInterfacedObject.h"

#include <map>
#include <string>

namespace OpenViBEDesigner
{
	class CApplication
	{
	public:
		CApplication(
			OpenViBE::Kernel::IKernel* pKernel);

		void initialize(void);

		/** \name Drag and drop management */
		//@{

		void dragDataGetCB(
			::GtkWidget* pWidget,
			::GdkDragContext* pDragContex,
			::GtkSelectionData* pSelectionData,
			guint uiInfo,
			guint uiT);

		//@}

		/** \name Selection management */
		//@{

		void copySelectionCB(void);

		void cutSelectionCB(void);

		void pasteSelectionCB(void);

		void deleteSelectionCB(void);

		//@}

		/** \name Scenario management */
		//@{

		OpenViBE::boolean hasScenarioRunning(void);

		OpenViBEDesigner::CInterfacedScenario* getCurrentInterfacedScenario(void);

		void newScenarioCB(void);

		void openScenarioCB(void);

		void saveScenarioCB(void);

		void saveScenarioAsCB(void);

		void closeScenarioCB(void);

		void stopScenarioCB(void);

		void pauseScenarioCB(void);

		void nextScenarioCB(void);

		void playScenarioCB(void);

		void forwardScenarioCB(void);

		void changeCurrentScenario(
			OpenViBE::int32 i32PageIndex);

		//@}

		/** \name Designer visualisation management */
		//@{

		void deleteDesignerVisualisationCB();

		void toggleDesignerVisualisationCB();

		//@}

		/** \name Player management */
		//@{

		OpenViBE::Kernel::IPlayer* getPlayer(void);

		void createPlayer(void);

		void releasePlayer(void);

		//@}

		/** \name Log management */
		//@{

		void logLevelCB(void);

		//@}

		/** \name CPU usage */
		//@{

		void CPUUsageCB(void);

		//@}

		/** \name Gtk callbacks */
		//@{

		static void drag_data_get_cb(::GtkWidget* pWidget, ::GdkDragContext* pDragContex, ::GtkSelectionData* pSelectionData, guint uiInfo, guint uiT, gpointer pUserData);
	#if 0
		static gboolean resource_query_tooltip_cb(::GtkWidget* pWidget, gint iX, gint iY, gboolean bKeyboardMode, ::GtkTooltip* pTooltip, gpointer pUserData);
	#endif
		static void menu_copy_selection_cb(::GtkMenuItem* pMenuItem, gpointer pUserData);
		static void menu_cut_selection_cb(::GtkMenuItem* pMenuItem, gpointer pUserData);
		static void menu_paste_selection_cb(::GtkMenuItem* pMenuItem, gpointer pUserData);
		static void menu_delete_selection_cb(::GtkMenuItem* pMenuItem, gpointer pUserData);
		static void menu_new_scenario_cb(::GtkMenuItem* pMenuItem, gpointer pUserData);
		static void menu_open_scenario_cb(::GtkMenuItem* pMenuItem, gpointer pUserData);
		static void menu_save_scenario_cb(::GtkMenuItem* pMenuItem, gpointer pUserData);
		static void menu_save_scenario_as_cb(::GtkMenuItem* pMenuItem, gpointer pUserData);
		static void menu_close_scenario_cb(::GtkMenuItem* pMenuItem, gpointer pUserData);
		static void button_new_scenario_cb(::GtkButton* pButton, gpointer pUserData);
		static void button_open_scenario_cb(::GtkButton* pButton, gpointer pUserData);
		static void button_save_scenario_cb(::GtkButton* pButton, gpointer pUserData);
		static void button_save_scenario_as_cb(::GtkButton* pButton, gpointer pUserData);
		static void button_close_scenario_cb(::GtkButton* pButton, gpointer pUserData);
		static void delete_designer_visualisation_cb(gpointer user_data);
		static void button_toggle_window_manager_cb(::GtkToggleToolButton* pButton, gpointer pUserData);
		static void stop_scenario_cb(::GtkButton* pButton, gpointer pUserData);
		static void pause_scenario_cb(::GtkButton* pButton, gpointer pUserData);
		static void next_scenario_cb(::GtkButton* pButton, gpointer pUserData);
		static void play_scenario_cb(::GtkButton* pButton, gpointer pUserData);
		static void forward_scenario_cb(::GtkButton* pButton, gpointer pUserData);
		static void log_level_cb(::GtkButton* pButton, gpointer pUserData);
		static void cpu_usage_cb(::GtkToggleButton* pButton, gpointer pUserData);
		static gboolean change_current_scenario_cb(::GtkNotebook* pNotebook, ::GtkNotebookPage* pNotebookPage, guint uiPageNumber, gpointer pUserData);
		static void box_algorithm_title_button_expand_cb(::GtkButton* pButton, gpointer pUserData);
		static void box_algorithm_title_button_collapse_cb(::GtkButton* pButton, gpointer pUserData);
		static void algorithm_title_button_expand_cb(::GtkButton* pButton, gpointer pUserData);
		static void algorithm_title_button_collapse_cb(::GtkButton* pButton, gpointer pUserData);
		static gboolean idle_application_loop(gpointer pUserData);
		static gboolean idle_scenario_loop(gpointer pUserData);

		//@}

	public:
		OpenViBE::Kernel::IKernel* m_pKernel;
		OpenViBE::Kernel::IPluginManager* m_pPluginManager;
		OpenViBE::Kernel::IScenarioManager* m_pScenarioManager;
		OpenViBE::Kernel::IScenario* m_pClipboardScenario;
		OpenViBE::Kernel::IVisualisationManager* m_pVisualisationManager;

		::GladeXML* m_pGladeInterface;
		::GtkWidget* m_pMainWindow;
		::GtkNotebook* m_pScenarioNotebook;
		::GtkNotebook* m_pResourceNotebook;
		::GtkTreeStore* m_pBoxAlgorithmTreeModel;
		::GtkTreeView* m_pBoxAlgorithmTreeView;
		::GtkTreeStore* m_pAlgorithmTreeModel;
		::GtkTreeView* m_pAlgorithmTreeView;

		std::vector < OpenViBEDesigner::CInterfacedScenario* > m_vInterfacedScenario;
	};
};

#endif // __OpenViBEDesigner_CApplication_H__
