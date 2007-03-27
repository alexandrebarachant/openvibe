#include "ovasCHeaderConfiguratorGlade.h"
#include "ovasIHeaderConfigurator.h"
#include "ovasIHeader.h"

#include <openvibe-toolkit/ovtk_all.h>

#include <glade/glade.h>
#include <gtk/gtk.h>

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>

using namespace OpenViBEAcquisitionServer;
using namespace std;

namespace OpenViBEAcquisitionServer
{
	class CHeaderConfigurator : virtual public OpenViBEAcquisitionServer::IHeaderConfigurator
	{
	public:

		CHeaderConfigurator(const char* sGladeXMLFileName, const char* sElectrodesFileName);
		virtual ~CHeaderConfigurator(void);
		virtual void release(void);

		virtual OpenViBEAcquisitionServer::boolean configure(
			OpenViBEAcquisitionServer::IHeader& rHeader);

		virtual void buttonChangeChannelNamesCB(::GtkButton* pButton);
		virtual void buttonApplyChannelNameCB(::GtkButton* pButton);
		virtual void buttonRemoveChannelNameCB(::GtkButton* pButton);

	private:

		CHeaderConfigurator(void);

	protected:

		::GladeXML* m_pGladeConfigureInterface;
		::GtkListStore* m_pElectrodeNameListStore;
		::GtkListStore* m_pChannelNameListStore;

		map<uint32, string> m_vChannelName;
		string m_sGladeXMLFileName;
		string m_sElectrodeFileName;
		IHeader* m_pHeader;
	};
};

//___________________________________________________________________//
//                                                                   //

static void gtk_combo_box_set_active_text(::GtkComboBox* pComboBox, const gchar* sActiveText)
{
	::GtkTreeModel* l_pTreeModel=gtk_combo_box_get_model(pComboBox);
	::GtkTreeIter itComboEntry;
	int l_iIndex=0;
	gchar* l_sComboEntryName=NULL;
	if(gtk_tree_model_get_iter_first(l_pTreeModel, &itComboEntry))
	{
		do
		{
			gtk_tree_model_get(l_pTreeModel, &itComboEntry, 0, &l_sComboEntryName, -1);
			if(string(l_sComboEntryName)==string(sActiveText))
			{
				gtk_combo_box_set_active(pComboBox, l_iIndex);
				return;
			}
			else
			{
				l_iIndex++;
			}
		}
		while(gtk_tree_model_iter_next(l_pTreeModel, &itComboEntry));
	}
}

static void button_change_channel_names_cb(::GtkButton* pButton, void* pUserData)
{
#if defined _DEBUG_Callbacks_
	cout << "button_change_channel_names_cb" << endl;
#endif
	static_cast<CHeaderConfigurator*>(pUserData)->buttonChangeChannelNamesCB(pButton);
}

static void button_apply_channel_name_cb(::GtkButton* pButton, void* pUserData)
{
#if defined _DEBUG_Callbacks_
	cout << "button_apply_channel_name_cb" << endl;
#endif
	static_cast<CHeaderConfigurator*>(pUserData)->buttonApplyChannelNameCB(pButton);
}

static void button_remove_channel_name_cb(::GtkButton* pButton, void* pUserData)
{
#if defined _DEBUG_Callbacks_
	cout << "button_remove_channel_name_cb" << endl;
#endif
	static_cast<CHeaderConfigurator*>(pUserData)->buttonRemoveChannelNameCB(pButton);
}

//___________________________________________________________________//
//                                                                   //

CHeaderConfigurator::CHeaderConfigurator(const char* sGladeXMLFileName, const char* sElectrodesFileName)
	:m_pGladeConfigureInterface(NULL)
	,m_pElectrodeNameListStore(NULL)
	,m_pChannelNameListStore(NULL)
	,m_sGladeXMLFileName(sGladeXMLFileName)
	,m_sElectrodeFileName(sElectrodesFileName)
	,m_pHeader(NULL)
{
}

CHeaderConfigurator::~CHeaderConfigurator(void)
{
}

void CHeaderConfigurator::release(void)
{
	delete this;
}

