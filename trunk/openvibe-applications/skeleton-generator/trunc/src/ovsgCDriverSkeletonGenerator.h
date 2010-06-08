#ifndef __OpenViBESkeletonGenerator_CDriverSkeletonGenerator_H__
#define __OpenViBESkeletonGenerator_CDriverSkeletonGenerator_H__

#include "ovsgCSkeletonGenerator.h"

#include <vector>

namespace OpenViBESkeletonGenerator
{
	class CDriverSkeletonGenerator : public CSkeletonGenerator
	{
	public:
		CDriverSkeletonGenerator(OpenViBE::Kernel::IKernelContext & rKernelContext, ::GladeXML * pGladeInterface);
		virtual ~CDriverSkeletonGenerator(void);

		void initialize(void);

		OpenViBE::CString              m_sDriverName;
		OpenViBE::CString              m_sClassName;
		std::vector<OpenViBE::CString> m_vSamplingFrequencies;
		OpenViBE::CString              m_sMinChannel;
		OpenViBE::CString              m_sMaxChannel;
		
		void buttonCheckCB(void);
		void buttonOkCB(void);
		void buttonTooltipCB(::GtkButton* pButton);

		
	};

}

#endif //__OpenViBESkeletonGenerator_CDriverSkeletonGenerator_H__
