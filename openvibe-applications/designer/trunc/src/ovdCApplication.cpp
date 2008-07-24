#include "ovd_base.h"

#include <system/Time.h>

#include <stack>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>

#define OVD_GUI_File "../share/openvibe-applications/designer/interface.glade"

#include "ovdCDesignerVisualisation.h"
#include "ovdCPlayerVisualisation.h"
#include "ovdCInterfacedObject.h"
#include "ovdCInterfacedScenario.h"
#include "ovdCApplication.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
using namespace OpenViBEDesigner;
using namespace std;

static ::GtkTargetEntry g_vTargetEntry[]= {
	{ "STRING", 0, 0 },
	{ "text/plain", 0, 0 } };

CApplication::CApplication(IKernel* pKernel)
	:m_pKernel(pKernel)
	,m_pPluginManager(NULL)
	,m_pScenarioManager(NULL)
	,m_pClipboardScenario(NULL)
	,m_pVisualisationManager(NULL)
	,m_pGladeInterface(NULL)
	,m_pMainWindow(NULL)
	,m_pScenarioNotebook(NULL)
	,m_pResourceNotebook(NULL)
	,m_pBoxAlgorithmTreeModel(NULL)
	,m_pBoxAlgorithmTreeView(NULL)
	,m_pAlgorithmTreeModel(NULL)
	,m_pAlgorithmTreeView(NULL)
{
	m_pPluginManager=&m_pKernel->getContext()->getPluginManager();
	m_pScenarioManager=&m_pKernel->getContext()->getScenarioManager();
	m_pVisualisationManager=&m_pKernel->getContext()->getVisualisationManager();
}

