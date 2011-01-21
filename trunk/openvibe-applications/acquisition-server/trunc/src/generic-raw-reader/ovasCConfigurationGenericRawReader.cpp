#include "ovasCConfigurationGenericRawReader.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBEAcquisitionServer;

CConfigurationGenericRawReader::CConfigurationGenericRawReader(const char* sGtkBuilderFileName,
	uint32& rSampleFormat,
	uint32& rSampleEndian,
	uint32& rHeaderSkip,
	uint32& rFrameSkip,
	CString& rFilename)
	:CConfigurationNetworkBuilder(sGtkBuilderFileName)
	,m_rSampleFormat(rSampleFormat)
	,m_rSampleEndian(rSampleEndian)
	,m_rHeaderSkip(rHeaderSkip)
	,m_rFrameSkip(rFrameSkip)
	,m_rFilename(rFilename)
{
}

boolean CConfigurationGenericRawReader::preConfigure(void)
{
	if(!CConfigurationNetworkBuilder::preConfigure())
	{
		return false;
	}

	::GtkEntry* l_pEntryFilename=GTK_ENTRY(gtk_builder_get_object(m_pBuilderConfigureInterface, "entry_filename"));
	::GtkComboBox* l_pComboBoxEndianness=GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_endianness"));
	::GtkComboBox* l_pComboBoxSampleType=GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_sample_type"));
	::GtkSpinButton* l_pSpinButtonHeaderSize=GTK_SPIN_BUTTON(gtk_builder_get_object(m_pBuilderConfigureInterface, "spinbutton_header_size"));
	::GtkSpinButton* l_pSpinButtonInterFrameSize=GTK_SPIN_BUTTON(gtk_builder_get_object(m_pBuilderConfigureInterface, "spinbutton_interframe_size"));

	::gtk_entry_set_text(l_pEntryFilename, m_rFilename.toASCIIString());
	::gtk_combo_box_set_active(l_pComboBoxEndianness, m_rSampleEndian);
	::gtk_combo_box_set_active(l_pComboBoxSampleType, m_rSampleFormat);
	::gtk_spin_button_set_value(l_pSpinButtonHeaderSize, m_rHeaderSkip);
	::gtk_spin_button_set_value(l_pSpinButtonInterFrameSize, m_rFrameSkip);

	return true;
}

boolean CConfigurationGenericRawReader::postConfigure(void)
{
	::GtkEntry* l_pEntryFilename=GTK_ENTRY(gtk_builder_get_object(m_pBuilderConfigureInterface, "entry_filename"));
	::GtkComboBox* l_pComboBoxEndianness=GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_endianness"));
	::GtkComboBox* l_pComboBoxSampleType=GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_sample_type"));
	::GtkSpinButton* l_pSpinButtonHeaderSize=GTK_SPIN_BUTTON(gtk_builder_get_object(m_pBuilderConfigureInterface, "spinbutton_header_size"));
	::GtkSpinButton* l_pSpinButtonInterFrameSize=GTK_SPIN_BUTTON(gtk_builder_get_object(m_pBuilderConfigureInterface, "spinbutton_interframe_size"));

	if(m_bApplyConfiguration)
	{
		m_rFilename=::gtk_entry_get_text(l_pEntryFilename);
		m_rSampleEndian=(uint32)::gtk_combo_box_get_active(l_pComboBoxEndianness);
		m_rSampleFormat=(uint32)::gtk_combo_box_get_active(l_pComboBoxSampleType);
		m_rHeaderSkip=(uint32)::gtk_spin_button_get_value(l_pSpinButtonHeaderSize);
		m_rFrameSkip=(uint32)::gtk_spin_button_get_value(l_pSpinButtonInterFrameSize);
	}

	if(!CConfigurationNetworkBuilder::postConfigure())
	{
		return false;
	}
	return true;
}
