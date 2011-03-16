#include "ovsgCBoxAlgorithmSkeletonGenerator.h"


#include <iostream>
#include <sstream>

#include <glib/gstdio.h>
#include <cstdio>

#include <boost/regex.hpp>

#include <ctime>
#include <cmath>

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
using namespace OpenViBESkeletonGenerator;

boolean CDummyAlgoProto::addInputParameter(const CIdentifier& rInputParameterIdentifier,const CString& sInputName,const EParameterType eParameterType,const CIdentifier& rSubTypeIdentifier)
{
	m_vInputs[sInputName] = eParameterType;
	return true;
}
boolean CDummyAlgoProto::addOutputParameter(const CIdentifier& rOutputParameterIdentifier,const CString& sOutputName,const EParameterType eParameterType,const CIdentifier& rSubTypeIdentifier)
{
	m_vOutputs[sOutputName] = eParameterType;
	return true;
}

boolean CDummyAlgoProto::addInputTrigger(const CIdentifier& rInputTriggerIdentifier,const CString& rInputTriggerName)
{
	m_vInputTriggers.push_back(rInputTriggerName);
	return true;
}

boolean CDummyAlgoProto::addOutputTrigger(const CIdentifier& rOutputTriggerIdentifier,const CString& rOutputTriggerName)
{
	m_vOutputTriggers.push_back(rOutputTriggerName);
	return true;
}

//-----------------------------------------------------------------------
static void button_check_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CBoxAlgorithmSkeletonGenerator*>(pUserData)->buttonCheckCB();
}
static void button_tooltip_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CBoxAlgorithmSkeletonGenerator*>(pUserData)->buttonTooltipCB(pButton);
}
static void button_ok_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CBoxAlgorithmSkeletonGenerator*>(pUserData)->buttonOkCB();
}

static void button_add_input_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CBoxAlgorithmSkeletonGenerator*>(pUserData)->buttonAddInputCB();
}
static void button_remove_input_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CBoxAlgorithmSkeletonGenerator*>(pUserData)->buttonRemoveInputCB();
}

static void button_add_output_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CBoxAlgorithmSkeletonGenerator*>(pUserData)->buttonAddOutputCB();
}
static void button_remove_output_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CBoxAlgorithmSkeletonGenerator*>(pUserData)->buttonRemoveOutputCB();
}

static void button_add_setting_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CBoxAlgorithmSkeletonGenerator*>(pUserData)->buttonAddSettingCB();
}
static void button_remove_setting_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CBoxAlgorithmSkeletonGenerator*>(pUserData)->buttonRemoveSettingCB();
}

static void button_add_algorithm_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CBoxAlgorithmSkeletonGenerator*>(pUserData)->buttonAddAlgorithmCB();
}
static void button_remove_algorithm_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CBoxAlgorithmSkeletonGenerator*>(pUserData)->buttonRemoveAlgorithmCB();
}
static void algorithm_selected_cb(::GtkComboBox* pCombobox, void* pUserData)
{
	static_cast<CBoxAlgorithmSkeletonGenerator*>(pUserData)->algorithmSelectedCB(gtk_combo_box_get_active(pCombobox));
}

static void button_exit_cb(::GtkButton* pButton, void* pUserData)
{
	static_cast<CBoxAlgorithmSkeletonGenerator*>(pUserData)->buttonExitCB();
	::gtk_exit(0);
}

