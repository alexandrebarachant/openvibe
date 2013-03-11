#include "ovasCAcquisitionServerGUI.h"
#include "ovasCAcquisitionServerThread.h"
#include "ovasCAcquisitionServer.h"
#include "ovasIAcquisitionServerPlugin.h"
// Drivers

#include "field-trip-protocol/ovasCDriverFieldtrip.h"
#include "generic-oscilator/ovasCDriverGenericOscilator.h"
#include "generic-sawtooth/ovasCDriverGenericSawTooth.h"
#include "generic-raw-reader/ovasCDriverGenericRawFileReader.h"
#include "generic-raw-reader/ovasCDriverGenericRawTelnetReader.h"
#include "brainmaster-discovery/ovasCDriverBrainmasterDiscovery.h"
#include "brainproducts-actichamp/ovasCDriverBrainProductsActiCHamp.h"
#include "brainproducts-brainampseries/ovasCDriverBrainProductsBrainampSeries.h"
#include "brainproducts-brainvisionrecorder/ovasCDriverBrainProductsBrainVisionRecorder.h"
#include "brainproducts-vamp/ovasCDriverBrainProductsVAmp.h"
#include "ctfvsm-meg/ovasCDriverCtfVsmMeg.h"
#include "egi-ampserver/ovasCDriverEGIAmpServer.h"
#include "emotiv-epoc/ovasCDriverEmotivEPOC.h"
#include "gtec-gmobilabplus/ovasCDriverGTecGMobiLabPlus.h"
#include "gtec-gusbamp/ovasCDriverGTecGUSBamp.h"
#include "micromed-systemplusevolution/ovasCDriverMicromedSystemPlusEvolution.h"
#include "mindmedia-nexus32b/ovasCDriverMindMediaNeXus32B.h"
#include "neurosky-mindset/ovasCDriverNeuroskyMindset.h"
#include "openeeg-modulareeg/ovasCDriverOpenEEGModularEEG.h"
#include "tmsi-refa32b/ovasCDriverTMSiRefa32B.h"
// #include "neuroscan-synamps2/ovasCDriverNeuroscanSynamps2.h"
#include "openal-mono16bit-audiocapture/ovasCDriverOpenALAudioCapture.h"

#include "mitsarEEG202A/ovasCDriverMitsarEEG202A.h"

#include <system/Memory.h>
#include <system/Time.h>
#include <limits>

#include <openvibe-toolkit/ovtk_all.h>

// Plugins

#include "plugins/external-stimulations/ovaspCExternalStimulations.h"


#include <fstream>
#include <sstream>

#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <cstring>

#include <cassert>
//

#define boolean OpenViBE::boolean

namespace
{
	// because std::tolower has multiple signatures,
	// it can not be easily used in std::transform
	// this workaround is taken from http://www.gcek.net/ref/books/sw/cpp/ticppv2/
	template <class charT>
	charT to_lower(charT c)
	{
		return std::tolower(c);
	}
};

#define OVAS_GUI_File            OpenViBE::Directories::getDataDir() + "/openvibe-applications/acquisition-server/interface.ui"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBEAcquisitionServer;
using namespace std;

//___________________________________________________________________//
//                                                                   //

static void button_preference_pressed_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CAcquisitionServerGUI*>(pUserData)->buttonPreferencePressedCB(pButton);
}

static void button_configure_pressed_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CAcquisitionServerGUI*>(pUserData)->buttonConfigurePressedCB(pButton);
}

static void button_connect_toggled_cb(::GtkToggleButton* pButton, void* pUserData)
{
	static_cast<CAcquisitionServerGUI*>(pUserData)->buttonConnectToggledCB(pButton);
}

static void button_start_pressed_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CAcquisitionServerGUI*>(pUserData)->buttonStartPressedCB(pButton);
}

static void button_stop_pressed_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CAcquisitionServerGUI*>(pUserData)->buttonStopPressedCB(pButton);
}

static void combobox_driver_changed_cb(::GtkComboBox* pComboBox, void* pUserData)
{
	static_cast<CAcquisitionServerGUI*>(pUserData)->comboBoxDriverChanged(pComboBox);
}

static void combobox_sample_count_per_sent_block_changed_cb(::GtkComboBox* pComboBox, void* pUserData)
{
	static_cast<CAcquisitionServerGUI*>(pUserData)->comboBoxSampleCountPerSentBlockChanged(pComboBox);
}

//___________________________________________________________________//
//                                                                   //

