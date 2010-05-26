#ifndef __OpenViBESkeletonGenerator_CSkeletonGenerator_H__
#define __OpenViBESkeletonGenerator_CSkeletonGenerator_H__

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <vector>

namespace OpenViBESkeletonGenerator
{
	class CSkeletonGenerator
	{
	public:
		CSkeletonGenerator(OpenViBE::Kernel::IKernelContext & rKernelContext, const char* sGladeXMLFileName);
		virtual ~CSkeletonGenerator(void);

		void go(void);

		void initializeAlgoGenerator(void);
		void initializeBoxGenerator(void);
		void initializeDriverGenerator(void);

		::GladeXML * m_pGladeInterface;

		OpenViBE::CString             m_sAuthor;
		OpenViBE::CString             m_sCompany;
		OpenViBE::CString             m_sDriverName;
		OpenViBE::CString             m_sClassName;
		std::vector<OpenViBE::CString> m_vSamplingFrequencies;
		OpenViBE::CString              m_sMinChannel;
		OpenViBE::CString              m_sMaxChannel;
		OpenViBE::CString             m_sTargetDirectory;

		void buttonCheckCB(void);
		void buttonOkCB(void);
		void buttonTooltipCB(::GtkButton* pButton);
	private:

		OpenViBE::Kernel::IKernelContext &m_rKernelContext;

		
	};

}

#endif //__OpenViBESkeletonGenerator_CSkeletonGenerator_H__