//-----------------------------------------------------------------------
void CBoxAlgorithmSkeletonGenerator::buttonExitCB()
{
	getCommonParameters();
	getCurrentParameters();
	cleanConfigurationFile(m_sConfigurationFile);
	saveCommonParameters(m_sConfigurationFile);
	save(m_sConfigurationFile);

	m_rKernelContext.getLogManager() << LogLevel_Info << "All entries saved in ["<< m_sConfigurationFile<<"]. Exiting.\n";
}
void CBoxAlgorithmSkeletonGenerator::buttonCheckCB()
{
	m_rKernelContext.getLogManager() << LogLevel_Info << "Extracting values... \n";
	//Author and Company
	getCommonParameters();
	//Box generator entries
	getCurrentParameters();

	m_rKernelContext.getLogManager() << LogLevel_Info << "Checking values... \n";

	boolean l_bSuccess = true;

	stringstream l_ssTextBuffer;
	l_ssTextBuffer << "----- STATUS -----\n";

	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// Box Description
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	if(string((const char *)m_sName) == "")
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "-- box name: INVALID (" << (const char *)m_sName << ")\n";
		l_ssTextBuffer << "[FAILED] No name found. Please provide a name for the box (all characters allowed).\n";
	}
	else
	{
		//m_sName = ensureSedCompliancy(m_sName);
		m_rKernelContext.getLogManager() << LogLevel_Info << "-- box name: VALID (" << (const char *)m_sName << ")\n";
		l_ssTextBuffer << "[   OK   ] Valid box name.\n";
	}

	const boost::regex l_RegExpClassName("([a-z]|[A-Z])+([a-z]|[A-Z]|[0-9][_])*",boost::regex::perl);
	if(boost::regex_match(string(m_sClassName),l_RegExpClassName) == false)
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "-- class name: INVALID (" << (const char *)m_sClassName << ")\n";
		l_ssTextBuffer << "[FAILED] Class name invalid. Please provide a class name using lower/upper case letters, numbers or underscores.\n";
	}
	else
	{
		m_rKernelContext.getLogManager() << LogLevel_Info << "-- class name: VALID (" << (const char *)m_sClassName << ")\n";
		l_ssTextBuffer << "[   OK   ] Valid class name.\n";
	}

	const boost::regex l_RegExpCategory("([a-z]|[A-Z])+([a-z]|[A-Z]|[ ]|[/])*",boost::regex::perl);
	if(boost::regex_match(string(m_sCategory),l_RegExpCategory) == false)
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "-- category: INVALID (" << (const char *)m_sCategory << ")\n";
		l_ssTextBuffer << "[FAILED] Category invalid. Please provide a category using only letters and spaces (for sub-category, use '/' separator).\n";
	}
	else
	{
		m_rKernelContext.getLogManager() << LogLevel_Info << "-- category: VALID (" << (const char *)m_sCategory << ")\n";
		l_ssTextBuffer << "[   OK   ] Valid category.\n";
	}

	const boost::regex l_RegExpboxVersion("([0-9])+([a-z]|[A-Z]|[0-9]|[\\.])*",boost::regex::perl);
	if(boost::regex_match(string(m_sVersion),l_RegExpboxVersion) == false)
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "-- box version: INVALID (" << (const char *)m_sVersion << ")\n";
		l_bSuccess = false;
		l_ssTextBuffer << "[FAILED] Invalid box version (" << (const char *)m_sVersion << "). Please use a number followed by either numbers, letters or '.'\n";
	}
	else
	{
		m_rKernelContext.getLogManager() << LogLevel_Info << "-- box version: VALID (" << (const char *)m_sVersion << ")\n";
		l_ssTextBuffer << "[   OK   ] Valid box version.\n";
	}

	{
		//m_sShortDescription = ensureSedCompliancy(m_sShortDescription);
		m_rKernelContext.getLogManager() << LogLevel_Info << "-- short description: VALID (" << (const char *)m_sShortDescription << ")\n";
		l_ssTextBuffer << "[   OK   ] Valid short description.\n";
	}

	if(((string)m_sDetailedDescription).length()<500)
	{
		//m_sDetailedDescription = ensureSedCompliancy(m_sDetailedDescription);
		m_rKernelContext.getLogManager() << LogLevel_Info << "-- detailed description: VALID (" << (const char *)m_sDetailedDescription << ")\n";
		l_ssTextBuffer << "[   OK   ] Valid detailed description.\n";
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// Box INPUTS OUTPUTS and SETTINGS
	//-------------------------------------------------------------------------------------------------------------------------------------------//

	//checking the inputs...
	if(m_vInputs.size() != 0)
	{
		l_ssTextBuffer << "Checking inputs... \n";
		m_rKernelContext.getLogManager() << LogLevel_Info << "-- checking inputs...\n";
	}
	else
	{
		l_ssTextBuffer << "[----//----] No input specified.\n";
		m_rKernelContext.getLogManager() << LogLevel_Info << "No input specified.\n";
	}
	for(uint32 i = 0; i < m_vInputs.size(); i++)
	{
		if(string((const char *)(m_vInputs[i]._name)) == "" || string((const char *)(m_vInputs[i]._type)) == "")
		{
			m_rKernelContext.getLogManager() << LogLevel_Warning << "--~~ Input "<<i<<": [" << (const char *)m_vInputs[i]._name<<"],["<< (const char *)m_vInputs[i]._type << "] INVALID.\n";
			l_ssTextBuffer << ">>[FAILED] Invalid input "<<i<<". Please provide a name and a type for each input.\n";
			l_bSuccess = false;
		}
		else
		{
			m_vInputs[i]._name = ensureSedCompliancy(m_vInputs[i]._name);
			m_rKernelContext.getLogManager() << LogLevel_Info << "--~~ Input "<<i<<": [" << (const char *)m_vInputs[i]._name<<"],["<< (const char *)m_vInputs[i]._type << "] VALID.\n";
			l_ssTextBuffer << ">>[   OK   ] Valid input "<<i<<" [" << (const char *)m_vInputs[i]._name<<"]\n";
		}
	}

	//checking the outputs...
	if(m_vOutputs.size() != 0)
	{
		l_ssTextBuffer << "Checking outputs... \n";
		m_rKernelContext.getLogManager() << LogLevel_Info << "-- checking outputs...\n";
	}
	else
	{
		l_ssTextBuffer << "[----//----] No output specified.\n";
		m_rKernelContext.getLogManager() << LogLevel_Info << "No output specified.\n";
	}
	for(uint32 i = 0; i < m_vOutputs.size(); i++)
	{
		if(string((const char *)(m_vOutputs[i]._name)) == "" || string((const char *)(m_vOutputs[i]._type)) == "")
		{
			m_rKernelContext.getLogManager() << LogLevel_Warning << "--~~ Output "<<i<<": [" << (const char *)m_vOutputs[i]._name<<"],["<< (const char *)m_vOutputs[i]._type << "] INVALID.\n";
			l_ssTextBuffer << ">>[FAILED] Invalid output "<<i<<". Please provide a name and a type for each output.\n";
			l_bSuccess = false;
		}
		else
		{
			m_vOutputs[i]._name = ensureSedCompliancy(m_vOutputs[i]._name);
			m_rKernelContext.getLogManager() << LogLevel_Info << "--~~ Output "<<i<<": [" << (const char *)m_vOutputs[i]._name<<"],["<< (const char *)m_vOutputs[i]._type << "] VALID.\n";
			l_ssTextBuffer << ">>[   OK   ] Valid output "<<i<<" [" << (const char *)m_vOutputs[i]._name<<"]\n";
		}
	}

	//checking the settings...
	if(m_vSettings.size() != 0)
	{
		l_ssTextBuffer << "Checking settings... \n";
		m_rKernelContext.getLogManager() << LogLevel_Info << "-- checking settings...\n";
	}
	else
	{
		l_ssTextBuffer << "[----//----] No setting specified.\n";
		m_rKernelContext.getLogManager() << LogLevel_Info << "No setting specified.\n";
	}
	for(uint32 i = 0; i < m_vSettings.size(); i++)
	{
		if(string((const char *)(m_vSettings[i]._name)) == "" || string((const char *)(m_vSettings[i]._type)) == "")
		{
			m_rKernelContext.getLogManager() << LogLevel_Warning << "--~~ Setting "<<i<<": [" << (const char *)m_vSettings[i]._name<<"],["<< (const char *)m_vSettings[i]._type << "] INVALID.\n";
			l_ssTextBuffer << ">>[FAILED] Invalid setting "<<i<<". Please provide a name, a type and a default value for each setting.\n";
			l_bSuccess = false;
		}
		else
		{
			m_vSettings[i]._name = ensureSedCompliancy(m_vSettings[i]._name);
			m_rKernelContext.getLogManager() << LogLevel_Info << "--~~ Setting "<<i<<": [" << (const char *)m_vSettings[i]._name<<"],["<< (const char *)m_vSettings[i]._type <<"],["<< (const char *)m_vSettings[i]._defaultValue << "] VALID.\n";
			l_ssTextBuffer << ">>[   OK   ] Valid setting "<<i<<" [" << (const char *)m_vSettings[i]._name<<"]\n";
		}
	}

	//checking the algorithms...
	if(m_vAlgorithms.size() != 0)
	{
		l_ssTextBuffer << "Checking algorithm... \n";
		m_rKernelContext.getLogManager() << LogLevel_Info << "-- checking algorithm...\n";
	}
	else
	{
		l_ssTextBuffer << "[----//----] No algorithm specified.\n";
		m_rKernelContext.getLogManager() << LogLevel_Info << "No algorithm specified.\n";
	}
	for(uint32 i = 0; i < m_vAlgorithms.size(); i++)
	{
		if(string((const char *)m_vAlgorithms[i]) == "")
		{
			m_rKernelContext.getLogManager() << LogLevel_Warning << "--~~ Algorithm "<<i<<": [" << (const char *)m_vAlgorithms[i]<<"] INVALID.\n";
			l_ssTextBuffer << ">>[FAILED] Invalid algorithm "<<i<<".\n";
			l_bSuccess = false;
		}
		else
		{
			m_rKernelContext.getLogManager() << LogLevel_Info << "--~~ Algorithm "<<i<<": [" << (const char *)m_vAlgorithms[i]<<"] VALID.\n";
			l_ssTextBuffer << ">>[   OK   ] Valid algorithm "<<i<<" [" << (const char *)m_vAlgorithms[i]<<"]\n";
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------//
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
	::GtkWidget * l_pTooltipTextview = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-tooltips-textview"));
	::GtkTextBuffer * l_pTextBuffer  = gtk_text_view_get_buffer(GTK_TEXT_VIEW(l_pTooltipTextview));
	if(l_bSuccess)
	{
		l_ssTextBuffer << "----- SUCCESS -----\nPress OK to generate the files. If you want to modify your choice(s), please press the \"Check\" button again.";
		::GtkWidget * l_pButtonOk = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-ok-button"));
		gtk_widget_set_sensitive(l_pButtonOk,true);

	}
	else
	{
		l_ssTextBuffer << "----- PROCESS FAILED -----\nModify your choices and press the \"Check\" button again.";
		::GtkWidget * l_pButtonOk = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-ok-button"));
		gtk_widget_set_sensitive(l_pButtonOk,false);
	}

	gtk_text_buffer_set_text (l_pTextBuffer,l_ssTextBuffer.str().c_str(), -1);

}

void CBoxAlgorithmSkeletonGenerator::buttonOkCB()
{

	m_rKernelContext.getLogManager() << LogLevel_Info << "Generating files... \n";
	boolean l_bSuccess = true;

	::GtkWidget * l_pTooltipTextview = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-tooltips-textview"));
	::GtkTextBuffer * l_pTextBuffer  = gtk_text_view_get_buffer(GTK_TEXT_VIEW(l_pTooltipTextview));

	stringstream l_ssTextBuffer;
	l_ssTextBuffer << "Generating files...\n";
	gtk_text_buffer_set_text (l_pTextBuffer,l_ssTextBuffer.str().c_str(),-1);
	
	CString l_sDate = getDate();
	
	// construction of the namespace name from category
	string l_sNamespace(m_sCategory);
	for(int s=0; s<l_sNamespace.length(); s++)
	{
		if(s == 0 && l_sNamespace[s] >= 'a' && l_sNamespace[s]<= 'z')
		{
			l_sNamespace.replace(s,1,1,(char)(l_sNamespace[s]+'A'-'a'));
		}
		boolean l_bErase = false;
		while(s<l_sNamespace.length() && (l_sNamespace[s]==' ' || l_sNamespace[s]=='/'))
		{
			l_sNamespace.erase(s,1);
			l_bErase = true;
		}
		if(l_bErase && s<l_sNamespace.length() && l_sNamespace[s] >= 'a' && l_sNamespace[s]<= 'z')
		{	
			l_sNamespace.replace(s,1,1,(char)(l_sNamespace[s]+'A'-'a'));
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// box.h
	// we check if the skeleton is in place.
	const gchar* l_sBoxHSkel="../share/openvibe-applications/skeleton-generator/box.h-skeleton";
	if(! g_file_test(l_sBoxHSkel, G_FILE_TEST_EXISTS))
	{
		l_ssTextBuffer << "[FAILED] the file 'box.h-skeleton' is missing.\n";
		m_rKernelContext.getLogManager() << LogLevel_Error << "Box: the file 'box.h-skeleton' is missing.\n";
		l_bSuccess = false;
	}
	else
	{
		l_ssTextBuffer << "[   OK   ] -- 'box.h-skeleton' found.\n";
		gtk_text_buffer_set_text (l_pTextBuffer,l_ssTextBuffer.str().c_str(), -1);
		m_rKernelContext.getLogManager() << LogLevel_Info << " -- 'box.h-skeleton' found.\n";

		//Using GNU sed for parsing and replacing tags
		CString l_sDest = m_sTargetDirectory + "/ovpCBoxAlgorithm" + m_sClassName + ".h";
		
		// generating some random identifiers
		CString l_sClassIdentifier = getRandomIdentifierString();
		CString l_sDescriptorIdentifier = getRandomIdentifierString();

		l_bSuccess &= executeSedSubstitution(l_sBoxHSkel,"@@Author@@",                     m_sAuthor, l_sDest);
		l_bSuccess &= executeSedSubstitution(l_sDest,    "@@Company@@",                    m_sCompany);
		l_bSuccess &= executeSedSubstitution(l_sDest,    "@@Date@@",                       l_sDate);
		l_bSuccess &= executeSedSubstitution(l_sDest,    "@@BoxName@@",                    m_sName);
		l_bSuccess &= executeSedSubstitution(l_sDest,    "@@ClassName@@",                  m_sClassName);
		l_bSuccess &= executeSedSubstitution(l_sDest,    "@@RandomIdentifierClass@@",      l_sClassIdentifier);
		l_bSuccess &= executeSedSubstitution(l_sDest,    "@@RandomIdentifierDescriptor@@", l_sDescriptorIdentifier);
		l_bSuccess &= executeSedSubstitution(l_sDest,    "@@ShortDescription@@",           m_sShortDescription);
		l_bSuccess &= executeSedSubstitution(l_sDest,    "@@DetailedDescription@@",        m_sDetailedDescription);
		l_bSuccess &= executeSedSubstitution(l_sDest,    "@@Category@@",                   m_sCategory);
		l_bSuccess &= executeSedSubstitution(l_sDest,    "@@Namespace@@",                  CString(l_sNamespace.c_str()));
		l_bSuccess &= executeSedSubstitution(l_sDest,    "@@Version@@",                    m_sVersion);
		l_bSuccess &= executeSedSubstitution(l_sDest,    "@@StockItemName@@",              m_sGtkStockItemName);
		

		//--------------------------------------------------------------------------------------
		//Inputs
		//--------------------------------------------------------------------------------------
		if(m_bCanAddInputs)
			l_bSuccess &= executeSedSubstitution(l_sDest, "@@InputFlagCanAdd@@","rBoxAlgorithmPrototype.addFlag(OpenViBE::Kernel::BoxFlag_CanAddInput);");
		else
			l_bSuccess &= executeSedSubstitution(l_sDest, "@@InputFlagCanAdd@@","//You cannot add or remove input.");
		if(m_bCanModifyInputs)
			l_bSuccess &= executeSedSubstitution(l_sDest, "@@InputFlagCanModify@@","rBoxAlgorithmPrototype.addFlag(OpenViBE::Kernel::BoxFlag_CanModifyInput);");
		else
			l_bSuccess &= executeSedSubstitution(l_sDest, "@@InputFlagCanModify@@","//You cannot modify input.");
		CString l_sCommandSed = "s/@@Inputs@@/";
		if(m_vInputs.empty()) l_sCommandSed = l_sCommandSed + "\\/\\/No input specified.To add inputs use :\\n\\/\\/rBoxAlgorithmPrototype.addInput(\\\"Input Name\\\",OV_TypeId_XXXX);\\n";
		for(vector<IOSStruct>::iterator it = m_vInputs.begin(); it != m_vInputs.end(); it++)
		{
			if(it != m_vInputs.begin()) 
				l_sCommandSed = l_sCommandSed + "\\t\\t\\t\\t";
			//add the CIdentifier corresponding to type
			//l_sCommandSed = l_sCommandSed + "rBoxAlgorithmPrototype.addInput(\\\""+(*it)._name+"\\\", OpenViBE::Cidentifier"+(*it)._typeId+");\\n";
			//reconstruct the type_id
			string l_sTypeName((const char *)(*it)._type);
			for(int s=0; s<l_sTypeName.length(); s++)
			{
				if(l_sTypeName[s]==' ')
				{
					l_sTypeName.erase(s,1);
					if(l_sTypeName[s] >= 'a' && l_sTypeName[s]<= 'z') l_sTypeName.replace(s,1,1,(char)(l_sTypeName[s]+'A'-'a'));
				}
			}
			l_sCommandSed = l_sCommandSed + "rBoxAlgorithmPrototype.addInput(\\\""+(*it)._name+"\\\",OV_TypeId_"+CString(l_sTypeName.c_str())+");\\n";
		}
		l_sCommandSed = l_sCommandSed +  "/g";
		l_bSuccess &= executeSedCommand(l_sDest, l_sCommandSed);

		//--------------------------------------------------------------------------------------
		//Outputs
		//--------------------------------------------------------------------------------------
		if(m_bCanAddOutputs)
			l_bSuccess &= executeSedSubstitution(l_sDest, "@@OutputFlagCanAdd@@","rBoxAlgorithmPrototype.addFlag(OpenViBE::Kernel::BoxFlag_CanAddOutput);");
		else
			l_bSuccess &= executeSedSubstitution(l_sDest, "@@OutputFlagCanAdd@@","//You cannot add or remove Output.");
		if(m_bCanModifyOutputs)
			l_bSuccess &= executeSedSubstitution(l_sDest, "@@OutputFlagCanModify@@","rBoxAlgorithmPrototype.addFlag(OpenViBE::Kernel::BoxFlag_CanModifyOutput);");
		else
			l_bSuccess &= executeSedSubstitution(l_sDest, "@@OutputFlagCanModify@@","//You cannot modify Output.");
		l_sCommandSed = " s/@@Outputs@@/";
		if(m_vOutputs.empty()) l_sCommandSed = l_sCommandSed + "\\/\\/No output specified.To add outputs use :\\n\\/\\/rBoxAlgorithmPrototype.addOutput(\\\"Output Name\\\",OV_TypeId_XXXX);\\n";
		for(vector<IOSStruct>::iterator it = m_vOutputs.begin(); it != m_vOutputs.end(); it++)
		{
			if(it != m_vOutputs.begin()) 
				l_sCommandSed = l_sCommandSed + "\\t\\t\\t\\t";
			//add the CIdentifier corresponding to type
			//l_sCommandSed = l_sCommandSed + "rBoxAlgorithmPrototype.addOutput(\\\""+(*it)._name+"\\\", OpenViBE::Cidentifier"+(*it)._typeId+");\\n";
			//reconstruct the type_id
			string l_sTypeName((const char *)(*it)._type);
			for(int s=0; s<l_sTypeName.length(); s++)
			{
				if(l_sTypeName[s]==' ')
				{
					l_sTypeName.erase(s,1);
					if(l_sTypeName[s] >= 'a' && l_sTypeName[s]<= 'z') l_sTypeName.replace(s,1,1,(char)(l_sTypeName[s]+'A'-'a'));
				}
			}
			l_sCommandSed = l_sCommandSed + "rBoxAlgorithmPrototype.addOutput(\\\""+(*it)._name+"\\\",OV_TypeId_"+CString(l_sTypeName.c_str())+");\\n";
		}
		l_sCommandSed = l_sCommandSed +  "/g";
		l_bSuccess &= executeSedCommand(l_sDest, l_sCommandSed);

		//--------------------------------------------------------------------------------------
		//Settings
		//--------------------------------------------------------------------------------------
		if(m_bCanAddOutputs)
			l_bSuccess &= executeSedSubstitution(l_sDest, "@@SettingFlagCanAdd@@","rBoxAlgorithmPrototype.addFlag(OpenViBE::Kernel::BoxFlag_CanAddSetting);");
		else
			l_bSuccess &= executeSedSubstitution(l_sDest, "@@SettingFlagCanAdd@@","//You cannot add or remove Setting.");
		if(m_bCanModifyOutputs)
			l_bSuccess &= executeSedSubstitution(l_sDest, "@@SettingFlagCanModify@@","rBoxAlgorithmPrototype.addFlag(OpenViBE::Kernel::BoxFlag_CanModifySetting);");
		else
			l_bSuccess &= executeSedSubstitution(l_sDest, "@@SettingFlagCanModify@@","//You cannot modify Setting.");
		l_sCommandSed = "s/@@Settings@@/";
		if(m_vSettings.empty()) l_sCommandSed = l_sCommandSed + "\\/\\/No setting specified.To add settings use :\\n\\/\\/rBoxAlgorithmPrototype.addSetting(\\\"Setting Name\\\",OV_TypeId_XXXX,\\\"default value\\\");\\n";
		for(vector<IOSStruct>::iterator it = m_vSettings.begin(); it != m_vSettings.end(); it++)
		{
			if(it != m_vSettings.begin()) 
				l_sCommandSed = l_sCommandSed + "\\t\\t\\t\\t";
			//add the CIdentifier corresponding to type
			//l_sCommandSed = l_sCommandSed + "rBoxAlgorithmPrototype.addSetting(\\\""+(*it)._name+"\\\", OpenViBE::Cidentifier"+(*it)._typeId+",\\\""+(*it)._defaultValue+"\\\");\\n";
			//reconstruct the type_id by erasing the spaces and upcasing the following letter
			string l_sTypeName((const char *)(*it)._type);
			for(int s=0; s<l_sTypeName.length(); s++)
			{
				if(l_sTypeName[s]==' ')
				{
					l_sTypeName.erase(s,1);
					if(l_sTypeName[s] >= 'a' && l_sTypeName[s]<= 'z') l_sTypeName.replace(s,1,1,(char)(l_sTypeName[s]+'A'-'a'));
				}
			}
			l_sCommandSed = l_sCommandSed + "rBoxAlgorithmPrototype.addSetting(\\\""+(*it)._name+"\\\",OV_TypeId_"+CString(l_sTypeName.c_str())+",\\\""+(*it)._defaultValue+"\\\");\\n";
		}
		l_sCommandSed = l_sCommandSed +  "/g";
		l_bSuccess &= executeSedCommand(l_sDest, l_sCommandSed);

		//--------------------------------------------------------------------------------------
		//Algorithms
		//--------------------------------------------------------------------------------------
		l_sCommandSed = "s/@@Algorithms@@/";
		for(uint32 a=0; a<m_vAlgorithms.size(); a++)
		{
			if(a != 0) 
				l_sCommandSed = l_sCommandSed + "\\t\\t\\t\\t";

			string l_sBlock = string((const char *)m_mAlgorithmHeaderDeclaration[m_vAlgorithms[a]]);
			stringstream ss; ss << "Algo" << a << "_";
			string l_sUniqueMarker = ss.str();
			for(int s=0; s<l_sBlock.length(); s++)
			{
				if(l_sBlock[s]=='@')
				{
					l_sBlock.erase(s,1);
					l_sBlock.insert(s,l_sUniqueMarker);
				}
			}
			l_sCommandSed = l_sCommandSed + CString(l_sBlock.c_str());
		}
		
		l_sCommandSed = l_sCommandSed + "/g";
		l_bSuccess &= executeSedCommand(l_sDest, l_sCommandSed);

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
	// box.cpp
	// we check if the skeleton is in place.
	const gchar* l_sBoxCppSkel="../share/openvibe-applications/skeleton-generator/box.cpp-skeleton";
	if(! g_file_test(l_sBoxCppSkel, G_FILE_TEST_EXISTS))
	{
		l_ssTextBuffer << "[FAILED] the file 'box.cpp-skeleton' is missing.\n";
		m_rKernelContext.getLogManager() << LogLevel_Error << "The file 'box.cpp-skeleton' is missing.\n";
		l_bSuccess = false;
	}
	else
	{
		l_ssTextBuffer << "[   OK   ] -- 'box.cpp-skeleton' found.\n";
		gtk_text_buffer_set_text (l_pTextBuffer,l_ssTextBuffer.str().c_str(), -1);
		m_rKernelContext.getLogManager() << LogLevel_Info << " -- 'box.cpp-skeleton' found.\n";

		//Using GNU sed for parsing and replacing tags
		CString l_sDest = m_sTargetDirectory + "/ovpCBoxAlgorithm" + m_sClassName + ".cpp";
		
		l_bSuccess &= executeSedSubstitution(l_sBoxCppSkel,"@@ClassName@@",m_sClassName,l_sDest);
		l_bSuccess &= executeSedSubstitution(l_sDest,      "@@Namespace@@",CString(l_sNamespace.c_str()));
		
		CString l_sCommandSed = "s/@@AlgorithmInitialisation@@/";
		for(uint32 a=0; a<m_vAlgorithms.size(); a++)
		{
			string l_sBlock = string((const char *)m_mAlgorithmInitialisation[m_vAlgorithms[a]]);
			stringstream ss; ss << "Algo" << a << "_";
			string l_sUniqueMarker = ss.str();
			for(int s=0; s<l_sBlock.length(); s++)
			{
				if(l_sBlock[s]=='@')
				{
					l_sBlock.erase(s,1);
					l_sBlock.insert(s,l_sUniqueMarker);
				}
			}
			l_sCommandSed = l_sCommandSed + CString(l_sBlock.c_str());
		}
		l_sCommandSed = l_sCommandSed + "/g";
		l_bSuccess &= executeSedCommand(l_sDest, l_sCommandSed);
		
		l_sCommandSed = "s/@@AlgorithmInitialisationReferenceTargets@@/";
		for(uint32 a=0; a<m_vAlgorithms.size(); a++)
		{
			string l_sBlock = string((const char *)m_mAlgorithmInitialisation_ReferenceTargets[m_vAlgorithms[a]]);
			stringstream ss; ss << "Algo" << a << "_";
			string l_sUniqueMarker = ss.str();
			for(int s=0; s<l_sBlock.length(); s++)
			{
				if(l_sBlock[s]=='@')
				{
					l_sBlock.erase(s,1);
					l_sBlock.insert(s,l_sUniqueMarker);
				}
			}
			l_sCommandSed = l_sCommandSed + CString(l_sBlock.c_str());
		}
		l_sCommandSed = l_sCommandSed + "/g";
		l_bSuccess &= executeSedCommand(l_sDest, l_sCommandSed);
		
		
		l_sCommandSed = "s/@@AlgorithmUninitialisation@@/";
		for(uint32 a=0; a<m_vAlgorithms.size(); a++)
		{
			string l_sBlock = string((const char *)m_mAlgorithmUninitialisation[m_vAlgorithms[a]]);
			stringstream ss; ss << "Algo" << a << "_";
			string l_sUniqueMarker = ss.str();
			for(uint32 s=0; s<l_sBlock.length(); s++)
			{
				if(l_sBlock[s]=='@')
				{
					l_sBlock.erase(s,1);
					l_sBlock.insert(s,l_sUniqueMarker);
				}
			}
			l_sCommandSed = l_sCommandSed + CString(l_sBlock.c_str());
		}
		l_sCommandSed = l_sCommandSed + "/g";
		l_bSuccess &= executeSedCommand(l_sDest, l_sCommandSed);

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
	// readme-box.cpp
	// we check if the skeleton is in place.
	const gchar* l_sReadmeSkel="../share/openvibe-applications/skeleton-generator/readme-box.txt-skeleton";
	if(! g_file_test(l_sReadmeSkel, G_FILE_TEST_EXISTS))
	{
		l_ssTextBuffer << "[FAILED] the file 'readme-box.txt-skeleton' is missing.\n";
		m_rKernelContext.getLogManager() << LogLevel_Error << "The file 'readme-box.txt-skeleton' is missing.\n";
		l_bSuccess = false;
	}
	else
	{
		l_ssTextBuffer << "[   OK   ] -- 'readme-box.txt-skeleton' found.\n";
		gtk_text_buffer_set_text (l_pTextBuffer,l_ssTextBuffer.str().c_str(), -1);
		m_rKernelContext.getLogManager() << LogLevel_Info << " -- 'readme-box.txt-skeleton' found.\n";

		//Using GNU sed for parsing and replacing tags
		CString l_sDest = m_sTargetDirectory + "/README-SKGEN-BOX.txt";
		
		l_bSuccess &= executeSedSubstitution(l_sReadmeSkel,"@@Date@@",     l_sDate,l_sDest);
		l_bSuccess &= executeSedSubstitution(l_sDest,      "@@ClassName@@",m_sClassName);
		
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
		l_bSuccess&=cleanConfigurationFile(m_sConfigurationFile);
		//re-load all entries, the internal variables may have been modified to be sed compliant.
		getCommonParameters();
		getCurrentParameters();
		//save the entries as the user typed them
		l_bSuccess&=saveCommonParameters(m_sConfigurationFile);
		l_bSuccess&=save(m_sConfigurationFile);
	}

	if(!l_bSuccess)
	{
		l_ssTextBuffer << "Generation process did not completly succeed. Some files may have not been produced.\n";
		m_rKernelContext.getLogManager() << LogLevel_Warning << "Generation process did not completly succeed. Some files may have not been produced.\n";
	}
	else
	{
		l_ssTextBuffer << "Generation process successful. All entries saved in [" << m_sConfigurationFile << "]\n";
		l_ssTextBuffer << "PLEASE LOOK AT THE README FILE PRODUCED !\n";
		m_rKernelContext.getLogManager() << LogLevel_Info << "Generation process successful. All entries saved in [" << m_sConfigurationFile << "]\n";
		
		// opening browser to see the produced files
		CString l_sBrowser = m_rKernelContext.getConfigurationManager().expand("${Designer_WebBrowserCommand_${OperatingSystem}}");
		CString l_sBrowserCmd = l_sBrowser + " \"" +  m_sTargetDirectory+"\"";

#ifdef OV_OS_Windows
		l_sBrowserCmd =  l_sBrowser + " file:///"+  m_sTargetDirectory; //otherwise the browser does not find the directory (problem with / and \ char)
#endif
		if(system((const char *)l_sBrowserCmd))
		{
		}
	}

	gtk_text_buffer_set_text(l_pTextBuffer,l_ssTextBuffer.str().c_str(),-1);
}

void CBoxAlgorithmSkeletonGenerator::buttonTooltipCB(::GtkButton* pButton)
{
	EWidgetName l_eWidgetName=m_vWidgetName[pButton];

	::GtkWidget * l_pTooltipTextview = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-tooltips-textview"));
	::GtkTextBuffer * l_pTextBuffer  = gtk_text_view_get_buffer(GTK_TEXT_VIEW(l_pTooltipTextview));

	if(l_eWidgetName == WidgetName_NameVersion)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Box Name: \nThis name will be the one displayed in the Designer.\nUsually, the box name reflects its main purpose.\nIf your box relies on GPL licensed libraries such as IT++, please precise it in the name.\nPlease also enter a version number for your box.\nAuthorized characters: letters (lower and upper case), numbers, special characters '()[]._-'\n------\nExample: Clock Stimulator (tic tac), version 1.2"
			, -1);
	}
	else if(l_eWidgetName == WidgetName_ClassName)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Class Name: \nThis name will be used in the code to build the class name.\nUsually, the class name is close to the box name, just without any blanck.\nIf your box relies on GPL licensed libraries such as IT++, please precise it in the class name with the postfix 'GPL'.\nAuthorized characters: letters (lower and upper case), numbers, NO special characters, NO blank.\n------\nExample: ClockStimulator\n"
			, -1);
	}
	else if(l_eWidgetName == WidgetName_Category)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Category: \nThe category decides where the box will be strored in designer's box panel.\nYou can refer to an existing category, already used in the designer, or choose a new one.\nIf you need to specifiy a subcategory, use the character '\\'.\nAuthorized characters: letters (lower and upper case) and spaces.\n------\nExample: Samples\\Seleton Generator\n"
			, -1);
	}
	else if(l_eWidgetName == WidgetName_Description)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Description: \nThe short description will be displayed next to the box in the designer box panel.\nThe detailed description is showed on the 'About Box...' panel.\nAll characters are authorized.\n------\nExample:\nShort Description : Periodic stimulation generator\nDetailed description : This box triggers stimulation at fixed frequency."
			, -1);
	}
	else if(l_eWidgetName == WidgetName_Icon)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Box Icon: \nThe icon used in the designer box panel for this box.\nThis is an optionnal field.\n------\nExample: 'gtk-help' will be the corresponding gtk stock item (depending on the gtk theme used)\n\n\n"
			, -1);
	}
	else if(l_eWidgetName == WidgetName_Inputs)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Inputs: \nUse the 'Add' and 'Remove' buttons to set all the inputs your box will have.\nWhen pressing 'Add' a dialog window will appear to know the name and type of the new input.\n------\nExample:\n'Incoming Signal' of type 'Signal'\n\n"
			, -1);
	}
	else if(l_eWidgetName == WidgetName_Inputs_CanModify)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Modify: \nCheck this option if the input(s) of your box can be modified (type and name) in the Designer by right-clicking the box.\nIn the implementation, this option decides whether or not the box will have the flag 'BoxFlag_CanModifyInput'.\n\n\n\n\n"
			, -1);
	}
	
	else if(l_eWidgetName == WidgetName_Inputs_CanAddRemove)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Add/Remove: \nCheck this option if the user must be able to add (or remove) inputs, by right-clicking the box.\nIn the implementation, this option decides whether or not the box will have the flag 'BoxFlag_CanAddInput'.\n\n\n\n"
			, -1);
	}
	else if(l_eWidgetName == WidgetName_Outputs)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Outputs: \nUse the 'Add' and 'Remove' buttons to set all the outputs your box will have.\nWhen pressing 'Add' a dialog window will appear to know the name and type of the new output.\n------\nExample:\n'Filtered Signal' of type 'Signal'\n\n"
			, -1);
	}
	else if(l_eWidgetName == WidgetName_Outputs_CanModify)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Modify: \nCheck this option if the output(s) of your box can be modified (type and name) in the Designer by right-clicking the box.\nIn the implementation, this option decides whether or not the box will have the flag 'BoxFlag_CanModifyOutput'.\n\n\n\n\n"
			, -1);
	}
	else if(l_eWidgetName == WidgetName_Outputs_CanAddRemove)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Add/Remove: \nCheck this option if the user must be able to add (or remove) outputs, by right-clicking the box.\nIn the implementation, this option decides whether or not the box will have the flag 'BoxFlag_CanAddOutput'.\n\n\n\n"
			, -1);
	}
	else if(l_eWidgetName == WidgetName_Settings)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Outputs: \nUse the 'Add' and 'Remove' buttons to set all the settings your box will have.\nWhen pressing 'Add' a dialog window will appear to know the name,type and default value of the new output.\n------\nExample:\n'Filter order' of type 'int' with default value '4'\n\n"
			, -1);
	}
	else if(l_eWidgetName == WidgetName_Settings_CanModify)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Modify: \nCheck this option if the setting(s) of your box can be modified (type and name) in the Designer by right-clicking the box.\nIn the implementation, this option decides whether or not the box will have the flag 'BoxFlag_CanModifySetting'.\n\n\n\n\n"
			, -1);
	}
	else if(l_eWidgetName == WidgetName_Settings_CanAddRemove)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Add/Remove: \nCheck this option if the user must be able to add (or remove) settings, by right-clicking the box.\nIn the implementation, this option decides whether or not the box will have the flag 'BoxFlag_CanAddSetting'.\n\n\n\n"
			, -1);
	}
	else if(l_eWidgetName == WidgetName_TargetDirectory)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Target directory: \nEnter the destination directory in which all files will be generated. \nAny existing files will be overwritten.\n------\nExample: ~/skeleton-generator/foobar-box/\n\n\n"
			, -1);
	}
	else if(l_eWidgetName == WidgetName_Algorithm)
	{
		gtk_text_buffer_set_text (l_pTextBuffer,
			"Algorithms: \nChoose the algorithm(s) used by the box. \nYou can choose between all the algorithms currently registered in the OpenViBE kernel.\nWhen choosing an algorithm, the dialog window will display the algorithm description (Inputs, outputs, triggers). \n------\nExample:Signal Decoder, that outputs a StreamMatrix from an EBML Stream.\n\n"
			, -1);
	}
	else
	{
	}
}

