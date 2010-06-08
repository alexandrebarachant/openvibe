#include "ovsgCSkeletonGenerator.h"

#include <string>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBESkeletonGenerator;


CSkeletonGenerator::CSkeletonGenerator(IKernelContext & rKernelContext, ::GladeXML * pGladeInterface)
: m_rKernelContext(rKernelContext),m_pGladeInterface(pGladeInterface)
{
}
CSkeletonGenerator::~CSkeletonGenerator(void)
{
	
}

void CSkeletonGenerator::getCommon()
{
	//Author and Company
	::GtkWidget * l_pEntryCompany = glade_xml_get_widget(m_pGladeInterface, "entry_company_name");
	m_sCompany = CString("");
	m_sCompany = gtk_entry_get_text(GTK_ENTRY(l_pEntryCompany));

	::GtkWidget * l_pEntryAuthor = glade_xml_get_widget(m_pGladeInterface, "entry_author_name");
	m_sAuthor =CString("");
	m_sAuthor= gtk_entry_get_text(GTK_ENTRY(l_pEntryAuthor));
	
	m_sTargetDirectory = "./dir/";
}