void CApplication::initialize(void)
{
	// Prepares scenario clipboard
	CIdentifier l_oClipboardScenarioIdentifier;
	if(m_pScenarioManager->createScenario(l_oClipboardScenarioIdentifier))
	{
		m_pClipboardScenario=&m_pScenarioManager->getScenario(l_oClipboardScenarioIdentifier);
	}

	// Loads main interface
	m_pGladeInterface=glade_xml_new(OVD_GUI_File, "openvibe", NULL);
	m_pMainWindow=glade_xml_get_widget(m_pGladeInterface, "openvibe");

	// Connects menu actions
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "menu_copy")),        "activate", G_CALLBACK(menu_copy_selection_cb),     this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "menu_cut")),         "activate", G_CALLBACK(menu_cut_selection_cb),      this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "menu_paste")),       "activate", G_CALLBACK(menu_paste_selection_cb),    this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "menu_delete")),      "activate", G_CALLBACK(menu_delete_selection_cb),   this);

	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "menu_new")),         "activate", G_CALLBACK(menu_new_scenario_cb),       this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "menu_open")),        "activate", G_CALLBACK(menu_open_scenario_cb),      this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "menu_save")),        "activate", G_CALLBACK(menu_save_scenario_cb),      this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "menu_save_as")),     "activate", G_CALLBACK(menu_save_scenario_as_cb),   this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "menu_close")),       "activate", G_CALLBACK(menu_close_scenario_cb),     this);

	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "button_new")),       "clicked",  G_CALLBACK(button_new_scenario_cb),     this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "button_open")),      "clicked",  G_CALLBACK(button_open_scenario_cb),    this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "button_save")),      "clicked",  G_CALLBACK(button_save_scenario_cb),    this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "button_save_as")),   "clicked",  G_CALLBACK(button_save_scenario_as_cb), this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "button_close")),     "clicked",  G_CALLBACK(button_close_scenario_cb),   this);

	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "button_windowmanager")),   "toggled",  G_CALLBACK(button_toggle_window_manager_cb), this);

	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "button_stop")),      "clicked",  G_CALLBACK(stop_scenario_cb),           this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "button_pause")),     "clicked",  G_CALLBACK(pause_scenario_cb),          this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "button_next")),      "clicked",  G_CALLBACK(next_scenario_cb),           this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "button_play")),      "clicked",  G_CALLBACK(play_scenario_cb),           this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "button_forward")),   "clicked",  G_CALLBACK(forward_scenario_cb),        this);

	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "button_log_level")), "clicked",  G_CALLBACK(log_level_cb),               this);

	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "box_algorithm_title_button_expand")),   "clicked", G_CALLBACK(box_algorithm_title_button_expand_cb),   this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "box_algorithm_title_button_collapse")), "clicked", G_CALLBACK(box_algorithm_title_button_collapse_cb), this);

	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "algorithm_title_button_expand")),   "clicked", G_CALLBACK(algorithm_title_button_expand_cb),   this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "algorithm_title_button_collapse")), "clicked", G_CALLBACK(algorithm_title_button_collapse_cb), this);

	g_idle_add(idle_application_loop, this);

	// Prepares main notebooks
	m_pScenarioNotebook=GTK_NOTEBOOK(glade_xml_get_widget(m_pGladeInterface, "scenario_notebook"));
	g_signal_connect(G_OBJECT(m_pScenarioNotebook), "switch-page", G_CALLBACK(change_current_scenario_cb), this);
	m_pResourceNotebook=GTK_NOTEBOOK(glade_xml_get_widget(m_pGladeInterface, "resource_notebook"));

	// Creates an empty scnenario
	gtk_notebook_remove_page(m_pScenarioNotebook, 0);
	//newScenarioCB();
	{
		// Prepares box algorithm view
		m_pBoxAlgorithmTreeView=GTK_TREE_VIEW(glade_xml_get_widget(m_pGladeInterface, "box_algorithm_tree"));
		::GtkTreeViewColumn* l_pTreeViewColumnName=gtk_tree_view_column_new();
		::GtkTreeViewColumn* l_pTreeViewColumnDesc=gtk_tree_view_column_new();
		::GtkCellRenderer* l_pCellRendererIcon=gtk_cell_renderer_pixbuf_new();
		::GtkCellRenderer* l_pCellRendererName=gtk_cell_renderer_text_new();
		::GtkCellRenderer* l_pCellRendererDesc=gtk_cell_renderer_text_new();
		gtk_tree_view_column_set_title(l_pTreeViewColumnName, "Name");
		gtk_tree_view_column_set_title(l_pTreeViewColumnDesc, "Description");
		gtk_tree_view_column_pack_start(l_pTreeViewColumnName, l_pCellRendererIcon, FALSE);
		gtk_tree_view_column_pack_start(l_pTreeViewColumnName, l_pCellRendererName, TRUE);
		gtk_tree_view_column_pack_start(l_pTreeViewColumnDesc, l_pCellRendererDesc, TRUE);
		gtk_tree_view_column_set_attributes(l_pTreeViewColumnName, l_pCellRendererIcon, "stock-id", Resource_StringStockIcon, NULL);
		gtk_tree_view_column_set_attributes(l_pTreeViewColumnName, l_pCellRendererName, "text", Resource_StringName, NULL);
		gtk_tree_view_column_set_attributes(l_pTreeViewColumnDesc, l_pCellRendererDesc, "text", Resource_StringShortDescription, NULL);
		gtk_tree_view_column_set_sizing(l_pTreeViewColumnName, GTK_TREE_VIEW_COLUMN_FIXED);
		gtk_tree_view_column_set_sizing(l_pTreeViewColumnDesc, GTK_TREE_VIEW_COLUMN_FIXED);
		gtk_tree_view_column_set_expand(l_pTreeViewColumnName, FALSE);
		gtk_tree_view_column_set_expand(l_pTreeViewColumnDesc, FALSE);
		gtk_tree_view_column_set_resizable(l_pTreeViewColumnName, TRUE);
		gtk_tree_view_column_set_resizable(l_pTreeViewColumnDesc, TRUE);
		gtk_tree_view_column_set_min_width(l_pTreeViewColumnName, 64);
		gtk_tree_view_column_set_min_width(l_pTreeViewColumnDesc, 64);
		gtk_tree_view_column_set_fixed_width(l_pTreeViewColumnName, 256);
		gtk_tree_view_column_set_fixed_width(l_pTreeViewColumnDesc, 512);
		gtk_tree_view_append_column(m_pBoxAlgorithmTreeView, l_pTreeViewColumnName);
		gtk_tree_view_append_column(m_pBoxAlgorithmTreeView, l_pTreeViewColumnDesc);
		// g_signal_connect(G_OBJECT(m_pBoxAlgorithmTreeView), "querry_tooltip", G_CALLBACK(resource_query_tooltip_cb), this);
		//
		// Prepares box algorithm model
		m_pBoxAlgorithmTreeModel=gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN);
		gtk_tree_view_set_model(m_pBoxAlgorithmTreeView, GTK_TREE_MODEL(m_pBoxAlgorithmTreeModel));
	}

	{
		// Prepares algorithm view
		m_pAlgorithmTreeView=GTK_TREE_VIEW(glade_xml_get_widget(m_pGladeInterface, "algorithm_tree"));
		::GtkTreeViewColumn* l_pTreeViewColumnName=gtk_tree_view_column_new();
		::GtkTreeViewColumn* l_pTreeViewColumnDesc=gtk_tree_view_column_new();
		::GtkCellRenderer* l_pCellRendererIcon=gtk_cell_renderer_pixbuf_new();
		::GtkCellRenderer* l_pCellRendererName=gtk_cell_renderer_text_new();
		::GtkCellRenderer* l_pCellRendererDesc=gtk_cell_renderer_text_new();
		gtk_tree_view_column_set_title(l_pTreeViewColumnName, "Name");
		gtk_tree_view_column_set_title(l_pTreeViewColumnDesc, "Description");
		gtk_tree_view_column_pack_start(l_pTreeViewColumnName, l_pCellRendererIcon, FALSE);
		gtk_tree_view_column_pack_start(l_pTreeViewColumnName, l_pCellRendererName, TRUE);
		gtk_tree_view_column_pack_start(l_pTreeViewColumnDesc, l_pCellRendererDesc, TRUE);
		gtk_tree_view_column_set_attributes(l_pTreeViewColumnName, l_pCellRendererIcon, "stock-id", Resource_StringStockIcon, NULL);
		gtk_tree_view_column_set_attributes(l_pTreeViewColumnName, l_pCellRendererName, "text", Resource_StringName, NULL);
		gtk_tree_view_column_set_attributes(l_pTreeViewColumnDesc, l_pCellRendererDesc, "text", Resource_StringShortDescription, NULL);
		gtk_tree_view_column_set_sizing(l_pTreeViewColumnName, GTK_TREE_VIEW_COLUMN_FIXED);
		gtk_tree_view_column_set_sizing(l_pTreeViewColumnDesc, GTK_TREE_VIEW_COLUMN_FIXED);
		gtk_tree_view_column_set_expand(l_pTreeViewColumnName, FALSE);
		gtk_tree_view_column_set_expand(l_pTreeViewColumnDesc, FALSE);
		gtk_tree_view_column_set_resizable(l_pTreeViewColumnName, TRUE);
		gtk_tree_view_column_set_resizable(l_pTreeViewColumnDesc, TRUE);
		gtk_tree_view_column_set_min_width(l_pTreeViewColumnName, 64);
		gtk_tree_view_column_set_min_width(l_pTreeViewColumnDesc, 64);
		gtk_tree_view_column_set_fixed_width(l_pTreeViewColumnName, 256);
		gtk_tree_view_column_set_fixed_width(l_pTreeViewColumnDesc, 512);
		gtk_tree_view_append_column(m_pAlgorithmTreeView, l_pTreeViewColumnName);
		gtk_tree_view_append_column(m_pAlgorithmTreeView, l_pTreeViewColumnDesc);
		// g_signal_connect(G_OBJECT(m_pAlgorithmTreeView), "querry_tooltip", G_CALLBACK(resource_query_tooltip_cb), this);

		// Prepares algorithm model
		m_pAlgorithmTreeModel=gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN);
		gtk_tree_view_set_model(m_pAlgorithmTreeView, GTK_TREE_MODEL(m_pAlgorithmTreeModel));
	}

	// Prepares drag & drop for box creation
	gtk_drag_source_set(GTK_WIDGET(m_pBoxAlgorithmTreeView), GDK_BUTTON1_MASK, g_vTargetEntry, sizeof(g_vTargetEntry)/sizeof(::GtkTargetEntry), GDK_ACTION_COPY);
	g_signal_connect(
		G_OBJECT(m_pBoxAlgorithmTreeView),
		"drag_data_get",
		G_CALLBACK(drag_data_get_cb),
		this);

	// Shows main window
	glade_xml_signal_autoconnect(m_pGladeInterface);
	gtk_window_maximize(GTK_WINDOW(m_pMainWindow));
	gtk_widget_show(m_pMainWindow);
	// gtk_window_set_icon_name(GTK_WINDOW(m_pMainWindow), "ov-logo");
	// gtk_window_set_icon_from_file(GTK_WINDOW(m_pMainWindow), "../share/openvibe-applications/designer/ov-logo.png", NULL);
}

