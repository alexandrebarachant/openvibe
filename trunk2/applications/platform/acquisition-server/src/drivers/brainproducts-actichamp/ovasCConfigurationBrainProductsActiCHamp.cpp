/*
 * ovasCConfigurationBrainProductsActiCHamp.cpp
 *
 * Copyright (c) 2012, Mensia Technologies SA. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */

#if defined TARGET_HAS_ThirdPartyActiCHampAPI

#include "ovasCConfigurationBrainProductsActiCHamp.h"

#include "../ovasIHeader.h"

#include <actichamp.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBEAcquisitionServer;
#define boolean OpenViBE::boolean

namespace
{
	void combobox_device_changed_cb(::GtkComboBox* pComboBox, gpointer pUserData)
	{
		reinterpret_cast<CConfigurationBrainProductsActiCHamp*>(pUserData)->comboBoxDeviceChangedCB();
	}

	void button_module_toggled_cb(::GtkToggleButton* pToggleButton, gpointer pUserData)
	{
		reinterpret_cast<CConfigurationBrainProductsActiCHamp*>(pUserData)->buttonModuleToggledCB(gtk_toggle_button_get_active(pToggleButton));
	}

	void button_aux_channels_toggled_cb(::GtkToggleButton* pToggleButton, gpointer pUserData)
	{
		reinterpret_cast<CConfigurationBrainProductsActiCHamp*>(pUserData)->buttonAuxChannelsToggledCB(gtk_toggle_button_get_active(pToggleButton));
	}
}

CConfigurationBrainProductsActiCHamp::CConfigurationBrainProductsActiCHamp(
	const char* sGtkBuilderFileName,
	uint32& rDeviceId,
	uint32& rMode,
	uint32& rPhysicalSampleRate,
	uint32& rADCDataFilter,
	uint32& rADCDataDecimation,
	uint32& rActiveShieldGain,
	uint32& rModuleEnabled,
	boolean& rUseAuxChannels,
	uint32& rGoodImpedanceLimit,
	uint32& rBadImpedanceLimit)
	:CConfigurationBuilder(sGtkBuilderFileName)
	,m_rDeviceId(rDeviceId)
	,m_rMode(rMode)
	,m_rPhysicalSampleRate(rPhysicalSampleRate)
	,m_rADCDataFilter(rADCDataFilter)
	,m_rADCDataDecimation(rADCDataDecimation)
	,m_rActiveShieldGain(rActiveShieldGain)
	,m_rModuleEnabled(rModuleEnabled)
	,m_rUseAuxChannels(rUseAuxChannels)
	,m_rGoodImpedanceLimit(rGoodImpedanceLimit)
	,m_rBadImpedanceLimit(rBadImpedanceLimit)
{
}

