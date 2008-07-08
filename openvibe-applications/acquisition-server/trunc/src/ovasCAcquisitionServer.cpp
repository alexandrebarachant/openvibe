#include "ovasCAcquisitionServer.h"

#include "ovasCDriverMindMediaNeXus32B.h"
#include "ovasCDriverGenericOscilator.h"

#include <system/Memory.h>
#include <system/Time.h>

#include <openvibe-toolkit/ovtk_all.h>

#include <iostream>
#include <fstream>

#include <assert.h>

#define OVAS_GUI_File            "../share/openvibe-applications/acquisition-server/interface.glade"

// #define _DEBUG_Callbacks_

using namespace OpenViBEAcquisitionServer;
using namespace std;

namespace OpenViBEAcquisitionServer
{
	class CConnectionHandler : public EBML::IWriterCallback
	{
	public:

		CConnectionHandler(Socket::IConnection& rConnection);
		virtual ~CConnectionHandler(void);

		// Accessors
		Socket::IConnection& getConnection(void);
		EBML::IWriter& getWriter(void);
		EBML::IWriterHelper& getWriterHelper(void);

		// EBML writer callback
		virtual void write(const void* pBuffer, const EBML::uint64 ui64BufferSize);

	protected:

		Socket::IConnection& m_rConnection;
		EBML::IWriter* m_pWriter;
		EBML::IWriterHelper* m_pWriterHelper;
	};
};

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
	// Monitors socket on reading to detect disconnection
	if(m_rConnection.isReadyToReceive())
	{
		uint8 l_pNullPacket[10224];
		m_rConnection.receiveBuffer(l_pNullPacket, sizeof(l_pNullPacket));
	}

	assert(ui64BufferSize <= 0xffffffff);
	m_rConnection.sendBuffer(pBuffer, (uint32)ui64BufferSize);
}

//___________________________________________________________________//
//                                                                   //

static int idle_cb(void* pUserData)
{
#if defined _DEBUG_Callbacks_
	// cout << "idle_cb" << endl;
#endif
	static_cast<CAcquisitionServer*>(pUserData)->idleCB();
	return true;
}

static void button_configure_pressed_cb(::GtkButton* pButton, void* pUserData)
{
#if defined _DEBUG_Callbacks_
	cout << "button_configure_pressed_cb" << endl;
#endif
	static_cast<CAcquisitionServer*>(pUserData)->buttonConfigurePressedCB(pButton);
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

static void combobox_driver_changed_cb(::GtkComboBox* pComboBox, void* pUserData)
{
#if defined _DEBUG_Callbacks_
	cout << "combobox_driver_changed_cb" << endl;
#endif
	static_cast<CAcquisitionServer*>(pUserData)->comboBoxDriverChanged(pComboBox);
}

//___________________________________________________________________//
//                                                                   //

CAcquisitionServer::CAcquisitionServer(void)
	:m_pGladeInterface(NULL)
	,m_pConnectionServer(NULL)
	,m_bInitialized(false)
	,m_bStarted(false)
	,m_pSampleBuffer(NULL)
	,m_bGotData(false)
	,m_pDriver(NULL)
{
	m_vDriver.push_back(new CDriverMindMediaNeXus32B());
	m_vDriver.push_back(new CDriverGenericOscillator());
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

	vector<IDriver*>::iterator i;
	for(i=m_vDriver.begin(); i!=m_vDriver.end(); i++)
	{
		if(*i)
		{
			(*i)->release();
			(*i)=NULL;
		}
	}
	m_vDriver.clear();
}

//___________________________________________________________________//
//                                                                   //

boolean CAcquisitionServer::initialize(void)
{
	m_pGladeInterface=glade_xml_new(OVAS_GUI_File, NULL, NULL);

	// Connects custom GTK signals

	g_signal_connect(glade_xml_get_widget(m_pGladeInterface, "button_configure"),     "pressed", G_CALLBACK(button_configure_pressed_cb), this);
	g_signal_connect(glade_xml_get_widget(m_pGladeInterface, "togglebutton_connect"), "toggled", G_CALLBACK(button_connect_toggled_cb),   this);
	g_signal_connect(glade_xml_get_widget(m_pGladeInterface, "button_play"),          "pressed", G_CALLBACK(button_start_pressed_cb),     this);
	g_signal_connect(glade_xml_get_widget(m_pGladeInterface, "button_stop"),          "pressed", G_CALLBACK(button_stop_pressed_cb),      this);
	g_signal_connect(glade_xml_get_widget(m_pGladeInterface, "combobox_driver"),      "changed", G_CALLBACK(combobox_driver_changed_cb),  this);
	glade_xml_signal_autoconnect(m_pGladeInterface);

	// Prepares combo boxes

	vector<IDriver*>::iterator i;
	gtk_combo_box_set_active(GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeInterface, "combobox_sample_count_per_sent_block")), 0);
	for(i=m_vDriver.begin(); i!=m_vDriver.end(); i++)
	{
		gtk_combo_box_append_text(GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeInterface, "combobox_driver")), (*i)->getName());
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeInterface, "combobox_driver")), 0);

	// Shows main window

	gtk_widget_show(glade_xml_get_widget(m_pGladeInterface, "openvibe-acquisition-server"));

	return true;
}

