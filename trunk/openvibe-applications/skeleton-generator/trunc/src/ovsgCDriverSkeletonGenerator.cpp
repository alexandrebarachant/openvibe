#include "ovsgCDriverSkeletonGenerator.h"

#include <iostream>
#include <sstream>
//#include <direct.h>

#include <glib/gstdio.h>
#include <cstdio>

#include <boost/regex.hpp>

#include <time.h>

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBESkeletonGenerator;

//-----------------------------------------------------------------------
static void button_check_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CDriverSkeletonGenerator*>(pUserData)->buttonCheckCB();
}
static void button_tooltip_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CDriverSkeletonGenerator*>(pUserData)->buttonTooltipCB(pButton);
}
static void button_ok_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CDriverSkeletonGenerator*>(pUserData)->buttonOkCB();
}
//-----------------------------------------------------------------------
void CDriverSkeletonGenerator::buttonCheckCB()
{
	//Author and Company
	getCommon();

	m_rKernelContext.getLogManager() << LogLevel_Info << "Checking values... \n";

	boolean l_bSuccess = true;

	stringstream l_ssTextBuffer;
	l_ssTextBuffer << "----- STATUS -----\n";

	//-------------------------------------------------------------------------------------------------------------------------------------------//
	::GtkWidget * l_pEntryDriverName = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "entry_driver_name"));
	m_sDriverName = gtk_entry_get_text(GTK_ENTRY(l_pEntryDriverName));
	const boost::regex l_RegExpDriverName("([a-z]|[A-Z]|[1-9])+([a-z]|[A-Z]|[1-9]|[ \t\r\n]|[\\.-_\\(\\)])*",boost::regex::perl);
	if(boost::regex_match(string(m_sDriverName),l_RegExpDriverName) == false)
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "-- Driver Name: INVALID\n";
		l_bSuccess = false;
		l_ssTextBuffer << "[FAILED] Invalid driver name. Please use only characters (lower or uppercase) and numbers (blanck allowed).\n";
	}
	else
	{
		m_rKernelContext.getLogManager() << LogLevel_Info << "-- Driver Name: VALID (" << (const char *)m_sDriverName << ")\n";
		l_ssTextBuffer << "[   OK   ] Valid driver name.\n";
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	::GtkWidget * l_pEntryClassName = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "entry_class_name"));
	m_sClassName = gtk_entry_get_text(GTK_ENTRY(l_pEntryClassName));
	const boost::regex l_RegExpClassName("([a-z]|[A-Z]|[1-9])+",boost::regex::perl);
	if(boost::regex_match(string(m_sClassName),l_RegExpClassName) == false)
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "-- Class Name: INVALID\n";
		l_bSuccess = false;
		l_ssTextBuffer << "[FAILED] Invalid class name. Please use only characters (lower or uppercase) and numbers  (no blanck allowed).\n";
	}
	else
	{
		m_rKernelContext.getLogManager() << LogLevel_Info << "-- Class Name: VALID (" << (const char *)m_sClassName << ")\n";
		l_ssTextBuffer << "[   OK   ] Valid class name.\n";
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	::GtkWidget * l_pSpinbuttonMinChannel = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "spinbutton_min_channel"));
	stringstream ss1,ss2;
	ss1 << (uint32) gtk_spin_button_get_value(GTK_SPIN_BUTTON(l_pSpinbuttonMinChannel));
	m_sMinChannel = CString(ss1.str().c_str());
	::GtkWidget * l_pSpinbuttonMaxChannel = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "spinbutton_max_channel"));
	ss2 << (uint32) gtk_spin_button_get_value(GTK_SPIN_BUTTON(l_pSpinbuttonMaxChannel));
	m_sMaxChannel = CString(ss2.str().c_str());
	if(gtk_spin_button_get_value(GTK_SPIN_BUTTON(l_pSpinbuttonMinChannel)) > gtk_spin_button_get_value(GTK_SPIN_BUTTON(l_pSpinbuttonMaxChannel)))
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "-- Channels: INVALID\n";
		l_bSuccess = false;
		l_ssTextBuffer << "[FAILED] Invalid channel count. Be sure that Min <= Max.\n";
	}
	else
	{
		m_rKernelContext.getLogManager() << LogLevel_Info << "-- Channels: VALID (" << (const char *)m_sMinChannel << "/" << (const char *)m_sMaxChannel << ")\n";
		l_ssTextBuffer << "[   OK   ] Valid channel count.\n";
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	::GtkWidget * l_pEntrySF = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "entry_sampling_frequencies"));
	CString l_sSamplingFrequencies = gtk_entry_get_text(GTK_ENTRY(l_pEntrySF));
	const boost::regex l_RegExpSamplingFrequencies("(([1-9][0-9]*);)*([1-9][0-9]*)",boost::regex::perl);
	if(boost::regex_match(string(l_sSamplingFrequencies),l_RegExpSamplingFrequencies) == false)
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "-- Sampling frequencies: INVALID\n";
		l_bSuccess = false;
		l_ssTextBuffer << "[FAILED] Invalid sampling frequencies. Please use only whole numbers separated with ';' (no blanck allowed).\n";
	}
	else
	{
		// Maximum 16 frequencies
		int l_pSF[16];
		uint32 l_ui32SamplingFrequencyCount =  sscanf((const char *)l_sSamplingFrequencies,
			"%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d",
			&l_pSF[0],&l_pSF[1],&l_pSF[2],&l_pSF[3],&l_pSF[4],&l_pSF[5],&l_pSF[6],&l_pSF[7],
			&l_pSF[8],&l_pSF[9],&l_pSF[10],&l_pSF[11],&l_pSF[12],&l_pSF[13],&l_pSF[14],&l_pSF[15]);

		m_rKernelContext.getLogManager() << LogLevel_Info << "-- Sampling frequencies: VALID\n";
		m_vSamplingFrequencies.clear();
		for(unsigned int i = 0;i<l_ui32SamplingFrequencyCount;i++)
		{
			stringstream ss;
			ss << l_pSF[i];
			m_vSamplingFrequencies.push_back( CString(ss.str().c_str()) );
			printf("- %s Hz\n", ss.str().c_str());
		}

		l_ssTextBuffer << "[   OK   ] " << l_ui32SamplingFrequencyCount << " valid sampling frequencie(s).\n";
		m_sSamplingFrequencies = CString(l_sSamplingFrequencies);
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	::GtkWidget * l_pFileChooser = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "filechooserbutton_target_directory"));
	char * l_pTargetDirectory = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(l_pFileChooser));
	m_sTargetDirectory = CString(l_pTargetDirectory);
	g_free(l_pTargetDirectory);

