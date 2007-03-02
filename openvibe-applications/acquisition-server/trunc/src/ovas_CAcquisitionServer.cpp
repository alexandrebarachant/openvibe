#include "ovas_CAcquisitionServer.h"

#include <system/Memory.h>
#include <system/Time.h>

#include <openvibe-toolkit/ovtk_all.h>

#include <iostream>
#include <fstream>

#include "ovas_driver_MindMedia_NeXus32B.h"
#include "ovas_driver_Generic_Oscilator.h"

// #define _DEBUG_Callbacks_

using namespace OpenViBEAcquisitionServer;
using namespace std;

/******************************************************************************/

enum SubjectSex
{
	SubjectSex_Undefined,
	SubjectSex_Female,
	SubjectSex_Male,
};

/******************************************************************************/

CConnectionHandler::CConnectionHandler(Socket::IConnection& rConnection)
	:m_rConnection(rConnection)
	,m_pWriter(NULL)
	,m_pWriterHelper(NULL)
{
	m_pWriter=EBML::createWriter(*this);
	m_pWriterHelper=EBML::createWriterHelper();
	m_pWriterHelper->connect(m_pWriter);
}

CConnectionHandler::~CConnectionHandler(void)
{
	m_pWriterHelper->release();
	m_pWriter->release();
	m_rConnection.release();
}

Socket::IConnection& CConnectionHandler::getConnection(void)
{
	return m_rConnection;
}

EBML::IWriter& CConnectionHandler::getWriter(void)
{
	return *m_pWriter;
}

EBML::IWriterHelper& CConnectionHandler::getWriterHelper(void)
{
	return *m_pWriterHelper;
}

void CConnectionHandler::write(const void* pBuffer, const EBML::uint64 ui64BufferSize)
{
	m_rConnection.sendBuffer(pBuffer, ui64BufferSize);
}

/******************************************************************************/

static int idle_cb(void* pUserData)
{
#if defined _DEBUG_Callbacks_
	// cout << "idle_cb" << endl;
#endif
	static_cast<CAcquisitionServer*>(pUserData)->idleCB();
	return true;
}

static void button_connect_toggled_cb(::GtkToggleButton* pButton, void* pUserData)
{
#if defined _DEBUG_Callbacks_
	cout << "button_connect_toggled_cb" << endl;
#endif
	static_cast<CAcquisitionServer*>(pUserData)->buttonConnectToggledCB(pButton);
}

static void button_start_pressed_cb(::GtkButton* pButton, void* pUserData)
{
#if defined _DEBUG_Callbacks_
	cout << "button_start_pressed_cb" << endl;
#endif
	static_cast<CAcquisitionServer*>(pUserData)->buttonStartPressedCB(pButton);
}

static void button_stop_pressed_cb(::GtkButton* pButton, void* pUserData)
{
#if defined _DEBUG_Callbacks_
	cout << "button_stop_pressed_cb" << endl;
#endif
	static_cast<CAcquisitionServer*>(pUserData)->buttonStopPressedCB(pButton);
}

static void button_show_electrode_names_cb(::GtkButton* pButton, void* pUserData)
{
#if defined _DEBUG_Callbacks_
	cout << "button_show_electrode_names_cb" << endl;
#endif
	static_cast<CAcquisitionServer*>(pUserData)->buttonShowElectrodeNamesCB(pButton);
}

static void button_apply_electrode_name_data_cb(::GtkButton* pButton, void* pUserData)
{
#if defined _DEBUG_Callbacks_
	cout << "button_apply_electrode_name_data_cb" << endl;
#endif
	static_cast<CAcquisitionServer*>(pUserData)->buttonApplyElectrodeNameCB(pButton);
}

static void button_remove_electrode_name_data_cb(::GtkButton* pButton, void* pUserData)
{
#if defined _DEBUG_Callbacks_
	cout << "button_remove_electrode_name_data_cb" << endl;
#endif
	static_cast<CAcquisitionServer*>(pUserData)->buttonRemoveElectrodeNameCB(pButton);
}

