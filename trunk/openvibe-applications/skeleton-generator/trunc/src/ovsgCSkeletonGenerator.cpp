#include "ovsgCSkeletonGenerator.h"

#include <string>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBESkeletonGenerator;


CSkeletonGenerator::CSkeletonGenerator(IKernelContext & rKernelContext, ::GtkBuilder * pBuilderInterface)
	:m_rKernelContext(rKernelContext)
	,m_pBuilderInterface(pBuilderInterface)
{
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

	m_sTargetDirectory = "";
}