IHeaderConfigurator* OpenViBEAcquisitionServer::createHeaderConfiguratorGlade(const char* sGladeXMLFileName, const char* sElectrodesFileName)
{
	if(sGladeXMLFileName==NULL || sElectrodesFileName==NULL)
	{
		return NULL;
	}
	return new CHeaderConfigurator(sGladeXMLFileName, sElectrodesFileName);
}

//___________________________________________________________________//
//                                                                   //

boolean CHeaderConfigurator::configure(IHeader& rHeader)
{
	m_pHeader=&rHeader;

	// Prepares interface

	m_pGladeConfigureInterface=glade_xml_new(m_sGladeXMLFileName.c_str(), NULL, NULL);

	// Prepares electrode name tree view

	::GtkTreeView* l_pElectrodeNameTreeView=GTK_TREE_VIEW(glade_xml_get_widget(m_pGladeConfigureInterface, "treeview_electrode_names"));
	::GtkCellRenderer* l_pElectrodeNameIndexCellRenderer=gtk_cell_renderer_text_new();
	::GtkTreeViewColumn* l_pElectrodeNameIndexTreeViewColumn=gtk_tree_view_column_new_with_attributes("Name", l_pElectrodeNameIndexCellRenderer, "text", 0, NULL);

	gtk_tree_view_append_column(l_pElectrodeNameTreeView, l_pElectrodeNameIndexTreeViewColumn);

	// Prepares channel name tree view

	::GtkTreeView* l_pChannelNameTreeView=GTK_TREE_VIEW(glade_xml_get_widget(m_pGladeConfigureInterface, "treeview_channel_names"));
	::GtkCellRenderer* l_pChannelNameIndexCellRenderer=gtk_cell_renderer_text_new();
	::GtkCellRenderer* l_pChannelNameValueCellRenderer=gtk_cell_renderer_text_new();
	::GtkTreeViewColumn* l_pChannelNameIndexTreeViewColumn=gtk_tree_view_column_new_with_attributes("Index", l_pChannelNameIndexCellRenderer, "text", 0, NULL);
	::GtkTreeViewColumn* l_pChannelNameValueTreeViewColumn=gtk_tree_view_column_new_with_attributes("Name", l_pChannelNameValueCellRenderer, "text", 1, NULL);

	gtk_tree_view_append_column(l_pChannelNameTreeView, l_pChannelNameIndexTreeViewColumn);
	gtk_tree_view_append_column(l_pChannelNameTreeView, l_pChannelNameValueTreeViewColumn);

	// Connects custom GTK signals

	g_signal_connect(glade_xml_get_widget(m_pGladeConfigureInterface, "button_change_channel_names"), "pressed", G_CALLBACK(button_change_channel_names_cb), this);
	g_signal_connect(glade_xml_get_widget(m_pGladeConfigureInterface, "button_apply_channel_name"),   "pressed", G_CALLBACK(button_apply_channel_name_cb),   this);
	g_signal_connect(glade_xml_get_widget(m_pGladeConfigureInterface, "button_remove_channel_name"),  "pressed", G_CALLBACK(button_remove_channel_name_cb),  this);
	glade_xml_signal_autoconnect(m_pGladeConfigureInterface);

	// Configures interface with preconfigured values

	if(m_pHeader->isExperimentIdentifierSet())
	{
		gtk_spin_button_set_value(
			GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeConfigureInterface, "spinbutton_identifier")),
			m_pHeader->getExperimentIdentifier());
	}
	if(m_pHeader->isSubjectAgeSet())
	{
		gtk_spin_button_set_value(
			GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeConfigureInterface, "spinbutton_age")),
			m_pHeader->getSubjectAge());
	}
	if(m_pHeader->isChannelCountSet())
	{
		gtk_spin_button_set_value(
			GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeConfigureInterface, "spinbutton_number_of_channels")),
			m_pHeader->getChannelCount());
		for(uint32 i=0; i<m_pHeader->getChannelCount(); i++)
		{
			m_vChannelName[i]=m_pHeader->getChannelName(i);
		}
	}
	if(m_pHeader->isSamplingFrequencySet())
	{
		char l_sSamplingFrequency[1024];
		sprintf(l_sSamplingFrequency, "%i", (int)m_pHeader->getSamplingFrequency());
		gtk_combo_box_set_active_text(
			GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeConfigureInterface, "combobox_sampling_frequency")),
			l_sSamplingFrequency);
	}
	else
	{
		gtk_combo_box_set_active(
			GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeConfigureInterface, "combobox_sampling_frequency")),
			0);
	}
	if(m_pHeader->isSubjectSexSet())
	{
		gtk_combo_box_set_active_text(
			GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeConfigureInterface, "combobox_sex")),
				m_pHeader->getSubjectSex()==OVTK_Value_Sex_Male?"male":
				m_pHeader->getSubjectSex()==OVTK_Value_Sex_Female?"female":
				m_pHeader->getSubjectSex()==OVTK_Value_Sex_Unknown?"unknown":
				"unspecified");
	}
	else
	{
		gtk_combo_box_set_active(
			GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeConfigureInterface, "combobox_sex")),
			0);
	}

	// Runs the dialog !

	::GtkDialog* l_pDialog=GTK_DIALOG(glade_xml_get_widget(m_pGladeConfigureInterface, "openvibe-acquisition-server-settings"));
	if(gtk_dialog_run(l_pDialog)==GTK_RESPONSE_APPLY)
	{
		string l_sSex=gtk_combo_box_get_active_text(GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeConfigureInterface, "combobox_sex")));
		m_pHeader->setExperimentIdentifier(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeConfigureInterface, "spinbutton_identifier"))));
		m_pHeader->setSubjectAge(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeConfigureInterface, "spinbutton_age"))));
		m_pHeader->setChannelCount(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeConfigureInterface, "spinbutton_number_of_channels"))));
		m_pHeader->setSamplingFrequency(atoi(gtk_combo_box_get_active_text(GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeConfigureInterface, "combobox_sampling_frequency")))));
		m_pHeader->setSubjectSex(
			l_sSex=="male"?OVTK_Value_Sex_Male:
			l_sSex=="female"?OVTK_Value_Sex_Female:
			l_sSex=="unknown"?OVTK_Value_Sex_Unknown:
			OVTK_Value_Sex_NotSpecified);
		for(uint32 i=0; i!=m_pHeader->getChannelCount(); i++)
		{
			if(m_vChannelName[i]!="")
			{
				m_pHeader->setChannelName(i, m_vChannelName[i].c_str());
			}
		}
	}

	gtk_widget_hide(GTK_WIDGET(l_pDialog));

	g_object_unref(m_pGladeConfigureInterface);
	m_pGladeConfigureInterface=NULL;

	m_vChannelName.clear();

	m_pHeader=NULL;

	return true;
}