void CBoxAlgorithmSkeletonGenerator::buttonAddInputCB(void)
{
	::GtkWidget * l_pDialog = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-IO-add-dialog"));
	::GtkWidget * l_pNameEntry = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-IO-add-name-entry"));
	::GtkWidget * l_pTypeCombobox = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-IO-add-type-combobox"));
		
	gtk_entry_set_text(GTK_ENTRY(l_pNameEntry),"");
		
	gint resp = gtk_dialog_run(GTK_DIALOG(l_pDialog));

	if(resp== GTK_RESPONSE_APPLY)
	{
		const gchar * l_sName = gtk_entry_get_text(GTK_ENTRY(l_pNameEntry));
		//we get the two types (user/ov)
		GtkTreeIter l_iterType;
		GtkTreeModel * l_treeModelType = gtk_combo_box_get_model(GTK_COMBO_BOX(l_pTypeCombobox));
		gtk_combo_box_get_active_iter(GTK_COMBO_BOX(l_pTypeCombobox),&l_iterType);
		gchar* l_dataTypeUser;
		gchar* l_dataTypeOv;
		gtk_tree_model_get(l_treeModelType,&l_iterType,0,&l_dataTypeUser,1,&l_dataTypeOv,-1);
		//const gchar * l_sType = gtk_combo_box_get_active_text(GTK_COMBO_BOX(l_pTypeCombobox));

		::GtkWidget * l_pInputTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-inputs-treeview"));
		::GtkTreeModel * l_pInputListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pInputTreeView));
		GtkTreeIter l_iter;
		gtk_list_store_append(GTK_LIST_STORE(l_pInputListStore),&l_iter);
		gtk_list_store_set (GTK_LIST_STORE(l_pInputListStore), &l_iter, 0, l_sName,1,l_dataTypeUser,2,l_dataTypeOv,-1);
		gtk_widget_hide(l_pDialog);

		g_free(l_dataTypeUser);g_free(l_dataTypeOv);
	}
	else
	{
		gtk_widget_hide(l_pDialog);
	}

}
void CBoxAlgorithmSkeletonGenerator::buttonRemoveInputCB(void)
{
	::GtkWidget * l_pInputTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-inputs-treeview"));
	::GtkTreeModel * l_pInputListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pInputTreeView));
	GtkTreeIter l_iter;
	GtkTreeSelection *l_select;
	l_select = gtk_tree_view_get_selection(GTK_TREE_VIEW(l_pInputTreeView));
	if(gtk_tree_selection_get_selected (l_select, &l_pInputListStore, &l_iter))
	{
		gtk_list_store_remove(GTK_LIST_STORE(l_pInputListStore),&l_iter);
	}
}