static void driver_process_data_cb(const float32* pSample, void* pUserData)
{
#if defined _DEBUG_Callbacks_
	cout << "driver_process_data_cb" << endl;
#endif
	static_cast<CAcquisitionServer*>(pUserData)->driverProcessDataCB(pSample);
}

CAcquisitionServer::CAcquisitionServer(void)
	:m_pGladeInterface(NULL)
	,m_pElectrodeNameListStore(NULL)
	,m_pChannelNameListStore(NULL)
	,m_pConnectionServer(NULL)
	,m_bInitialized(false)
	,m_bStarted(false)
	,m_pSampleBuffer(NULL)
	,m_bGotData(false)
{
	// m_oDriver=getDriver_MindMedia_NeXus32B();
	m_oDriver=getDriver_Generic_Oscilator();
}

CAcquisitionServer::~CAcquisitionServer(void)
{
	delete [] m_pSampleBuffer;
	m_pSampleBuffer=NULL;

	if(m_pConnectionServer)
	{
		m_pConnectionServer->release();
		m_pConnectionServer=NULL;
	}

	g_object_unref(m_pChannelNameListStore);
	g_object_unref(m_pElectrodeNameListStore);
}

boolean CAcquisitionServer::initialize(void)
{
	m_pGladeInterface=glade_xml_new("../share/openvibe-acquisition-server-interface.glade", NULL, NULL);

	// Prepares combo boxes

	gtk_combo_box_set_active(GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeInterface, "combobox_sex")), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeInterface, "combobox_sampling_frequency")), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeInterface, "combobox_sample_count_per_sent_block")), 0);

	// Prepares electrode name tree view

	::GtkTreeView* l_pElectrodeNameTreeView=GTK_TREE_VIEW(glade_xml_get_widget(m_pGladeInterface, "treeview_electrode_name"));

	::GtkCellRenderer* l_pElectrodeNameIndexCellRenderer=gtk_cell_renderer_text_new();
	::GtkTreeViewColumn* l_pElectrodeNameIndexTreeViewColumn=gtk_tree_view_column_new_with_attributes("Name", l_pElectrodeNameIndexCellRenderer, "text", 0, NULL);
	gtk_tree_view_append_column(l_pElectrodeNameTreeView, l_pElectrodeNameIndexTreeViewColumn);

	m_pElectrodeNameListStore=gtk_list_store_new(1, G_TYPE_STRING);

	// Prepares channel name tree view

	::GtkTreeView* l_pChannelNameTreeView=GTK_TREE_VIEW(glade_xml_get_widget(m_pGladeInterface, "treeview_channel_name"));

	::GtkCellRenderer* l_pChannelNameIndexCellRenderer=gtk_cell_renderer_text_new();
	::GtkTreeViewColumn* l_pChannelNameIndexTreeViewColumn=gtk_tree_view_column_new_with_attributes("Index", l_pChannelNameIndexCellRenderer, "text", 0, NULL);
	gtk_tree_view_append_column(l_pChannelNameTreeView, l_pChannelNameIndexTreeViewColumn);

	::GtkCellRenderer* l_pChannelNameValueCellRenderer=gtk_cell_renderer_text_new();
	::GtkTreeViewColumn* l_pChannelNameValueTreeViewColumn=gtk_tree_view_column_new_with_attributes("Name", l_pChannelNameValueCellRenderer, "text", 1, NULL);
	gtk_tree_view_append_column(l_pChannelNameTreeView, l_pChannelNameValueTreeViewColumn);

	m_pChannelNameListStore=gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

	// Connects custom GTK signals

	g_signal_connect(glade_xml_get_widget(m_pGladeInterface, "togglebutton_connect"),         "toggled", G_CALLBACK(button_connect_toggled_cb),            this);
	g_signal_connect(glade_xml_get_widget(m_pGladeInterface, "button_play"),                  "pressed", G_CALLBACK(button_start_pressed_cb),              this);
	g_signal_connect(glade_xml_get_widget(m_pGladeInterface, "button_stop"),                  "pressed", G_CALLBACK(button_stop_pressed_cb),               this);
	g_signal_connect(glade_xml_get_widget(m_pGladeInterface, "button_show_electrode_names"),  "pressed", G_CALLBACK(button_show_electrode_names_cb),       this);
	g_signal_connect(glade_xml_get_widget(m_pGladeInterface, "button_apply_electrode_name"),  "pressed", G_CALLBACK(button_apply_electrode_name_data_cb),  this);
	g_signal_connect(glade_xml_get_widget(m_pGladeInterface, "button_remove_electrode_name"), "pressed", G_CALLBACK(button_remove_electrode_name_data_cb), this);
	glade_xml_signal_autoconnect(m_pGladeInterface);

	gtk_widget_show(glade_xml_get_widget(m_pGladeInterface, "openvibe-acquisition-server"));

	return true;
}

