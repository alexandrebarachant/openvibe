#ifndef __OpenViBE_AcquisitionServer_CConfigurationGTecGUSBamp_H__
#define __OpenViBE_AcquisitionServer_CConfigurationGTecGUSBamp_H__

#if defined TARGET_HAS_ThirdPartyGUSBampCAPI

#include "../ovasCConfigurationGlade.h"

#include <gtk/gtk.h>

namespace OpenViBEAcquisitionServer
{
	class CConfigurationGTecGUSBamp : public OpenViBEAcquisitionServer::CConfigurationGlade
	{
	public:
		CConfigurationGTecGUSBamp(const char* sGladeXMLFileName, OpenViBE::uint32& rUSBIndex);

		virtual OpenViBE::boolean preConfigure(void);
		virtual OpenViBE::boolean postConfigure(void);

	protected:
		OpenViBE::uint32& m_rUSBIndex;
	};
};

#endif // TARGET_HAS_ThirdPartyGUSBampCAPI

#endif // __OpenViBE_AcquisitionServer_CConfigurationGTecGUSBamp_H__
