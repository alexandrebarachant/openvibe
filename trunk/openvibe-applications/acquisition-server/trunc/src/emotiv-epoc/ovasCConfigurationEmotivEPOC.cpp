#if defined TARGET_HAS_ThirdPartyEmotivAPI

#include "ovasCConfigurationEmotivEPOC.h"



using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBEAcquisitionServer;
using namespace std;

#define boolean OpenViBE::boolean

//____________________________________________________________________________________

CConfigurationEmotivEPOC::CConfigurationEmotivEPOC(IDriverContext& rDriverContext, const char* sGtkBuilderFileName, boolean& rUseGyroscope)
	:CConfigurationBuilder(sGtkBuilderFileName)
	,m_rDriverContext(rDriverContext)
	,m_rUseGyroscope(rUseGyroscope)
{
}

boolean CConfigurationEmotivEPOC::preConfigure(void)
{
	if(! CConfigurationBuilder::preConfigure())
	{
		return false;
	}

	::GtkToggleButton* l_pCheckbuttonGyro = GTK_TOGGLE_BUTTON(gtk_builder_get_object(m_pBuilderConfigureInterface, "checkbutton_gyro"));
	gtk_toggle_button_set_active(l_pCheckbuttonGyro,m_rUseGyroscope);

	return true;
}

boolean CConfigurationEmotivEPOC::postConfigure(void)
{

	if(m_bApplyConfiguration)
	{
		::GtkToggleButton* l_pCheckbuttonGyro = GTK_TOGGLE_BUTTON(gtk_builder_get_object(m_pBuilderConfigureInterface, "checkbutton_gyro"));
		m_rUseGyroscope = ::gtk_toggle_button_get_active(l_pCheckbuttonGyro);
	}

	if(! CConfigurationBuilder::postConfigure()) // normal header is filled, ressources are realesed
	{
		return false;
	}

	return true;
}

#endif // TARGET_HAS_ThirdPartyEmotivAPI