void CAcquisitionServer::driverProcessDataCB(const float32* pSample)
{
	if(m_bStarted)
	{
		if(m_bGotData)
		{
			// some data will be dropped !
		}
		else
		{
			const float32* l_pSource=pSample;
			uint8* l_pDestination=m_pSampleBuffer;
			for(uint32 j=0; j<m_ui32ChannelCount; j++)
			{
				for(uint32 i=0; i<m_ui32SampleCountPerSentBlock; i++)
				{
					System::Memory::hostToLittleEndian(*l_pSource, l_pDestination);
					l_pSource++;
					l_pDestination+=sizeof(float32);
				}
			}
		}
		m_bGotData=true;
	}
}

void CAcquisitionServer::idleCB(void)
{
	if(m_bStarted)
	{
		if(m_oDriver.m_fpLoop)
		{
			m_oDriver.m_fpLoop();
		}
	}

	boolean l_bLabelNeedsUpdate=false;

	// Searches for new connection(s)
	if(m_pConnectionServer)
	{
		if(m_pConnectionServer->isReadyToReceive())
		{
			Socket::IConnection* l_pConnection=m_pConnectionServer->accept();
			CConnectionHandler* l_pConnectionHandler=new CConnectionHandler(*l_pConnection);
			m_vConnectionHandler.push_back(l_pConnectionHandler);
			l_bLabelNeedsUpdate=true;

			uint32 i;
			EBML::IWriterHelper& l_rWriterHelper=l_pConnectionHandler->getWriterHelper();
			l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_Header);
				l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_AcquisitionInformation);
					l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_ExperimentId);
						l_rWriterHelper.setUIntegerAsChildData(m_ui32ExperimentIdentifier);
					l_rWriterHelper.closeChild();
					l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_SubjectAge);
						l_rWriterHelper.setUIntegerAsChildData(m_ui32SubjectAge);
					l_rWriterHelper.closeChild();
					l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_SubjectSex);
						l_rWriterHelper.setUIntegerAsChildData(m_ui32SubjectSex);
					l_rWriterHelper.closeChild();
				l_rWriterHelper.closeChild();
				l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_ChannelCount);
					l_rWriterHelper.setUIntegerAsChildData(m_ui32ChannelCount);
				l_rWriterHelper.closeChild();
				l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_SamplingFrequency);
					l_rWriterHelper.setUIntegerAsChildData(m_ui32SamplingFrequency);
				l_rWriterHelper.closeChild();
				l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_ChannelNames);
					for(i=0; i<m_ui32ChannelCount; i++)
					{
						l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_ChannelName);
							l_rWriterHelper.setASCIIStringAsChildData(m_vChannelName[i].c_str());
						l_rWriterHelper.closeChild();
					}
				l_rWriterHelper.closeChild();
			l_rWriterHelper.closeChild();
		}
	}

	// Sends data to connected client(s)
	// and clean disconnected client(s)
	std::list<CConnectionHandler*>::iterator itConnectionHandler=m_vConnectionHandler.begin();
	while(itConnectionHandler!=m_vConnectionHandler.end())
	{
		if(!(*itConnectionHandler)->getConnection().isConnected())
		{
			delete *itConnectionHandler;
			itConnectionHandler=m_vConnectionHandler.erase(itConnectionHandler);
			l_bLabelNeedsUpdate=true;
		}
		else
		{
			if(m_bStarted && m_bGotData)
			{
				EBML::IWriterHelper& l_rWriterHelper=(*itConnectionHandler)->getWriterHelper();
				l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_Buffer);
					l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_Samples);
						l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_SamplesPerChannelCount);
							l_rWriterHelper.setUIntegerAsChildData(m_ui32SampleCountPerSentBlock);
						l_rWriterHelper.closeChild();
						for(uint32 i=0; i<m_ui32ChannelCount; i++)
						{
							l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_SampleBlock);
								l_rWriterHelper.setBinaryAsChildData(m_pSampleBuffer, m_ui32ChannelCount*m_ui32SampleCountPerSentBlock*sizeof(float32));
							l_rWriterHelper.closeChild();
						}
					l_rWriterHelper.closeChild();
				l_rWriterHelper.closeChild();
			}
			else
			{
				// Sends sort-of keep alive empty packet
				Socket::IConnection& l_rConnection=(*itConnectionHandler)->getConnection();
				l_rConnection.sendBuffer(NULL, 0);
			}
			itConnectionHandler++;
		}
	}

	// Updates 'host count' label when needed
	if(l_bLabelNeedsUpdate)
	{
		char l_sLabel[1024];
		sprintf(l_sLabel, "%i host%s connected...", m_vConnectionHandler.size(), m_vConnectionHandler.size()?"s":"");
		gtk_label_set_label(GTK_LABEL(glade_xml_get_widget(m_pGladeInterface, "label_connected_host_count")), l_sLabel);
	}

	m_bGotData=false;
}