CAcquisitionServerGUI::CAcquisitionServerGUI(const IKernelContext& rKernelContext)
	:m_rKernelContext(rKernelContext)
	,m_pDriver(NULL)
	,m_pDriverContext(NULL)
	,m_pAcquisitionServer(NULL)
	,m_pAcquisitionServerThread(NULL)
	,m_pBuilderInterface(NULL)
	,m_pImpedanceWindow(NULL)
	,m_pThread(NULL)
{
	boolean l_bShowUnstable=m_rKernelContext.getConfigurationManager().expandAsBoolean("${AcquisitionServer_ShowUnstable}", false);

	m_pAcquisitionServer=new CAcquisitionServer(rKernelContext);

	m_vDriver.push_back(new CDriverGenericOscillator(m_pAcquisitionServer->getDriverContext()));
	m_vDriver.push_back(new CDriverGenericSawTooth(m_pAcquisitionServer->getDriverContext()));
	m_vDriver.push_back(new CDriverGenericRawFileReader(m_pAcquisitionServer->getDriverContext()));
	m_vDriver.push_back(new CDriverGenericRawTelnetReader(m_pAcquisitionServer->getDriverContext()));
	m_vDriver.push_back(new CDriverFieldtrip(m_pAcquisitionServer->getDriverContext()));
#if defined OVAS_OS_Windows
	m_vDriver.push_back(new CDriverMitsarEEG202A(m_pAcquisitionServer->getDriverContext()));
#if defined TARGET_HAS_ThirdPartyBrainmasterCodeMakerAPI
	m_vDriver.push_back(new CDriverBrainmasterDiscovery(m_pAcquisitionServer->getDriverContext()));
#endif
#if defined TARGET_HAS_ThirdPartyActiCHampAPI
	m_vDriver.push_back(new CDriverBrainProductsActiCHamp(m_pAcquisitionServer->getDriverContext()));
#endif
	m_vDriver.push_back(new CDriverBrainProductsBrainampSeries(m_pAcquisitionServer->getDriverContext()));
#endif
	m_vDriver.push_back(new CDriverBrainProductsBrainVisionRecorder(m_pAcquisitionServer->getDriverContext()));
#if defined TARGET_HAS_ThirdPartyUSBFirstAmpAPI
	m_vDriver.push_back(new CDriverBrainProductsVAmp(m_pAcquisitionServer->getDriverContext()));
#endif
	if(l_bShowUnstable) m_vDriver.push_back(new CDriverCtfVsmMeg(m_pAcquisitionServer->getDriverContext()));
	if(l_bShowUnstable) m_vDriver.push_back(new CDriverEGIAmpServer(m_pAcquisitionServer->getDriverContext()));
#if defined TARGET_HAS_ThirdPartyEmotivAPI
	m_vDriver.push_back(new CDriverEmotivEPOC(m_pAcquisitionServer->getDriverContext()));
#endif
#if defined TARGET_HAS_ThirdPartyGMobiLabPlusAPI
	if(l_bShowUnstable) m_vDriver.push_back(new CDriverGTecGMobiLabPlus(m_pAcquisitionServer->getDriverContext()));
#endif
#if defined TARGET_HAS_ThirdPartyGUSBampCAPI
	m_vDriver.push_back(new CDriverGTecGUSBamp(m_pAcquisitionServer->getDriverContext()));
#endif
#if defined OVAS_OS_Windows
	m_vDriver.push_back(new CDriverMicromedSystemPlusEvolution(m_pAcquisitionServer->getDriverContext()));
#endif
#if defined OVAS_OS_Windows
	m_vDriver.push_back(new CDriverMindMediaNeXus32B(m_pAcquisitionServer->getDriverContext()));
#endif
#if defined TARGET_HAS_ThirdPartyThinkGearAPI
	m_vDriver.push_back(new CDriverNeuroskyMindset(m_pAcquisitionServer->getDriverContext()));
#endif
	m_vDriver.push_back(new CDriverOpenEEGModularEEG(m_pAcquisitionServer->getDriverContext()));
#if defined OVAS_OS_Windows
	m_vDriver.push_back(new CDriverTMSiRefa32B(m_pAcquisitionServer->getDriverContext()));
#endif
	// if(l_bShowUnstable) m_vDriver.push_back(new CDriverNeuroscanSynamps2(m_pAcquisitionServer->getDriverContext()));
#if defined TARGET_HAS_ThirdPartyOpenAL
	m_vDriver.push_back(new CDriverOpenALAudioCapture(m_pAcquisitionServer->getDriverContext()));
#endif


	registerPlugin(new OpenViBEAcquisitionServerPlugins::CExternalStimulations(*m_pAcquisitionServer));

	scanPluginSettings();

	// Load plugin settings

	for (size_t setting_index = 0; setting_index < m_vPluginSettings.size(); ++setting_index)
	{
		PluginSetting* l_pCurrentSetting = m_vPluginSettings[setting_index].setting_ptr;

		if (m_rKernelContext.getConfigurationManager().lookUpConfigurationTokenIdentifier(m_vPluginSettings[setting_index].unique_name) != OV_UndefinedIdentifier)
		{
			CString l_sConfigurationNameExpression = CString("${" + m_vPluginSettings[setting_index].unique_name + "}");

			if (l_pCurrentSetting->type == OVTK_TypeId_Boolean)
			{
				l_pCurrentSetting->value = m_rKernelContext.getConfigurationManager().expandAsBoolean(l_sConfigurationNameExpression);
			}
			else if (l_pCurrentSetting->type == OVTK_TypeId_Integer)
			{
				l_pCurrentSetting->value = m_rKernelContext.getConfigurationManager().expandAsInteger(l_sConfigurationNameExpression);
			}
			else if (l_pCurrentSetting->type == OVTK_TypeId_String)
			{
				l_pCurrentSetting->value = m_rKernelContext.getConfigurationManager().expand(l_sConfigurationNameExpression);
			}
			else
			{
				// in the case where no valid type is defined we do nothing
			}
		}
	}

	m_pAcquisitionServerThread=new CAcquisitionServerThread(m_rKernelContext, *this, *m_pAcquisitionServer);

	// Initialize GTK objects as the thread started below may refer to them quickly
	this->initialize(); 

	m_pThread=new boost::thread(CAcquisitionServerThreadHandle(*m_pAcquisitionServerThread));
}