boolean CApplication::hasScenarioRunning(void)
{
	vector<CInterfacedScenario*>::const_iterator itInterfacedScenario;
	for(itInterfacedScenario=m_vInterfacedScenario.begin(); itInterfacedScenario!=m_vInterfacedScenario.end(); itInterfacedScenario++)
	{
		if((*itInterfacedScenario)->m_pPlayer)
		{
			return true;
		}
	}
	return false;
}

CInterfacedScenario* CApplication::getCurrentInterfacedScenario(void)
{
	uint32 l_ui32Index=(uint32)gtk_notebook_get_current_page(m_pScenarioNotebook);
	if(l_ui32Index<m_vInterfacedScenario.size())
	{
		return m_vInterfacedScenario[l_ui32Index];
	}
	return NULL;
}

void CApplication::dragDataGetCB(::GtkWidget* pWidget, ::GdkDragContext* pDragContex, ::GtkSelectionData* pSelectionData, guint uiInfo, guint uiT)
{
	m_pKernel->getContext()->getLogManager() << LogLevel_Trace << "dragDataGetCB\n";

	::GtkTreeView* l_pTreeView=GTK_TREE_VIEW(glade_xml_get_widget(m_pGladeInterface, "box_algorithm_tree"));
	::GtkTreeSelection* l_pTreeSelection=gtk_tree_view_get_selection(l_pTreeView);
	::GtkTreeModel* l_pTreeModel=NULL;
	::GtkTreeIter l_oTreeIter;
	if(gtk_tree_selection_get_selected(l_pTreeSelection, &l_pTreeModel, &l_oTreeIter))
	{
		const char* l_sBoxAlgorithmIdentifier=NULL;
		gtk_tree_model_get(
			l_pTreeModel, &l_oTreeIter,
			Resource_StringIdentifier, &l_sBoxAlgorithmIdentifier,
			-1);
		if(l_sBoxAlgorithmIdentifier)
		{
			gtk_selection_data_set(
				pSelectionData,
				GDK_SELECTION_TYPE_STRING,
				8,
				(const guchar*)l_sBoxAlgorithmIdentifier,
				strlen(l_sBoxAlgorithmIdentifier)+1);
		}
	}
}

void CApplication::copySelectionCB(void)
{
	m_pKernel->getContext()->getLogManager() << LogLevel_Trace << "copySelectionCB\n";

	IScenario& l_rCurrentScenario=getCurrentInterfacedScenario()->m_rScenario;
	map<CIdentifier, boolean>& l_vCurrentObject=getCurrentInterfacedScenario()->m_vCurrentObject;
	map<CIdentifier, boolean>::iterator o;
	map<CIdentifier, CIdentifier> l_vBoxBox;

	// Clears clipboard scenario
	m_pClipboardScenario->clear();

	// adds boxes
	for(o=l_vCurrentObject.begin(); o!=l_vCurrentObject.end(); o++)
	{
		if(o->second)
		{
			if(l_rCurrentScenario.isBox(o->first))
			{
				CIdentifier l_oClipboardBoxIdentifier;
				IBox& l_rBox=*l_rCurrentScenario.getBoxDetails(o->first);
				m_pClipboardScenario->addBox(l_rBox, l_oClipboardBoxIdentifier);
				l_vBoxBox[o->first]=l_oClipboardBoxIdentifier;
			}
		}
	}

	// adds links
	for(o=l_vCurrentObject.begin(); o!=l_vCurrentObject.end(); o++)
	{
		if(o->second)
		{
			if(l_rCurrentScenario.isLink(o->first))
			{
				CIdentifier l_oClipboardLinkIdentifier;
				ILink& l_rLink=*l_rCurrentScenario.getLinkDetails(o->first);
				m_pClipboardScenario->connect(
					l_vBoxBox[l_rLink.getSourceBoxIdentifier()],
					l_rLink.getSourceBoxOutputIndex(),
					l_vBoxBox[l_rLink.getTargetBoxIdentifier()],
					l_rLink.getTargetBoxInputIndex(),
					l_oClipboardLinkIdentifier);
			}
		}
	}
}

void CApplication::cutSelectionCB(void)
{
	m_pKernel->getContext()->getLogManager() << LogLevel_Trace << "cutSelectionCB\n";
}

void CApplication::pasteSelectionCB(void)
{
	m_pKernel->getContext()->getLogManager() << LogLevel_Trace << "pasteSelectionCB\n";

	IScenario& l_rCurrentScenario=getCurrentInterfacedScenario()->m_rScenario;

	l_rCurrentScenario.merge(*m_pClipboardScenario);
}

void CApplication::deleteSelectionCB(void)
{
	m_pKernel->getContext()->getLogManager() << LogLevel_Trace << "deleteSelectionCB\n";
}

