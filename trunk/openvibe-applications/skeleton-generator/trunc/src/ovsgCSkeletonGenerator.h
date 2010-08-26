#ifndef __OpenViBESkeletonGenerator_CSkeletonGenerator_H__
#define __OpenViBESkeletonGenerator_CSkeletonGenerator_H__

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>

namespace OpenViBESkeletonGenerator
{
	class CSkeletonGenerator
	{
	public:

		CSkeletonGenerator(OpenViBE::Kernel::IKernelContext & rKernelContext, ::GtkBuilder * pBuilderInterface);
		virtual ~CSkeletonGenerator(void);

		virtual void initialize(void)=0;

		OpenViBE::CString             m_sAuthor;
		OpenViBE::CString             m_sCompany;
		OpenViBE::CString             m_sTargetDirectory;

		void getCommon(void);

	protected:

		OpenViBE::Kernel::IKernelContext &m_rKernelContext;

	public:

		::GtkBuilder * m_pBuilderInterface;
	};

}

#endif //__OpenViBESkeletonGenerator_CSkeletonGenerator_H__
