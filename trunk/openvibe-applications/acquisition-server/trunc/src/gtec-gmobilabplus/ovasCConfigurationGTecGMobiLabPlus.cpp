/**
 * The gMobilab driver was contributed
 * by Lucie Daubigney from Supelec Metz
 */

#include "ovasCConfigurationGTecGMobiLabPlus.h"

#if defined TARGET_HAS_ThirdPartyGMobiLabPlusAPI

#define boolean OpenViBE::boolean

using namespace OpenViBEAcquisitionServer;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace std;

CConfigurationGTecGMobiLabPlus::CConfigurationGTecGMobiLabPlus(const char* sGladeXMLFileName)
	:CConfigurationGlade(sGladeXMLFileName)
{
}

boolean CConfigurationGTecGMobiLabPlus::postConfigure(void)
{
	::GtkComboBox* l_pComboBox=GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeConfigureInterface, "combobox_port"));
	if(m_bApplyConfiguration)
	{
		m_oPortName=::gtk_combo_box_get_active_text(l_pComboBox);
	}

	if(!CConfigurationGlade::postConfigure())
	{
		return false;
	}

	return true;
}

std::string CConfigurationGTecGMobiLabPlus::getPortName(void)
{
  return m_oPortName;
}

#endif // TARGET_HAS_ThirdPartyGMobiLabPlusAPI
