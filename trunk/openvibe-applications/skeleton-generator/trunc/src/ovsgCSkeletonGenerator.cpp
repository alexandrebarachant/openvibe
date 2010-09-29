#include "ovsgCSkeletonGenerator.h"

#include <string>
#include <cstdlib>

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBESkeletonGenerator;

CSkeletonGenerator::CSkeletonGenerator(IKernelContext & rKernelContext, ::GtkBuilder * pBuilderInterface)
	:m_rKernelContext(rKernelContext)
	,m_pBuilderInterface(pBuilderInterface)
{
	m_sConfigurationFile = m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_ConfigurationFile}");
	m_rKernelContext.getLogManager() << LogLevel_Trace << "Configuration file [" << m_sConfigurationFile << "]\n";
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

boolean CSkeletonGenerator::saveCommon(void)
{
	// we get the latest values
	getCommon();

	FILE* l_pFile=::fopen(m_sConfigurationFile.toASCIIString(), "ab");
	if(!l_pFile)
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "Saving the common entries in [" << m_sConfigurationFile << "] failed !\n";
		return false;
	}

	::fprintf(l_pFile, "SkeletonGenerator_Common_Author = %s\n", m_sAuthor.toASCIIString());
	::fprintf(l_pFile, "SkeletonGenerator_Common_Company = %s\n", m_sCompany.toASCIIString());
	::fclose(l_pFile);
	m_rKernelContext.getLogManager() << LogLevel_Info << "Common entries saved in [" << m_sConfigurationFile << "]\n";

	return true;
}

boolean CSkeletonGenerator::cleanConfigurationFile(void)
{
	FILE* l_pFile=::fopen(m_sConfigurationFile.toASCIIString(), "wb");
	if(!l_pFile)
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "Failed to clean [" << m_sConfigurationFile << "]\n";
		return false;
	}

	m_rKernelContext.getLogManager() << LogLevel_Info << "Configuration file [" << m_sConfigurationFile << "] cleaned.\n";
	::fclose(l_pFile);
	return true;
}

boolean CSkeletonGenerator::loadCommon(void)
{
	if(!m_rKernelContext.getConfigurationManager().addConfigurationFromFile(m_sConfigurationFile))
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "Common: Configuration file [" << m_sConfigurationFile << "] could not be loaded. It will be automatically generated after first use.\n";
		return false;
	}

	::GtkWidget* l_pEntryCompany=GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "entry_company_name"));
	gtk_entry_set_text(GTK_ENTRY(l_pEntryCompany),m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_Common_Company}"));

	::GtkWidget * l_pEntryAuthorName = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "entry_author_name"));
	gtk_entry_set_text(GTK_ENTRY(l_pEntryAuthorName),m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_Common_Author}"));

	m_rKernelContext.getLogManager() << LogLevel_Info << "Common entries from [" << m_sConfigurationFile << "] loaded.\n";

	return true;
}
