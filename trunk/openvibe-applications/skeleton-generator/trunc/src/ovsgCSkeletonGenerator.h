#ifndef __OpenViBESkeletonGenerator_CSkeletonGenerator_H__
#define __OpenViBESkeletonGenerator_CSkeletonGenerator_H__

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

namespace OpenViBESkeletonGenerator
{
	class CSkeletonGenerator
	{
	public:
		CSkeletonGenerator(OpenViBE::Kernel::IKernelContext & rKernelContext, ::GladeXML * pGladeInterface);
		virtual ~CSkeletonGenerator(void);

		virtual void initialize(void)=0;

		::GladeXML * m_pGladeInterface;

		OpenViBE::CString             m_sAuthor;
		OpenViBE::CString             m_sCompany;
		OpenViBE::CString             m_sTargetDirectory;

		void getCommon(void);

	protected:

		OpenViBE::Kernel::IKernelContext &m_rKernelContext;

		
	};

}

#endif //__OpenViBESkeletonGenerator_CSkeletonGenerator_H__