CAcquisitionServerGUI::~CAcquisitionServerGUI(void)
{
	m_pAcquisitionServerThread->terminate();
	m_pThread->join();

	// Saves current configuration
	FILE* l_pFile=::fopen(m_rKernelContext.getConfigurationManager().expand("${CustomConfigurationApplication}").toASCIIString(), "wt");
	if(l_pFile)
	{
		::fprintf(l_pFile, "# This file is generated\n");
		::fprintf(l_pFile, "# Do not modify\n");
		::fprintf(l_pFile, "\n");
		::fprintf(l_pFile, "# Last settings set in the acquisition server\n");
		::fprintf(l_pFile, "AcquisitionServer_LastDriver = %s\n", m_pDriver->getName());
		::fprintf(l_pFile, "AcquisitionServer_LastSampleCountPerBuffer = %i\n", this->getSampleCountPerBuffer());
		::fprintf(l_pFile, "AcquisitionServer_LastConnectionPort = %i\n", this->getTCPPort());
		::fprintf(l_pFile, "# Last Preferences set in the acquisition server\n");
		::fprintf(l_pFile, "AcquisitionServer_DriftCorrectionPolicy = %s\n", m_pAcquisitionServer->getDriftCorrectionPolicyStr().toASCIIString());
		::fprintf(l_pFile, "AcquisitionServer_JitterEstimationCountForDrift = %llu\n", m_pAcquisitionServer->getJitterEstimationCountForDrift());
		::fprintf(l_pFile, "AcquisitionServer_DriftToleranceDuration = %llu\n", m_pAcquisitionServer->getDriftToleranceDuration());
		::fprintf(l_pFile, "AcquisitionServer_OverSamplingFactor = %llu\n", m_pAcquisitionServer->getOversamplingFactor());
		::fprintf(l_pFile, "AcquisitionServer_CheckImpedance = %s\n", (m_pAcquisitionServer->isImpedanceCheckRequested() ? "True" : "False"));
		::fprintf(l_pFile, "AcquisitionServer_NaNReplacementPolicy = %s\n", m_pAcquisitionServer->getNaNReplacementPolicyStr().toASCIIString());
		::fprintf(l_pFile, "# Path to emotiv SDK\n");
		::fprintf(l_pFile, "AcquisitionServer_PathToEmotivResearchSDK = %s\n", (const char *)m_rKernelContext.getConfigurationManager().expand("${AcquisitionServer_PathToEmotivResearchSDK}"));

		for (size_t setting_index = 0; setting_index < m_vPluginSettings.size(); ++setting_index)
		{
			PluginSetting* l_pCurrentSetting = m_vPluginSettings[setting_index].setting_ptr;

			if (l_pCurrentSetting->type == OVTK_TypeId_Boolean)
			{
				::fprintf(l_pFile, m_vPluginSettings[setting_index].unique_name + " = %s\n", boost::get<bool>(l_pCurrentSetting->value) ? "True" : "False");
			}
			else if (l_pCurrentSetting->type == OVTK_TypeId_Integer)
			{
				::fprintf(l_pFile, m_vPluginSettings[setting_index].unique_name + " = %llu\n", boost::get<int64>(l_pCurrentSetting->value));
			}
			else if (l_pCurrentSetting->type == OVTK_TypeId_String)
			{
				::fprintf(l_pFile, m_vPluginSettings[setting_index].unique_name + " = %s\n", boost::get<CString>(l_pCurrentSetting->value).toASCIIString());
			}
			else
			{
				// in the case where no valid type is defined we do nothing
			}
		}


		::fclose(l_pFile);
	}

	vector<IDriver*>::iterator itDriver;
	for(itDriver=m_vDriver.begin(); itDriver!=m_vDriver.end(); itDriver++)
	{
		delete (*itDriver);
	}
	m_vDriver.clear();
	m_pDriver=NULL;

	delete m_pThread;
	m_pThread=NULL;

	delete m_pAcquisitionServerThread;
	m_pAcquisitionServerThread=NULL;

	delete m_pAcquisitionServer;
	m_pAcquisitionServer=NULL;
}

//___________________________________________________________________//
//                                                                   //