void CHeaderConfigurator::buttonChangeChannelNamesCB(::GtkButton* pButton)
{
	uint32 i;
	::GtkTreeIter itElectrodeName, itChannelName;
	::GtkDialog* l_pDialog=GTK_DIALOG(glade_xml_get_widget(m_pGladeConfigureInterface, "channel-names"));
	::GtkTreeView* l_pElectrodeNameTreeView=GTK_TREE_VIEW(glade_xml_get_widget(m_pGladeConfigureInterface, "treeview_electrode_names"));
	::GtkTreeView* l_pChannelNameTreeView=GTK_TREE_VIEW(glade_xml_get_widget(m_pGladeConfigureInterface, "treeview_channel_names"));

	// Creates electrode name and channel name models

	m_pElectrodeNameListStore=gtk_list_store_new(1, G_TYPE_STRING);
	m_pChannelNameListStore=gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

	// Fills in electrode name model

	list<string> l_vElectrodeName;
	list<string>::iterator l;
	ifstream l_oFile(m_sElectrodeFileName.c_str());
	if(l_oFile.is_open())
	{
		while(!l_oFile.eof())
		{
			string l_sElectrodeName;
			l_oFile >> l_sElectrodeName;
			l_vElectrodeName.push_back(l_sElectrodeName);
		}
		l_oFile.close();

		for(l=l_vElectrodeName.begin(); l!=l_vElectrodeName.end(); l++)
		{
			gtk_list_store_append(m_pElectrodeNameListStore, &itElectrodeName);
			gtk_list_store_set(m_pElectrodeNameListStore, &itElectrodeName, 0, l->c_str(), -1);
		}
	}

	// Fills in channel name model

	uint32 l_ui32ChannelCount=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeConfigureInterface, "spinbutton_number_of_channels")));
	for(i=0; i<l_ui32ChannelCount; i++)
	{
		char l_sChannelName[1024];
		sprintf(l_sChannelName, "%i", (int)i);
		gtk_list_store_append(m_pChannelNameListStore, &itChannelName);
		gtk_list_store_set(m_pChannelNameListStore, &itChannelName, 0, l_sChannelName, -1);
		gtk_list_store_set(m_pChannelNameListStore, &itChannelName, 1, m_vChannelName[i].c_str(), -1);
	}

	// Attachs model to views

	gtk_tree_view_set_model(l_pElectrodeNameTreeView, GTK_TREE_MODEL(m_pElectrodeNameListStore));
	gtk_tree_view_set_model(l_pChannelNameTreeView, GTK_TREE_MODEL(m_pChannelNameListStore));

	// Runs dialog !

	if(gtk_dialog_run(l_pDialog)==GTK_RESPONSE_APPLY)
	{
		int i=0;
		gchar* l_sChannelName=NULL;
		if(gtk_tree_model_get_iter_first(GTK_TREE_MODEL(m_pChannelNameListStore), &itChannelName))
		{
			do
			{
				gtk_tree_model_get(GTK_TREE_MODEL(m_pChannelNameListStore), &itChannelName, 1, &l_sChannelName, -1);
				m_vChannelName[i++]=l_sChannelName;
			}
			while(gtk_tree_model_iter_next(GTK_TREE_MODEL(m_pChannelNameListStore), &itChannelName));
		}
	}

	gtk_widget_hide(GTK_WIDGET(l_pDialog));
	g_object_unref(m_pChannelNameListStore);
	g_object_unref(m_pElectrodeNameListStore);
	m_pChannelNameListStore=NULL;
	m_pElectrodeNameListStore=NULL;
}