#ifdef OV_OS_Windows
	string space("%20");
	if(((string)m_sTargetDirectory).rfind(space) != string::npos)
	{
		l_ssTextBuffer << "[FAILED] Invalid destination folder :" << (const char *)m_sTargetDirectory << ".\n";
		m_rKernelContext.getLogManager() << LogLevel_Error << "Invalid destination folder :" << (const char *)m_sTargetDirectory << ".\n";
		l_bSuccess = false;
	}
	else
#endif
	{
		m_rKernelContext.getLogManager() << LogLevel_Info << "-- Target directory: " << (const char *)m_sTargetDirectory << "\n";
		l_ssTextBuffer << "[   OK   ] Valid target directory: " << (const char *)m_sTargetDirectory << "\n";
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	::GtkWidget * l_pTooltipTextview = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-driver-tooltips-textview"));
	::GtkTextBuffer * l_pTextBuffer  = gtk_text_view_get_buffer(GTK_TEXT_VIEW(l_pTooltipTextview));
	if(l_bSuccess)
	{
		l_ssTextBuffer << "----- SUCCESS -----\nPress OK to generate the files. If you want to modify your choice(s), please press the \"Check\" button again.";
		::GtkWidget * l_pButtonOk = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-driver-ok-button"));
		gtk_widget_set_sensitive(l_pButtonOk,true);
	}
	else
	{
		l_ssTextBuffer << "----- PROCESS FAILED -----\nModify your choices and press the \"Check\" button again.";
		::GtkWidget * l_pButtonOk = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-driver-ok-button"));
		gtk_widget_set_sensitive(l_pButtonOk,false);
	}

	gtk_text_buffer_set_text (l_pTextBuffer,
		l_ssTextBuffer.str().c_str()
		, -1);
}