void CAcquisitionServer::buttonConnectToggledCB(::GtkToggleButton* pButton)
{
	if(gtk_toggle_button_get_active(pButton))
	{
		string l_sSex=gtk_combo_box_get_active_text(GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeInterface, "combobox_sex")));
		m_ui32ExperimentIdentifier=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeInterface, "spinbutton_identifier")));
		m_ui32SubjectAge=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeInterface, "spinbutton_age")));
		m_ui32ChannelCount=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeInterface, "spinbutton_number_of_electrodes")));
		m_ui32SamplingFrequency=atoi(gtk_combo_box_get_active_text(GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeInterface, "combobox_sampling_frequency"))));
		m_ui32SampleCountPerSentBlock=atoi(gtk_combo_box_get_active_text(GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeInterface, "combobox_sample_count_per_sent_block"))));
		m_ui32SubjectSex=(l_sSex=="male"?SubjectSex_Male:(l_sSex=="female"?SubjectSex_Female:SubjectSex_Undefined));

		uint32 l_ui32ConnectionPort=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeInterface, "spinbutton_connection_port")));
		m_pConnectionServer=Socket::createConnectionServer();
		if(m_pConnectionServer->listen(l_ui32ConnectionPort))
		{
			// Initializes driver
			if(m_oDriver.m_fpInitialize)
			{
				m_oDriver.m_fpInitialize(
					m_ui32SamplingFrequency,
					m_ui32ChannelCount,
					m_ui32SampleCountPerSentBlock,
					driver_process_data_cb,
					this);
			}

			gtk_button_set_label(GTK_BUTTON(pButton), "gtk-disconnect");

			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_play"), true);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_stop"), false);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_show_electrode_names"), false);

			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "spinbutton_identifier"),                false);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "spinbutton_age"),                       false);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "combobox_sex"),                         false);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "spinbutton_number_of_electrodes"),      false);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "combobox_sampling_frequency"),          false);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "spinbutton_connection_port"),           false);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "combobox_sample_count_per_sent_block"), false);

			gtk_label_set_label(GTK_LABEL(glade_xml_get_widget(m_pGladeInterface, "label_status")), "Connected ! Ready...");
			gtk_label_set_label(GTK_LABEL(glade_xml_get_widget(m_pGladeInterface, "label_connected_host_count")), "0 host connected...");

			m_ui32IdleCallbackId=gtk_idle_add(idle_cb, this);
			m_pSampleBuffer=new uint8[m_ui32ChannelCount*m_ui32SampleCountPerSentBlock*sizeof(float32)];
			m_bGotData=false;
			m_bInitialized=true;
		}
		else
		{
			gtk_toggle_button_set_active(pButton, false);

			gtk_label_set_label(GTK_LABEL(glade_xml_get_widget(m_pGladeInterface, "label_status")), "Connection failed !");

			m_bInitialized=false;
		}
	}
	else
	{
		if(m_bStarted)
		{
			gtk_button_pressed(GTK_BUTTON(glade_xml_get_widget(m_pGladeInterface, "button_stop")));
		}

		// Uninitializes driver
		if(m_oDriver.m_fpUninitialize)
		{
			m_oDriver.m_fpUninitialize();
		}

		gtk_idle_remove(m_ui32IdleCallbackId);

		list<CConnectionHandler*>::iterator i=m_vConnectionHandler.begin();
		while(i!=m_vConnectionHandler.end())
		{
			delete *i;
			i=m_vConnectionHandler.erase(i);
		}

		delete [] m_pSampleBuffer;
		m_pSampleBuffer=NULL;

		m_pConnectionServer->release();
		m_pConnectionServer=NULL;

		gtk_button_set_label(GTK_BUTTON(pButton), "gtk-connect");

		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_play"), false);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_stop"), false);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_show_electrode_names"), true);

		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "spinbutton_identifier"),                true);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "spinbutton_age"),                       true);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "combobox_sex"),                         true);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "spinbutton_number_of_electrodes"),      true);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "combobox_sampling_frequency"),          true);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "spinbutton_connection_port"),           true);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "combobox_sample_count_per_sent_block"), true);

		gtk_label_set_label(GTK_LABEL(glade_xml_get_widget(m_pGladeInterface, "label_status")), "");
		gtk_label_set_label(GTK_LABEL(glade_xml_get_widget(m_pGladeInterface, "label_connected_host_count")), "");

		m_bInitialized=false;
	}
}