boolean CConfigurationBrainProductsActiCHamp::preConfigure(void)
{
	if(!CConfigurationBuilder::preConfigure())
	{
		return false;
	}

	m_pComboBoxDeviceId=GTK_COMBO_BOX(::gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_device_id"));
	m_pComboBoxMode=GTK_COMBO_BOX(::gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_mode"));
	m_pComboBoxPhysicalSampleRate=GTK_COMBO_BOX(::gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_physical_sample_rate"));
	m_pComboBoxADCDataFilter=GTK_COMBO_BOX(::gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_adc_data_filter"));
	m_pComboBoxADCDataDecimation=GTK_COMBO_BOX(::gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_adc_data_decimation"));
	m_pSpinButtonActiveShieldGain=GTK_SPIN_BUTTON(::gtk_builder_get_object(m_pBuilderConfigureInterface, "spinbutton_active_shield_gain"));
	m_pCheckButtonUseAuxChannels=GTK_TOGGLE_BUTTON(::gtk_builder_get_object(m_pBuilderConfigureInterface, "checkbutton_aux_channels"));
	m_pSpinButtonGoodImpedanceLimit=GTK_SPIN_BUTTON(::gtk_builder_get_object(m_pBuilderConfigureInterface, "spinbutton_good_imp"));
	m_pSpinButtonBadImpedanceLimit=GTK_SPIN_BUTTON(::gtk_builder_get_object(m_pBuilderConfigureInterface, "spinbutton_bad_imp"));

	m_pToggleModule1=GTK_TOGGLE_BUTTON(::gtk_builder_get_object(m_pBuilderConfigureInterface, "togglebutton_module_1"));
	m_pToggleModule2=GTK_TOGGLE_BUTTON(::gtk_builder_get_object(m_pBuilderConfigureInterface, "togglebutton_module_2"));
	m_pToggleModule3=GTK_TOGGLE_BUTTON(::gtk_builder_get_object(m_pBuilderConfigureInterface, "togglebutton_module_3"));
	m_pToggleModule4=GTK_TOGGLE_BUTTON(::gtk_builder_get_object(m_pBuilderConfigureInterface, "togglebutton_module_4"));
	m_pToggleModule5=GTK_TOGGLE_BUTTON(::gtk_builder_get_object(m_pBuilderConfigureInterface, "togglebutton_module_5"));

	g_signal_connect(G_OBJECT(m_pToggleModule1), "toggled", G_CALLBACK(button_module_toggled_cb), this);
	g_signal_connect(G_OBJECT(m_pToggleModule2), "toggled", G_CALLBACK(button_module_toggled_cb), this);
	g_signal_connect(G_OBJECT(m_pToggleModule3), "toggled", G_CALLBACK(button_module_toggled_cb), this);
	g_signal_connect(G_OBJECT(m_pToggleModule4), "toggled", G_CALLBACK(button_module_toggled_cb), this);
	g_signal_connect(G_OBJECT(m_pToggleModule5), "toggled", G_CALLBACK(button_module_toggled_cb), this);

	g_signal_connect(G_OBJECT(m_pCheckButtonUseAuxChannels), "toggled", G_CALLBACK(button_aux_channels_toggled_cb), this);

	g_signal_connect(G_OBJECT(m_pComboBoxDeviceId), "changed", G_CALLBACK(combobox_device_changed_cb), this);

	uint32 l_ui32DeviceCount=uint32(::champGetCount());
	char l_sBuffer[1024];
	boolean l_bSelected=false;

	// autodetection of the connected device(s)
	for(uint32 i=0; i<l_ui32DeviceCount; i++)
	{
		::sprintf(l_sBuffer, "Device %i", i);
		gtk_combo_box_append_text(m_pComboBoxDeviceId, l_sBuffer);
		{
			if(m_rDeviceId==i)
			{
				gtk_combo_box_set_active(m_pComboBoxDeviceId, i);
				l_bSelected=true;
			}
		}
	}

	if(!l_bSelected && l_ui32DeviceCount != 0)
	{
		::gtk_combo_box_set_active(m_pComboBoxDeviceId, 0);
	}

	::gtk_combo_box_set_active(m_pComboBoxMode, m_rMode);
	::gtk_combo_box_set_active(m_pComboBoxPhysicalSampleRate, m_rPhysicalSampleRate);
	::gtk_combo_box_set_active(m_pComboBoxADCDataFilter, m_rADCDataFilter);
	::gtk_combo_box_set_active(m_pComboBoxADCDataDecimation, (m_rADCDataDecimation == CHAMP_DECIMATION_2 ? 1 : 0));
	::gtk_spin_button_set_value(m_pSpinButtonActiveShieldGain, m_rActiveShieldGain);
	::gtk_spin_button_set_value(m_pSpinButtonGoodImpedanceLimit, m_rGoodImpedanceLimit);
	::gtk_spin_button_set_value(m_pSpinButtonBadImpedanceLimit, m_rBadImpedanceLimit);
	
	return true;
}

boolean CConfigurationBrainProductsActiCHamp::postConfigure(void)
{
	::GtkComboBox* l_pComboBox=GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_device"));

	if(m_bApplyConfiguration)
	{
		m_rDeviceId=::gtk_combo_box_get_active(m_pComboBoxDeviceId);
		m_rMode=::gtk_combo_box_get_active(m_pComboBoxMode);
		m_rPhysicalSampleRate=::gtk_combo_box_get_active(m_pComboBoxPhysicalSampleRate);
		m_rADCDataFilter=::gtk_combo_box_get_active((m_pComboBoxADCDataFilter));
		m_rADCDataDecimation=((::gtk_combo_box_get_active(m_pComboBoxADCDataDecimation) == 1) ? CHAMP_DECIMATION_2 : CHAMP_DECIMATION_0); // The decimation enum gives FACTOR_2 = 2 and not 1.
		m_rActiveShieldGain=uint32(::gtk_spin_button_get_value(m_pSpinButtonActiveShieldGain));
		m_rGoodImpedanceLimit=uint32(::gtk_spin_button_get_value(m_pSpinButtonGoodImpedanceLimit));
		m_rBadImpedanceLimit=uint32(::gtk_spin_button_get_value(m_pSpinButtonBadImpedanceLimit));
		m_rUseAuxChannels=::gtk_toggle_button_get_active(m_pCheckButtonUseAuxChannels);
		m_rModuleEnabled=0x01
			|(::gtk_toggle_button_get_active(m_pToggleModule1)?0x02:0x00)
			|(::gtk_toggle_button_get_active(m_pToggleModule2)?0x04:0x00)
			|(::gtk_toggle_button_get_active(m_pToggleModule3)?0x08:0x00)
			|(::gtk_toggle_button_get_active(m_pToggleModule4)?0x10:0x00)
			|(::gtk_toggle_button_get_active(m_pToggleModule5)?0x20:0x00);
	}

	if(!CConfigurationBuilder::postConfigure())
	{
		return false;
	}
	return true;
}

void CConfigurationBrainProductsActiCHamp::comboBoxDeviceChangedCB(void)
{
	void* l_pHandle;
	uint32 l_ui32DeviceId=::gtk_combo_box_get_active(m_pComboBoxDeviceId);

	// Opens device
	if((l_pHandle=::champOpen(l_ui32DeviceId))!=NULL)
	{
		// Gets properties
		t_champProperty l_oProperties;
		::champGetProperty(l_pHandle, &l_oProperties);

		// Sets channel count
		//m_pHeader->setSamplingFrequency(uint32(l_oProperties.Rate));
		//m_pHeader->setChannelCount(l_oProperties.CountEeg + l_oProperties.CountAux);
		//gtk_spin_button_set_value(GTK_SPIN_BUTTON(m_pNumberOfChannels), l_oProperties.CountEeg + l_oProperties.CountAux);
		
		// The channel count is updated with the toggled button callbacks.
		m_pHeader->setChannelCount(0);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(m_pNumberOfChannels), 0);

		// Gets modules
		t_champModules l_oModules;
		::champGetModules(l_pHandle, &l_oModules);
		::gtk_widget_set_sensitive(GTK_WIDGET(m_pToggleModule1), (l_oModules.Present&0x02)?TRUE:FALSE);
		::gtk_widget_set_sensitive(GTK_WIDGET(m_pToggleModule2), (l_oModules.Present&0x04)?TRUE:FALSE);
		::gtk_widget_set_sensitive(GTK_WIDGET(m_pToggleModule3), (l_oModules.Present&0x08)?TRUE:FALSE);
		::gtk_widget_set_sensitive(GTK_WIDGET(m_pToggleModule4), (l_oModules.Present&0x10)?TRUE:FALSE);
		::gtk_widget_set_sensitive(GTK_WIDGET(m_pToggleModule5), (l_oModules.Present&0x20)?TRUE:FALSE);
		::gtk_toggle_button_set_active(m_pToggleModule1, (m_rModuleEnabled&0x02 && l_oModules.Present&0x02)?TRUE:FALSE); // calls the buttonModuleToggledCB callback.
		::gtk_toggle_button_set_active(m_pToggleModule2, (m_rModuleEnabled&0x04 && l_oModules.Present&0x04)?TRUE:FALSE); // calls the buttonModuleToggledCB callback.
		::gtk_toggle_button_set_active(m_pToggleModule3, (m_rModuleEnabled&0x08 && l_oModules.Present&0x08)?TRUE:FALSE); // calls the buttonModuleToggledCB callback.
		::gtk_toggle_button_set_active(m_pToggleModule4, (m_rModuleEnabled&0x10 && l_oModules.Present&0x10)?TRUE:FALSE); // calls the buttonModuleToggledCB callback.
		::gtk_toggle_button_set_active(m_pToggleModule5, (m_rModuleEnabled&0x20 && l_oModules.Present&0x20)?TRUE:FALSE); // calls the buttonModuleToggledCB callback.

		//Aux channels
		::gtk_toggle_button_set_active(m_pCheckButtonUseAuxChannels, m_rUseAuxChannels);

		// Closes the device
		::champClose(l_pHandle);
	}
}

void CConfigurationBrainProductsActiCHamp::buttonModuleToggledCB(boolean bToggleState)
{
	uint32 l_ui32CurrentChannelCount = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(m_pNumberOfChannels));

	if(bToggleState)
	{
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(m_pNumberOfChannels), l_ui32CurrentChannelCount + (l_ui32CurrentChannelCount == 1 ? 31 : 32)); // there is minimum 1 channel.
	}
	else 
	{
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(m_pNumberOfChannels), l_ui32CurrentChannelCount - (l_ui32CurrentChannelCount == 1 ? 31 : 32));
	}

}
void CConfigurationBrainProductsActiCHamp::buttonAuxChannelsToggledCB(boolean bToggleState)
{
	uint32 l_ui32CurrentChannelCount = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(m_pNumberOfChannels));

	if(bToggleState)
	{
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(m_pNumberOfChannels), l_ui32CurrentChannelCount + (l_ui32CurrentChannelCount == 1 ? 7 : 8));
	}
	else 
	{
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(m_pNumberOfChannels), l_ui32CurrentChannelCount - (l_ui32CurrentChannelCount == 1 ? 7 : 8));
	}

}
#endif // TARGET_OS_Windows
