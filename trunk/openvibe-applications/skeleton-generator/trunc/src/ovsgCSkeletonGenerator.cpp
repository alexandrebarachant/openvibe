#include "ovsgCSkeletonGenerator.h"

#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>

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
	m_sCompany = gtk_entry_get_text(GTK_ENTRY(l_pEntryCompany));

	::GtkWidget* l_pEntryAuthor =GTK_WIDGET(gtk_builder_get_object(m_pBuilderInterface, "entry_author_name"));
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

CString CSkeletonGenerator::ensureSedCompliancy(CString sExpression)
{
	string l_sExpression((const char*)sExpression);
	string::iterator it=l_sExpression.begin();
	while(it<l_sExpression.end())
	{
		if((*it)=='\\')
		{
			l_sExpression.insert(it,'\\');
			l_sExpression.insert(it,'\\');
			it++;
			it++;
		}
		else if((*it)=='/')
		{
			l_sExpression.insert(it,'\\');
			it++;
		}
		it++;
	}

	size_t l_iStringIndex = l_sExpression.find('\n');
	while(l_iStringIndex != string::npos)
	{
		l_sExpression.replace(l_iStringIndex,1,"\\\\n");
		l_iStringIndex = l_sExpression.find('\n',l_iStringIndex+2);
	}

	return CString(l_sExpression.c_str());
}

boolean CSkeletonGenerator::executeSedCommand(CString sTemplateFile, CString sCommand, CString sDestinationFile)
{
	CString l_sSed;
	CString l_sMove;
#ifdef OV_OS_Windows
	l_sSed = "..\\share\\openvibe-applications\\skeleton-generator\\sed";
	l_sMove = "move";
#else
#ifdef OV_OS_Linux
	l_sSed = "sed";
	l_sMove = "mv";
#endif
#endif

	boolean l_bSuccess = false;

	CString l_sCommandSed = l_sSed + " \"" + sCommand+"\" \"" + sTemplateFile + "\"";
	if(string(sDestinationFile) != string(""))
	{
		l_sCommandSed =  l_sCommandSed + " > \"" + sDestinationFile + "\"";
		l_bSuccess = (system(((string)l_sCommandSed).c_str()) == 0);
	}
	else
	{
		l_sCommandSed =  l_sCommandSed + " > tmp-sed";
		l_bSuccess = (system(l_sCommandSed) == 0);
		CString l_sMoveCommand = l_sMove + " tmp-sed \"" + sTemplateFile + "\" >> NULL";
		l_bSuccess &= (system(l_sMoveCommand) == 0);
		m_rKernelContext.getLogManager() << LogLevel_Trace << " -- Move command : [" << l_sMoveCommand << "]\n";
	}
	
	l_bSuccess = (system(((string)l_sCommandSed).c_str()) == 0);

	if(l_bSuccess)
	{
		m_rKernelContext.getLogManager() << LogLevel_Trace << " -- Sed command successfully called : [" << l_sCommandSed << "]\n";
	}
	else
	{
		m_rKernelContext.getLogManager() << LogLevel_Error << " -- Faulty substitution command : [" << l_sCommandSed <<"]\n";
	}

	return l_bSuccess;

}

boolean CSkeletonGenerator::executeSedSubstitution(CString sTemplateFile, CString sTag, CString sSubstitute, CString sDestinationFile)
{
	CString l_sSed;
	CString l_sMove;
	CString l_sNull;
#ifdef OV_OS_Windows
	l_sSed = "..\\share\\openvibe-applications\\skeleton-generator\\sed";
	l_sMove = "move";
	l_sNull = "NULL";
#else
#ifdef OV_OS_Linux
	l_sSed = "sed";
	l_sMove = "mv";
	l_sNull = "/dev/null";
#endif
#endif

	boolean l_bSuccess = false;

	CString l_sSubstitute = ensureSedCompliancy(sSubstitute);

	CString l_sCommandSed = l_sSed + " \"s/" + sTag + "/" + l_sSubstitute+ "/g\" \"" + sTemplateFile + "\"";
	if(string(sDestinationFile) != string(""))
	{
		l_sCommandSed =  l_sCommandSed + " > \"" + sDestinationFile + "\"";
		l_bSuccess = (system(((string)l_sCommandSed).c_str()) == 0);
	}
	else
	{
		l_sCommandSed =  l_sCommandSed + " > tmp-sed";
		l_bSuccess = (system(l_sCommandSed) == 0);
		CString l_sMoveCommand = l_sMove + " tmp-sed \"" + sTemplateFile + "\" >> NULL";
		l_bSuccess &= (system(l_sMoveCommand) == 0);
		m_rKernelContext.getLogManager() << LogLevel_Trace << " -- Move command : [" << l_sMoveCommand << "]\n";
#ifdef OV_OS_Windows
		l_bSuccess &= (system("del NULL") == 0);
#endif
	}

	if(l_bSuccess)
	{
		m_rKernelContext.getLogManager() << LogLevel_Trace << " -- Sed command successfully called : [" << l_sCommandSed << "]\n";
	}
	else
	{
		m_rKernelContext.getLogManager() << LogLevel_Error << " -- Faulty substitution command : [" << l_sCommandSed <<"]\n";
	}

	return l_bSuccess;
}

CString CSkeletonGenerator::getDate()
{
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	stringstream ssTime;
	string string_time(asctime (timeinfo));
	string_time = string_time.substr(0,string_time.size()-1); // the ascitime ends with a "\n"
	CString l_sDate(string_time.c_str());
	
	return l_sDate;
}