boolean CAcquisitionServerGUI::initialize(void)
{
	m_pBuilderInterface=gtk_builder_new(); // glade_xml_new(OVAS_GUI_File, NULL, NULL);
	gtk_builder_add_from_file(m_pBuilderInterface, OVAS_GUI_File, NULL);

	// Connects custom GTK signals

	g_signal_connect(gtk_builder_get_object(m_pBuilderInterface, "button_preference"),                    "pressed", G_CALLBACK(button_preference_pressed_cb), this);
	g_signal_connect(gtk_builder_get_object(m_pBuilderInterface, "button_configure"),                     "pressed", G_CALLBACK(button_configure_pressed_cb),  this);
	g_signal_connect(gtk_builder_get_object(m_pBuilderInterface, "togglebutton_connect"),                 "toggled", G_CALLBACK(button_connect_toggled_cb),    this);
	g_signal_connect(gtk_builder_get_object(m_pBuilderInterface, "button_play"),                          "pressed", G_CALLBACK(button_start_pressed_cb),      this);
	g_signal_connect(gtk_builder_get_object(m_pBuilderInterface, "button_stop"),                          "pressed", G_CALLBACK(button_stop_pressed_cb),       this);
	g_signal_connect(gtk_builder_get_object(m_pBuilderInterface, "combobox_driver"),                      "changed", G_CALLBACK(combobox_driver_changed_cb),   this);
	g_signal_connect(gtk_builder_get_object(m_pBuilderInterface, "combobox_sample_count_per_sent_block"), "changed", G_CALLBACK(combobox_sample_count_per_sent_block_changed_cb),  this);
	gtk_builder_connect_signals(m_pBuilderInterface, NULL);

	::GtkComboBox* l_pComboBoxDriver=GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderInterface, "combobox_driver"));

	enum
	{
		Resource_StringMarkup,
	};

	// Prepares drivers combo box

	gtk_combo_box_set_model(l_pComboBoxDriver, NULL);

	::GtkCellRenderer* l_pCellRendererName=gtk_cell_renderer_text_new();

	gtk_cell_layout_clear(GTK_CELL_LAYOUT(l_pComboBoxDriver));
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(l_pComboBoxDriver), l_pCellRendererName, TRUE);
	gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(l_pComboBoxDriver), l_pCellRendererName, "markup", Resource_StringMarkup);

	::GtkTreeStore* l_pDriverTreeStore=gtk_tree_store_new(1, G_TYPE_STRING);
	gtk_combo_box_set_model(l_pComboBoxDriver, GTK_TREE_MODEL(l_pDriverTreeStore));

	vector<IDriver*>::size_type i;
	string l_sDefaultDriverName=m_rKernelContext.getConfigurationManager().expand("${AcquisitionServer_DefaultDriver}").toASCIIString();
	transform(l_sDefaultDriverName.begin(), l_sDefaultDriverName.end(), l_sDefaultDriverName.begin(), ::to_lower<string::value_type>);
	for(i=0; i<m_vDriver.size(); i++)
	{
		::GtkTreeIter l_oIter;
		gtk_tree_store_append(l_pDriverTreeStore, &l_oIter, NULL);

		string l_sDriverName=m_vDriver[i]->getName();
		if(m_vDriver[i]->isFlagSet(DriverFlag_IsUnstable))
		{
			gtk_tree_store_set(l_pDriverTreeStore, &l_oIter,
				Resource_StringMarkup, (string("<span foreground=\"#6f6f6f\">")+l_sDriverName+string("</span> <span size=\"smaller\" style=\"italic\">(<span foreground=\"#202060\">unstable</span>)</span>")).c_str(),
				-1);
		}
		else
		{
			gtk_tree_store_set(l_pDriverTreeStore, &l_oIter,
				Resource_StringMarkup, (string("")+l_sDriverName+string("")).c_str(),
				-1);
		}

		transform(l_sDriverName.begin(), l_sDriverName.end(), l_sDriverName.begin(), ::to_lower<string::value_type>);
		if(l_sDefaultDriverName==l_sDriverName)
		{
			gtk_combo_box_set_active(l_pComboBoxDriver, i);
		}
	}
	if(gtk_combo_box_get_active(l_pComboBoxDriver)==-1)
	{
		gtk_combo_box_set_active(l_pComboBoxDriver, 0);
	}

	// Prepares sample count per buffer combo box

	boolean l_bFound=false;
	string l_sDefaultSampleCountPerBuffer=m_rKernelContext.getConfigurationManager().expand("${AcquisitionServer_DefaultSampleCountPerBuffer}").toASCIIString();
	::GtkComboBox* l_pComboBoxSampleCountPerBuffer=GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderInterface, "combobox_sample_count_per_sent_block"));
	for(int i=0; ; i++)
	{
		gtk_combo_box_set_active(l_pComboBoxSampleCountPerBuffer, i);
		if(gtk_combo_box_get_active(l_pComboBoxSampleCountPerBuffer)==-1)
		{
			break;
		}
		if(l_sDefaultSampleCountPerBuffer==gtk_combo_box_get_active_text(l_pComboBoxSampleCountPerBuffer))
		{
			l_bFound=true;
			break;
		}
	}
	if(!l_bFound)
	{
		if(l_sDefaultSampleCountPerBuffer != "-1")
		{
			gtk_combo_box_prepend_text(l_pComboBoxSampleCountPerBuffer, l_sDefaultSampleCountPerBuffer.c_str());
		}
		gtk_combo_box_set_active(l_pComboBoxSampleCountPerBuffer, 0);
	}

	// Prepares default connection port

	::GtkSpinButton* l_pSpinButtonConnectionPort=GTK_SPIN_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "spinbutton_connection_port"));
	uint64 l_ui64DefaultConnectionPort=m_rKernelContext.getConfigurationManager().expandAsUInteger("${AcquisitionServer_DefaultConnectionPort}", 1024);
	gtk_spin_button_set_value(l_pSpinButtonConnectionPort, (gdouble)l_ui64DefaultConnectionPort);

	// Shows main window

	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "openvibe-acquisition-server")));

	return true;
}

//___________________________________________________________________//
//                                                                   //

IDriver& CAcquisitionServerGUI::getDriver(void)
{
	return *m_pDriver;
}

uint32 CAcquisitionServerGUI::getSampleCountPerBuffer(void)
{
	return m_ui32SampleCountPerBuffer;
}

uint32 CAcquisitionServerGUI::getTCPPort(void)
{
	return ::gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(::gtk_builder_get_object(m_pBuilderInterface, "spinbutton_connection_port")));
}

IHeader& CAcquisitionServerGUI::getHeaderCopy(void)
{
	return m_oHeaderCopy;
}

