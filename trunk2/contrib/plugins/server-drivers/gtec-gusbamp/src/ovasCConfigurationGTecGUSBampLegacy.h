#ifndef __OpenViBE_AcquisitionServer_CConfigurationGTecGUSBampLegacy_H__
#define __OpenViBE_AcquisitionServer_CConfigurationGTecGUSBampLegacy_H__

#if defined TARGET_HAS_ThirdPartyGUSBampCAPI

#include "../ovasCConfigurationBuilder.h"

#include <gtk/gtk.h>

namespace OpenViBEAcquisitionServer
{
	class CConfigurationGTecGUSBampLegacy : public OpenViBEAcquisitionServer::CConfigurationBuilder
	{
	public:
		CConfigurationGTecGUSBampLegacy(const char* sGtkBuilderFileName,OpenViBE::uint32& rUSBIndex,OpenViBE::uint8& rCommonGndAndRefBitmap, OpenViBE::int32& rNotchFilterIndex, OpenViBE::int32& rBandPassFilterIndex,OpenViBE::boolean& rTriggerInput);

		virtual OpenViBE::boolean preConfigure(void);
		virtual OpenViBE::boolean postConfigure(void);

		void buttonCalibratePressedCB(void);
		void idleCalibrateCB(void);

		void buttonCommonGndRefPressedCB(void);
		void buttonFiltersPressedCB(void);

	protected:
		OpenViBE::uint32& m_rUSBIndex;
		OpenViBE::uint8& m_rCommonGndAndRefBitmap;

		OpenViBE::int32& m_rNotchFilterIndex;
		OpenViBE::int32& m_rBandPassFilterIndex;
		OpenViBE::boolean& m_rTriggerInput;
		
	private:
		::GtkWidget* m_pCalibrateDialog;
		OpenViBE::boolean m_bCalibrationDone;
	};
};

#endif // TARGET_HAS_ThirdPartyGUSBampCAPI

#endif // __OpenViBE_AcquisitionServer_CConfigurationGTecGUSBampLegacy_H__