void CHeaderConfigurator::buttonApplyChannelNameCB(::GtkButton* pButton)
{
	::GtkTreeIter itElectrodeName, itChannelName;
	::GtkTreeView* l_pElectrodeNameTreeView=GTK_TREE_VIEW(glade_xml_get_widget(m_pGladeConfigureInterface, "treeview_electrode_names"));
	::GtkTreeView* l_pChannelNameTreeView=GTK_TREE_VIEW(glade_xml_get_widget(m_pGladeConfigureInterface, "treeview_channel_names"));

	::GtkTreeSelection* l_pChannelNameTreeViewSelection=gtk_tree_view_get_selection(l_pChannelNameTreeView);
	::GtkTreeSelection* l_pElectrodeNameTreeViewSelection=gtk_tree_view_get_selection(l_pElectrodeNameTreeView);

	if(gtk_tree_selection_get_selected(l_pChannelNameTreeViewSelection, NULL, &itChannelName))
	{
		if(gtk_tree_selection_get_selected(l_pElectrodeNameTreeViewSelection, NULL, &itElectrodeName))
		{
			gchar* l_sElectrodeName=NULL;
			gtk_tree_model_get(GTK_TREE_MODEL(m_pElectrodeNameListStore), &itElectrodeName, 0, &l_sElectrodeName, -1);
			gtk_list_store_set(m_pChannelNameListStore, &itChannelName, 1, l_sElectrodeName, -1);
		}
	}
}

void CHeaderConfigurator::buttonRemoveChannelNameCB(::GtkButton* pButton)
{
	::GtkTreeIter itChannelName;
	::GtkTreeView* l_pChannelNameTreeView=GTK_TREE_VIEW(glade_xml_get_widget(m_pGladeConfigureInterface, "treeview_channel_names"));

	::GtkTreeSelection* l_pChannelNameTreeViewSelection=gtk_tree_view_get_selection(l_pChannelNameTreeView);
	if(gtk_tree_selection_get_selected(l_pChannelNameTreeViewSelection, NULL, &itChannelName))
	{
		gtk_list_store_set(m_pChannelNameListStore, &itChannelName, 1, "", -1);
	}
}