void CBoxAlgorithmSkeletonGenerator::buttonAddOutputCB(void)
{
	::GtkWidget * l_pDialog = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-IO-add-dialog"));
	::GtkWidget * l_pNameEntry = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-IO-add-name-entry"));
	::GtkWidget * l_pTypeCombobox = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-IO-add-type-combobox"));
		
	gtk_entry_set_text(GTK_ENTRY(l_pNameEntry),"");
		
	gint resp = gtk_dialog_run(GTK_DIALOG(l_pDialog));

	if(resp== GTK_RESPONSE_APPLY)
	{
		const gchar * l_sName = gtk_entry_get_text(GTK_ENTRY(l_pNameEntry));
		//we get the two types (user/ov)
		GtkTreeIter l_iterType;
		GtkTreeModel * l_treeModelType = gtk_combo_box_get_model(GTK_COMBO_BOX(l_pTypeCombobox));
		gtk_combo_box_get_active_iter(GTK_COMBO_BOX(l_pTypeCombobox),&l_iterType);
		gchar* l_dataTypeUser;
		gchar* l_dataTypeOv;
		gtk_tree_model_get(l_treeModelType,&l_iterType,0,&l_dataTypeUser,1,&l_dataTypeOv,-1);
		//const gchar * l_sType = gtk_combo_box_get_active_text(GTK_COMBO_BOX(l_pTypeCombobox));

		::GtkWidget * l_pOutputTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-outputs-treeview"));
		::GtkTreeModel * l_pOutputListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pOutputTreeView));
		GtkTreeIter l_iter;
	
		gtk_list_store_append(GTK_LIST_STORE(l_pOutputListStore),&l_iter);
		gtk_list_store_set (GTK_LIST_STORE(l_pOutputListStore), &l_iter, 0, l_sName,1,l_dataTypeUser,2,l_dataTypeOv,-1);
		gtk_widget_hide(l_pDialog);

		g_free(l_dataTypeUser);g_free(l_dataTypeOv);
	}
	else
	{
		gtk_widget_hide(l_pDialog);
	}
}
void CBoxAlgorithmSkeletonGenerator::buttonRemoveOutputCB(void)
{
	::GtkWidget * l_pOutputTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-outputs-treeview"));
	::GtkTreeModel * l_pOutputListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pOutputTreeView));
	GtkTreeIter l_iter;
	GtkTreeSelection *l_select;
	l_select = gtk_tree_view_get_selection(GTK_TREE_VIEW(l_pOutputTreeView));
	if(gtk_tree_selection_get_selected (l_select, &l_pOutputListStore, &l_iter))
	{
		gtk_list_store_remove(GTK_LIST_STORE(l_pOutputListStore),&l_iter);
	}
}

void CBoxAlgorithmSkeletonGenerator::buttonAddSettingCB(void)
{
	::GtkWidget * l_pDialog = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-add-dialog"));
	::GtkWidget * l_pNameEntry = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-add-name-entry"));
	::GtkWidget * l_pTypeCombobox = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-add-type-combobox"));
	::GtkWidget * l_pValueEntry = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-add-default-value-entry"));
	
	gtk_entry_set_text(GTK_ENTRY(l_pNameEntry),"");
	gtk_entry_set_text(GTK_ENTRY(l_pValueEntry),"");
	
	gint resp = gtk_dialog_run(GTK_DIALOG(l_pDialog));
		
	if(resp== GTK_RESPONSE_APPLY)
	{
		const gchar * l_sName = gtk_entry_get_text(GTK_ENTRY(l_pNameEntry));
		const gchar * l_sValue = gtk_entry_get_text(GTK_ENTRY(l_pValueEntry));
		//we get the two types (user/ov)
		GtkTreeIter l_iterType;
		GtkTreeModel * l_treeModelType = gtk_combo_box_get_model(GTK_COMBO_BOX(l_pTypeCombobox));
		gtk_combo_box_get_active_iter(GTK_COMBO_BOX(l_pTypeCombobox),&l_iterType);
		gchar* l_dataTypeUser;
		gchar* l_dataTypeOv;
		gtk_tree_model_get(l_treeModelType,&l_iterType,0,&l_dataTypeUser,1,&l_dataTypeOv,-1);
		//const gchar * l_sType = gtk_combo_box_get_active_text(GTK_COMBO_BOX(l_pTypeCombobox));

		::GtkWidget * l_pOutputTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-treeview"));
		::GtkTreeModel * l_pOutputListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pOutputTreeView));
		GtkTreeIter l_iter;
	
		gtk_list_store_append(GTK_LIST_STORE(l_pOutputListStore),&l_iter);
		gtk_list_store_set (GTK_LIST_STORE(l_pOutputListStore), &l_iter, 0, l_sName,1,l_dataTypeUser,2,l_sValue,3,l_dataTypeOv,-1);
		gtk_widget_hide(l_pDialog);

		g_free(l_dataTypeUser);g_free(l_dataTypeOv);
	}
	else
	{
		gtk_widget_hide(l_pDialog);
	}
}
void CBoxAlgorithmSkeletonGenerator::buttonRemoveSettingCB(void)
{
	::GtkWidget * l_pSettingTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-treeview"));
	::GtkTreeModel * l_pSettingListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pSettingTreeView));
	GtkTreeIter l_iter;
	GtkTreeSelection *l_select;
	l_select = gtk_tree_view_get_selection(GTK_TREE_VIEW(l_pSettingTreeView));
	if(gtk_tree_selection_get_selected (l_select, &l_pSettingListStore, &l_iter))
	{
		gtk_list_store_remove(GTK_LIST_STORE(l_pSettingListStore),&l_iter);
	}
}

void CBoxAlgorithmSkeletonGenerator::buttonAddAlgorithmCB(void)
{
	::GtkWidget * l_pDialog = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-dialog"));
	::GtkWidget * l_pAlgoCombobox = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-combobox"));
	
	::GtkWidget * l_pAlgoInputsTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-inputs-treeview"));
	::GtkTreeModel * l_pAlgoInputsListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pAlgoInputsTreeView));
	::GtkWidget * l_pAlgoOutputsTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-outputs-treeview"));
	::GtkTreeModel * l_pAlgoOutputsListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pAlgoOutputsTreeView));
	::GtkWidget * l_pAlgoInputTriggersTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-input-triggers-treeview"));
	::GtkTreeModel * l_pAlgoInputTriggersListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pAlgoInputTriggersTreeView));
	::GtkWidget * l_pAlgoOutputTriggersTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-output-triggers-treeview"));
	::GtkTreeModel * l_pAlgoOutputTriggersListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pAlgoOutputTriggersTreeView));
	
	::GtkWidget * l_pAlgoCategoryEntry = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-category-entry"));
	gtk_entry_set_text(GTK_ENTRY(l_pAlgoCategoryEntry),"");
	::GtkWidget * l_pAlgoShortEntry = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-short-description-entry"));
	gtk_entry_set_text(GTK_ENTRY(l_pAlgoShortEntry),"");
	::GtkWidget * l_pAlgoDetailedTextview = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-detailed-description-textview"));
	::GtkTextBuffer * l_pTextBuffer  = gtk_text_view_get_buffer(GTK_TEXT_VIEW(l_pAlgoDetailedTextview));
	gtk_text_buffer_set_text(l_pTextBuffer,"",-1);


	gtk_list_store_clear(GTK_LIST_STORE(l_pAlgoInputsListStore));
	gtk_list_store_clear(GTK_LIST_STORE(l_pAlgoOutputsListStore));
	gtk_list_store_clear(GTK_LIST_STORE(l_pAlgoInputTriggersListStore));
	gtk_list_store_clear(GTK_LIST_STORE(l_pAlgoOutputTriggersListStore));
	gtk_combo_box_set_active(GTK_COMBO_BOX(l_pAlgoCombobox),-1);

	gint resp = gtk_dialog_run(GTK_DIALOG(l_pDialog));

	if(resp== GTK_RESPONSE_APPLY)
	{
		const gchar * l_sAlgo = gtk_combo_box_get_active_text(GTK_COMBO_BOX(l_pAlgoCombobox));

		::GtkWidget * l_pAlgoTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-treeview"));
		::GtkTreeModel * l_pAlgoListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pAlgoTreeView));
		GtkTreeIter l_iter;
		gtk_list_store_append(GTK_LIST_STORE(l_pAlgoListStore),&l_iter);
		gtk_list_store_set (GTK_LIST_STORE(l_pAlgoListStore), &l_iter, 0, l_sAlgo,-1);
		gtk_widget_hide(l_pDialog);
	}
	else
	{
		gtk_widget_hide(l_pDialog);
	}

}
void CBoxAlgorithmSkeletonGenerator::buttonRemoveAlgorithmCB(void)
{
	::GtkWidget * l_pAlgoTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-treeview"));
	::GtkTreeModel * l_pAlgoListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pAlgoTreeView));
	GtkTreeIter l_iter;
	GtkTreeSelection *l_select;
	l_select = gtk_tree_view_get_selection(GTK_TREE_VIEW(l_pAlgoTreeView));
	if(gtk_tree_selection_get_selected (l_select, &l_pAlgoListStore, &l_iter))
	{
		gtk_list_store_remove(GTK_LIST_STORE(l_pAlgoListStore),&l_iter);
	}
}
void CBoxAlgorithmSkeletonGenerator::algorithmSelectedCB(int32 i32IndexSelected)
{
	::GtkWidget * l_pAlgoInputsTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-inputs-treeview"));
	::GtkTreeModel * l_pAlgoInputsListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pAlgoInputsTreeView));
	::GtkWidget * l_pAlgoOutputsTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-outputs-treeview"));
	::GtkTreeModel * l_pAlgoOutputsListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pAlgoOutputsTreeView));
	::GtkWidget * l_pAlgoInputTriggersTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-input-triggers-treeview"));
	::GtkTreeModel * l_pAlgoInputTriggersListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pAlgoInputTriggersTreeView));
	::GtkWidget * l_pAlgoOutputTriggersTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-output-triggers-treeview"));
	::GtkTreeModel * l_pAlgoOutputTriggersListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pAlgoOutputTriggersTreeView));
	
	gtk_list_store_clear(GTK_LIST_STORE(l_pAlgoInputsListStore));
	gtk_list_store_clear(GTK_LIST_STORE(l_pAlgoOutputsListStore));
	gtk_list_store_clear(GTK_LIST_STORE(l_pAlgoInputTriggersListStore));
	gtk_list_store_clear(GTK_LIST_STORE(l_pAlgoOutputTriggersListStore));

	::GtkWidget * l_pAlgoCategoryEntry = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-category-entry"));
	gtk_entry_set_text(GTK_ENTRY(l_pAlgoCategoryEntry),"");
	::GtkWidget * l_pAlgoShortEntry = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-short-description-entry"));
	gtk_entry_set_text(GTK_ENTRY(l_pAlgoShortEntry),"");
	::GtkWidget * l_pAlgoDetailedTextview = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-detailed-description-textview"));
	::GtkTextBuffer * l_pTextBuffer  = gtk_text_view_get_buffer(GTK_TEXT_VIEW(l_pAlgoDetailedTextview));
	gtk_text_buffer_set_text(l_pTextBuffer,"",-1);

	if(i32IndexSelected != -1)
	{
		::GtkWidget * l_pAlgoCombobox = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-combobox"));
		::GtkTreeModel * l_pAlgoListStore = gtk_combo_box_get_model(GTK_COMBO_BOX(l_pAlgoCombobox));
		::GtkTreeIter l_iter;
		gchar *l_sIdentifier;
		gtk_tree_model_iter_nth_child(l_pAlgoListStore,&l_iter,NULL,i32IndexSelected);
		gtk_tree_model_get(l_pAlgoListStore,&l_iter,1,&l_sIdentifier,-1);
		CIdentifier l_identifier;
		l_identifier.fromString(CString(l_sIdentifier));
		
		//we need to create a dummy instance of the algorithm proto to know its input/output/triggers
		const IPluginObjectDesc * l_pDesc = m_rKernelContext.getPluginManager().getPluginObjectDesc(l_identifier);
		CDummyAlgoProto l_oDummyProto;
		((IAlgorithmDesc *)l_pDesc)->getAlgorithmPrototype(l_oDummyProto);

		//inputs of the algorithm
		::GtkWidget * l_pAlgoInputsTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-inputs-treeview"));
		::GtkTreeModel * l_pAlgoInputsListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pAlgoInputsTreeView));
		for(map<CString,EParameterType>::iterator i = l_oDummyProto.m_vInputs.begin(); i!=l_oDummyProto.m_vInputs.end(); i++)
		{
			GtkTreeIter l_iter;
			gtk_list_store_append(GTK_LIST_STORE(l_pAlgoInputsListStore),&l_iter);
			gtk_list_store_set(GTK_LIST_STORE(l_pAlgoInputsListStore), &l_iter, 0, (const char *)(*i).first,-1);
		}

		//outputs of the algorithm
		::GtkWidget * l_pAlgoOutputsTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-outputs-treeview"));
		::GtkTreeModel * l_pAlgoOutputsListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pAlgoOutputsTreeView));
		for(map<CString,EParameterType>::iterator i = l_oDummyProto.m_vOutputs.begin(); i!=l_oDummyProto.m_vOutputs.end(); i++)
		{
			GtkTreeIter l_iter;
			gtk_list_store_append(GTK_LIST_STORE(l_pAlgoOutputsListStore),&l_iter);
			gtk_list_store_set(GTK_LIST_STORE(l_pAlgoOutputsListStore), &l_iter, 0, (const char *)(*i).first,-1);
		}

		//Input triggers of the algorithm
		::GtkWidget * l_pAlgoInputTriggersTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-input-triggers-treeview"));
		::GtkTreeModel * l_pAlgoInputTriggersListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pAlgoInputTriggersTreeView));
		for(uint32 i = 0; i<l_oDummyProto.m_vInputTriggers.size(); i++)
		{
			GtkTreeIter l_iter;
			gtk_list_store_append(GTK_LIST_STORE(l_pAlgoInputTriggersListStore),&l_iter);
			gtk_list_store_set(GTK_LIST_STORE(l_pAlgoInputTriggersListStore), &l_iter, 0, (const char *)l_oDummyProto.m_vInputTriggers[i],-1);
		}
		//Output triggers of the algorithm
		::GtkWidget * l_pAlgoOutputTriggersTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-output-triggers-treeview"));
		::GtkTreeModel * l_pAlgoOutputTriggersListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pAlgoOutputTriggersTreeView));
		for(uint32 i = 0; i<l_oDummyProto.m_vOutputTriggers.size(); i++)
		{
			GtkTreeIter l_iter;
			gtk_list_store_append(GTK_LIST_STORE(l_pAlgoOutputTriggersListStore),&l_iter);
			gtk_list_store_set(GTK_LIST_STORE(l_pAlgoOutputTriggersListStore), &l_iter, 0, (const char *)l_oDummyProto.m_vOutputTriggers[i],-1);
		}

		::GtkWidget * l_pAlgoCategoryEntry = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-category-entry"));
		gtk_entry_set_text(GTK_ENTRY(l_pAlgoCategoryEntry),(const char*)l_pDesc->getCategory());
		::GtkWidget * l_pAlgoShortEntry = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-short-description-entry"));
		gtk_entry_set_text(GTK_ENTRY(l_pAlgoShortEntry),(const char*)l_pDesc->getShortDescription());
		::GtkWidget * l_pAlgoDetailedTextview = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-detailed-description-textview"));
		::GtkTextBuffer * l_pTextBuffer  = gtk_text_view_get_buffer(GTK_TEXT_VIEW(l_pAlgoDetailedTextview));
		gtk_text_buffer_set_text(l_pTextBuffer,(const char*)l_pDesc->getDetailedDescription(),-1);

	}
}
//--------------------------------------------------------------------------
CBoxAlgorithmSkeletonGenerator::CBoxAlgorithmSkeletonGenerator(IKernelContext & rKernelContext, ::GtkBuilder * pBuilderInterface)
	:CSkeletonGenerator(rKernelContext, pBuilderInterface)
{
}
CBoxAlgorithmSkeletonGenerator::~CBoxAlgorithmSkeletonGenerator(void)
{
}

