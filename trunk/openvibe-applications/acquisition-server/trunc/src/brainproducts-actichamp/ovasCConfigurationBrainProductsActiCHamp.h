/*
 * ovasCConfigurationBrainProductsActiCHamp.h
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

#ifndef __OpenViBE_AcquisitionServer_CConfigurationBrainProductsActiCHamp_H__
#define __OpenViBE_AcquisitionServer_CConfigurationBrainProductsActiCHamp_H__

#if defined TARGET_HAS_ThirdPartyActiCHampAPI

#include "../ovasCConfigurationBuilder.h"

#include <gtk/gtk.h>

#include <vector>

namespace OpenViBEAcquisitionServer
{
	class CConfigurationBrainProductsActiCHamp : public OpenViBEAcquisitionServer::CConfigurationBuilder
	{
	public:

		CConfigurationBrainProductsActiCHamp(
			const char* sGtkBuilderFileName,
			OpenViBE::uint32& rDeviceId,
			OpenViBE::uint32& rMode,
			OpenViBE::uint32& rPhysicalSampleRate,
			OpenViBE::uint32& rADCDataFilter,
			OpenViBE::uint32& rADCDataDecimation,
			OpenViBE::uint32& rActiveShieldGain,
			OpenViBE::uint32& rModuleEnabled,
			OpenViBE::boolean& rUseAuxChannels,
			OpenViBE::uint32& rGoodImpedanceLimit,
			OpenViBE::uint32& rBadImpedanceLimit);

		virtual OpenViBE::boolean preConfigure(void);
		virtual OpenViBE::boolean postConfigure(void);

		void comboBoxDeviceChangedCB(void);
		void buttonModuleToggledCB(OpenViBE::boolean bToggleState);
		void buttonAuxChannelsToggledCB(OpenViBE::boolean bToggleState);

	protected:

		OpenViBE::uint32& m_rDeviceId;
		OpenViBE::uint32& m_rMode;
		OpenViBE::uint32& m_rPhysicalSampleRate;
		OpenViBE::uint32& m_rADCDataFilter;
		OpenViBE::uint32& m_rADCDataDecimation;
		OpenViBE::uint32& m_rActiveShieldGain;
		OpenViBE::uint32& m_rModuleEnabled;
		OpenViBE::boolean& m_rUseAuxChannels;
		OpenViBE::uint32& m_rGoodImpedanceLimit;
		OpenViBE::uint32& m_rBadImpedanceLimit;

		::GtkComboBox* m_pComboBoxDeviceId;
		::GtkComboBox* m_pComboBoxMode;
		::GtkComboBox* m_pComboBoxPhysicalSampleRate;
		::GtkComboBox* m_pComboBoxADCDataFilter;
		::GtkComboBox* m_pComboBoxADCDataDecimation;
		::GtkSpinButton* m_pSpinButtonActiveShieldGain;
		::GtkToggleButton* m_pCheckButtonUseAuxChannels;
		::GtkSpinButton* m_pSpinButtonGoodImpedanceLimit;
		::GtkSpinButton* m_pSpinButtonBadImpedanceLimit;

		::GtkToggleButton* m_pToggleModule1;
		::GtkToggleButton* m_pToggleModule2;
		::GtkToggleButton* m_pToggleModule3;
		::GtkToggleButton* m_pToggleModule4;
		::GtkToggleButton* m_pToggleModule5;
	};
};

#endif // TARGET_HAS_ThirdPartyActiCHampAPI

#endif // __OpenViBE_AcquisitionServer_CConfigurationBrainProductsActiCHamp_H__