void CAcquisitionServerGUI::setClientCount(uint32 ui32ClientCount)
{
	// Updates 'host count' label when needed
	char l_sLabel[1024];
	::sprintf(l_sLabel, "%u host%s connected...", (unsigned int)ui32ClientCount, ui32ClientCount?"s":"");
	::gtk_label_set_label(GTK_LABEL(gtk_builder_get_object(m_pBuilderInterface, "label_connected_host_count")), l_sLabel);
}

void CAcquisitionServerGUI::setDrift(float64 f64Drift)
{
	float64 l_f64DriftToleranceDuration=(float64)m_pAcquisitionServer->getDriftToleranceDuration();
	float64 l_f64DriftRatio=f64Drift/l_f64DriftToleranceDuration;
	boolean l_bDriftWarning=false;
	char l_sLabel[1024];

	// std::cout << f64Drift << " " << l_f64DriftRatio << "\n";

	if(l_f64DriftRatio<-1)
	{
		l_f64DriftRatio=-1;
		l_bDriftWarning=true;
	}

	if(l_f64DriftRatio>1)
	{
		l_f64DriftRatio=1;
		l_bDriftWarning=true;
	}

	if(l_f64DriftRatio<0)
	{
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(gtk_builder_get_object(m_pBuilderInterface, "progressbar_drift_1")), -l_f64DriftRatio);
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(gtk_builder_get_object(m_pBuilderInterface, "progressbar_drift_2")), 0);
	}
	else
	{
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(gtk_builder_get_object(m_pBuilderInterface, "progressbar_drift_1")), 0);
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(gtk_builder_get_object(m_pBuilderInterface, "progressbar_drift_2")), l_f64DriftRatio);
	}

	if(l_bDriftWarning)
	{
		::sprintf(l_sLabel, "<b>Device drift is too high</b> : %3.2lf ms\n<small>(tolerance is set to %3.2lf ms)</small>", f64Drift, l_f64DriftToleranceDuration);
	}
	else
	{
		::sprintf(l_sLabel, "Device drift : %3.2lf ms\n<small>(tolerance is set to %3.2lf ms)</small>", f64Drift, l_f64DriftToleranceDuration);
	}
	::gtk_label_set_markup(GTK_LABEL(gtk_builder_get_object(m_pBuilderInterface, "label_drift")), l_sLabel);
}

void CAcquisitionServerGUI::setImpedance(OpenViBE::uint32 ui32ChannelIndex, OpenViBE::float64 f64Impedance)
{
	if(m_pImpedanceWindow)
	{
		if(f64Impedance>=0)
		{
			//float64 l_dFraction=(f64Impedance*.001/20); With fixed impedance limit, 20kOhm max / 25%=5kOhm to be good
			float64 l_dFraction=(f64Impedance / (m_rKernelContext.getConfigurationManager().expandAsFloat("${AcquisitionServer_DefaultImpedanceLimit}",5000) * 4));
			if(l_dFraction>1) l_dFraction=1;

			char l_sMessage[1024];
			char l_sLabel[1024];
			char l_sImpedance[1024];
			char l_sStatus[1024];

			if(::strcmp(m_oHeaderCopy.getChannelName(ui32ChannelIndex), ""))
			{
				::strcpy(l_sLabel, m_oHeaderCopy.getChannelName(ui32ChannelIndex));
			}
			else
			{
				::sprintf(l_sLabel, "Channel %i", ui32ChannelIndex+1);
			}

			if(l_dFraction==1)
			{
				::sprintf(l_sImpedance, "Too high !");
			}
			else
			{
				::sprintf(l_sImpedance, "%.2f kOhm", f64Impedance*.001);
			}

			::sprintf(l_sStatus, "%s", l_dFraction<.25?"Good !":"Bad...");
			::sprintf(l_sMessage, "%s\n%s\n\n%s", l_sLabel, l_sImpedance, l_sStatus);

			::gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(m_vLevelMesure[ui32ChannelIndex]), l_dFraction);
			::gtk_progress_bar_set_text(GTK_PROGRESS_BAR(m_vLevelMesure[ui32ChannelIndex]), l_sMessage);
		}
		else
		{
			::gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(m_vLevelMesure[ui32ChannelIndex]), 0);
			if(f64Impedance==OVAS_Impedance_Unknown)
			{
				::gtk_progress_bar_set_text(GTK_PROGRESS_BAR(m_vLevelMesure[ui32ChannelIndex]), "Measuring...");
			}
			else if (f64Impedance==OVAS_Impedance_NotAvailable)
			{
				::gtk_progress_bar_set_text(GTK_PROGRESS_BAR(m_vLevelMesure[ui32ChannelIndex]), "n/a");
			}
			else
			{
				::gtk_progress_bar_set_text(GTK_PROGRESS_BAR(m_vLevelMesure[ui32ChannelIndex]), "Unknown");
			}
		}
	}
}

void CAcquisitionServerGUI::disconnect(void)
{
	::GtkToggleButton* l_pConnectButton=GTK_TOGGLE_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "togglebutton_connect"));

	if(gtk_toggle_button_get_active(l_pConnectButton))
	{
		gtk_toggle_button_set_active(l_pConnectButton, false);
	}
}

//___________________________________________________________________//
//                                                                   //