void CBoxAlgorithmSkeletonGenerator::initialize( void )
{
	::GtkWidget * l_pBox = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-window"));
	
	// Main Buttons and signals
	::GtkWidget * l_pButtonCheck  = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-check-button"));
	::GtkWidget * l_pButtonOk     = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-ok-button"));

	g_signal_connect(l_pButtonCheck, "pressed",G_CALLBACK(button_check_cb), this);
	g_signal_connect(l_pButtonOk,    "pressed",G_CALLBACK(button_ok_cb), this);

	// Tooltips buttons and signal
	::GtkButton * l_pTooltipButton_nameVersion         = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-name-version-tooltip-button"));
	::GtkButton * l_pTooltipButton_category            = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-category-tooltip-button"));
	::GtkButton * l_pTooltipButton_description         = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-description-tooltip-button"));
	::GtkButton * l_pTooltipButton_icon                = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-icon-tooltip-button"));
	::GtkButton * l_pTooltipButton_inputs              = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-inputs-list-tooltip-button"));
	::GtkButton * l_pTooltipButton_inputs_modify       = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-inputs-modify-tooltip-button"));
	::GtkButton * l_pTooltipButton_inputs_addRemove    = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-inputs-add-tooltip-button"));
	::GtkButton * l_pTooltipButton_outputs             = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-outputs-list-tooltip-button"));
	::GtkButton * l_pTooltipButton_outputs_modify      = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-outputs-modify-tooltip-button"));
	::GtkButton * l_pTooltipButton_outputs_addRemove   = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-outputs-add-tooltip-button"));
	::GtkButton * l_pTooltipButton_settings            = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-list-tooltip-button"));
	::GtkButton * l_pTooltipButton_settings_modify     = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-modify-tooltip-button"));
	::GtkButton * l_pTooltipButton_settings_addRemove  = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-add-tooltip-button"));
	::GtkButton * l_pTooltipButton_algorithms          = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-tooltip-button"));
	::GtkButton * l_pTooltipButton_className           = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-class-name-tooltip-button"));
	::GtkButton * l_pTooltipButton_targetDirectory     = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-target-directory-tooltip-button"));

	m_vWidgetName[l_pTooltipButton_nameVersion]        = WidgetName_NameVersion;
	m_vWidgetName[l_pTooltipButton_category]           = WidgetName_Category;
	m_vWidgetName[l_pTooltipButton_description]        = WidgetName_Description;
	m_vWidgetName[l_pTooltipButton_icon]               = WidgetName_Icon;
	m_vWidgetName[l_pTooltipButton_inputs]             = WidgetName_Inputs;
	m_vWidgetName[l_pTooltipButton_inputs_modify]      = WidgetName_Inputs_CanModify;
	m_vWidgetName[l_pTooltipButton_inputs_addRemove]   = WidgetName_Inputs_CanAddRemove;
	m_vWidgetName[l_pTooltipButton_outputs]            = WidgetName_Outputs;
	m_vWidgetName[l_pTooltipButton_outputs_modify]     = WidgetName_Outputs_CanModify;
	m_vWidgetName[l_pTooltipButton_outputs_addRemove]  = WidgetName_Outputs_CanAddRemove;
	m_vWidgetName[l_pTooltipButton_settings]           = WidgetName_Settings;
	m_vWidgetName[l_pTooltipButton_settings_modify]    = WidgetName_Settings_CanModify;
	m_vWidgetName[l_pTooltipButton_settings_addRemove] = WidgetName_Settings_CanAddRemove;
	m_vWidgetName[l_pTooltipButton_algorithms]         = WidgetName_Algorithm;
	m_vWidgetName[l_pTooltipButton_className]          = WidgetName_ClassName;
	m_vWidgetName[l_pTooltipButton_targetDirectory]    = WidgetName_TargetDirectory;

	g_signal_connect(l_pTooltipButton_nameVersion,        "pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_category,           "pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_description,        "pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_icon,               "pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_inputs,             "pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_inputs_modify,      "pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_inputs_addRemove,   "pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_outputs,            "pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_outputs_modify,     "pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_outputs_addRemove,  "pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_settings,           "pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_settings_modify,    "pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_settings_addRemove, "pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_algorithms,         "pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_className,          "pressed",G_CALLBACK(button_tooltip_cb), this);
	g_signal_connect(l_pTooltipButton_targetDirectory,    "pressed",G_CALLBACK(button_tooltip_cb), this);

	//'Inputs' buttons
	::GtkButton * l_pInputsButton_add            = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-inputs-add-button"));
	::GtkButton * l_pInputsButton_remove         = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-inputs-remove-button"));
	
	g_signal_connect(l_pInputsButton_add,    "pressed",G_CALLBACK(button_add_input_cb), this);
	g_signal_connect(l_pInputsButton_remove, "pressed",G_CALLBACK(button_remove_input_cb), this);
	
	//'outputs' buttons
	::GtkButton * l_pOutputsButton_add            = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-outputs-add-button"));
	::GtkButton * l_pOutputsButton_remove         = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-outputs-remove-button"));
	
	g_signal_connect(l_pOutputsButton_add,    "pressed",G_CALLBACK(button_add_output_cb), this);
	g_signal_connect(l_pOutputsButton_remove, "pressed",G_CALLBACK(button_remove_output_cb), this);
	
	//'settings' buttons
	::GtkButton * l_pSettingsButton_add            = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-add-button"));
	::GtkButton * l_pSettingsButton_remove         = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-remove-button"));
	
	g_signal_connect(l_pSettingsButton_add,    "pressed",G_CALLBACK(button_add_setting_cb), this);
	g_signal_connect(l_pSettingsButton_remove, "pressed",G_CALLBACK(button_remove_setting_cb), this);

	//'algos' buttons
	::GtkButton * l_pAlgorithmsButton_add          = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-button"));
	::GtkButton * l_pAlgorithmsButton_remove       = GTK_BUTTON(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-remove-button"));
	
	g_signal_connect(l_pAlgorithmsButton_add,    "pressed",G_CALLBACK(button_add_algorithm_cb), this);
	g_signal_connect(l_pAlgorithmsButton_remove, "pressed",G_CALLBACK(button_remove_algorithm_cb), this);

	//Add IO dialog buttons 
	::GtkWidget * l_pDialog = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-IO-add-dialog"));
	gtk_dialog_add_button (GTK_DIALOG (l_pDialog),
		GTK_STOCK_APPLY,
		GTK_RESPONSE_APPLY);

	gtk_dialog_add_button (GTK_DIALOG (l_pDialog),
		GTK_STOCK_CANCEL,
		GTK_RESPONSE_CANCEL);

	//Add Setting dialog buttons 
	l_pDialog = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-add-dialog"));
	gtk_dialog_add_button (GTK_DIALOG (l_pDialog),
		GTK_STOCK_APPLY,
		GTK_RESPONSE_APPLY);

	gtk_dialog_add_button (GTK_DIALOG (l_pDialog),
		GTK_STOCK_CANCEL,
		GTK_RESPONSE_CANCEL);

	//Add Algo dialog buttons 
	l_pDialog = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-dialog"));
	gtk_dialog_add_button (GTK_DIALOG (l_pDialog),
		GTK_STOCK_APPLY,
		GTK_RESPONSE_APPLY);

	gtk_dialog_add_button (GTK_DIALOG (l_pDialog),
		GTK_STOCK_CANCEL,
		GTK_RESPONSE_CANCEL);

	//initialize the icon combo box with gtk stock items
	::GtkWidget * l_pIconCombobox = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-icon-combobox"));
	::GtkTreeModel * l_pIconListStore = gtk_combo_box_get_model(GTK_COMBO_BOX(l_pIconCombobox));
	::GSList * l_StockIdList = gtk_stock_list_ids();
	while(l_StockIdList->next!=NULL)
	{
		GtkTreeIter l_iter;
		gtk_list_store_append(GTK_LIST_STORE(l_pIconListStore), &l_iter);
		gtk_list_store_set (GTK_LIST_STORE(l_pIconListStore), &l_iter, 0, (char *)l_StockIdList->data,1,(char *)l_StockIdList->data,-1);
		l_StockIdList = g_slist_next(l_StockIdList);
	}
	g_slist_free(l_StockIdList);

	//types when adding IOS
	::GtkWidget * l_pTypeCombobox = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-IO-add-type-combobox"));
	::GtkTreeModel * l_pTypeListStore = gtk_combo_box_get_model(GTK_COMBO_BOX(l_pTypeCombobox));
	::GtkWidget * l_pSettingTypeCombobox = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-add-type-combobox"));
	::GtkTreeModel * l_pSettingTypeListStore = gtk_combo_box_get_model(GTK_COMBO_BOX(l_pSettingTypeCombobox));
	//we iterate over all identifiers
	CIdentifier l_Identifier = m_rKernelContext.getTypeManager().getNextTypeIdentifier(OV_UndefinedIdentifier);
	while(l_Identifier != OV_UndefinedIdentifier)
	{
		CString l_sType = m_rKernelContext.getTypeManager().getTypeName(l_Identifier);
		CString l_sTypeId = l_Identifier.toString();
		//Streams are possible inputs and outputs
		if(m_rKernelContext.getTypeManager().isStream(l_Identifier))
		{
			GtkTreeIter l_iter;
			gtk_list_store_append(GTK_LIST_STORE(l_pTypeListStore), &l_iter);
			gtk_list_store_set (GTK_LIST_STORE(l_pTypeListStore), &l_iter, 0, (const char *)l_sType,1,(const char *)l_sTypeId,-1);
		}
		else // other types are possible settings
		{
			GtkTreeIter l_iter;
			gtk_list_store_append(GTK_LIST_STORE(l_pSettingTypeListStore), &l_iter);
			gtk_list_store_set (GTK_LIST_STORE(l_pSettingTypeListStore), &l_iter, 0, (const char *)l_sType,1,(const char *)l_sTypeId,-1);
		}
		l_Identifier = m_rKernelContext.getTypeManager().getNextTypeIdentifier(l_Identifier);
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(l_pTypeCombobox),0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(l_pSettingTypeCombobox),0);
	//types when adding Algorithms
	m_vParameterType_EnumTypeCorrespondance.resize(ParameterType_Pointer+1);
	m_vParameterType_EnumTypeCorrespondance[ParameterType_None]           = "TYPE-NOT-AVAILABLE";
	m_vParameterType_EnumTypeCorrespondance[ParameterType_Integer]        = "OpenViBE::int64";
	m_vParameterType_EnumTypeCorrespondance[ParameterType_UInteger]       = "OpenViBE::uint64";
	m_vParameterType_EnumTypeCorrespondance[ParameterType_Enumeration]    = "ENUMERATION-NOT-AVAILABLE";
	m_vParameterType_EnumTypeCorrespondance[ParameterType_Boolean]        = "OpenViBE::boolean";
	m_vParameterType_EnumTypeCorrespondance[ParameterType_Float]          = "OpenViBE::float64";
	m_vParameterType_EnumTypeCorrespondance[ParameterType_String]         = "OpenViBE::CString";
	m_vParameterType_EnumTypeCorrespondance[ParameterType_Identifier]     = "OpenViBE::CIdentifier";
	m_vParameterType_EnumTypeCorrespondance[ParameterType_Matrix]         = "OpenViBE::IMatrix *";
	m_vParameterType_EnumTypeCorrespondance[ParameterType_StimulationSet] = "OpenViBE::IStimulationSet *";
	m_vParameterType_EnumTypeCorrespondance[ParameterType_MemoryBuffer]   = "OpenViBE::IMemoryBuffer *";
	m_vParameterType_EnumTypeCorrespondance[ParameterType_Object]         = "OpenViBE::IObject *";
	m_vParameterType_EnumTypeCorrespondance[ParameterType_Pointer]        = "OpenViBE::uint8*";


	::GtkWidget * l_pAlgoCombobox = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-add-combobox"));
	::GtkTreeModel * l_pAlgoListStore = gtk_combo_box_get_model(GTK_COMBO_BOX(l_pAlgoCombobox));
	//we iterate over all plugin descriptor identifiers
	l_Identifier = m_rKernelContext.getPluginManager().getNextPluginObjectDescIdentifier(OV_UndefinedIdentifier);
	while(l_Identifier != OV_UndefinedIdentifier)
	{
		const IPluginObjectDesc * l_pDesc =  m_rKernelContext.getPluginManager().getPluginObjectDesc(l_Identifier);
		if(l_pDesc != NULL && l_pDesc->isDerivedFromClass(OV_ClassId_Plugins_AlgorithmDesc)) // we select only algorithm descriptors
		{
			CString l_sAlgo = l_pDesc->getName();
			string l_sTest((const char *) l_sAlgo);
			string l_sEncoder("encoder");
			string l_sDecoder("decoder");
			// we only keep decoders and encoders
			if(l_sTest.find(l_sEncoder)!=string::npos || l_sTest.find(l_sDecoder)!=string::npos)
			{
				CString l_sAlgoID = l_Identifier.toString();
				GtkTreeIter l_iter;
				gtk_list_store_append(GTK_LIST_STORE(l_pAlgoListStore), &l_iter);
				gtk_list_store_set (GTK_LIST_STORE(l_pAlgoListStore), &l_iter, 0, (const char *)l_sAlgo,1,(const char *)l_sAlgoID,-1);
			}
			// now we map every decoder/encoder to its string description that will be added in the skeleton (algorithmProxy + parameter handlers for I/O)
			CString l_sHeaderDeclaration ="\\t\\t\\t\\/\\/Algorithm : "+ l_sAlgo +"\\n";
			CString l_sInitialisation ="\\t\\/\\/Algorithm : "+ l_sAlgo +"\\n";
			CString l_sInitialisation_ReferenceTargets = "\\t\\/\\/"+ l_sAlgo +" Reference Targets :\\n";
			CString l_sUninitialisation;
			//we need to create a dummy instance of the algorithm proto to know its input/output/triggers
			const IPluginObjectDesc * l_pDesc = m_rKernelContext.getPluginManager().getPluginObjectDesc(l_Identifier);
			CDummyAlgoProto l_oDummyProto;
			((IAlgorithmDesc *)l_pDesc)->getAlgorithmPrototype(l_oDummyProto);
			//algorithm proxy
			string l_sAlgoNameStdSTr((const char *)l_sAlgo);
			for(int s=0; s<l_sAlgoNameStdSTr.length(); s++)
			{
				if(l_sAlgoNameStdSTr[s]==' ')
				{
					l_sAlgoNameStdSTr.erase(s,1);
					if(l_sAlgoNameStdSTr[s] >= 'a' && l_sAlgoNameStdSTr[s]<= 'z') l_sAlgoNameStdSTr.replace(s,1,1,(char)(l_sAlgoNameStdSTr[s]+'A'-'a'));
				}
			}
			CString l_sAlgorithmProxy = "m_p@" + CString(l_sAlgoNameStdSTr.c_str());
			l_sHeaderDeclaration = l_sHeaderDeclaration + "\\t\\t\\tOpenViBE::Kernel::IAlgorithmProxy* "+ l_sAlgorithmProxy +";\\n";
			l_sInitialisation = l_sInitialisation + "\\t" +
				l_sAlgorithmProxy +" = \\&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_"+CString(l_sAlgoNameStdSTr.c_str())+"));\\n" +
				"\\t"+ l_sAlgorithmProxy +"->initialize();\\n";
			l_sUninitialisation = "\\tthis->getAlgorithmManager().releaseAlgorithm(*"+l_sAlgorithmProxy+");\\n" + l_sUninitialisation;
			//inputs of the algorithm
			for(map<CString,EParameterType>::iterator it = l_oDummyProto.m_vInputs.begin(); it!=l_oDummyProto.m_vInputs.end(); it++)
			{
				string l_sInputNameStdSTr((const char *)(*it).first);
				for(int s=0; s<l_sInputNameStdSTr.length(); s++)
				{
					if(l_sInputNameStdSTr[s]==' ')
					{
						l_sInputNameStdSTr.erase(s,1);
						if(l_sInputNameStdSTr[s] >= 'a' && l_sInputNameStdSTr[s]<= 'z') l_sInputNameStdSTr.replace(s,1,1,(char)(l_sInputNameStdSTr[s]+'A'-'a'));
					}
				}
				CString l_sInputHandler ="ip_p@"+ CString(l_sInputNameStdSTr.c_str());
				// input handlers for pointer must be "const"
				CString l_sConst = "";
				if(l_oDummyProto.m_vInputs[(*it).first] >= ParameterType_Matrix)
				{
					l_sConst = "const ";
				}
				l_sHeaderDeclaration = l_sHeaderDeclaration + "\\t\\t\\tOpenViBE::Kernel::TParameterHandler < " + l_sConst + (const char *)m_vParameterType_EnumTypeCorrespondance[l_oDummyProto.m_vInputs[(*it).first]] + "> "+ l_sInputHandler +";\\n";
				l_sInitialisation = l_sInitialisation + "\\t" +
					 l_sInputHandler + ".initialize("+ l_sAlgorithmProxy +"->getInputParameter(OVP_GD_Algorithm_"+CString(l_sAlgoNameStdSTr.c_str())+"_InputParameterId_"+CString(l_sInputNameStdSTr.c_str())+"));\\n";
				l_sInitialisation_ReferenceTargets = l_sInitialisation_ReferenceTargets + "\\t\\/\\/"+l_sInputHandler +".setReferenceTarget( ... )\\n";
				l_sUninitialisation = "\\t"+l_sInputHandler+ ".uninitialize();\\n" + l_sUninitialisation;
			}
			//outputs of the algorithm
			for(map<CString,EParameterType>::iterator it = l_oDummyProto.m_vOutputs.begin(); it!=l_oDummyProto.m_vOutputs.end(); it++)
			{
				string l_sOutputNameStdSTr((const char *)(*it).first);
				for(int s=0; s<l_sOutputNameStdSTr.length(); s++)
				{
					if(l_sOutputNameStdSTr[s]==' ')
					{
						l_sOutputNameStdSTr.erase(s,1);
						if(l_sOutputNameStdSTr[s] >= 'a' && l_sOutputNameStdSTr[s]<= 'z') l_sOutputNameStdSTr.replace(s,1,1,(char)(l_sOutputNameStdSTr[s]+'A'-'a'));
					}
				}
				CString l_sOutputHandler ="op_p@"+ CString(l_sOutputNameStdSTr.c_str());
				l_sHeaderDeclaration = l_sHeaderDeclaration + "\\t\\t\\tOpenViBE::Kernel::TParameterHandler < " + (const char *)m_vParameterType_EnumTypeCorrespondance[l_oDummyProto.m_vOutputs[(*it).first]] + "> "+ l_sOutputHandler +";\\n";
				l_sInitialisation = l_sInitialisation + "\\t" +
					l_sOutputHandler+ ".initialize("+ l_sAlgorithmProxy +"->getOutputParameter(OVP_GD_Algorithm_"+CString(l_sAlgoNameStdSTr.c_str())+"_OutputParameterId_"+CString(l_sOutputNameStdSTr.c_str())+"));\\n";
				l_sUninitialisation = "\\t"+ l_sOutputHandler+ ".uninitialize();\\n" + l_sUninitialisation;
			}
			l_sHeaderDeclaration = l_sHeaderDeclaration + "\\n";
			l_sInitialisation = l_sInitialisation + "\\n";
			l_sInitialisation_ReferenceTargets = l_sInitialisation_ReferenceTargets + "\\n";
			l_sUninitialisation ="\\t\\/\\/Algorithm : "+ l_sAlgo +"\\n" + l_sUninitialisation + "\\t"+l_sAlgorithmProxy+" = NULL;\\n\\n";
			
			m_mAlgorithmHeaderDeclaration[l_sAlgo]               = l_sHeaderDeclaration;
			m_mAlgorithmInitialisation[l_sAlgo]                  = l_sInitialisation;
			m_mAlgorithmUninitialisation[l_sAlgo]                = l_sUninitialisation;
			m_mAlgorithmInitialisation_ReferenceTargets[l_sAlgo] = l_sInitialisation_ReferenceTargets;
			m_rKernelContext.getLogManager() << LogLevel_Debug << "The algorithm [" << l_sAlgo << "] has description [" << l_sHeaderDeclaration << "\n";
		}
		l_Identifier =m_rKernelContext.getPluginManager().getNextPluginObjectDescIdentifier(l_Identifier);
	}

	

	gtk_combo_box_set_active(GTK_COMBO_BOX(l_pAlgoCombobox),-1);
	//callback to update algo description
	g_signal_connect(G_OBJECT(l_pAlgoCombobox),"changed", G_CALLBACK(algorithm_selected_cb),this);
	
	//Close with X and "cancel" button
	g_signal_connect (G_OBJECT(l_pBox),"delete_event",G_CALLBACK(::gtk_exit),0);
	::GtkWidget * l_pButtonCancel = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-exit-button"));
	g_signal_connect(l_pButtonCancel,"pressed", G_CALLBACK(button_exit_cb), this);

	//load everything from config file
	load(m_sConfigurationFile);

	gtk_widget_show_all(l_pBox);
}

boolean CBoxAlgorithmSkeletonGenerator::save(CString sFileName)
{
	FILE* l_pFile=::fopen(sFileName.toASCIIString(), "ab");
	if(!l_pFile)
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "Saving the box entries in [" << sFileName << "] failed !\n";
		return false;
	}
	::fprintf(l_pFile, "# ----------------------BOX GENERATOR-------------------------\n");
	// we need to replace the \ by / in the path for cross compatibility
	string::iterator it_directory;
	string l_sTempTargetDirectory(m_sTargetDirectory.toASCIIString());
	for(it_directory=l_sTempTargetDirectory.begin(); it_directory<l_sTempTargetDirectory.end(); it_directory++)
	{
		if((*it_directory)=='\\')
		{
			l_sTempTargetDirectory.replace(it_directory, it_directory+1, 1, '/');
		}
	}
	::fprintf(l_pFile, "SkeletonGenerator_Box_TargetDirectory = %s\n", l_sTempTargetDirectory.c_str());

	::fprintf(l_pFile, "SkeletonGenerator_Box_Name = %s\n",(const char *)m_sName);
	::fprintf(l_pFile, "SkeletonGenerator_Box_Version = %s\n",(const char *) m_sVersion);
	::fprintf(l_pFile, "SkeletonGenerator_Box_Category = %s\n",(const char *)m_sCategory);
	::fprintf(l_pFile, "SkeletonGenerator_Box_ClassName = %s\n",(const char *)m_sClassName);
	
	//we need to escape with '\' the special characters of the configuration manager files
	string l_sTempShortDescr((const char *)m_sShortDescription);
	l_sTempShortDescr.reserve(1000); // if we need to insert characters
	string::iterator it_sdescr=l_sTempShortDescr.begin();
	while(it_sdescr<l_sTempShortDescr.end())
	{
		//characters to escape
		if((*it_sdescr)=='\\' || (*it_sdescr)=='=' || (*it_sdescr)=='$' || (*it_sdescr)=='\t')
		{
			l_sTempShortDescr.insert(it_sdescr, '\\');
			it_sdescr++;
		}
		it_sdescr++;
	}
	::fprintf(l_pFile, "SkeletonGenerator_Box_ShortDescription = %s\n", l_sTempShortDescr.c_str());
	
	//we need to escape with '\' the special characters of the configuration manager files
	string l_sTempDetailedDescr((const char *)m_sDetailedDescription);
	m_rKernelContext.getLogManager() << LogLevel_Debug << "SAVE > DESCRIPTION FROM WIDGET: "<<l_sTempDetailedDescr.c_str()<<"\n";
	l_sTempDetailedDescr.reserve(1000); // if we need to insert characters
	string::iterator it_descr=l_sTempDetailedDescr.begin();
	while(it_descr<l_sTempDetailedDescr.end())
	{
		//characters to escape
		if((*it_descr)=='\\' || (*it_descr)=='=' || (*it_descr)=='$' || (*it_descr)=='\t')
		{
			l_sTempDetailedDescr.insert(it_descr, '\\');
			it_descr++;
		}
		//the special character we use for \n must also be escaped when used in the text
		else if((*it_descr)=='@')
		{
			l_sTempDetailedDescr.insert(it_descr, '\\');
			l_sTempDetailedDescr.insert(it_descr, '\\');
			it_descr+=2;
		}
		//we add a special character @ representing a \n for further loading. the \ ensure that the config manager will read the token past the \n
		else if((*it_descr)=='\n')
		{
			l_sTempDetailedDescr.insert(it_descr, '\\');
			l_sTempDetailedDescr.insert(it_descr, '@');
			it_descr+=2;
		}
		it_descr++;
	}
	m_rKernelContext.getLogManager() << LogLevel_Debug << "SAVE > DESCR MODIFIED: "<<l_sTempDetailedDescr.c_str()<<"\n";
	
	::fprintf(l_pFile, "SkeletonGenerator_Box_DetailedDescription = %s\n", (const char *) l_sTempDetailedDescr.c_str());
	
	::fprintf(l_pFile, "SkeletonGenerator_Box_IconIndex = %i\n",m_i32GtkStockItemIndex);
	::fprintf(l_pFile, "SkeletonGenerator_Box_IconName = %s\n",(const char *)m_sGtkStockItemName);
	
	// ADD/MODIFY FLAGS
	::fprintf(l_pFile, "SkeletonGenerator_Box_CanModifyInputs = %s\n",(m_bCanModifyInputs?"TRUE":"FALSE"));
	::fprintf(l_pFile, "SkeletonGenerator_Box_CanAddInputs = %s\n",(m_bCanAddInputs?"TRUE":"FALSE"));
	::fprintf(l_pFile, "SkeletonGenerator_Box_CanModifyOutputs = %s\n",(m_bCanModifyOutputs?"TRUE":"FALSE"));
	::fprintf(l_pFile, "SkeletonGenerator_Box_CanAddOutputs = %s\n",(m_bCanModifyOutputs?"TRUE":"FALSE"));
	::fprintf(l_pFile, "SkeletonGenerator_Box_CanModifySettings = %s\n",(m_bCanModifySettings?"TRUE":"FALSE"));
	::fprintf(l_pFile, "SkeletonGenerator_Box_CanAddSettings = %s\n",(m_bCanAddSettings?"TRUE":"FALSE"));
	
	// Inputs
	::fprintf(l_pFile, "SkeletonGenerator_Box_InputCount = %i\n",m_vInputs.size());
	for(uint32 i = 0; i < m_vInputs.size(); i++)
	{
		::fprintf(l_pFile, "SkeletonGenerator_Box_Input%i_Name = %s\n",i,(const char *)m_vInputs[i]._name);
		::fprintf(l_pFile, "SkeletonGenerator_Box_Input%i_Type = %s\n",i,(const char *)m_vInputs[i]._type);
		::fprintf(l_pFile, "SkeletonGenerator_Box_Input%i_TypeId = %s\n",i,(const char *)m_vInputs[i]._typeId);
	}
	// Outputs
	::fprintf(l_pFile, "SkeletonGenerator_Box_OutputCount = %i\n",m_vOutputs.size());
	for(uint32 i = 0; i < m_vOutputs.size(); i++)
	{
		::fprintf(l_pFile, "SkeletonGenerator_Box_Output%i_Name = %s\n",i,(const char *)m_vOutputs[i]._name);
		::fprintf(l_pFile, "SkeletonGenerator_Box_Output%i_Type = %s\n",i,(const char *)m_vOutputs[i]._type);
		::fprintf(l_pFile, "SkeletonGenerator_Box_Output%i_TypeId = %s\n",i,(const char *)m_vOutputs[i]._typeId);
	}
	// Inputs
	::fprintf(l_pFile, "SkeletonGenerator_Box_SettingCount = %i\n",m_vSettings.size());
	for(uint32 i = 0; i < m_vSettings.size(); i++)
	{
		::fprintf(l_pFile, "SkeletonGenerator_Box_Setting%i_Name = %s\n",i,(const char *)m_vSettings[i]._name);
		::fprintf(l_pFile, "SkeletonGenerator_Box_Setting%i_Type = %s\n",i,(const char *)m_vSettings[i]._type);
		::fprintf(l_pFile, "SkeletonGenerator_Box_Setting%i_TypeId = %s\n",i,(const char *)m_vSettings[i]._typeId);
		::fprintf(l_pFile, "SkeletonGenerator_Box_Setting%i_DefaultValue = %s\n",i,(const char *)m_vSettings[i]._defaultValue);
	}
	// Algorithms
	::fprintf(l_pFile, "SkeletonGenerator_Box_AlgorithmCount = %i\n",m_vAlgorithms.size());
	for(uint32 i = 0; i < m_vAlgorithms.size(); i++)
	{
		::fprintf(l_pFile, "SkeletonGenerator_Box_Algorithm%i_Name = %s\n",i,(const char *)m_vAlgorithms[i]);
	}
	::fprintf(l_pFile, "# --------------------------------------------------\n");
	::fclose(l_pFile);
	m_rKernelContext.getLogManager() << LogLevel_Info << "box entries saved in [" << sFileName << "]\n";

	m_bConfigurationFileLoaded = false; 

	return true;
}

