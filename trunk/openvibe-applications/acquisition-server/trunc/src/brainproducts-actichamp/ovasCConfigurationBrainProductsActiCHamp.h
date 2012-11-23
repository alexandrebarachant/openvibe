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
