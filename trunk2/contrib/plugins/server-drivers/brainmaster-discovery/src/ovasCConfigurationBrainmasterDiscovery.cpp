/*
 * ovasCConfigurationBrainmasterDiscovery.cpp
 *
 * Copyright (c) 2012, Yann Renard. All rights reserved.
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

#if defined TARGET_HAS_ThirdPartyBrainmasterCodeMakerAPI

#include "ovasCConfigurationBrainmasterDiscovery.h"

#include "ovas_defines_brainmaster_discovery.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBEAcquisitionServer;

#define boolean OpenViBE::boolean

namespace
{
	static bool bGuardFlag=false;

	void on_spin_button_changed(::GtkSpinButton* pAnySpinButton, ::GtkComboBox* pDevicePresetComboBox)
	{
		if(!bGuardFlag)
		{
			::gtk_combo_box_set_active(pDevicePresetComboBox, 0); // Custom
		}
	}

	void on_combo_box_changed(::GtkComboBox* pAnyComboBox, ::GtkComboBox* pDevicePresetComboBox)
	{
		if(!bGuardFlag)
		{
			::gtk_combo_box_set_active(pDevicePresetComboBox, 0); // Custom
		}
	}

	void on_device_preset_changed(::GtkComboBox* pComboBox, CConfigurationBrainmasterDiscovery* pThis)
	{
		bGuardFlag=true;

		int i;
		int l_iActive=::gtk_combo_box_get_active(pComboBox);
		switch(l_iActive)
		{
			default:
			case Preset_Custom:
				break;
			case Preset_Discovery_24:
				::gtk_spin_button_set_value(pThis->m_pSpinButtonChannelCount, 24); // 22 EEG channels + 2 AUX
				::gtk_combo_box_set_active(pThis->m_pComboBoxType, 0); // Discovery
				::gtk_combo_box_set_active(pThis->m_pComboBoxSamplingRate, 0); // default
//				::gtk_combo_box_set_active(pThis->m_pComboBoxDevice, );
//				::gtk_combo_box_set_active(pThis->m_pComboBoxPreset, );
				::gtk_combo_box_set_active(pThis->m_pComboBoxBaudRate, 0); // default
				::gtk_combo_box_set_active(pThis->m_pComboBoxBitDepth, 0); // default
				::gtk_combo_box_set_active(pThis->m_pComboBoxNotchFilters, 0); // default
				::gtk_widget_set_sensitive(GTK_WIDGET(pThis->m_pComboBoxBitDepth), FALSE); // default
				pThis->m_rvChannelType.clear();
//				for(i=0;  i<22; i++) pThis->m_rvChannelType[i]=ChannelType_EEG;
//				for(i=22; i<24; i++) pThis->m_rvChannelType[i]=ChannelType_AUX;
				pThis->m_vChannelName[0]="Fp1";
				pThis->m_vChannelName[1]="F3";
				pThis->m_vChannelName[2]="C3";
				pThis->m_vChannelName[3]="P3";
				pThis->m_vChannelName[4]="O1";
				pThis->m_vChannelName[5]="F7";
				pThis->m_vChannelName[6]="T3";
				pThis->m_vChannelName[7]="T5";
				pThis->m_vChannelName[8]="Fz";
				pThis->m_vChannelName[9]="Fp2";
				pThis->m_vChannelName[10]="F4";
				pThis->m_vChannelName[11]="C4";
				pThis->m_vChannelName[12]="P4";
				pThis->m_vChannelName[13]="O2";
				pThis->m_vChannelName[14]="F8";
				pThis->m_vChannelName[15]="T4";
				pThis->m_vChannelName[16]="T6";
				pThis->m_vChannelName[17]="Cz";
				pThis->m_vChannelName[18]="Pz";
				pThis->m_vChannelName[19]="A2";
				pThis->m_vChannelName[20]="Fpz";
				pThis->m_vChannelName[21]="Oz";
				pThis->m_vChannelName[22]="AUX1";
				pThis->m_vChannelName[23]="AUX2";
				break;
			case Preset_Atlantis_2x2:
				::gtk_spin_button_set_value(pThis->m_pSpinButtonChannelCount, 4); // 2 EEG channels + 2 AUX
				::gtk_combo_box_set_active(pThis->m_pComboBoxType, 1); // Atlantis
				::gtk_combo_box_set_active(pThis->m_pComboBoxSamplingRate, 0); // default
//				::gtk_combo_box_set_active(pThis->m_pComboBoxDevice, );
//				::gtk_combo_box_set_active(pThis->m_pComboBoxPreset, );
				::gtk_combo_box_set_active(pThis->m_pComboBoxBaudRate, 0); // default
				::gtk_combo_box_set_active(pThis->m_pComboBoxBitDepth, 0); // default
				::gtk_combo_box_set_active(pThis->m_pComboBoxNotchFilters, 0); // default
				::gtk_widget_set_sensitive(GTK_WIDGET(pThis->m_pComboBoxBitDepth), TRUE); // default
				pThis->m_rvChannelType.clear();
				pThis->m_rvChannelType[0]=ChannelType_AUX;
				pThis->m_rvChannelType[1]=ChannelType_AUX;
				pThis->m_rvChannelType[2]=ChannelType_EEG;
				pThis->m_rvChannelType[3]=ChannelType_EEG;
				pThis->m_vChannelName[0]="AUX1";
				pThis->m_vChannelName[1]="AUX2";
				pThis->m_vChannelName[2]="A1-R1";
				pThis->m_vChannelName[3]="A2-R2";
				break;
			case Preset_Atlantis_4x4:
				::gtk_spin_button_set_value(pThis->m_pSpinButtonChannelCount, 8); // 2 EEG channels + 2 AUX + 2 EEG channels + 2 AUX
				::gtk_combo_box_set_active(pThis->m_pComboBoxType, 1); // Atlantis
				::gtk_combo_box_set_active(pThis->m_pComboBoxSamplingRate, 0); // default
//				::gtk_combo_box_set_active(pThis->m_pComboBoxDevice, );
//				::gtk_combo_box_set_active(pThis->m_pComboBoxPreset, );
				::gtk_combo_box_set_active(pThis->m_pComboBoxBaudRate, 0); // default
				::gtk_combo_box_set_active(pThis->m_pComboBoxBitDepth, 0); // default
				::gtk_combo_box_set_active(pThis->m_pComboBoxNotchFilters, 0); // default
				::gtk_widget_set_sensitive(GTK_WIDGET(pThis->m_pComboBoxBitDepth), TRUE); // default
				pThis->m_rvChannelType.clear();
				pThis->m_rvChannelType[0]=ChannelType_AUX;
				pThis->m_rvChannelType[1]=ChannelType_AUX;
				pThis->m_rvChannelType[2]=ChannelType_EEG;
				pThis->m_rvChannelType[3]=ChannelType_EEG;
				pThis->m_rvChannelType[4]=ChannelType_AUX;
				pThis->m_rvChannelType[5]=ChannelType_AUX;
				pThis->m_rvChannelType[6]=ChannelType_EEG;
				pThis->m_rvChannelType[7]=ChannelType_EEG;
				pThis->m_vChannelName[0]="AUX1";
				pThis->m_vChannelName[1]="AUX2";
				pThis->m_vChannelName[2]="A1-R1";
				pThis->m_vChannelName[3]="A2-R2";
				pThis->m_vChannelName[4]="AUX3";
				pThis->m_vChannelName[5]="AUX4";
				pThis->m_vChannelName[6]="A3-R3";
				pThis->m_vChannelName[7]="A4-R4";
				break;
		}

		bGuardFlag=false;
	}
};

CConfigurationBrainmasterDiscovery::CConfigurationBrainmasterDiscovery(const char* sGtkBuilderFileName, OpenViBE::uint32& rPort, OpenViBE::uint32& rPreset, OpenViBE::uint32& rType, OpenViBE::uint32& rBaudRate, OpenViBE::uint32& rSamplingRate, OpenViBE::uint32& rBitDepth, OpenViBE::uint32& rNotchFilters, std::map < OpenViBE::uint32, OpenViBE::uint32 >& rvChannelType, std::string& sDeviceSerial, std::string& sDevicePasskey)
	:CConfigurationBuilder(sGtkBuilderFileName)
	,m_rPort(rPort)
	,m_rPreset(rPreset)
	,m_rType(rType)
	,m_rBaudRate(rBaudRate)
	,m_rSamplingRate(rSamplingRate)
	,m_rBitDepth(rBitDepth)
	,m_rNotchFilters(rNotchFilters)
	,m_rvChannelType(rvChannelType)
	,m_sDeviceSerial(sDeviceSerial)
	,m_sDevicePasskey(sDevicePasskey)
{
	m_pListStore=gtk_list_store_new(1, G_TYPE_STRING);
}

CConfigurationBrainmasterDiscovery::~CConfigurationBrainmasterDiscovery(void)
{
	g_object_unref(m_pListStore);
}

boolean CConfigurationBrainmasterDiscovery::preConfigure(void)
{
	if(!CConfigurationBuilder::preConfigure())
	{
		return false;
	}

	uint32 l_ui32Port=m_rPort;

	m_pSpinButtonChannelCount=GTK_SPIN_BUTTON(this->m_pNumberOfChannels);
	m_pComboBoxDevice=GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_device"));
	m_pComboBoxPreset=GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_device_preset"));
	m_pComboBoxType=GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_type"));
	m_pComboBoxBaudRate=GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_baud_rate"));
	m_pComboBoxSamplingRate=GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_sampling_frequency"));
	m_pComboBoxBitDepth=GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_bit_depth"));
	m_pComboBoxNotchFilters=GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_notch_filters"));
	m_pEntryDeviceSerial=GTK_ENTRY(gtk_builder_get_object(m_pBuilderConfigureInterface, "entry_device_serial"));
	m_pEntryDevicePasskey=GTK_ENTRY(gtk_builder_get_object(m_pBuilderConfigureInterface, "entry_device_passkey"));

	g_signal_connect(G_OBJECT(m_pSpinButtonChannelCount), "value-changed", G_CALLBACK(on_spin_button_changed), m_pComboBoxPreset);
	g_signal_connect(G_OBJECT(m_pComboBoxPreset), "changed", G_CALLBACK(on_device_preset_changed), this);
	g_signal_connect(G_OBJECT(m_pComboBoxType), "changed", G_CALLBACK(on_combo_box_changed), m_pComboBoxPreset);
	g_signal_connect(G_OBJECT(m_pComboBoxBaudRate), "changed", G_CALLBACK(on_combo_box_changed), m_pComboBoxPreset);
	g_signal_connect(G_OBJECT(m_pComboBoxSamplingRate), "changed", G_CALLBACK(on_combo_box_changed), m_pComboBoxPreset);
	g_signal_connect(G_OBJECT(m_pComboBoxBitDepth), "changed", G_CALLBACK(on_combo_box_changed), m_pComboBoxPreset);
	g_signal_connect(G_OBJECT(m_pComboBoxNotchFilters), "changed", G_CALLBACK(on_combo_box_changed), m_pComboBoxPreset);

	g_object_unref(m_pListStore);
	m_pListStore=gtk_list_store_new(1, G_TYPE_STRING);

	gtk_combo_box_set_model(m_pComboBoxDevice, GTK_TREE_MODEL(m_pListStore));

	char l_sBuffer[1024];
	boolean l_bSelected=false;

	::gtk_combo_box_append_text(m_pComboBoxDevice, "Auto");
	for(uint32 i=1; i<17; i++)
	{
		::sprintf(l_sBuffer, "COM%i", i);
		::gtk_combo_box_append_text(m_pComboBoxDevice, l_sBuffer);
		if(l_ui32Port==i)
		{
			::gtk_combo_box_set_active(m_pComboBoxDevice, i);
			l_bSelected=true;
		}
	}

	if(!l_bSelected)
	{
		::gtk_combo_box_set_active(m_pComboBoxDevice, 0);
	}

	switch(m_rType)
	{
		default:
		case Type_Discovery:   ::gtk_combo_box_set_active(m_pComboBoxType, 0); break;
		case Type_Atlantis:    ::gtk_combo_box_set_active(m_pComboBoxType, 1); break;
	}

	switch(m_rBaudRate)
	{
		default:
		case BaudRate_Default: ::gtk_combo_box_set_active(m_pComboBoxBaudRate, 0); break;
		case BaudRate_9600:    ::gtk_combo_box_set_active(m_pComboBoxBaudRate, 1); break;
		case BaudRate_115200:  ::gtk_combo_box_set_active(m_pComboBoxBaudRate, 2); break;
		case BaudRate_460800:  ::gtk_combo_box_set_active(m_pComboBoxBaudRate, 3); break;
	}

	switch(m_rSamplingRate)
	{
		default:
		case SamplingFrequency_256:  ::gtk_combo_box_set_active(m_pComboBoxSamplingRate, 0); break;
		case SamplingFrequency_512:  ::gtk_combo_box_set_active(m_pComboBoxSamplingRate, 1); break;
		case SamplingFrequency_1024: ::gtk_combo_box_set_active(m_pComboBoxSamplingRate, 2); break;
		case SamplingFrequency_2048: ::gtk_combo_box_set_active(m_pComboBoxSamplingRate, 3); break;
	}

	switch(m_rBitDepth)
	{
		default:
		case BitDepth_Default: ::gtk_combo_box_set_active(m_pComboBoxBitDepth, 0); break;
		case BitDepth_8:       ::gtk_combo_box_set_active(m_pComboBoxBitDepth, 1); break;
		case BitDepth_16:      ::gtk_combo_box_set_active(m_pComboBoxBitDepth, 2); break;
		case BitDepth_24:      ::gtk_combo_box_set_active(m_pComboBoxBitDepth, 3); break;
	}

	switch(m_rNotchFilters)
	{
		default:
		case NotchFilter_Default: ::gtk_combo_box_set_active(m_pComboBoxNotchFilters, 0); break;
		case NotchFilter_Off:     ::gtk_combo_box_set_active(m_pComboBoxNotchFilters, 1); break;
		case NotchFilter_50:      ::gtk_combo_box_set_active(m_pComboBoxNotchFilters, 2); break;
		case NotchFilter_60:      ::gtk_combo_box_set_active(m_pComboBoxNotchFilters, 3); break;
	}

	switch(m_rPreset)
	{
		default:
		case Preset_Custom:        ::gtk_combo_box_set_active(m_pComboBoxPreset, 0); break;
		case Preset_Discovery_24:  ::gtk_combo_box_set_active(m_pComboBoxPreset, 1); break;
		case Preset_Atlantis_4x4:  ::gtk_combo_box_set_active(m_pComboBoxPreset, 2); break;
		case Preset_Atlantis_2x2:  ::gtk_combo_box_set_active(m_pComboBoxPreset, 3); break;
	}

	::gtk_entry_set_text(m_pEntryDeviceSerial, m_sDeviceSerial.c_str());
	::gtk_entry_set_text(m_pEntryDevicePasskey, m_sDevicePasskey.c_str());

	return true;
}

boolean CConfigurationBrainmasterDiscovery::postConfigure(void)
{
	if(m_bApplyConfiguration)
	{
		int l_iPort=gtk_combo_box_get_active(m_pComboBoxDevice);
		if(l_iPort>=0)
		{
			m_rPort=(uint32)l_iPort;
		}

		int l_iPreset=gtk_combo_box_get_active(m_pComboBoxPreset);
		if(l_iPreset>=0)
		{
			m_rPreset=(uint32)l_iPreset;
		}

		int l_iType=gtk_combo_box_get_active(m_pComboBoxType);
		if(l_iType>=0)
		{
			m_rType=(uint32)l_iType;
		}

		int l_iBaudRate=gtk_combo_box_get_active(m_pComboBoxBaudRate);
		if(l_iBaudRate>=0)
		{
			m_rBaudRate=(uint32)l_iBaudRate;
		}

		int l_iSamplingRate=gtk_combo_box_get_active(m_pComboBoxSamplingRate);
		if(l_iSamplingRate>=0)
		{
			m_rSamplingRate=(uint32)l_iSamplingRate;
		}

		int l_iBitDepth=gtk_combo_box_get_active(m_pComboBoxBitDepth);
		if(l_iBitDepth>=0)
		{
			m_rBitDepth=(uint32)l_iBitDepth;
		}

		int l_iNotchFilters=gtk_combo_box_get_active(m_pComboBoxNotchFilters);
		if(l_iNotchFilters>=0)
		{
			m_rNotchFilters=(uint32)l_iNotchFilters;
		}

		m_sDeviceSerial=::gtk_entry_get_text(m_pEntryDeviceSerial);
		m_sDevicePasskey=::gtk_entry_get_text(m_pEntryDevicePasskey);
	}

	if(!CConfigurationBuilder::postConfigure())
	{
		return false;
	}
	return true;
}

#endif // TARGET_HAS_ThirdPartyBrainmasterCodeMakerAPI
