/*
 * ovasCConfigurationBrainmasterDiscovery.h
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

#ifndef __OpenViBE_AcquisitionServer_CConfigurationBrainmasterDiscovery_H__
#define __OpenViBE_AcquisitionServer_CConfigurationBrainmasterDiscovery_H__

#if defined TARGET_HAS_ThirdPartyBrainmasterCodeMakerAPI

#include "../ovasCConfigurationBuilder.h"

#include <gtk/gtk.h>
#include <vector>
#include <string>
#include <map>

namespace OpenViBEAcquisitionServer
{
	class CConfigurationBrainmasterDiscovery : public OpenViBEAcquisitionServer::CConfigurationBuilder
	{
	public:
		CConfigurationBrainmasterDiscovery(const char* sGtkBuilderFileName,
			OpenViBE::uint32& rPort,
			OpenViBE::uint32& rPreset,
			OpenViBE::uint32& rType,
			OpenViBE::uint32& rBaudRate,
			OpenViBE::uint32& rSamplingFrequency,
			OpenViBE::uint32& rBitDepth,
			OpenViBE::uint32& rNotchFilters,
			std::map < OpenViBE::uint32, OpenViBE::uint32 >& rvChannelType,
			std::string& sDeviceSerial,
			std::string& sDevicePasskey);
		virtual ~CConfigurationBrainmasterDiscovery(void);

		virtual OpenViBE::boolean preConfigure(void);
		virtual OpenViBE::boolean postConfigure(void);

		::GtkSpinButton* m_pSpinButtonChannelCount;
		::GtkComboBox* m_pComboBoxDevice;
		::GtkComboBox* m_pComboBoxPreset;
		::GtkComboBox* m_pComboBoxType;
		::GtkComboBox* m_pComboBoxBaudRate;
		::GtkComboBox* m_pComboBoxSamplingRate;
		::GtkComboBox* m_pComboBoxBitDepth;
		::GtkComboBox* m_pComboBoxNotchFilters;
		::GtkEntry* m_pEntryDeviceSerial;
		::GtkEntry* m_pEntryDevicePasskey;

		using OpenViBEAcquisitionServer::CConfigurationBuilder::m_vChannelName;


		::GtkListStore* m_pListStore;

		OpenViBE::uint32& m_rPort;
		OpenViBE::uint32& m_rPreset;
		OpenViBE::uint32& m_rType;
		OpenViBE::uint32& m_rBaudRate;
		OpenViBE::uint32& m_rSamplingRate;
		OpenViBE::uint32& m_rBitDepth;
		OpenViBE::uint32& m_rNotchFilters;
		std::map < OpenViBE::uint32, OpenViBE::uint32 >& m_rvChannelType;
		std::string& m_sDeviceSerial;
		std::string& m_sDevicePasskey;

		std::vector < ::GtkWidget* > m_vSensitiveWidget;
	};
};

#endif // TARGET_HAS_ThirdPartyBrainmasterCodeMakerAPI

#endif // __OpenViBE_AcquisitionServer_CConfigurationBrainmasterDiscovery_H__