void CApplication::newScenarioCB(void)
{
	m_pKernel->getContext()->getLogManager() << LogLevel_Trace << "newScenarioCB\n";

	CIdentifier l_oScenarioIdentifier;
	if(m_pScenarioManager->createScenario(l_oScenarioIdentifier))
	{
		IScenario& l_rScenario=m_pScenarioManager->getScenario(l_oScenarioIdentifier);
		CInterfacedScenario* l_pInterfacedScenario=new CInterfacedScenario(*m_pKernel, l_rScenario, l_oScenarioIdentifier, *m_pScenarioNotebook, OVD_GUI_File);
		if(l_pInterfacedScenario->m_pDesignerVisualisation != NULL)
		{
			l_pInterfacedScenario->m_pDesignerVisualisation->setDeleteEventCB(&CApplication::delete_designer_visualisation_cb, this);
			l_pInterfacedScenario->m_pDesignerVisualisation->newVisualisationWindow("Default window");
		}
		l_pInterfacedScenario->updateScenarioLabel();
		m_vInterfacedScenario.push_back(l_pInterfacedScenario);
		gtk_notebook_set_current_page(m_pScenarioNotebook, gtk_notebook_get_n_pages(m_pScenarioNotebook)-1);
		//this->changeCurrentScenario(gtk_notebook_get_n_pages(m_pScenarioNotebook)-1);
	}
}

void CApplication::openScenarioCB(void)
{
	m_pKernel->getContext()->getLogManager() << LogLevel_Trace << "openScenarioCB\n";

	::GtkWidget* l_pWidgetDialogOpen=gtk_file_chooser_dialog_new(
		"Select scenario to open...",
		NULL,
		GTK_FILE_CHOOSER_ACTION_OPEN,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
		NULL);
	if(gtk_dialog_run(GTK_DIALOG(l_pWidgetDialogOpen))==GTK_RESPONSE_ACCEPT)
	{
		char* l_sFileName=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(l_pWidgetDialogOpen));

		CIdentifier l_oScenarioIdentifier;
		if(m_pScenarioManager->createScenario(l_oScenarioIdentifier))
		{
			IScenario& l_rScenario=m_pScenarioManager->getScenario(l_oScenarioIdentifier);
			if(l_rScenario.load(l_sFileName, CIdentifier(0x440BF3AC, 0x2D960300))) // $$$
			{
				// Closes first unnamed scenario
				if(m_vInterfacedScenario.size()==1)
				{
					if(m_vInterfacedScenario[0]->m_bHasBeenModified==false && !m_vInterfacedScenario[0]->m_bHasFileName)
					{
						CIdentifier l_oScenarioIdentifier=m_vInterfacedScenario[0]->m_oScenarioIdentifier;
						delete m_vInterfacedScenario[0];
						m_pScenarioManager->releaseScenario(l_oScenarioIdentifier);
						m_vInterfacedScenario.clear();
					}
				}

				// Creates interfaced scenario
				CInterfacedScenario* l_pInterfacedScenario=new CInterfacedScenario(*m_pKernel, l_rScenario, l_oScenarioIdentifier, *m_pScenarioNotebook, OVD_GUI_File);
				if(l_pInterfacedScenario->m_pDesignerVisualisation != NULL)
				{
					l_pInterfacedScenario->m_pDesignerVisualisation->setDeleteEventCB(&CApplication::delete_designer_visualisation_cb, this);
					l_pInterfacedScenario->m_pDesignerVisualisation->reset();
				}
				l_pInterfacedScenario->m_sFileName=l_sFileName;
				l_pInterfacedScenario->m_bHasFileName=true;
				l_pInterfacedScenario->m_bHasBeenModified=false;
				l_pInterfacedScenario->updateScenarioLabel();
				m_vInterfacedScenario.push_back(l_pInterfacedScenario);
				gtk_notebook_set_current_page(m_pScenarioNotebook, gtk_notebook_get_n_pages(m_pScenarioNotebook)-1);
				//this->changeCurrentScenario(gtk_notebook_get_n_pages(m_pScenarioNotebook)-1);
			}
			else
			{
				m_pScenarioManager->releaseScenario(l_oScenarioIdentifier);

				::GtkWidget* l_pErrorDialog=gtk_message_dialog_new(
					NULL,
					GTK_DIALOG_MODAL,
					GTK_MESSAGE_WARNING,
					GTK_BUTTONS_OK,
					"Scenario importation process failed !");
				gtk_message_dialog_format_secondary_text(
					GTK_MESSAGE_DIALOG(l_pErrorDialog),
					"The requested file may either not be an OpenViBE "
					"scenario file, be corrupted or not be compatible with "
					"the selected scenario importer...");
				gtk_dialog_run(GTK_DIALOG(l_pErrorDialog));
				gtk_widget_destroy(l_pErrorDialog);
			}
		}

		g_free(l_sFileName);
	}
	gtk_widget_destroy(l_pWidgetDialogOpen);
}

void CApplication::saveScenarioCB(void)
{
	m_pKernel->getContext()->getLogManager() << LogLevel_Trace << "saveScenarioCB\n";

	CInterfacedScenario* l_pCurrentInterfacedScenario=getCurrentInterfacedScenario();
	if(!l_pCurrentInterfacedScenario)
	{
		return;
	}
	if(!l_pCurrentInterfacedScenario->m_bHasFileName)
	{
		saveScenarioAsCB();
	}
	else
	{
		l_pCurrentInterfacedScenario->m_rScenario.save(l_pCurrentInterfacedScenario->m_sFileName.c_str(), CIdentifier(0x77075b3b, 0x3d632492));
		l_pCurrentInterfacedScenario->m_bHasFileName=true;
		l_pCurrentInterfacedScenario->m_bHasBeenModified=false;
		l_pCurrentInterfacedScenario->updateScenarioLabel();
	}
}

