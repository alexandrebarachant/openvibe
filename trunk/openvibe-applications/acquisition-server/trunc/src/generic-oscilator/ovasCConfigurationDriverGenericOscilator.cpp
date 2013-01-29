#include "ovasCConfigurationDriverGenericOscilator.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBEAcquisitionServer;
using namespace std;

CConfigurationDriverGenericOscilator::CConfigurationDriverGenericOscilator(IDriverContext& rDriverContext, const char* sGtkBuilderFileName, boolean& rSendPeriodicStimulations)
	:CConfigurationBuilder(sGtkBuilderFileName)
	 ,m_rDriverContext(rDriverContext)
	 ,m_rSendPeriodicStimulations(rSendPeriodicStimulations)
{
}

boolean CConfigurationDriverGenericOscilator::preConfigure(void)
{
	if (!CConfigurationBuilder::preConfigure())
	{
		return false;
	}

	::GtkToggleButton* l_pToggleSendPeriodicStimulations = GTK_TOGGLE_BUTTON(gtk_builder_get_object(m_pBuilderConfigureInterface, "checkbutton_send_periodic_stimulations"));

	gtk_toggle_button_set_active(l_pToggleSendPeriodicStimulations, m_rSendPeriodicStimulations);

	return true;
}

boolean CConfigurationDriverGenericOscilator::postConfigure(void)
{
	if (m_bApplyConfiguration)
	{
		::GtkToggleButton* l_pToggleSendPeriodicStimulations = GTK_TOGGLE_BUTTON(gtk_builder_get_object(m_pBuilderConfigureInterface, "checkbutton_send_periodic_stimulations"));

		m_rSendPeriodicStimulations = ::gtk_toggle_button_get_active(l_pToggleSendPeriodicStimulations);
	}

	if (!CConfigurationBuilder::postConfigure())
	{
		return false;
	}

	return true;
}

