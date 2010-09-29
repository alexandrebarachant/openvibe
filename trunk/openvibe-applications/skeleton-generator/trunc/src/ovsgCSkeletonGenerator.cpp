#include "ovsgCSkeletonGenerator.h"

#include <string>

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBESkeletonGenerator;


CSkeletonGenerator::CSkeletonGenerator(IKernelContext & rKernelContext, ::GtkBuilder * pBuilderInterface)
	:m_rKernelContext(rKernelContext)
	,m_pBuilderInterface(pBuilderInterface)
{
	m_sConfigurationFile = m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_ConfigurationFile}");
	if(m_sConfigurationFile == CString("")) 
	{
		m_sConfigurationFile = m_rKernelContext.getConfigurationManager().expand("${UserHome}")+"\\openvibe-sk-generator.conf";
		m_rKernelContext.getLogManager() << LogLevel_Trace << "No configuration file specified. Using Default ("+m_sConfigurationFile+").\n";	
	}
	else
	{
		m_rKernelContext.getLogManager() << LogLevel_Trace << "Configuration file : "+m_sConfigurationFile+".\n";	
	}
	loadCommon();
}
CSkeletonGenerator::~CSkeletonGenerator(void)
{
}

void CSkeletonGenerator::getCommon()
{
	//Author and Company
	::GtkWidget* l_pEntryCompany=GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "entry_company_name"));
	m_sCompany=CString("");
	m_sCompany=gtk_entry_get_text(GTK_ENTRY(l_pEntryCompany));

	::GtkWidget* l_pEntryAuthor =GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "entry_author_name"));
	m_sAuthor=CString("");
	m_sAuthor=gtk_entry_get_text(GTK_ENTRY(l_pEntryAuthor));
}

void CSkeletonGenerator::saveCommon(void)
{
	// we get the latest values
	getCommon();
	
	CString l_sCommand = "echo SkeletonGenerator_Common_Author="+ m_sAuthor +" >> \"" + m_sConfigurationFile + "\"\n";
	m_rKernelContext.getLogManager() << LogLevel_Trace << "Command: "+l_sCommand+"\n";
	boolean l_bSuccess = (system(((string)l_sCommand).c_str()) == 0);
	
	l_sCommand = "echo SkeletonGenerator_Common_Company="+ m_sCompany +" >> \"" + m_sConfigurationFile + "\"\n";
	m_rKernelContext.getLogManager() << LogLevel_Trace << "Command: "+l_sCommand+"\n";
	l_bSuccess &= (system(((string)l_sCommand).c_str()) == 0);
	
	if(l_bSuccess)
	{
		m_rKernelContext.getLogManager() << LogLevel_Info << "Common entries saved in "+m_sConfigurationFile+"\n";
	}
	else
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "Saving the common entries in "+m_sConfigurationFile+" failed !\n";
	}
}

void CSkeletonGenerator::cleanConfigurationFile(void)
{

	CString l_sCommand = "echo #Skeleton-generator configuration file > \"" + m_sConfigurationFile + "\"\n";
	m_rKernelContext.getLogManager() << LogLevel_Trace << "Command: "+l_sCommand+"\n";

	boolean l_bSuccess = (system(((string)l_sCommand).c_str()) == 0);
	if(l_bSuccess)
	{
		m_rKernelContext.getLogManager() << LogLevel_Info << m_sConfigurationFile+" cleaned.\n";
	}
	else
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "Failed to clean "+m_sConfigurationFile+"\n";
	}
}


void CSkeletonGenerator::loadCommon(void)
{
	if(m_rKernelContext.getConfigurationManager().addConfigurationFromFile(m_sConfigurationFile))
	{
		::GtkWidget* l_pEntryCompany=GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "entry_company_name"));
		gtk_entry_set_text(GTK_ENTRY(l_pEntryCompany),m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_Common_Company}"));
		
		::GtkWidget * l_pEntryAuthorName = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "entry_author_name"));
		gtk_entry_set_text(GTK_ENTRY(l_pEntryAuthorName),m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_Common_Author}"));
		
		m_rKernelContext.getLogManager() << LogLevel_Info << "Common entries from "+m_sConfigurationFile+" loaded.\n";
	}
	else
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "Common: No configuration file specified. It will be automatically generated after first use. \n";
	}


}