void CApplication::saveScenarioAsCB(void)
{
	m_pKernel->getContext()->getLogManager() << LogLevel_Trace << "saveScenarioAsCB\n";

	CInterfacedScenario* l_pCurrentInterfacedScenario=getCurrentInterfacedScenario();
	if(!l_pCurrentInterfacedScenario)
	{
		return;
	}
	::GtkWidget* l_pWidgetDialogSaveAs=gtk_file_chooser_dialog_new(
		"Select scenario to save...",
		NULL,
		GTK_FILE_CHOOSER_ACTION_SAVE,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
		NULL);
	// gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(l_pWidgetDialogSaveAs), true);
	if(l_pCurrentInterfacedScenario->m_bHasFileName)
	{
		gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(l_pWidgetDialogSaveAs), l_pCurrentInterfacedScenario->m_sFileName.c_str());
	}
	if(gtk_dialog_run(GTK_DIALOG(l_pWidgetDialogSaveAs))==GTK_RESPONSE_ACCEPT)
	{
		char* l_sFileName=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(l_pWidgetDialogSaveAs));

		l_pCurrentInterfacedScenario->m_sFileName=l_sFileName;
		l_pCurrentInterfacedScenario->m_bHasFileName=true;
		l_pCurrentInterfacedScenario->m_bHasBeenModified=false;
		saveScenarioCB();

		g_free(l_sFileName);
	}
	gtk_widget_destroy(l_pWidgetDialogSaveAs);
}

void CApplication::closeScenarioCB(void)
{
	m_pKernel->getContext()->getLogManager() << LogLevel_Trace << "closeScenarioCB\n";

	CInterfacedScenario* l_pCurrentInterfacedScenario=getCurrentInterfacedScenario();
	if(!l_pCurrentInterfacedScenario)
	{
		return;
	}
	if(l_pCurrentInterfacedScenario->isLocked())
	{
		::GtkWidget* l_pDialog=gtk_message_dialog_new(
			GTK_WINDOW(m_pMainWindow),
			GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_WARNING,
			GTK_BUTTONS_OK,
			"Scenario is locked !");
		gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(l_pDialog),
			"The scenario you are trying to close is locked. "
			"It is most probably beeing used by a player engine "
			"and started or paused. If you really want to close "
			"it, you'll have to stop its execution before.");
		gtk_window_set_title(GTK_WINDOW(l_pDialog), "Warning");
		gtk_dialog_run(GTK_DIALOG(l_pDialog));
		gtk_widget_destroy(l_pDialog);
		return;
	}

	vector<CInterfacedScenario*>::iterator i=m_vInterfacedScenario.begin();
	while(i!=m_vInterfacedScenario.end() && *i!=l_pCurrentInterfacedScenario) i++;
	if(i!=m_vInterfacedScenario.end())
	{
		CIdentifier l_oScenarioIdentifier=l_pCurrentInterfacedScenario->m_oScenarioIdentifier;
		delete l_pCurrentInterfacedScenario;
		m_pScenarioManager->releaseScenario(l_oScenarioIdentifier);
		m_vInterfacedScenario.erase(i);
		//when closing last open scenario, no "switch-page" event is triggered so we manually handle this case
		if(m_vInterfacedScenario.empty() == true)
		{
			changeCurrentScenario(-1);
		}
	}
}

void CApplication::deleteDesignerVisualisationCB()
{
	//untoggle visualisation button
	gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(m_pGladeInterface, "button_windowmanager")), FALSE);
}

void CApplication::toggleDesignerVisualisationCB()
{
	CInterfacedScenario* l_pCurrentInterfacedScenario = getCurrentInterfacedScenario();
	if(l_pCurrentInterfacedScenario != NULL && l_pCurrentInterfacedScenario->isLocked() == false)
	{
		uint32 l_ui32Index=(uint32)gtk_notebook_get_current_page(m_pScenarioNotebook);
		if(l_ui32Index<m_vInterfacedScenario.size())
		{
			m_vInterfacedScenario[l_ui32Index]->toggleDesignerVisualisation();
		}
	}
}

IPlayer* CApplication::getPlayer(void)
{
	CInterfacedScenario* l_pCurrentInterfacedScenario=getCurrentInterfacedScenario();
	return (l_pCurrentInterfacedScenario?l_pCurrentInterfacedScenario->m_pPlayer:NULL);
}

void CApplication::createPlayer(void)
{
	m_pKernel->getContext()->getLogManager() << LogLevel_Trace << "createPlayer\n";

	CInterfacedScenario* l_pCurrentInterfacedScenario=getCurrentInterfacedScenario();
	if(l_pCurrentInterfacedScenario && !l_pCurrentInterfacedScenario->m_pPlayer)
	{
		// generate player windows
		l_pCurrentInterfacedScenario->createPlayerVisualisation();

		m_pKernel->getContext()->getPlayerManager().createPlayer(l_pCurrentInterfacedScenario->m_oPlayerIdentifier);
		CIdentifier l_oScenarioIdentifier=l_pCurrentInterfacedScenario->m_oScenarioIdentifier;
		CIdentifier l_oPlayerIdentifier=l_pCurrentInterfacedScenario->m_oPlayerIdentifier;
		l_pCurrentInterfacedScenario->m_pPlayer=&m_pKernel->getContext()->getPlayerManager().getPlayer(l_oPlayerIdentifier);
		l_pCurrentInterfacedScenario->m_pPlayer->setScenario(l_oScenarioIdentifier);
		l_pCurrentInterfacedScenario->m_ui64LastLoopTime=System::Time::zgetTime();

		//set up idle function
		g_idle_add(idle_scenario_loop, l_pCurrentInterfacedScenario);

		// redraws scenario
		l_pCurrentInterfacedScenario->redraw();
	}
}