void CAcquisitionServerGUI::buttonConnectToggledCB(::GtkToggleButton* pButton)
{
	m_rKernelContext.getLogManager() << LogLevel_Debug << "buttonConnectToggledCB\n";

	if(gtk_toggle_button_get_active(pButton))
	{
		if(m_ui32SampleCountPerBuffer!=uint32(-1) && m_pAcquisitionServerThread->connect())
		{
			// Impedance window creation
			{
				uint64 l_ui64ColumnCount=m_rKernelContext.getConfigurationManager().expandAsInteger("${AcquisitionServer_CheckImpedance_ColumnCount}", 8);
				uint32 l_ui32LineCount=(uint32)(m_oHeaderCopy.getChannelCount()/l_ui64ColumnCount);
				uint32 l_ui32LastCount=(uint32)(m_oHeaderCopy.getChannelCount()%l_ui64ColumnCount);

				::GtkWidget* l_pTable=gtk_table_new((gint)(l_ui32LineCount+(l_ui32LastCount?1:0)), (gint)((l_ui32LineCount?l_ui64ColumnCount:l_ui32LastCount)), true);

				for(uint32 i=0; i<m_oHeaderCopy.getChannelCount(); i++)
				{
					uint32 j=(uint32)(i/l_ui64ColumnCount);
					uint32 k=(uint32)(i%l_ui64ColumnCount);
					::GtkWidget* l_pProgressBar=::gtk_progress_bar_new();
					::gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(l_pProgressBar), GTK_PROGRESS_BOTTOM_TO_TOP);
					::gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(l_pProgressBar), 0);
					::gtk_progress_bar_set_text(GTK_PROGRESS_BAR(l_pProgressBar), "n/a");
					::gtk_table_attach_defaults(GTK_TABLE(l_pTable), l_pProgressBar, k, k+1, j, j+1);
					m_vLevelMesure.push_back(l_pProgressBar);
				}

				m_pImpedanceWindow=::gtk_window_new(GTK_WINDOW_TOPLEVEL);
				::gtk_window_set_title(GTK_WINDOW(m_pImpedanceWindow), "Impedance check");
				::gtk_container_add(GTK_CONTAINER(m_pImpedanceWindow), l_pTable);
				if(m_pAcquisitionServer->isImpedanceCheckRequested())
				{
					::gtk_widget_show_all(m_pImpedanceWindow);
				}
			}

			gtk_button_set_label(GTK_BUTTON(pButton), "gtk-disconnect");

			gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "button_configure")), false);
			gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "button_preference")), false);
			gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "button_play")), true);
			gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "button_stop")), false);

			gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "spinbutton_connection_port")), false);
			gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "combobox_sample_count_per_sent_block")), false);
			gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "combobox_driver")), false);

			gtk_label_set_label(GTK_LABEL(gtk_builder_get_object(m_pBuilderInterface, "label_status")), "Connected ! Ready...");
			gtk_label_set_label(GTK_LABEL(gtk_builder_get_object(m_pBuilderInterface, "label_connected_host_count")), "0 host connected...");
		}
		else
		{
			if(m_ui32SampleCountPerBuffer==uint32(-1))
			{
				m_rKernelContext.getLogManager() << LogLevel_Warning << "Sample count per sent block is invalid.\n";
			}

			gtk_toggle_button_set_active(pButton, false);
		}
	}
	else
	{
		m_pAcquisitionServerThread->disconnect();

		if(m_pImpedanceWindow)
		{
			::gtk_widget_destroy(m_pImpedanceWindow);
			m_vLevelMesure.clear();
			m_pImpedanceWindow=NULL;
		}

		gtk_button_set_label(GTK_BUTTON(pButton), "gtk-connect");

		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "button_configure")), m_pDriver->isConfigurable());
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "button_preference")), true);
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "button_play")), false);
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "button_stop")), false);

		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "spinbutton_connection_port")), true);
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "combobox_sample_count_per_sent_block")), true);
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "combobox_driver")), true);

		gtk_label_set_label(GTK_LABEL(gtk_builder_get_object(m_pBuilderInterface, "label_status")), "");
		gtk_label_set_label(GTK_LABEL(gtk_builder_get_object(m_pBuilderInterface, "label_connected_host_count")), "");
	}
}

void CAcquisitionServerGUI::buttonStartPressedCB(::GtkButton* pButton)
{
	m_rKernelContext.getLogManager() << LogLevel_Debug << "buttonStartPressedCB\n";

	if(m_pAcquisitionServerThread->start())
	{
		if(m_pImpedanceWindow)
		{
			::gtk_widget_hide(m_pImpedanceWindow);
		}

		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "button_play")), false);
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "button_stop")), true);

		gtk_label_set_label(GTK_LABEL(gtk_builder_get_object(m_pBuilderInterface, "label_status")), "Sending...");
	}
	else
	{
		gtk_label_set_label(GTK_LABEL(gtk_builder_get_object(m_pBuilderInterface, "label_status")), "Failed !");
		gtk_label_set_label(GTK_LABEL(gtk_builder_get_object(m_pBuilderInterface, "label_connected_host_count")), "");
	}
}

void CAcquisitionServerGUI::buttonStopPressedCB(::GtkButton* pButton)
{
	m_rKernelContext.getLogManager() << LogLevel_Debug << "buttonStopPressedCB\n";

	if(m_pAcquisitionServerThread->stop())
	{
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "button_play")), true);
		gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "button_stop")), false);

		gtk_label_set_label(GTK_LABEL(gtk_builder_get_object(m_pBuilderInterface, "label_status")), "Connected ! Ready...");
	}
	else
	{
		gtk_label_set_label(GTK_LABEL(gtk_builder_get_object(m_pBuilderInterface, "label_status")), "Failed !");
		gtk_label_set_label(GTK_LABEL(gtk_builder_get_object(m_pBuilderInterface, "label_connected_host_count")), "");
	}
}