boolean CBoxAlgorithmSkeletonGenerator::load(CString sFileName)
{
	if(!m_bConfigurationFileLoaded && !m_rKernelContext.getConfigurationManager().addConfigurationFromFile(sFileName))
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "box: Configuration file [" << sFileName << "] could not be loaded.\n";
		return false;
	}

	::GtkWidget * l_pClassNameEntry = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-class-name-entry"));
	CString l_sClassName = m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_Box_ClassName}");
	gtk_entry_set_text(GTK_ENTRY(l_pClassNameEntry),(const char *) l_sClassName);
	::GtkWidget * l_pCategoryEntry = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-category-entry"));
	CString l_sCategory = m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_Box_Category}");
	gtk_entry_set_text(GTK_ENTRY(l_pCategoryEntry),(const char *) l_sCategory);
	::GtkWidget * l_pNameEntry = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-box-name-entry"));
	CString l_sName = m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_Box_Name}");
	gtk_entry_set_text(GTK_ENTRY(l_pNameEntry),(const char *) l_sName);
	::GtkWidget * l_pVersionEntry = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-version-entry"));
	CString l_sVersion = m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_Box_Version}");
	gtk_entry_set_text(GTK_ENTRY(l_pVersionEntry),(const char *) l_sVersion);
	::GtkWidget * l_pSDEntry = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-short-description-entry"));
	
	CString l_sShortDescr = m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_Box_ShortDescription}");
	//we need to UNescape the special characters of the configuration manager files
	string::iterator it_sdescr;
	string l_sTempShortDescr(l_sShortDescr.toASCIIString());
	for(it_sdescr=l_sTempShortDescr.begin(); it_sdescr<l_sTempShortDescr.end(); it_sdescr++)
	{
		// juste erase the escape character
		if((*it_sdescr)=='\\' && (it_sdescr+1) != l_sTempShortDescr.end())
		{
			if((*(it_sdescr+1))=='\\' || (*(it_sdescr+1))=='=' || (*(it_sdescr+1))=='$' || (*(it_sdescr+1))=='\t' || (*(it_sdescr+1))=='@')
			{
				l_sTempShortDescr.erase(it_sdescr);
			}
		}
		// replace the special character @ by \n in the textview
		else if((*it_sdescr)=='@')
		{
			l_sTempShortDescr.erase(it_sdescr);
			l_sTempShortDescr.insert(it_sdescr,'\n');
		}
	}	
	gtk_entry_set_text(GTK_ENTRY(l_pSDEntry),l_sTempShortDescr.c_str());
	
	::GtkWidget * l_pDetailedDescrTextView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-detailed-description-textview"));
	::GtkTextBuffer * l_pDetailedDescrTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(l_pDetailedDescrTextView));
	CString l_sDetailedDescr = m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_Box_DetailedDescription}");
	
	//we need to UNescape the special characters of the configuration manager files
	string::iterator it_descr;
	string l_sTempDetailedDescr(l_sDetailedDescr.toASCIIString());
	m_rKernelContext.getLogManager() << LogLevel_Debug << "LOAD > DESCR LOADED: "<<l_sTempDetailedDescr.c_str()<<"\n";
	for(it_descr=l_sTempDetailedDescr.begin(); it_descr<l_sTempDetailedDescr.end(); it_descr++)
	{
		// juste erase the escape character
		if((*it_descr)=='\\' && (it_descr+1) != l_sTempDetailedDescr.end())
		{
			if((*(it_sdescr+1))=='\\' || (*(it_sdescr+1))=='=' || (*(it_sdescr+1))=='$' || (*(it_sdescr+1))=='\t' || (*(it_sdescr+1))=='@')
			{
				l_sTempDetailedDescr.erase(it_descr);
			}
		}
		// replace the special character @ by \n in the textview
		else if((*it_descr)=='@')
		{
			l_sTempDetailedDescr.erase(it_descr);
			l_sTempDetailedDescr.insert(it_descr,'\n');
		}
	}	
	m_rKernelContext.getLogManager() << LogLevel_Debug << "LOAD > DESCR MODIFIED: "<<l_sTempDetailedDescr.c_str()<<"\n";
	gtk_text_buffer_set_text(l_pDetailedDescrTextBuffer,l_sTempDetailedDescr.c_str(),((string)l_sTempDetailedDescr).length());

	GtkWidget * l_pIconCombobox =  GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-icon-combobox"));
	int32 l_iIconSelected = m_rKernelContext.getConfigurationManager().expandAsInteger("${SkeletonGenerator_Box_IconIndex}");
	gtk_combo_box_set_active(GTK_COMBO_BOX(l_pIconCombobox),l_iIconSelected);
	
	GtkWidget * l_pCanModifyInputsCheckbox =  GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-inputs-modify-checkbutton"));
	boolean l_pCanModifyInputs = m_rKernelContext.getConfigurationManager().expandAsBoolean("${SkeletonGenerator_Box_CanModifyInputs}");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(l_pCanModifyInputsCheckbox),l_pCanModifyInputs);

	GtkWidget * l_pCanAddInputsCheckbox =  GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-inputs-add-checkbutton"));
	boolean l_pCanAddInputs = m_rKernelContext.getConfigurationManager().expandAsBoolean("${SkeletonGenerator_Box_CanAddInputs}",false);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(l_pCanAddInputsCheckbox),l_pCanAddInputs);

	GtkWidget * l_pCanModifyOutputsCheckbox =  GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-outputs-modify-checkbutton"));
	boolean l_pCanModifyOutputs = m_rKernelContext.getConfigurationManager().expandAsBoolean("${SkeletonGenerator_Box_CanModifyOutputs}",false);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(l_pCanModifyOutputsCheckbox),l_pCanModifyOutputs);

	GtkWidget * l_pCanAddOutputsCheckbox =  GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-outputs-add-checkbutton"));
	boolean l_pCanAddOutputs = m_rKernelContext.getConfigurationManager().expandAsBoolean("${SkeletonGenerator_Box_CanAddOutputs}",false);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(l_pCanAddOutputsCheckbox),l_pCanAddOutputs);

	GtkWidget * l_pCanModifySettingsCheckbox =  GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-modify-checkbutton"));
	boolean l_pCanModifySettings = m_rKernelContext.getConfigurationManager().expandAsBoolean("${SkeletonGenerator_Box_CanModifySettings}",false);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(l_pCanModifySettingsCheckbox),l_pCanModifySettings);

	GtkWidget * l_pCanAddSettingsCheckbox =  GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-add-checkbutton"));
	boolean l_pCanAddSettings = m_rKernelContext.getConfigurationManager().expandAsBoolean("${SkeletonGenerator_Box_CanAddSettings}",false);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(l_pCanAddSettingsCheckbox),l_pCanAddSettings);

	::GtkWidget * l_pInputsTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-inputs-treeview"));
	::GtkTreeModel * l_pInputsListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pInputsTreeView));
	int32 l_i32InputCount = m_rKernelContext.getConfigurationManager().expandAsInteger("${SkeletonGenerator_Box_InputCount}",0);
	for(int32 i = 0; i < l_i32InputCount; i++)
	{
		char l_sTokenName [128];
		sprintf(l_sTokenName,"${SkeletonGenerator_Box_Input%i_Name}",i);
		CString l_sName =  m_rKernelContext.getConfigurationManager().expand(CString(l_sTokenName));
		char l_sTokenType [128];
		sprintf(l_sTokenType,"${SkeletonGenerator_Box_Input%i_Type}",i);
		CString l_sType =  m_rKernelContext.getConfigurationManager().expand(CString(l_sTokenType));
		char l_sTokenTypeId [128];
		sprintf(l_sTokenTypeId,"${SkeletonGenerator_Box_Input%i_TypeId}",i);
		CString l_sTypeId =  m_rKernelContext.getConfigurationManager().expand(CString(l_sTokenTypeId));
		GtkTreeIter l_iter;
		gtk_list_store_append(GTK_LIST_STORE(l_pInputsListStore), &l_iter);
		gtk_list_store_set (GTK_LIST_STORE(l_pInputsListStore), &l_iter, 0, (const char *)l_sName,1,(const char *)l_sType,2,(const char *)l_sTypeId,-1);
	}

	::GtkWidget * l_pOutputsTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-outputs-treeview"));
	::GtkTreeModel * l_pOutputsListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pOutputsTreeView));
	int32 l_i32OutputCount = m_rKernelContext.getConfigurationManager().expandAsInteger("${SkeletonGenerator_Box_OutputCount}",0);
	for(int32 i = 0; i < l_i32OutputCount; i++)
	{
		char l_sTokenName [128];
		sprintf(l_sTokenName,"${SkeletonGenerator_Box_Output%i_Name}",i);
		CString l_sName =  m_rKernelContext.getConfigurationManager().expand(CString(l_sTokenName));
		char l_sTokenType [128];
		sprintf(l_sTokenType,"${SkeletonGenerator_Box_Output%i_Type}",i);
		CString l_sType =  m_rKernelContext.getConfigurationManager().expand(CString(l_sTokenType));
		char l_sTokenTypeId [128];
		sprintf(l_sTokenTypeId,"${SkeletonGenerator_Box_Output%i_TypeId}",i);
		CString l_sTypeId =  m_rKernelContext.getConfigurationManager().expand(CString(l_sTokenTypeId));
		GtkTreeIter l_iter;
		gtk_list_store_append(GTK_LIST_STORE(l_pOutputsListStore), &l_iter);
		gtk_list_store_set (GTK_LIST_STORE(l_pOutputsListStore), &l_iter, 0, (const char *)l_sName,1,(const char *)l_sType,2,(const char *)l_sTypeId,-1);
	}

	::GtkWidget * l_pSettingsTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-treeview"));
	::GtkTreeModel * l_pSettingsListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pSettingsTreeView));
	int32 l_i32SettingCount = m_rKernelContext.getConfigurationManager().expandAsInteger("${SkeletonGenerator_Box_SettingCount}",0);
	for(int32 i = 0; i < l_i32SettingCount; i++)
	{
		char l_sTokenName [128];
		sprintf(l_sTokenName,"${SkeletonGenerator_Box_Setting%i_Name}",i);
		CString l_sName =  m_rKernelContext.getConfigurationManager().expand(CString(l_sTokenName));
		char l_sTokenType [128];
		sprintf(l_sTokenType,"${SkeletonGenerator_Box_Setting%i_Type}",i);
		CString l_sType =  m_rKernelContext.getConfigurationManager().expand(CString(l_sTokenType));
		char l_sTokenTypeId [128];
		sprintf(l_sTokenTypeId,"${SkeletonGenerator_Box_Setting%i_TypeId}",i);
		CString l_sTypeId =  m_rKernelContext.getConfigurationManager().expand(CString(l_sTokenTypeId));
		char l_sTokenDefaultValue [128];
		sprintf(l_sTokenDefaultValue,"${SkeletonGenerator_Box_Setting%i_DefaultValue}",i);
		CString l_sDefaultValue =  m_rKernelContext.getConfigurationManager().expand(CString(l_sTokenDefaultValue));
		GtkTreeIter l_iter;
		gtk_list_store_append(GTK_LIST_STORE(l_pSettingsListStore), &l_iter);
		gtk_list_store_set (GTK_LIST_STORE(l_pSettingsListStore), &l_iter, 0, (const char *)l_sName,1,(const char *)l_sType,2,(const char *)l_sDefaultValue,3,(const char *)l_sTypeId,-1);
	}
	
	::GtkWidget * l_pAlgoTreeView = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-treeview"));
	::GtkTreeModel * l_pAlgoListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pAlgoTreeView));
	int32 l_i32AlgoCount = m_rKernelContext.getConfigurationManager().expandAsInteger("${SkeletonGenerator_Box_AlgorithmCount}",0);
	for(int32 i = 0; i < l_i32AlgoCount; i++)
	{
		char l_sTokenName [128];
		sprintf(l_sTokenName,"${SkeletonGenerator_Box_Algorithm%i_Name}",i);
		CString l_sName =  m_rKernelContext.getConfigurationManager().expand(CString(l_sTokenName));
		GtkTreeIter l_iter;
		gtk_list_store_append(GTK_LIST_STORE(l_pAlgoListStore), &l_iter);
		gtk_list_store_set (GTK_LIST_STORE(l_pAlgoListStore), &l_iter, 0, (const char *)l_sName,-1);
	}

	::GtkWidget * l_pFileChooser = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-target-directory-filechooserbutton"));
	CString l_sTargetDirectory;
	// if the user specified a target directory, it has full priority
	l_sTargetDirectory = m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_TargetDirectory}");
	boolean l_bNeedFilePrefix = false;
	if((string)l_sTargetDirectory != string(""))
	{
		m_rKernelContext.getLogManager() << LogLevel_Debug << "Target dir user  [" << l_sTargetDirectory << "]\n";
		l_bNeedFilePrefix = true;
	}
	else
	{
		//previous entry
		l_sTargetDirectory = m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_Box_TargetDirectory}");
		if((string)l_sTargetDirectory != string(""))
		{
			m_rKernelContext.getLogManager() << LogLevel_Debug << "Target previous  [" << l_sTargetDirectory << "]\n";
			l_bNeedFilePrefix = true;
		}
		else
		{
			//default path = dist
			m_rKernelContext.getLogManager() << LogLevel_Debug << "Target default  [dist]\n";
#ifdef OV_OS_Linux
			l_sTargetDirectory = CString(gtk_file_chooser_get_current_folder_uri(GTK_FILE_CHOOSER(l_pFileChooser)));
			l_sTargetDirectory = l_sTargetDirectory + "/..";
#elif defined OV_OS_Windows
			l_sTargetDirectory = "..";
#endif
		}
	}