void CApplication::releasePlayer(void)
{
	m_pKernel->getContext()->getLogManager() << LogLevel_Trace << "releasePlayer\n";

	CInterfacedScenario* l_pCurrentInterfacedScenario=getCurrentInterfacedScenario();
	if(l_pCurrentInterfacedScenario && l_pCurrentInterfacedScenario->m_pPlayer)
	{
		// removes idle function
		g_idle_remove_by_data(l_pCurrentInterfacedScenario);

		m_pKernel->getContext()->getPlayerManager().releasePlayer(l_pCurrentInterfacedScenario->m_oPlayerIdentifier);
		l_pCurrentInterfacedScenario->m_oPlayerIdentifier=OV_UndefinedIdentifier;
		l_pCurrentInterfacedScenario->m_pPlayer=NULL;

		// destroy player windows
		l_pCurrentInterfacedScenario->releasePlayerVisualisation();

		// redraws scenario
		l_pCurrentInterfacedScenario->redraw();
	}
}

void CApplication::stopScenarioCB(void)
{
	m_pKernel->getContext()->getLogManager() << LogLevel_Trace << "stopScenarioCB\n";

	this->getPlayer()->stop();
	this->releasePlayer();

	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_stop"),    false);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_pause"),   false);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_next"),    true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_play"),    true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_forward"), true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_windowmanager"), true);
}

void CApplication::pauseScenarioCB(void)
{
	m_pKernel->getContext()->getLogManager() << LogLevel_Trace << "pauseScenarioCB\n";

	this->createPlayer();
	this->getPlayer()->pause();

	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_stop"),    true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_pause"),   false);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_next"),    true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_play"),    true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_forward"), true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_windowmanager"), false);
}

void CApplication::nextScenarioCB(void)
{
	m_pKernel->getContext()->getLogManager() << LogLevel_Trace << "nextScenarioCB\n";

	this->createPlayer();
	this->getPlayer()->step();

	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_stop"),    true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_pause"),   false);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_next"),    true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_play"),    true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_forward"), true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_windowmanager"), false);
}

void CApplication::playScenarioCB(void)
{
	m_pKernel->getContext()->getLogManager() << LogLevel_Trace << "playScenarioCB\n";

	this->createPlayer();
	this->getPlayer()->play();

	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_stop"),    true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_pause"),   true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_next"),    true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_play"),    false);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_forward"), true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_windowmanager"), false);
}

void CApplication::forwardScenarioCB(void)
{
	m_pKernel->getContext()->getLogManager() << LogLevel_Trace << "forwardScenarioCB\n";

	this->createPlayer();
	this->getPlayer()->forward();

	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_stop"),    true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_pause"),   true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_next"),    true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_play"),    true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_forward"), false);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_windowmanager"), false);
}

void CApplication::logLevelCB(void)
{
	// Loads log level dialog
	::GladeXML* l_pGladeInterface=glade_xml_new(OVD_GUI_File, "openvibe_loglevel", NULL);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(l_pGladeInterface, "checkbutton_loglevel_fatal")),             m_pKernel->getContext()->getLogManager().isActive(LogLevel_Fatal));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(l_pGladeInterface, "checkbutton_loglevel_error")),             m_pKernel->getContext()->getLogManager().isActive(LogLevel_Error));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(l_pGladeInterface, "checkbutton_loglevel_important_warning")), m_pKernel->getContext()->getLogManager().isActive(LogLevel_ImportantWarning));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(l_pGladeInterface, "checkbutton_loglevel_warning")),           m_pKernel->getContext()->getLogManager().isActive(LogLevel_Warning));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(l_pGladeInterface, "checkbutton_loglevel_info")),              m_pKernel->getContext()->getLogManager().isActive(LogLevel_Info));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(l_pGladeInterface, "checkbutton_loglevel_trace")),             m_pKernel->getContext()->getLogManager().isActive(LogLevel_Trace));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(l_pGladeInterface, "checkbutton_loglevel_benchmark")),         m_pKernel->getContext()->getLogManager().isActive(LogLevel_Benchmark));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(l_pGladeInterface, "checkbutton_loglevel_debug")),             m_pKernel->getContext()->getLogManager().isActive(LogLevel_Debug));

	::GtkDialog* l_pLogLevelDialog=GTK_DIALOG(glade_xml_get_widget(l_pGladeInterface, "openvibe_loglevel"));
	gint l_iResult=gtk_dialog_run(l_pLogLevelDialog);
	if(l_iResult==GTK_RESPONSE_APPLY)
	{
		m_pKernel->getContext()->getLogManager().activate(LogLevel_Fatal,            gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(l_pGladeInterface, "checkbutton_loglevel_fatal")))?true:false);
		m_pKernel->getContext()->getLogManager().activate(LogLevel_Error,            gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(l_pGladeInterface, "checkbutton_loglevel_error")))?true:false);
		m_pKernel->getContext()->getLogManager().activate(LogLevel_ImportantWarning, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(l_pGladeInterface, "checkbutton_loglevel_important_warning")))?true:false);
		m_pKernel->getContext()->getLogManager().activate(LogLevel_Warning,          gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(l_pGladeInterface, "checkbutton_loglevel_warning")))?true:false);
		m_pKernel->getContext()->getLogManager().activate(LogLevel_Info,             gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(l_pGladeInterface, "checkbutton_loglevel_info")))?true:false);
		m_pKernel->getContext()->getLogManager().activate(LogLevel_Trace,            gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(l_pGladeInterface, "checkbutton_loglevel_trace")))?true:false);
		m_pKernel->getContext()->getLogManager().activate(LogLevel_Benchmark,        gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(l_pGladeInterface, "checkbutton_loglevel_benchmark")))?true:false);
		m_pKernel->getContext()->getLogManager().activate(LogLevel_Debug,            gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(l_pGladeInterface, "checkbutton_loglevel_debug")))?true:false);
	}

	gtk_widget_destroy(GTK_WIDGET(l_pLogLevelDialog));
	g_object_unref(l_pGladeInterface);
}