void CAcquisitionServer::buttonStartPressedCB(::GtkButton* pButton)
{
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_play"), false);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_stop"), true);

	gtk_label_set_label(GTK_LABEL(glade_xml_get_widget(m_pGladeInterface, "label_status")), "Sending...");

	// Starts driver
	if(m_oDriver.m_fpStart)
	{
		m_oDriver.m_fpStart();
	}

	m_bStarted=true;
}

void CAcquisitionServer::buttonStopPressedCB(::GtkButton* pButton)
{
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_play"), true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_stop"), false);

	gtk_label_set_label(GTK_LABEL(glade_xml_get_widget(m_pGladeInterface, "label_status")), "Connected ! Ready...");

	// Stops driver
	if(m_oDriver.m_fpStop)
	{
		m_oDriver.m_fpStop();
	}

	m_bStarted=false;
}

void CAcquisitionServer::buttonShowElectrodeNamesCB(::GtkButton* pButton)
{
	uint32 i;
	::GtkTreeIter itElectrodeName, itChannelName;
	::GtkDialog* l_pDialog=GTK_DIALOG(glade_xml_get_widget(m_pGladeInterface, "electrode_names"));
	::GtkTreeView* l_pElectrodeNameTreeView=GTK_TREE_VIEW(glade_xml_get_widget(m_pGladeInterface, "treeview_electrode_name"));
	::GtkTreeView* l_pChannelNameTreeView=GTK_TREE_VIEW(glade_xml_get_widget(m_pGladeInterface, "treeview_channel_name"));

	gtk_list_store_clear(m_pElectrodeNameListStore);
	gtk_list_store_clear(m_pChannelNameListStore);

	list<string> l_vElectrodeName;
	ifstream l_oFile("../share/openvibe-acquisition-server-elecrtode-names.txt");
	if(l_oFile.is_open())
	{
		while(!l_oFile.eof())
		{
			string l_sElectrodeName;
			l_oFile >> l_sElectrodeName;
			l_vElectrodeName.push_back(l_sElectrodeName);
		}
		l_oFile.close();

		list<string>::iterator l;
		for(l=l_vElectrodeName.begin(); l!=l_vElectrodeName.end(); l++)
		{
			gtk_list_store_append(m_pElectrodeNameListStore, &itElectrodeName);
			gtk_list_store_set(m_pElectrodeNameListStore, &itElectrodeName, 0, l->c_str(), -1);
		}
	}

	uint32 l_ui32ChannelCount=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeInterface, "spinbutton_number_of_electrodes")));
	for(i=0; i<l_ui32ChannelCount; i++)
	{
		char l_sChannelName[1024];
		sprintf(l_sChannelName, "%i", i);
		gtk_list_store_append(m_pChannelNameListStore, &itChannelName);
		gtk_list_store_set(m_pChannelNameListStore, &itChannelName, 0, l_sChannelName, -1);
		gtk_list_store_set(m_pChannelNameListStore, &itChannelName, 1, m_vChannelName[i].c_str(), -1);
	}

	gtk_tree_view_set_model(l_pElectrodeNameTreeView, GTK_TREE_MODEL(m_pElectrodeNameListStore));
	gtk_tree_view_set_model(l_pChannelNameTreeView, GTK_TREE_MODEL(m_pChannelNameListStore));

	if(gtk_dialog_run(l_pDialog)==GTK_RESPONSE_APPLY)
	{
		gchar* l_sChannelName=NULL;
		m_vChannelName.clear();
		if(gtk_tree_model_get_iter_first(GTK_TREE_MODEL(m_pChannelNameListStore), &itChannelName))
		{
			i=0;
			do
			{
				gtk_tree_model_get(GTK_TREE_MODEL(m_pChannelNameListStore), &itChannelName, 1, &l_sChannelName, -1);
				m_vChannelName[i++]=l_sChannelName;
			}
			while(gtk_tree_model_iter_next(GTK_TREE_MODEL(m_pChannelNameListStore), &itChannelName));
		}
	}

	gtk_tree_view_set_model(l_pElectrodeNameTreeView, NULL);
	gtk_tree_view_set_model(l_pChannelNameTreeView, NULL);

	gtk_widget_hide(GTK_WIDGET(l_pDialog));
}

void CAcquisitionServer::buttonApplyElectrodeNameCB(::GtkButton* pButton)
{
	::GtkTreeIter itElectrodeName, itChannelName;
	::GtkTreeView* l_pElectrodeNameTreeView=GTK_TREE_VIEW(glade_xml_get_widget(m_pGladeInterface, "treeview_electrode_name"));
	::GtkTreeView* l_pChannelNameTreeView=GTK_TREE_VIEW(glade_xml_get_widget(m_pGladeInterface, "treeview_channel_name"));

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

void CAcquisitionServer::buttonRemoveElectrodeNameCB(::GtkButton* pButton)
{
	::GtkTreeIter itChannelName;
	::GtkTreeView* l_pChannelNameTreeView=GTK_TREE_VIEW(glade_xml_get_widget(m_pGladeInterface, "treeview_channel_name"));

	::GtkTreeSelection* l_pChannelNameTreeViewSelection=gtk_tree_view_get_selection(l_pChannelNameTreeView);
	if(gtk_tree_selection_get_selected(l_pChannelNameTreeViewSelection, NULL, &itChannelName))
	{
		gtk_list_store_set(m_pChannelNameListStore, &itChannelName, 1, "", -1);
	}
}