void CAcquisitionServerGUI::buttonPreferencePressedCB(::GtkButton* pButton)
{
	m_rKernelContext.getLogManager() << LogLevel_Debug << "buttonPreferencePressedCB\n";

	::GtkBuilder* l_pInterface=gtk_builder_new();
	::gtk_builder_add_from_file(l_pInterface, OVAS_GUI_File, NULL);
	::GtkDialog* l_pDialog=GTK_DIALOG(::gtk_builder_get_object(l_pInterface, "openvibe-acquisition-server-configuration"));
	::GtkComboBox* l_pDriftCorrectionPolicy=GTK_COMBO_BOX(::gtk_builder_get_object(l_pInterface, "combobox_drift_correction"));
	::GtkComboBox* l_pNaNReplacementPolicy=GTK_COMBO_BOX(::gtk_builder_get_object(l_pInterface, "combobox_nan_replacement"));
	::GtkSpinButton* l_pDriftTolerance=GTK_SPIN_BUTTON(::gtk_builder_get_object(l_pInterface, "spinbutton_drift_tolerance"));
	::GtkSpinButton* l_pJitterMeasureCount=GTK_SPIN_BUTTON(::gtk_builder_get_object(l_pInterface, "spinbutton_jitter_measure_count"));
	::GtkSpinButton* l_pOverSamplingFactor=GTK_SPIN_BUTTON(::gtk_builder_get_object(l_pInterface, "spinbutton_oversampling_factor"));
	::GtkToggleButton* l_pImpedanceCheck=GTK_TOGGLE_BUTTON(::gtk_builder_get_object(l_pInterface, "checkbutton_impedance"));

	::gtk_combo_box_set_active(l_pDriftCorrectionPolicy, (int)m_pAcquisitionServer->getDriftCorrectionPolicy());
	::gtk_spin_button_set_value(l_pDriftTolerance, (gdouble)m_pAcquisitionServer->getDriftToleranceDuration());
	::gtk_spin_button_set_value(l_pJitterMeasureCount, (gdouble)m_pAcquisitionServer->getJitterEstimationCountForDrift());
	::gtk_spin_button_set_value(l_pOverSamplingFactor, (gdouble)m_pAcquisitionServer->getOversamplingFactor());
	::gtk_toggle_button_set_active(l_pImpedanceCheck, m_pAcquisitionServer->isImpedanceCheckRequested()?TRUE:FALSE);
	::gtk_combo_box_set_active(l_pNaNReplacementPolicy, (int)m_pAcquisitionServer->getNaNReplacementPolicy());

	// Load the settings for the plugins

	::GtkTable* l_pSettingsTable = GTK_TABLE(::gtk_builder_get_object(l_pInterface, "table-pluginsettings"));

	gtk_table_resize(l_pSettingsTable, m_vPluginSettings.size(), 2);

	for (size_t setting_index = 0; setting_index < m_vPluginSettings.size(); ++setting_index)
	{
		const PluginSetting* l_pCurrentSetting = m_vPluginSettings[setting_index].setting_ptr;

		// Create label
		::GtkWidget* l_pSettingLabel = gtk_label_new(m_vPluginSettings[setting_index].setting_name);

		// Create the setting controller widget
		::GtkWidget* l_pSettingControl;

		if (l_pCurrentSetting->type == OVTK_TypeId_Boolean)
		{
			// create the check button and assign it the current value of the setting
			l_pSettingControl = gtk_check_button_new();
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(l_pSettingControl), boost::get<boolean>(l_pCurrentSetting->value));
		}
		else if (l_pCurrentSetting->type == OVTK_TypeId_Integer)
		{
			// create the check button and assign it the current value of the setting

			l_pSettingControl = gtk_spin_button_new_with_range(std::numeric_limits<int64>::min(), std::numeric_limits<int64>::max(), 1);
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(l_pSettingControl), boost::get<int64>(l_pCurrentSetting->value));

		}
		else if (l_pCurrentSetting->type == OVTK_TypeId_String)
		{
			// create the check button and assign it the current value of the setting
			l_pSettingControl = gtk_entry_new();
			gtk_entry_append_text(GTK_ENTRY(l_pSettingControl), boost::get<OpenViBE::CString>(l_pCurrentSetting->value));
		}
		else
		{
			// in the case where no valid type is defined we create a placeholder label
			l_pSettingControl = gtk_label_new("Undefined Type");
		}

		// insert the settings into the table
		gtk_table_attach_defaults(l_pSettingsTable, l_pSettingLabel, 0, 1, setting_index, setting_index+1);
		gtk_table_attach_defaults(l_pSettingsTable, l_pSettingControl, 1, 2, setting_index, setting_index+1);

		m_vPluginSettings[setting_index].gui_widget = l_pSettingControl;
		gtk_widget_show(l_pSettingLabel);
		gtk_widget_show(l_pSettingControl);
	}

	
	gint l_iResponseId=::gtk_dialog_run(l_pDialog);
	switch(l_iResponseId)
	{
		case GTK_RESPONSE_APPLY:
		case GTK_RESPONSE_OK:
		case GTK_RESPONSE_YES:
			m_pAcquisitionServer->setNaNReplacementPolicy((ENaNReplacementPolicy)::gtk_combo_box_get_active(l_pNaNReplacementPolicy));
			m_pAcquisitionServer->setDriftCorrectionPolicy((EDriftCorrectionPolicy)::gtk_combo_box_get_active(l_pDriftCorrectionPolicy));
			m_pAcquisitionServer->setDriftToleranceDuration(::gtk_spin_button_get_value_as_int(l_pDriftTolerance));
			m_pAcquisitionServer->setJitterEstimationCountForDrift(::gtk_spin_button_get_value_as_int(l_pJitterMeasureCount));
			m_pAcquisitionServer->setOversamplingFactor(::gtk_spin_button_get_value_as_int(l_pOverSamplingFactor));
			m_pAcquisitionServer->setImpedanceCheckRequest(::gtk_toggle_button_get_active(l_pImpedanceCheck)?true:false);

			for (size_t setting_index = 0; setting_index < m_vPluginSettings.size(); ++setting_index)
			{
				PluginSetting* l_pCurrentSetting = m_vPluginSettings[setting_index].setting_ptr;

				if (l_pCurrentSetting->type == OVTK_TypeId_Boolean)
				{
					l_pCurrentSetting->value = ::gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(m_vPluginSettings[setting_index].gui_widget)) ? true : false;
				}
				else if (l_pCurrentSetting->type == OVTK_TypeId_Integer)
				{
					l_pCurrentSetting->value = static_cast<int64>(::gtk_spin_button_get_value(GTK_SPIN_BUTTON(m_vPluginSettings[setting_index].gui_widget)));
				}
				else if (l_pCurrentSetting->type == OVTK_TypeId_String)
				{
					l_pCurrentSetting->value = CString(::gtk_entry_get_text(GTK_ENTRY(m_vPluginSettings[setting_index].gui_widget)));
				}
				else
				{
					// in the case where no valid type is defined we do nothing
				}
			}


			break;
		case GTK_RESPONSE_CANCEL:
		case GTK_RESPONSE_NO:
			break;
	}
	::gtk_widget_destroy(GTK_WIDGET(l_pDialog));
	g_object_unref(l_pInterface);
}