//___________________________________________________________________//
//                                                                   //

void CAcquisitionServer::idleCB(void)
{
	uint32 i;

	m_pDriver->loop();

	boolean l_bLabelNeedsUpdate=false;

	// Searches for new connection(s)
	if(m_pConnectionServer)
	{
		if(m_pConnectionServer->isReadyToReceive())
		{
			// Accespts new client
			Socket::IConnection* l_pConnection=m_pConnectionServer->accept();
			if(l_pConnection!=NULL)
			{
				CConnectionHandler* l_pConnectionHandler=new CConnectionHandler(*l_pConnection);
				m_vConnectionHandler.push_back(l_pConnectionHandler);
				l_bLabelNeedsUpdate=true;

				// Sends header
				EBML::IWriterHelper& l_rWriterHelper=l_pConnectionHandler->getWriterHelper();
				l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_Header);
					l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_AcquisitionInformation);
						if(m_pDriver->getHeader()->isExperimentIdentifierSet())
						{
							l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_ExperimentId);
								l_rWriterHelper.setUIntegerAsChildData(m_pDriver->getHeader()->getExperimentIdentifier());
							l_rWriterHelper.closeChild();
						}
						if(m_pDriver->getHeader()->isSubjectAgeSet())
						{
							l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_SubjectAge);
								l_rWriterHelper.setUIntegerAsChildData(m_pDriver->getHeader()->getSubjectAge());
							l_rWriterHelper.closeChild();
						}
						if(m_pDriver->getHeader()->isSubjectSexSet())
						{
							l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_SubjectSex);
								l_rWriterHelper.setUIntegerAsChildData(m_pDriver->getHeader()->getSubjectSex());
							l_rWriterHelper.closeChild();
						}
					l_rWriterHelper.closeChild();
					l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_ChannelCount);
						l_rWriterHelper.setUIntegerAsChildData(m_pDriver->getHeader()->getChannelCount());
					l_rWriterHelper.closeChild();
					l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_SamplingFrequency);
						l_rWriterHelper.setUIntegerAsChildData(m_pDriver->getHeader()->getSamplingFrequency());
					l_rWriterHelper.closeChild();
					l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_ChannelNames);
						for(i=0; i<m_pDriver->getHeader()->getChannelCount(); i++)
						{
							l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_ChannelName);
								l_rWriterHelper.setASCIIStringAsChildData(m_pDriver->getHeader()->getChannelName(i));
							l_rWriterHelper.closeChild();
						}
					l_rWriterHelper.closeChild();
					l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_GainFactors);
						for(i=0; i<m_pDriver->getHeader()->getChannelCount(); i++)
						{
							l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_GainFactor);
								l_rWriterHelper.setFloat32AsChildData(m_pDriver->getHeader()->getChannelGain(i));
							l_rWriterHelper.closeChild();
						}
					l_rWriterHelper.closeChild();
				l_rWriterHelper.closeChild();
			}
		}
	}

	// Sends data to connected client(s)
	// and clean disconnected client(s)
	list<CConnectionHandler*>::iterator itConnectionHandler=m_vConnectionHandler.begin();
	while(itConnectionHandler!=m_vConnectionHandler.end())
	{
		Socket::IConnection& l_rConnection=(*itConnectionHandler)->getConnection();

		if(!l_rConnection.isConnected())
		{
			delete *itConnectionHandler;
			itConnectionHandler=m_vConnectionHandler.erase(itConnectionHandler);
			l_bLabelNeedsUpdate=true;
		}
		else
		{
			// Sends buffer
			if(m_bStarted && m_bGotData)
			{
				EBML::IWriterHelper& l_rWriterHelper=(*itConnectionHandler)->getWriterHelper();
				l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_Buffer);
					l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_Samples);
						l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_SamplesPerChannelCount);
							l_rWriterHelper.setUIntegerAsChildData(m_ui32SampleCountPerSentBlock);
						l_rWriterHelper.closeChild();
						for(uint32 i=0; i<m_pDriver->getHeader()->getChannelCount(); i++)
						{
							l_rWriterHelper.openChild(OVTK_NodeId_Acquisition_SampleBlock);
								l_rWriterHelper.setBinaryAsChildData(m_pSampleBuffer+i*m_ui32SampleCountPerSentBlock*sizeof(float32), m_ui32SampleCountPerSentBlock*sizeof(float32));
							l_rWriterHelper.closeChild();
						}
					l_rWriterHelper.closeChild();
				l_rWriterHelper.closeChild();
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

	if(m_bGotData)
	{
		m_bGotData=false;
		System::Time::sleep(0);
	}
}

