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
		
		OpenViBE::CString m_sConfigurationFile; // basic application config file
		OpenViBE::boolean m_bConfigurationFileLoaded;

		void getCommonParameters(void);
		OpenViBE::boolean saveCommonParameters(OpenViBE::CString sFileName);
		OpenViBE::boolean loadCommonParameters(OpenViBE::CString sFileName);

		OpenViBE::boolean cleanConfigurationFile(OpenViBE::CString sFileName);
		
		virtual void getCurrentParameters(void) = 0;
		virtual OpenViBE::boolean save(OpenViBE::CString sFileName) = 0;
		virtual OpenViBE::boolean load(OpenViBE::CString sFileName) = 0;
		

	};

}

#endif //__OpenViBESkeletonGenerator_CSkeletonGenerator_H__