void CDriverSkeletonGenerator::buttonOkCB()
{

	m_rKernelContext.getLogManager() << LogLevel_Info << "Generating files... \n";
	boolean l_bSuccess = true;

	::GtkWidget * l_pTooltipTextview = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-driver-tooltips-textview"));
	::GtkTextBuffer * l_pTextBuffer  = gtk_text_view_get_buffer(GTK_TEXT_VIEW(l_pTooltipTextview));

	stringstream l_ssTextBuffer;
	l_ssTextBuffer << "Generating files...\n";
	gtk_text_buffer_set_text (l_pTextBuffer,
		l_ssTextBuffer.str().c_str(),
		-1);

	CString l_sSed;
#ifdef OV_OS_Windows
		l_sSed = "..\\share\\openvibe-applications\\skeleton-generator\\sed";
#else
#ifdef OV_OS_Linux
		l_sSed = "sed";
#endif
#endif

	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	stringstream ssTime;
	string string_time(asctime (timeinfo));
	string_time = string_time.substr(0,string_time.size()-1); // the ascitime ends with a "\n"
	CString l_sDate(string_time.c_str());

	m_rKernelContext.getLogManager() << LogLevel_Info << "AUTHOR: " << m_sAuthor << "\n";
	m_rKernelContext.getLogManager() << LogLevel_Info << "COMPANY: " << m_sCompany << "\n";
	m_rKernelContext.getLogManager() << LogLevel_Info << "DRIVER: " << m_sDriverName << "\n";
	m_rKernelContext.getLogManager() << LogLevel_Info << "CLASS: " << m_sClassName << "\n";
	m_rKernelContext.getLogManager() << LogLevel_Info << "MINCHAN: " << m_sMinChannel << "\n";
	m_rKernelContext.getLogManager() << LogLevel_Info << "MAXCHAN: " << m_sMaxChannel << "\n";
	m_rKernelContext.getLogManager() << LogLevel_Info << "DATE: " << l_sDate << "\n";
	m_rKernelContext.getLogManager() << LogLevel_Info << "Target directory: " << m_sTargetDirectory << "\n";

	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// driver.h
	// we check if the skeleton is in place.
	const gchar* l_sDriverHSkel="../share/openvibe-applications/skeleton-generator/driver.h-skeleton";
	if(! g_file_test(l_sDriverHSkel, G_FILE_TEST_EXISTS))
	{
		l_ssTextBuffer << "[FAILED] the file 'driver.h-skeleton' is missing.\n";
		m_rKernelContext.getLogManager() << LogLevel_Error << "Driver: the file 'driver.h-skeleton' is missing.\n";
		l_bSuccess = false;
	}
	else
	{
		l_ssTextBuffer << "[   OK   ] -- 'driver.h-skeleton' found.\n";
		gtk_text_buffer_set_text (l_pTextBuffer,
			l_ssTextBuffer.str().c_str()
			, -1);
		m_rKernelContext.getLogManager() << LogLevel_Info << " -- 'driver.h-skeleton' found.\n";

		//Using GNU sed for parsing and replacing tags
		CString l_sDest = m_sTargetDirectory + "/ovasCDriver" + m_sClassName + ".h";

		CString l_sCommandSed = l_sSed;
		l_sCommandSed = l_sCommandSed + " \"s/@@AuthorName@@/"+m_sAuthor+"/g";
		l_sCommandSed = l_sCommandSed + ";s/@@CompanyName@@/"+m_sCompany+"/g";
		l_sCommandSed = l_sCommandSed + ";s/@@Date@@/"+l_sDate+"/g";
		l_sCommandSed = l_sCommandSed + ";s/@@DriverName@@/"+m_sDriverName+"/g";
		l_sCommandSed = l_sCommandSed + ";s/@@ClassName@@/"+m_sClassName+"/g\" ";
		m_rKernelContext.getLogManager() << LogLevel_Debug << "CMD: "+l_sCommandSed+"\n";

		// execute the sed command !
		l_sCommandSed = l_sCommandSed + CString("\"") + CString(l_sDriverHSkel) + CString("\" > \"") + l_sDest + CString("\"");

		m_rKernelContext.getLogManager() << LogLevel_Trace << "Invoking [" << l_sCommandSed << "]...\n";

		l_bSuccess &= (system(((string)l_sCommandSed).c_str()) == 0);

		if(l_bSuccess)
		{
			l_ssTextBuffer << "[   OK   ] -- " << l_sDest << " written.\n";
			m_rKernelContext.getLogManager() << LogLevel_Info << " -- " << l_sDest << " written.\n";
		}
		else
		{
			l_ssTextBuffer << "[FAILED] -- " << l_sDest << " cannot be written.\n";
			m_rKernelContext.getLogManager() << LogLevel_Error << " -- " << l_sDest << " cannot be written.\n";
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// driver.cpp
	// we check if the skeleton is in place.
	const gchar* l_sDriverCppSkel="../share/openvibe-applications/skeleton-generator/driver.cpp-skeleton";
	if(! g_file_test(l_sDriverCppSkel, G_FILE_TEST_EXISTS))
	{
		l_ssTextBuffer << "[FAILED] the file 'driver.cpp-skeleton' is missing.\n";
		m_rKernelContext.getLogManager() << LogLevel_Error << "Driver: the file 'driver.cpp-skeleton' is missing.\n";
		l_bSuccess = false;
	}
	else
	{
		l_ssTextBuffer << "[   OK   ] -- 'driver.cpp-skeleton' found.\n";
		m_rKernelContext.getLogManager() << LogLevel_Info << " -- 'driver.cpp-skeleton' found.\n";

		//Using GNU sed for parsing and replacing tags
		CString l_sDest = m_sTargetDirectory + "/ovasCDriver" + m_sClassName + ".cpp";

		CString l_sCommandSed = l_sSed;
		l_sCommandSed = l_sCommandSed + " \"s/@@DriverName@@/"+m_sDriverName+"/g";
		l_sCommandSed = l_sCommandSed + ";s/@@ClassName@@/"+m_sClassName+"/g";
		l_sCommandSed = l_sCommandSed + ";s/@@SamplingFrequency@@/"+ m_vSamplingFrequencies[0]+"/g";
		l_sCommandSed = l_sCommandSed + ";s/@@MaxChannel@@/"+m_sMaxChannel+"/g\" ";
		m_rKernelContext.getLogManager() << LogLevel_Debug << "CMD: "+l_sCommandSed+"\n";

		// execute the sed command !
//#ifdef OV_OS_Windows
//		l_sDriverCppSkel = "driver.cpp-skeleton";
//#endif
		l_sCommandSed = l_sCommandSed + CString("\"") + CString(l_sDriverCppSkel) + CString("\" > \"") + l_sDest + CString("\"");

		m_rKernelContext.getLogManager() << LogLevel_Trace << "Invoking [" << l_sCommandSed << "]...\n";

		l_bSuccess &= (system(((string)l_sCommandSed).c_str()) == 0);

		if(l_bSuccess)
		{
			l_ssTextBuffer << "[   OK   ] -- " << l_sDest << " written.\n";
			m_rKernelContext.getLogManager() << LogLevel_Info << " -- " << l_sDest << " written.\n";
		}
		else
		{
			l_ssTextBuffer << "[FAILED] -- " << l_sDest << " cannot be written.\n";
			m_rKernelContext.getLogManager() << LogLevel_Error << " -- " << l_sDest << " cannot be written.\n";
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// config.h
	// we check if the skeleton is in place.
	const gchar* l_sConfigurationHSkel="../share/openvibe-applications/skeleton-generator/configuration.h-skeleton";
	if(! g_file_test(l_sConfigurationHSkel, G_FILE_TEST_EXISTS))
	{
		l_ssTextBuffer << "[FAILED] the file 'configuration.h-skeleton' is missing.\n";
		m_rKernelContext.getLogManager() << LogLevel_Error << "Driver: the file 'configuration.h-skeleton' is missing.\n";
		l_bSuccess = false;
	}
	else
	{
		l_ssTextBuffer << "[   OK   ] -- 'configuration.h-skeleton' found.\n";
		m_rKernelContext.getLogManager() << LogLevel_Info << " -- 'configuration.h-skeleton' found.\n";

		//Using GNU sed for parsing and replacing tags
		CString l_sDest = m_sTargetDirectory + "/ovasCConfiguration" + m_sClassName + ".h";

		CString l_sCommandSed = l_sSed;
		l_sCommandSed = l_sCommandSed + " \"s/@@AuthorName@@/"+m_sAuthor+"/g";
		l_sCommandSed = l_sCommandSed + ";s/@@CompanyName@@/"+m_sCompany+"/g";
		l_sCommandSed = l_sCommandSed + ";s/@@Date@@/"+l_sDate+"/g";
		l_sCommandSed = l_sCommandSed + ";s/@@DriverName@@/"+m_sDriverName+"/g";
		l_sCommandSed = l_sCommandSed + ";s/@@ClassName@@/"+m_sClassName+"/g";
		l_sCommandSed = l_sCommandSed + ";s/@@SamplingFrequency@@/"+m_vSamplingFrequencies[0]+"/g";
		l_sCommandSed = l_sCommandSed + ";s/@@MaxChannel@@/"+m_sMaxChannel+"/g\" ";
		m_rKernelContext.getLogManager() << LogLevel_Debug << "CMD: "+l_sCommandSed+"\n";

		// execute the sed command !
//#ifdef OV_OS_Windows
//		l_sConfigurationHSkel = "configuration.h-skeleton";
//#endif
		l_sCommandSed = l_sCommandSed + CString("\"") + CString(l_sConfigurationHSkel) + CString("\" > \"") + l_sDest + CString("\"");

		m_rKernelContext.getLogManager() << LogLevel_Trace << "Invoking [" << l_sCommandSed << "]...\n";

		l_bSuccess &= (system(((string)l_sCommandSed).c_str()) == 0);

		if(l_bSuccess)
		{
			l_ssTextBuffer << "[   OK   ] -- " << l_sDest << " written.\n";
			m_rKernelContext.getLogManager() << LogLevel_Info << " -- " << l_sDest << " written.\n";
		}
		else
		{
			l_ssTextBuffer << "[FAILED] -- " << l_sDest << " cannot be written.\n";
			m_rKernelContext.getLogManager() << LogLevel_Error << " -- " << l_sDest << " cannot be written.\n";
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// config.cpp
	// we check if the skeleton is in place.
	const gchar* l_sConfigurationCppSkel="../share/openvibe-applications/skeleton-generator/configuration.cpp-skeleton";
	if(! g_file_test(l_sConfigurationCppSkel, G_FILE_TEST_EXISTS))
	{
		l_ssTextBuffer << "[FAILED] the file 'configuration.cpp-skeleton' is missing.\n";
		m_rKernelContext.getLogManager() << LogLevel_Error << "the file 'configuration.cpp-skeleton' is missing.\n";
		l_bSuccess = false;
	}
	else
	{
		l_ssTextBuffer << "[   OK   ] -- 'configuration.cpp-skeleton' found.\n";
		m_rKernelContext.getLogManager() << LogLevel_Info << " -- 'configuration.cpp-skeleton' found.\n";

		//Using GNU sed for parsing and replacing tags
		CString l_sDest = m_sTargetDirectory + "/ovasCConfiguration" + m_sClassName + ".cpp";

		CString l_sCommandSed = l_sSed;
		l_sCommandSed = l_sCommandSed + " \"s/@@ClassName@@/"+m_sClassName+"/g";
		l_sCommandSed = l_sCommandSed + ";s/@@MaxChannel@@/"+m_sMaxChannel+"/g\" ";
		m_rKernelContext.getLogManager() << LogLevel_Debug << "CMD: "+l_sCommandSed+"\n";

		// execute the sed command !
//#ifdef OV_OS_Windows
//		l_sConfigurationCppSkel = "configuration.cpp-skeleton";
//#endif
		l_sCommandSed = l_sCommandSed + CString("\"") + CString(l_sConfigurationCppSkel) + + CString("\" > \"") + l_sDest + CString("\"");

		m_rKernelContext.getLogManager() << LogLevel_Trace << "Invoking [" << l_sCommandSed << "]...\n";

		l_bSuccess &= (system(((string)l_sCommandSed).c_str()) == 0);

		if(l_bSuccess)
		{
			l_ssTextBuffer << "[   OK   ] -- " << l_sDest << " written.\n";
			m_rKernelContext.getLogManager() << LogLevel_Info << " -- " << l_sDest << " written.\n";
		}
		else
		{
			l_ssTextBuffer << "[FAILED] -- " << l_sDest << " cannot be written.\n";
			m_rKernelContext.getLogManager() << LogLevel_Error << " -- " << l_sDest << " cannot be written.\n";
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// interface.ui
	// we check if the skeleton is in place.
	const gchar* l_sInterfaceUISkel="../share/openvibe-applications/skeleton-generator/interface.ui-skeleton";
	if(! g_file_test(l_sInterfaceUISkel, G_FILE_TEST_EXISTS))
	{
		l_ssTextBuffer << "[FAILED] the file 'interface.ui-skeleton' is missing.\n";
		m_rKernelContext.getLogManager() << LogLevel_Error << "the file 'interface.ui-skeleton' is missing.\n";
		l_bSuccess = false;
	}
	else
	{
		l_ssTextBuffer << "[   OK   ] -- 'interface.ui-skeleton' found.\n";
		m_rKernelContext.getLogManager() << LogLevel_Info << " -- 'interface.ui-skeleton' found.\n";

		//Using GNU sed for parsing and replacing tags
		CString l_sDest = m_sTargetDirectory + "/interface-" + m_sClassName + ".ui";

		CString l_sCommandSed = l_sSed;
		l_sCommandSed = l_sCommandSed + " \"s/@@DriverName@@/"+m_sDriverName+"/g";
		l_sCommandSed = l_sCommandSed + ";s/@@MinChannel@@/"+m_sMinChannel+"/g";
		l_sCommandSed = l_sCommandSed + ";s/@@MaxChannel@@/"+m_sMaxChannel+"/g";
		l_sCommandSed = l_sCommandSed + ";s/@@SamplingFrequencyList@@/";
		for(vector<CString>::iterator it = m_vSamplingFrequencies.begin(); it != m_vSamplingFrequencies.end(); )
		{
			l_sCommandSed = l_sCommandSed + (*it++);
			if(it!=m_vSamplingFrequencies.end()) l_sCommandSed = l_sCommandSed + "<\\/col><\\/row><row><col id=\\\"0\\\" translatable=\\\"yes\\\">";
		}
		l_sCommandSed = l_sCommandSed + "/g\" ";
		m_rKernelContext.getLogManager() << LogLevel_Debug << "CMD: "+l_sCommandSed+"\n";

		// execute the sed command !
//#ifdef OV_OS_Windows
//		l_sInterfaceUISkel = "interface.ui-skeleton";
//#endif
		l_sCommandSed = l_sCommandSed + CString("\"") + CString(l_sInterfaceUISkel) + CString("\" > \"") + l_sDest + CString("\"");

		m_rKernelContext.getLogManager() << LogLevel_Trace << "Invoking [" << l_sCommandSed << "]...\n";

		l_bSuccess &= (system(((string)l_sCommandSed).c_str()) == 0);

		if(l_bSuccess)
		{
			l_ssTextBuffer << "[   OK   ] -- " << l_sDest << " written.\n";
			m_rKernelContext.getLogManager() << LogLevel_Info << " -- " << l_sDest << " written.\n";
		}
		else
		{
			l_ssTextBuffer << "[FAILED] -- " << l_sDest << " cannot be written.\n";
			m_rKernelContext.getLogManager() << LogLevel_Error << " -- " << l_sDest << " cannot be written.\n";
		}
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// readme-driver.txt
	// we check if the skeleton is in place.
	const gchar* l_sReadMeDriverTxtSkel="../share/openvibe-applications/skeleton-generator/readme-driver.txt-skeleton";
	if(! g_file_test(l_sReadMeDriverTxtSkel, G_FILE_TEST_EXISTS))
	{
		l_ssTextBuffer << "[FAILED] the file 'readme-driver.txt-skeleton' is missing.\n";
		m_rKernelContext.getLogManager() << LogLevel_Error << "the file 'readme-driver.txt-skeleton' is missing.\n";
		l_bSuccess = false;
	}
	else
	{
		l_ssTextBuffer << "[   OK   ] -- 'readme-driver.txt-skeleton' found.\n";
		m_rKernelContext.getLogManager() << LogLevel_Info << " -- 'readme-driver.txt-skeleton' found.\n";

		//Using GNU sed for parsing and replacing tags
		CString l_sDest = m_sTargetDirectory + "/readme-driver.txt";

		CString l_sCommandSed = l_sSed;
		l_sCommandSed = l_sCommandSed + " \"s/@@ClassName@@/"+m_sClassName+"/g";
		l_sCommandSed = l_sCommandSed + " ;s/@@Date@@/"+l_sDate+"/g\" ";

		m_rKernelContext.getLogManager() << LogLevel_Debug << "CMD: "+l_sCommandSed+"\n";

		// execute the sed command !
//#ifdef OV_OS_Windows
//		l_sReadMeDriverTxtSkel = "readme-driver.txt-skeleton";
//#endif
		l_sCommandSed = l_sCommandSed + CString("\"") + CString(l_sReadMeDriverTxtSkel) + CString("\" > \"") + l_sDest + CString("\"");

		m_rKernelContext.getLogManager() << LogLevel_Trace << "Invoking [" << l_sCommandSed << "]...\n";

		l_bSuccess &= (system(((string)l_sCommandSed).c_str()) == 0);

		if(l_bSuccess)
		{
			l_ssTextBuffer << "[   OK   ] -- " << l_sDest << " written.\n";
			m_rKernelContext.getLogManager() << LogLevel_Info << " -- " << l_sDest << " written.\n";
		}
		else
		{
			l_ssTextBuffer << "[FAILED] -- " << l_sDest << " cannot be written.\n";
			m_rKernelContext.getLogManager() << LogLevel_Error << " -- " << l_sDest << " cannot be written.\n";
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------//

	if(l_bSuccess)
	{
		l_bSuccess&=cleanConfigurationFile();
		l_bSuccess&=saveCommon();
		l_bSuccess&=save();
	}

	if(!l_bSuccess)
	{
		l_ssTextBuffer << "Generation process did not completly succeed. Some files may have not been produced.\n";
		m_rKernelContext.getLogManager() << LogLevel_Warning << "Generation process did not completly succeed. Some files may have not been produced.\n";
	}
	else
	{
		l_ssTextBuffer << "Generation process successful. All entries saved in [" << m_sConfigurationFile << "]\n";
		m_rKernelContext.getLogManager() << LogLevel_Info << "Generation process successful. All entries saved in [" << m_sConfigurationFile << "]\n";
	}

	CString l_sBrowser = m_rKernelContext.getConfigurationManager().expand("${Designer_WebBrowserCommand_${OperatingSystem}}");
	CString l_sBrowserCmd = l_sBrowser + " " +  m_sTargetDirectory;

#ifdef OV_OS_Windows
	l_sBrowserCmd =  l_sBrowser + " file:///"+  m_sTargetDirectory; //otherwise the browser does not find the directory (problem with / and \ char)
#endif

	if(system((const char *)l_sBrowserCmd))
	{
	}

	gtk_text_buffer_set_text(
		l_pTextBuffer,
		l_ssTextBuffer.str().c_str(),
		-1);
}

void CDriverSkeletonGenerator::buttonTooltipCB(::GtkButton* pButton)
{
	EWidgetName l_eWidgetName=m_vWidgetName[pButton];

	::GtkWidget * l_pTooltipTextview = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-driver-tooltips-textview"));
	::GtkTextBuffer * l_pTextBuffer  = gtk_text_view_get_buffer(GTK_TEXT_VIEW(l_pTooltipTextview));

	if(l_eWidgetName == WidgetName_DriverName)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Driver Name: \nThis name will be the one dislayed in the Acquisition Server selection combobox.\nUsually, the driver is named according to the EEG device (with precisions such as a version number).\n------\nExample: OpenEEG Modular EEG (P2)\n\n\n"
			, -1);
	}
	else if(l_eWidgetName == WidgetName_ClassName)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Class Name: \nThis name will be used to generate all source and GUI files.\nYou should choose a class name close to the device name (no blank allowed !).\n------\nExample: OpenEEGModularEEG will generate\n - ovasCDriverOpenEEGModularEEG.h/.cpp, the driver skeleton \n - ovasCConfigurationOpenEEGModularEEG.h/.cpp, the configuration class skeleton\n - interface-OpenEEG-ModularEEG.ui, the GUI description file"
			, -1);
	}
	else if(l_eWidgetName == WidgetName_ChannelCount)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Channel count: \nEnter in the two fields the minimum and maximum number of channels the device is capable of.\nOf course you can still change it later in the source code.\n------\nExample: Min(1) Max(16)\n\n\n"
			, -1);
	}
	else if(l_eWidgetName == WidgetName_SamplingFrequencies)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Sampling frequencies: \nEnter in the text field the sampling frequencies your device is capable of.\nYou can specify a list of defined frequencies (value separator ';').\n------\nExample:\n\"128;256;512\" for three defined frequencies.\n\n"
			, -1);
	}
	else if(l_eWidgetName == WidgetName_TargetDirectory)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Target directory: \nEnter the destination directory in which all files will be generated. \nAny existing files will be overwritten.\n------\nExample: ~/skeleton-generator/foobar-driver/\n\n\n"
			, -1);
	}
	else
	{
	}
}

CDriverSkeletonGenerator::CDriverSkeletonGenerator(IKernelContext & rKernelContext, ::GtkBuilder * pBuilderInterface)
	:CSkeletonGenerator(rKernelContext, pBuilderInterface)
{
	load();
}
CDriverSkeletonGenerator::~CDriverSkeletonGenerator(void)
{
}

void CDriverSkeletonGenerator::initialize( void )
{
	::GtkWidget * l_pWindowDriver = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-driver-window"));

	// Buttons and signals
	::GtkWidget * l_pButtonCheck  = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-driver-check-button"));
	::GtkWidget * l_pButtonOk     = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-driver-ok-button"));

	g_signal_connect(l_pButtonCheck,"pressed",G_CALLBACK(button_check_cb), this);
	g_signal_connect(l_pButtonOk,"pressed",G_CALLBACK(button_ok_cb), this);

	// Tooltips buttons and signal
	::GtkButton * l_pTooltipButton_driverName          = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-driver-driver-name-tooltip-button"));
	::GtkButton * l_pTooltipButton_className           = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-driver-class-name-tooltip-button"));
	::GtkButton * l_pTooltipButton_channelCount        = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-driver-channel-count-tooltip-button"));
	::GtkButton * l_pTooltipButton_samplingFrequencies = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-driver-sampling-frequencies-tooltip-button"));
	::GtkButton * l_pTooltipButton_targetDirectory     = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-driver-target-directory-tooltip-button"));

	m_vWidgetName[l_pTooltipButton_driverName]=WidgetName_DriverName;
	m_vWidgetName[l_pTooltipButton_className]=WidgetName_ClassName;
	m_vWidgetName[l_pTooltipButton_channelCount]=WidgetName_ChannelCount;
	m_vWidgetName[l_pTooltipButton_samplingFrequencies]=WidgetName_SamplingFrequencies;
	m_vWidgetName[l_pTooltipButton_targetDirectory]=WidgetName_TargetDirectory;

	g_signal_connect(l_pTooltipButton_driverName,         "pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_className,          "pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_channelCount,       "pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_samplingFrequencies,"pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_targetDirectory,    "pressed",G_CALLBACK(button_tooltip_cb), this);

	//Close with X and "cancel" button
	g_signal_connect (
		G_OBJECT(l_pWindowDriver),
		"delete_event",
		G_CALLBACK(::gtk_exit),
		0);

	::GtkWidget * l_pButtonCancel = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-driver-cancel-button"));
	g_signal_connect(l_pButtonCancel,"pressed", G_CALLBACK(::gtk_exit), 0);

	gtk_widget_show_all(l_pWindowDriver);
}

boolean CDriverSkeletonGenerator::save(void)
{
	FILE* l_pFile=::fopen(m_sConfigurationFile.toASCIIString(), "ab");
	if(!l_pFile)
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "Saving the driver entries in [" << m_sConfigurationFile << "] failed !\n";
		return false;
	}

	string::iterator it;
	string l_sTempTargetDirectory(m_sTargetDirectory.toASCIIString());
	for(it=l_sTempTargetDirectory.begin(); it<l_sTempTargetDirectory.end(); it++)
	{
		if((*it)=='\\')
		{
			l_sTempTargetDirectory.replace(it, it+1, 1, '/');
		}
	}

	::fprintf(l_pFile, "SkeletonGenerator_Driver_DriverName = %s\n", m_sDriverName.toASCIIString());
	::fprintf(l_pFile, "SkeletonGenerator_Driver_ClassName = %s\n", m_sClassName.toASCIIString());
	::fprintf(l_pFile, "SkeletonGenerator_Driver_MinChannel = %s\n", m_sMinChannel.toASCIIString());
	::fprintf(l_pFile, "SkeletonGenerator_Driver_MaxChannel = %s\n", m_sMaxChannel.toASCIIString());
	::fprintf(l_pFile, "SkeletonGenerator_Driver_SamplingFrequencies = %s\n", m_sSamplingFrequencies.toASCIIString());
	::fprintf(l_pFile, "SkeletonGenerator_Driver_TargetDirectory = %s\n", l_sTempTargetDirectory.c_str());
	::fclose(l_pFile);
	m_rKernelContext.getLogManager() << LogLevel_Info << "Driver entries saved in [" << m_sConfigurationFile << "]\n";

	return true;
}

