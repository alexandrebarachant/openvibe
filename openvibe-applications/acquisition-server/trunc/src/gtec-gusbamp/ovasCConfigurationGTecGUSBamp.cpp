#if defined TARGET_HAS_ThirdPartyGUSBampCAPI

#include "ovasCConfigurationGTecGUSBamp.h"

#include <windows.h>
#include <gUSBamp.h>
#define boolean OpenViBE::boolean

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBEAcquisitionServer;

CConfigurationGTecGUSBamp::CConfigurationGTecGUSBamp(const char* sGladeXMLFileName, OpenViBE::uint32& rUSBIndex)
	:CConfigurationGlade(sGladeXMLFileName)
	,m_rUSBIndex(rUSBIndex)
{
}

boolean CConfigurationGTecGUSBamp::preConfigure(void)
{
	if(!CConfigurationGlade::preConfigure())
	{
		return false;
	}

	::GtkComboBox* l_pComboBox=GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeConfigureInterface, "combobox_device"));

	char l_sBuffer[1024];
	int l_iCount=0;
	boolean l_bSelected=false;

	// autodetection of the connected device
	for(uint32 i=1; i<11; i++)
	{
		::HANDLE l_pHandle=::GT_OpenDevice(i);
		if(l_pHandle)
		{
			::GT_CloseDevice(&l_pHandle);

			sprintf(l_sBuffer, "USB port %i", i);
			::gtk_combo_box_append_text(l_pComboBox, l_sBuffer);
			if(m_rUSBIndex==i)
			{
				::gtk_combo_box_set_active(l_pComboBox, l_iCount);
				l_bSelected=true;
			}
			l_iCount++;
		}
	}

	if(!l_bSelected && l_iCount!=0)
	{
		::gtk_combo_box_set_active(l_pComboBox, 0);
	}

	return true;
}

boolean CConfigurationGTecGUSBamp::postConfigure(void)
{
	::GtkComboBox* l_pComboBox=GTK_COMBO_BOX(glade_xml_get_widget(m_pGladeConfigureInterface, "combobox_device"));

	if(m_bApplyConfiguration)
	{
		int l_iUSBIndex=0;
		if(::sscanf(gtk_combo_box_get_active_text(l_pComboBox), "USB port %i", &l_iUSBIndex)==1)
		{
			m_rUSBIndex=(uint32)l_iUSBIndex;
		}
	}

	if(!CConfigurationGlade::postConfigure())
	{
		return false;
	}
	return true;
}

#endif // TARGET_HAS_ThirdPartyGUSBampCAPI
