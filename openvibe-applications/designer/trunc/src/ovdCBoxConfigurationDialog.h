#ifndef __OpenViBEDesigner_CBoxConfigurationDialog_H__
#define __OpenViBEDesigner_CBoxConfigurationDialog_H__

#include "ovd_base.h"

#include <string>

namespace OpenViBEDesigner
{
	class CBoxConfigurationDialog
	{
	public:

		CBoxConfigurationDialog(OpenViBE::Kernel::IKernel& rKernel, OpenViBE::Kernel::IBox& rBox, const char* sGUIFilename);
		virtual ~CBoxConfigurationDialog(void);
		virtual void run(void);

	protected:

		OpenViBE::Kernel::IKernel& m_rKernel;
		OpenViBE::Kernel::IBox& m_rBox;
		OpenViBE::CString m_sGUIFilename;
	};
};

#endif // __OpenViBEDesigner_CBoxConfigurationDialog_H__