//___________________________________________________________________//
//                                                                   //

void CAcquisitionServer::buttonConnectToggledCB(::GtkToggleButton* pButton)
{
	if(gtk_toggle_button_get_active(pButton))
	{
		m_ui32SampleCountPerSentBlock=atoi(gtk_combo_box_get_active_text(GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeInterface, "combobox_sample_count_per_sent_block"))));

		// Initializes driver
		m_pDriver->initialize(m_ui32SampleCountPerSentBlock, *this);

		uint32 l_ui32ConnectionPort=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeInterface, "spinbutton_connection_port")));
		m_pConnectionServer=Socket::createConnectionServer();
		if(m_pConnectionServer->listen(l_ui32ConnectionPort))
		{
			gtk_button_set_label(GTK_BUTTON(pButton), "gtk-disconnect");

			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_configure"), false);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_play"), true);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_stop"), false);

			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "spinbutton_connection_port"), false);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "combobox_sample_count_per_sent_block"), false);

			gtk_label_set_label(GTK_LABEL(glade_xml_get_widget(m_pGladeInterface, "label_status")), "Connected ! Ready...");
			gtk_label_set_label(GTK_LABEL(glade_xml_get_widget(m_pGladeInterface, "label_connected_host_count")), "0 host connected...");

			m_ui32IdleCallbackId=gtk_idle_add(idle_cb, this);
			m_pSampleBuffer=new uint8[1024/*m_pDriver->getHeader()->getChannelCount()*/*m_ui32SampleCountPerSentBlock*sizeof(float32)];
			m_bGotData=false;
			m_bInitialized=true;
		}
		else
		{
			// Uninitializes driver
			m_pDriver->uninitialize();

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
		m_pDriver->uninitialize();

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

		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_configure"), m_pDriver->isConfigurable());
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_play"), false);
		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_stop"), false);

		gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "spinbutton_connection_port"), true);
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
	m_pDriver->start();

	m_bStarted=true;
}

void CAcquisitionServer::buttonStopPressedCB(::GtkButton* pButton)
{
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_play"), true);
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_stop"), false);

	gtk_label_set_label(GTK_LABEL(glade_xml_get_widget(m_pGladeInterface, "label_status")), "Connected ! Ready...");

	// Stops driver
	m_pDriver->stop();

	m_bStarted=false;
}

void CAcquisitionServer::buttonConfigurePressedCB(::GtkButton* pButton)
{
	if(m_pDriver->isConfigurable())
	{
		m_pDriver->configure();
	}
}

void CAcquisitionServer::comboBoxDriverChanged(::GtkComboBox* pComboBox)
{
	m_pDriver=m_vDriver[gtk_combo_box_get_active(pComboBox)];
	gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "button_configure"), m_pDriver->isConfigurable());
}

//___________________________________________________________________//
//                                                                   //

void CAcquisitionServer::setSamples(const float32* pSample)
{
	if(m_bStarted)
	{
		if(m_bGotData)
		{
			// some data will be dropped !
			cout << "dropped data\n";
		}
		else
		{
			const float32* l_pSource=pSample;
			uint8* l_pDestination=m_pSampleBuffer;
			for(uint32 j=0; j<m_pDriver->getHeader()->getChannelCount(); j++)
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
	else
	{
		cout << "not started\n";
	}
}
