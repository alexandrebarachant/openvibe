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
	,m_bConfigurationFileLoaded(false)
{
	m_sConfigurationFile = m_rKernelContext.getConfigurationManager().expand("${CustomConfigurationApplication}");
	m_rKernelContext.getLogManager() << LogLevel_Trace << "Configuration file [" << m_sConfigurationFile << "]\n";
	loadCommonParameters(m_sConfigurationFile);
}

CSkeletonGenerator::~CSkeletonGenerator(void)
{
}

void CSkeletonGenerator::getCommonParameters()
{
	//Author and Company
	::GtkWidget* l_pEntryCompany=GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "entry_company_name"));
	m_sCompany=CString("");
	m_sCompany=gtk_entry_get_text(GTK_ENTRY(l_pEntryCompany));

	::GtkWidget* l_pEntryAuthor =GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "entry_author_name"));
	m_sAuthor=CString("");
	m_sAuthor=gtk_entry_get_text(GTK_ENTRY(l_pEntryAuthor));
}

boolean CSkeletonGenerator::saveCommonParameters(CString sFileName)
{
	// we get the latest values
	getCommonParameters();

	FILE* l_pFile=::fopen(sFileName.toASCIIString(), "ab");
	if(!l_pFile)
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "Saving the common entries in [" << sFileName << "] failed !\n";
		return false;
	}

	::fprintf(l_pFile, "SkeletonGenerator_Common_Author = %s\n", m_sAuthor.toASCIIString());
	::fprintf(l_pFile, "SkeletonGenerator_Common_Company = %s\n", m_sCompany.toASCIIString());
	::fclose(l_pFile);
	m_rKernelContext.getLogManager() << LogLevel_Info << "Common entries saved in [" << sFileName << "]\n";

	//we can reload the file, it may have changed
	m_bConfigurationFileLoaded = false;

	return true;
}

boolean CSkeletonGenerator::cleanConfigurationFile(CString sFileName)
{
	FILE* l_pFile=::fopen(sFileName.toASCIIString(), "wb");
	if(!l_pFile)
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "Failed to clean [" << sFileName << "]\n";
		return false;
	}

	m_rKernelContext.getLogManager() << LogLevel_Info << "Configuration file [" << sFileName << "] cleaned.\n";
	::fclose(l_pFile);
	return true;
}

boolean CSkeletonGenerator::loadCommonParameters(CString sFileName)
{
	if(!m_bConfigurationFileLoaded && !m_rKernelContext.getConfigurationManager().addConfigurationFromFile(sFileName))
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "Common: Configuration file [" << sFileName << "] could not be loaded. \n";
		return false;
	}

	::GtkWidget* l_pEntryCompany=GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "entry_company_name"));
	gtk_entry_set_text(GTK_ENTRY(l_pEntryCompany),m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_Common_Company}"));

	::GtkWidget * l_pEntryAuthorName = GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "entry_author_name"));
	gtk_entry_set_text(GTK_ENTRY(l_pEntryAuthorName),m_rKernelContext.getConfigurationManager().expand("${SkeletonGenerator_Common_Author}"));

	m_rKernelContext.getLogManager() << LogLevel_Info << "Common entries from [" << sFileName << "] loaded.\n";

	m_bConfigurationFileLoaded = true;

	return true;
}