boolean CDriverSkeletonGenerator::load(void)
{
	if(!m_rKernelContext.getConfigurationManager().addConfigurationFromFile(m_sConfigurationFile))
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "Driver: Configuration file [" << m_sConfigurationFile << "] could not be loaded. It will be automatically generated after first use.\n";
		return false;
	}

	::GtkWidget * l_pEntryDriverName = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "entry_driver_name"));
	gtk_entry_set_text(GTK_ENTRY(l_pEntryDriverName),m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_Driver_DriverName}"));

	::GtkWidget * l_pEntryClassName = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "entry_class_name"));
	gtk_entry_set_text(GTK_ENTRY(l_pEntryClassName),m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_Driver_ClassName}"));

	::GtkWidget * l_pSpinbuttonMinChannel = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "spinbutton_min_channel"));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(l_pSpinbuttonMinChannel),(gdouble)m_rKernelContext.getConfigurationManager().expandAsInteger("${SkeletonGenerator_Driver_MinChannel}"));

	::GtkWidget * l_pSpinbuttonMaxChannel = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "spinbutton_max_channel"));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(l_pSpinbuttonMaxChannel),(gdouble)m_rKernelContext.getConfigurationManager().expandAsInteger("${SkeletonGenerator_Driver_MaxChannel}"));

	::GtkWidget * l_pEntrySF = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "entry_sampling_frequencies"));
	gtk_entry_set_text(GTK_ENTRY(l_pEntrySF),m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_Driver_SamplingFrequencies}"));

	::GtkWidget * l_pFileChooser = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "filechooserbutton_target_directory"));
	CString l_sTargetDirectory = m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_Driver_TargetDirectory}");

	gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(l_pFileChooser),(const char *)l_sTargetDirectory);
	m_rKernelContext.getLogManager() << LogLevel_Info << "Driver entries from [" << m_sConfigurationFile << "] loaded.\n";

	return true;
}