void CApplication::changeCurrentScenario(int32 i32PageIndex)
{
	//hide window manager of previously active scenario, if any
	int i = gtk_notebook_get_current_page(m_pScenarioNotebook);
	if(i >= 0 && i < (int)m_vInterfacedScenario.size())
	{
		m_vInterfacedScenario[i]->hideCurrentVisualisation();
	}

	//closing last open scenario
	if(i32PageIndex == -1)
	{
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_stop"),    false);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_pause"),   false);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_next"),    false);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_play"),    false);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_forward"), false);

		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_windowmanager"), false);

		//toggle off window manager button
		GtkWidget* l_pWindowManagerButton = glade_xml_get_widget(m_pGladeInterface, "button_windowmanager");
		g_signal_handlers_disconnect_by_func(l_pWindowManagerButton, G_CALLBACK2(button_toggle_window_manager_cb), this);
		gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(l_pWindowManagerButton), false);
		g_signal_connect(l_pWindowManagerButton, "toggled", G_CALLBACK(button_toggle_window_manager_cb), this);
	}
	//switching to an existing scenario
	else if(i32PageIndex<(int32)m_vInterfacedScenario.size())
	{
		CInterfacedScenario* l_pCurrentInterfacedScenario=m_vInterfacedScenario[i32PageIndex];
		EPlayerStatus l_ePlayerStatus=(l_pCurrentInterfacedScenario->m_pPlayer?l_pCurrentInterfacedScenario->m_pPlayer->getStatus():PlayerStatus_Stop);

		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_stop"),    l_ePlayerStatus!=PlayerStatus_Stop);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_pause"),   l_ePlayerStatus!=PlayerStatus_Stop && l_ePlayerStatus!=PlayerStatus_Pause);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_next"),    true);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_play"),    l_ePlayerStatus!=PlayerStatus_Play);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_forward"), l_ePlayerStatus!=PlayerStatus_Forward);

		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_windowmanager"), l_ePlayerStatus==PlayerStatus_Stop);
		// gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_save"), l_pCurrentInterfacedScenario->m_bHasFileName && l_pCurrentInterfacedScenario->m_bHasBeenModified);
		// gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "menu_save"),   l_pCurrentInterfacedScenario->m_bHasFileName && l_pCurrentInterfacedScenario->m_bHasBeenModified);

		//don't show window manager if in offline mode and it is toggled off
		if(l_ePlayerStatus==PlayerStatus_Stop && m_vInterfacedScenario[i32PageIndex]->isDesignerVisualisationToggled() == false)
		{
			m_vInterfacedScenario[i32PageIndex]->hideCurrentVisualisation();
		}
		else
		{
			m_vInterfacedScenario[i32PageIndex]->showCurrentVisualisation();
		}

		//update window manager button state
		GtkWidget* l_pWindowManagerButton = glade_xml_get_widget(m_pGladeInterface, "button_windowmanager");
		g_signal_handlers_disconnect_by_func(l_pWindowManagerButton, G_CALLBACK2(button_toggle_window_manager_cb), this);
		gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(l_pWindowManagerButton), m_vInterfacedScenario[i32PageIndex]->isDesignerVisualisationToggled() ? true : false);
		g_signal_connect(l_pWindowManagerButton, "toggled", G_CALLBACK(button_toggle_window_manager_cb), this);
	}
	else //first scenario is created (or a scenario is opened and replaces first unnamed unmodified scenario)
	{
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_stop"),    false);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_pause"),   false);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_next"),    true);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_play"),    true);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_forward"), true);

		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_windowmanager"), true);

		//toggle off window manager button
		GtkWidget* l_pWindowManagerButton = glade_xml_get_widget(m_pGladeInterface, "button_windowmanager");
		g_signal_handlers_disconnect_by_func(l_pWindowManagerButton, G_CALLBACK2(button_toggle_window_manager_cb), this);
		gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(l_pWindowManagerButton), false);
		g_signal_connect(l_pWindowManagerButton, "toggled", G_CALLBACK(button_toggle_window_manager_cb), this);
	}
}

void CApplication::drag_data_get_cb(::GtkWidget* pWidget, ::GdkDragContext* pDragContex, ::GtkSelectionData* pSelectionData, guint uiInfo, guint uiT, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->dragDataGetCB(pWidget, pDragContex, pSelectionData, uiInfo, uiT);
}
#if 0
static gboolean CApplication::resource_query_tooltip_cb(::GtkWidget* pWidget, gint iX, gint iY, gboolean bKeyboardMode, ::GtkTooltip* pTooltip, gpointer pUserData)
{
	// gtk_tooltip_set_text(pTooltip, "text");
	// gtk_tooltip_set_markup(pTooltip, "markup");

	return TRUE;
}
#endif
void CApplication::menu_copy_selection_cb(::GtkMenuItem* pMenuItem, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->copySelectionCB();
}
void CApplication::menu_cut_selection_cb(::GtkMenuItem* pMenuItem, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->cutSelectionCB();
}
void CApplication::menu_paste_selection_cb(::GtkMenuItem* pMenuItem, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->pasteSelectionCB();
}
void CApplication::menu_delete_selection_cb(::GtkMenuItem* pMenuItem, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->deleteSelectionCB();
}

void CApplication::menu_new_scenario_cb(::GtkMenuItem* pMenuItem, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->newScenarioCB();
}
void CApplication::menu_open_scenario_cb(::GtkMenuItem* pMenuItem, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->openScenarioCB();
}
void CApplication::menu_save_scenario_cb(::GtkMenuItem* pMenuItem, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->saveScenarioCB();
}
void CApplication::menu_save_scenario_as_cb(::GtkMenuItem* pMenuItem, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->saveScenarioAsCB();
}
void CApplication::menu_close_scenario_cb(::GtkMenuItem* pMenuItem, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->closeScenarioCB();
}