#ifdef OV_OS_Linux
	if(l_bNeedFilePrefix) l_sTargetDirectory = "file://"+l_sTargetDirectory;
	gtk_file_chooser_set_current_folder_uri(GTK_FILE_CHOOSER(l_pFileChooser),(const char *)l_sTargetDirectory);
#elif defined OV_OS_Windows
	gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(l_pFileChooser),(const char *)l_sTargetDirectory);
#endif

	m_rKernelContext.getLogManager() << LogLevel_Info << "box entries from [" << m_sConfigurationFile << "] loaded.\n";
	
	return true;
}

void CBoxAlgorithmSkeletonGenerator::getCurrentParameters(void){
	::GtkWidget * l_pEntryBoxName = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-box-name-entry"));
	m_sName = gtk_entry_get_text(GTK_ENTRY(l_pEntryBoxName));
	
	::GtkWidget * l_pEntryClassName = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-class-name-entry"));
	m_sClassName = gtk_entry_get_text(GTK_ENTRY(l_pEntryClassName));
	
	::GtkWidget * l_pEntryCategory = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-category-entry"));
	m_sCategory = gtk_entry_get_text(GTK_ENTRY(l_pEntryCategory));

	::GtkWidget * l_pEntryVersion = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-version-entry"));
	m_sVersion = gtk_entry_get_text(GTK_ENTRY(l_pEntryVersion));
	
	::GtkWidget * l_pEntryShortDescr = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-short-description-entry"));
	m_sShortDescription = gtk_entry_get_text(GTK_ENTRY(l_pEntryShortDescr));
	
	::GtkWidget * l_pEntryDetailedDescr = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-detailed-description-textview"));
	::GtkTextBuffer * l_pTextBufferDetailedDescr = gtk_text_view_get_buffer(GTK_TEXT_VIEW(l_pEntryDetailedDescr));
	::GtkTextIter l_TextIterStart,l_TextIterEnd;
	gtk_text_buffer_get_start_iter(l_pTextBufferDetailedDescr,&l_TextIterStart);
	gtk_text_buffer_get_end_iter(l_pTextBufferDetailedDescr,&l_TextIterEnd);
	m_sDetailedDescription = gtk_text_buffer_get_text(l_pTextBufferDetailedDescr,&l_TextIterStart,&l_TextIterEnd, false);
	
	::GtkWidget * l_pIconCombobox = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-icon-combobox"));
	::GtkTreeModel * l_pIconListStore = gtk_combo_box_get_model(GTK_COMBO_BOX(l_pIconCombobox));
	::GtkTreeIter l_iterIcon;
	m_i32GtkStockItemIndex = gtk_combo_box_get_active(GTK_COMBO_BOX(l_pIconCombobox)); // can be -1 if nothing selected
	gtk_tree_model_iter_nth_child(l_pIconListStore,&l_iterIcon,NULL,m_i32GtkStockItemIndex);
	gchar * l_sData;
	gtk_tree_model_get(l_pIconListStore, &l_iterIcon,0, &l_sData,-1);
	m_sGtkStockItemName = CString((const char *)l_sData);

	::GtkWidget * l_pCanModifyInputsCheckbox = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-inputs-modify-checkbutton"));
	m_bCanModifyInputs = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(l_pCanModifyInputsCheckbox));
	::GtkWidget * l_pCanAddInputsCheckbox = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-inputs-add-checkbutton"));
	m_bCanAddInputs = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(l_pCanAddInputsCheckbox));
	::GtkWidget * l_pCanModifyOutputsCheckbox = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-outputs-modify-checkbutton"));
	m_bCanModifyOutputs = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(l_pCanModifyOutputsCheckbox));
	::GtkWidget * l_pCanAddOutputsCheckbox = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-outputs-add-checkbutton"));
	m_bCanAddOutputs = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(l_pCanAddOutputsCheckbox));
	::GtkWidget * l_pCanModifySettingsCheckbox = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-modify-checkbutton"));
	m_bCanModifySettings = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(l_pCanModifySettingsCheckbox));
	::GtkWidget * l_pCanAddSettingsCheckbox = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-add-checkbutton"));
	m_bCanAddSettings = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(l_pCanAddSettingsCheckbox));

	::GtkWidget * l_pFileChooser = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-target-directory-filechooserbutton"));
	char * l_pTargetDirectory = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(l_pFileChooser));
	m_sTargetDirectory = CString(l_pTargetDirectory);
	g_free(l_pTargetDirectory);

	::GtkWidget * l_pInputsTreeview = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-inputs-treeview"));
	::GtkTreeModel * l_pInputListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pInputsTreeview));
	GtkTreeIter l_iter;
	boolean l_bValid = gtk_tree_model_get_iter_first(l_pInputListStore,&l_iter);
	gint l_iRowCount = 0;
	m_vInputs.clear();
	while(l_bValid)
	{
		/* Walk through the list, reading each row */
		gchar * l_sName;
		gchar * l_sType;
		gchar * l_sTypeOv;
		gtk_tree_model_get(l_pInputListStore, &l_iter,0, &l_sName,1, &l_sType,2,&l_sTypeOv,-1);
		
		IOSStruct l_struct;
		l_struct._name=l_sName;
		l_struct._type=l_sType;
		l_struct._typeId=l_sTypeOv;
		m_vInputs.push_back(l_struct);
		
		g_free(l_sName);g_free (l_sType);g_free(l_sTypeOv);
		l_iRowCount++;
		l_bValid = gtk_tree_model_iter_next (l_pInputListStore, &l_iter);
	}

	::GtkWidget * l_pOutputsTreeview = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-outputs-treeview"));
	::GtkTreeModel * l_pOutputListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pOutputsTreeview));
	GtkTreeIter l_iterOutput;
	l_bValid = gtk_tree_model_get_iter_first(l_pOutputListStore,&l_iterOutput);
	l_iRowCount = 0;
	m_vOutputs.clear();
	while(l_bValid)
	{
		/* Walk through the list, reading each row */
		gchar * l_sName;
		gchar * l_sType;
		gchar * l_sTypeOv;
		gtk_tree_model_get(l_pOutputListStore, &l_iterOutput,0, &l_sName,1, &l_sType,2,&l_sTypeOv,-1);
		
		IOSStruct l_struct;
		l_struct._name=l_sName;
		l_struct._type=l_sType;
		l_struct._typeId=l_sTypeOv;
		m_vOutputs.push_back(l_struct);
			
		g_free(l_sName);g_free(l_sType);g_free(l_sTypeOv);
		l_iRowCount++;
		l_bValid = gtk_tree_model_iter_next (l_pOutputListStore, &l_iterOutput);
	}

	::GtkWidget * l_pSettingsTreeview = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-settings-treeview"));
	::GtkTreeModel * l_pSettingListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pSettingsTreeview));
	GtkTreeIter l_iterSetting;
	l_bValid = gtk_tree_model_get_iter_first(l_pSettingListStore,&l_iterSetting);
	l_iRowCount = 0;
	m_vSettings.clear();
	while(l_bValid)
	{
		/* Walk through the list, reading each row */
		gchar * l_sName;
		gchar * l_sType;gchar * l_sTypeOv;
		gchar * l_sDefaultValue;
		gtk_tree_model_get(l_pSettingListStore, &l_iterSetting,0, &l_sName,1, &l_sType, 2, &l_sDefaultValue,3,&l_sTypeOv,-1);
		
		IOSStruct l_struct;
		l_struct._name = l_sName;
		l_struct._type = l_sType;
		l_struct._typeId=l_sTypeOv;
		l_struct._defaultValue = l_sDefaultValue;
		m_vSettings.push_back(l_struct);

		g_free(l_sName);g_free(l_sType);g_free(l_sDefaultValue);g_free(l_sTypeOv);
		l_iRowCount++;
		l_bValid = gtk_tree_model_iter_next (l_pSettingListStore, &l_iterSetting);
	}

	::GtkWidget * l_pAlgosTreeview = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "sg-box-algorithms-treeview"));
	::GtkTreeModel * l_pAlgosListStore = gtk_tree_view_get_model(GTK_TREE_VIEW(l_pAlgosTreeview));
	GtkTreeIter l_iterAlgo;
	l_bValid = gtk_tree_model_get_iter_first(l_pAlgosListStore,&l_iterAlgo);
	l_iRowCount = 0;
	m_vAlgorithms.clear();
	while(l_bValid)
	{
		/* Walk through the list, reading each row */
		gchar * l_sName;
		gtk_tree_model_get(l_pAlgosListStore, &l_iterAlgo,0, &l_sName,-1);

		m_vAlgorithms.push_back(l_sName);

		g_free(l_sName);
		l_iRowCount++;
		l_bValid = gtk_tree_model_iter_next (l_pAlgosListStore, &l_iterAlgo);
	}
}

CString  CBoxAlgorithmSkeletonGenerator::getRandomIdentifierString(void)
{
	srand((unsigned int)time(NULL));
	unsigned short int l_ui16Value1=(rand()&0xffff);
	unsigned short int l_ui16Value2=(rand()&0xffff);
	unsigned short int l_ui16Value3=(rand()&0xffff);
	unsigned short int l_ui16Value4=(rand()&0xffff);

	char buffer[50];
	sprintf(buffer,"OpenViBE::CIdentifier(0x%04X%04X, 0x%04X%04X)",(int)l_ui16Value1, (int)l_ui16Value2, (int)l_ui16Value3, (int)l_ui16Value4);
	return CString(buffer);
}
