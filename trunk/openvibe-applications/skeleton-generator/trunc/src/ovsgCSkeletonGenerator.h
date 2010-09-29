#ifndef __OpenViBESkeletonGenerator_CSkeletonGenerator_H__
#define __OpenViBESkeletonGenerator_CSkeletonGenerator_H__

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>
//#include <configuration/ovkCConfigurationManager.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>

namespace OpenViBESkeletonGenerator
{
	class CSkeletonGenerator
	{
	public:

		CSkeletonGenerator(OpenViBE::Kernel::IKernelContext & rKernelContext, ::GtkBuilder * pBuilderInterface);
		virtual ~CSkeletonGenerator(void);

	protected:
		OpenViBE::Kernel::IKernelContext &m_rKernelContext;
		
		::GtkBuilder * m_pBuilderInterface;

		OpenViBE::CString             m_sAuthor;
		OpenViBE::CString             m_sCompany;
		OpenViBE::CString             m_sTargetDirectory;

		virtual void initialize(void)=0;
		
		void getCommon(void);
		OpenViBE::boolean saveCommon(void);
		OpenViBE::boolean loadCommon(void);

		OpenViBE::CString m_sConfigurationFile;
		OpenViBE::boolean cleanConfigurationFile(void);
		
		virtual OpenViBE::boolean save(void) = 0;
		virtual OpenViBE::boolean load(void) = 0;
		

	};

}

#endif //__OpenViBESkeletonGenerator_CSkeletonGenerator_H__