void CApplication::button_new_scenario_cb(::GtkButton* pButton, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->newScenarioCB();
}
void CApplication::button_open_scenario_cb(::GtkButton* pButton, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->openScenarioCB();
}
void CApplication::button_save_scenario_cb(::GtkButton* pButton, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->saveScenarioCB();
}
void CApplication::button_save_scenario_as_cb(::GtkButton* pButton, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->saveScenarioAsCB();
}
void CApplication::button_close_scenario_cb(::GtkButton* pButton, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->closeScenarioCB();
}

void CApplication::delete_designer_visualisation_cb(gpointer user_data)
{
	if(user_data != NULL)
		static_cast<CApplication*>(user_data)->deleteDesignerVisualisationCB();
}
void CApplication::button_toggle_window_manager_cb(::GtkToggleToolButton* pButton, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->toggleDesignerVisualisationCB();
}

void CApplication::stop_scenario_cb(::GtkButton* pButton, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->stopScenarioCB();
}
void CApplication::pause_scenario_cb(::GtkButton* pButton, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->pauseScenarioCB();
}
void CApplication::next_scenario_cb(::GtkButton* pButton, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->nextScenarioCB();
}
void CApplication::play_scenario_cb(::GtkButton* pButton, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->playScenarioCB();
}
void CApplication::forward_scenario_cb(::GtkButton* pButton, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->forwardScenarioCB();
}

void CApplication::log_level_cb(::GtkButton* pButton, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->logLevelCB();
}

gboolean CApplication::change_current_scenario_cb(::GtkNotebook* pNotebook, ::GtkNotebookPage* pNotebookPage, guint uiPageNumber, gpointer pUserData)
{
	static_cast<CApplication*>(pUserData)->changeCurrentScenario((int32)uiPageNumber);
	return TRUE;
}

void CApplication::box_algorithm_title_button_expand_cb(::GtkButton* pButton, gpointer pUserData)
{
	gtk_tree_view_expand_all(GTK_TREE_VIEW(glade_xml_get_widget(static_cast<CApplication*>(pUserData)->m_pGladeInterface, "box_algorithm_tree")));
	gtk_notebook_set_current_page(GTK_NOTEBOOK(glade_xml_get_widget(static_cast<CApplication*>(pUserData)->m_pGladeInterface, "resource_notebook")), 0);
}
void CApplication::box_algorithm_title_button_collapse_cb(::GtkButton* pButton, gpointer pUserData)
{
	gtk_tree_view_collapse_all(GTK_TREE_VIEW(glade_xml_get_widget(static_cast<CApplication*>(pUserData)->m_pGladeInterface, "box_algorithm_tree")));
	gtk_notebook_set_current_page(GTK_NOTEBOOK(glade_xml_get_widget(static_cast<CApplication*>(pUserData)->m_pGladeInterface, "resource_notebook")), 0);
}

void CApplication::algorithm_title_button_expand_cb(::GtkButton* pButton, gpointer pUserData)
{
	gtk_tree_view_expand_all(GTK_TREE_VIEW(glade_xml_get_widget(static_cast<CApplication*>(pUserData)->m_pGladeInterface, "algorithm_tree")));
	gtk_notebook_set_current_page(GTK_NOTEBOOK(glade_xml_get_widget(static_cast<CApplication*>(pUserData)->m_pGladeInterface, "resource_notebook")), 1);
}
void CApplication::algorithm_title_button_collapse_cb(::GtkButton* pButton, gpointer pUserData)
{
	gtk_tree_view_collapse_all(GTK_TREE_VIEW(glade_xml_get_widget(static_cast<CApplication*>(pUserData)->m_pGladeInterface, "algorithm_tree")));
	gtk_notebook_set_current_page(GTK_NOTEBOOK(glade_xml_get_widget(static_cast<CApplication*>(pUserData)->m_pGladeInterface, "resource_notebook")), 1);
}

gboolean CApplication::idle_application_loop(gpointer pUserData)
{
	CApplication* l_pApplication=static_cast<CApplication*>(pUserData);
	CInterfacedScenario* l_pInterfacedScenario=l_pApplication->getCurrentInterfacedScenario();
	float64 l_f64Time=(l_pInterfacedScenario&&l_pInterfacedScenario->m_pPlayer?((l_pInterfacedScenario->m_pPlayer->getCurrentSimulatedTime()>>22)/1024.0):0);
	uint32 l_ui32Milli  = ((uint32)(l_f64Time*1000)%1000);
	uint32 l_ui32Seconds=  ((uint32)l_f64Time)%60;
	uint32 l_ui32Minutes= (((uint32)l_f64Time)/60)%60;
	uint32 l_ui32Hours  =((((uint32)l_f64Time)/60)/60);

	std::stringstream ss;
	ss << "Time : ";
	if(l_ui32Hours)                                            ss << l_ui32Hours << "h ";
	if(l_ui32Hours||l_ui32Minutes)                             ss << (l_ui32Minutes<10?"0":"") << l_ui32Minutes << "m ";
	if(l_ui32Hours||l_ui32Minutes||l_ui32Seconds)              ss << (l_ui32Seconds<10?"0":"") << l_ui32Seconds << "s ";
	ss << (l_ui32Milli<100?"0":"") << (l_ui32Milli<10?"0":"") << l_ui32Milli << "ms";

	gtk_label_set_text(GTK_LABEL(glade_xml_get_widget(l_pApplication->m_pGladeInterface, "label_current_time")), ss.str().c_str());

	if(!l_pApplication->hasScenarioRunning())
	{
		System::Time::sleep(5);
	}

	return TRUE;
}

gboolean CApplication::idle_scenario_loop(gpointer pUserData)
{
	CInterfacedScenario* l_pInterfacedScenario=static_cast<CInterfacedScenario*>(pUserData);
	uint64 l_ui64CurrentTime=System::Time::zgetTime();
	l_pInterfacedScenario->m_pPlayer->loop(l_ui64CurrentTime-l_pInterfacedScenario->m_ui64LastLoopTime);
	l_pInterfacedScenario->m_ui64LastLoopTime=l_ui64CurrentTime;
	return TRUE;
}
