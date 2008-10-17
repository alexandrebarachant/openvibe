#ifndef __OpenViBEDesigner_CSettingEditorDialog_H__
#define __OpenViBEDesigner_CSettingEditorDialog_H__

#include "ovd_base.h"

#include <string>
#include <map>

namespace OpenViBEDesigner
{
	class CSettingEditorDialog
	{
	public:

		CSettingEditorDialog(OpenViBE::Kernel::IKernel& rKernel, OpenViBE::Kernel::IBox& rBox, OpenViBE::uint32 ui32SettingIndex, const char* sGUIFilename);
		virtual ~CSettingEditorDialog(void);
		virtual OpenViBE::boolean run(void);

		virtual void typeChangedCB(void);

	protected:

		OpenViBE::Kernel::IKernel& m_rKernel;
		OpenViBE::Kernel::IBox& m_rBox;
		OpenViBE::uint32 m_ui32SettingIndex;
		OpenViBE::CString m_sGUIFilename;
		::GtkWidget* m_pTable;
		::GtkWidget* m_pType;
		::GtkWidget* m_pDefaultValue;
		std::map<std::string, OpenViBE::CIdentifier> m_vSettingTypes;
	};
};

#endif // __OpenViBEDesigner_CSettingEditorDialog_H__