void CAcquisitionServerGUI::buttonConfigurePressedCB(::GtkButton* pButton)
{
	m_rKernelContext.getLogManager() << LogLevel_Debug << "buttonConfigurePressedCB\n";

	if(m_pDriver->isConfigurable())
	{
		m_pDriver->configure();
	}
}

void CAcquisitionServerGUI::comboBoxDriverChanged(::GtkComboBox* pComboBox)
{
	m_rKernelContext.getLogManager() << LogLevel_Debug << "comboBoxDriverChanged\n";
	m_pDriver=m_vDriver[gtk_combo_box_get_active(pComboBox)];
	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "button_configure")), m_pDriver->isConfigurable());
}

void CAcquisitionServerGUI::comboBoxSampleCountPerSentBlockChanged(::GtkComboBox* pComboBox)
{
	int l_iSampleCountPerSentBlock=0;
	m_rKernelContext.getLogManager() << LogLevel_Debug << "comboBoxSampleCountPerSentBlockChanged\n";
	if(::sscanf(::gtk_combo_box_get_active_text(GTK_COMBO_BOX(::gtk_builder_get_object(m_pBuilderInterface, "combobox_sample_count_per_sent_block"))), "%i", &l_iSampleCountPerSentBlock)==1 && l_iSampleCountPerSentBlock>0)
	{
		m_ui32SampleCountPerBuffer=uint32(l_iSampleCountPerSentBlock);
	}
	else
	{
		m_ui32SampleCountPerBuffer=uint32(-1);
	}
}

void CAcquisitionServerGUI::registerPlugin(IAcquisitionServerPlugin* plugin)
{
	if (m_pAcquisitionServer != NULL)
	{
		m_pAcquisitionServer->m_vPlugins.push_back(plugin);
	}
}

/**
  * \brief This function scans all registered plugins for settings.
  *
  * All of the plugins are inserted into a vector containing the pointer to the actual settings structure
  * along with a unique name for settings.
  */
void CAcquisitionServerGUI::scanPluginSettings()
{
	vector<IAcquisitionServerPlugin*> l_vPlugins = m_pAcquisitionServer->getPlugins();

	for(std::vector<IAcquisitionServerPlugin*>::iterator itp = l_vPlugins.begin(); itp != l_vPlugins.end(); ++itp)
	{
		IAcquisitionServerPlugin* l_pPlugin = dynamic_cast<IAcquisitionServerPlugin*>(*itp);

		typedef std::map<OpenViBE::CString, PluginSetting> PluginSettingMap;
		PluginSettingMap& l_rSettings = l_pPlugin->getProperties().settings;

		// Iterate over the settings of the plugin
		for (PluginSettingMap::iterator settings_it = l_rSettings.begin(); settings_it != l_rSettings.end(); ++settings_it)
		{
			// Creates a setting reference, containing the name of the plugin (not used yet, except for the creation of the unique name)
			// the name of the setting and a pointer to the setting in the plugin itself
			PluginSettingReference l_sSettingReference = PluginSettingReference(
				std::string(l_pPlugin->getProperties().name.toASCIIString()),
				std::string((*settings_it).first.toASCIIString()),
				&((*settings_it).second));

			m_vPluginSettings.push_back(l_sSettingReference);
		}

	}


}
