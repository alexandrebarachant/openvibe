#ifndef __OpenViBE_AcquisitionServer_CHeaderConfiguratorGlade_H__
#define __OpenViBE_AcquisitionServer_CHeaderConfiguratorGlade_H__

#include "ovas_base.h"

namespace OpenViBEAcquisitionServer
{
	class IHeaderConfigurator;

	extern OpenViBEAcquisitionServer::IHeaderConfigurator* createHeaderConfiguratorGlade(
		const char* sGladeXMLFileName,
		const char* sElectrodesFileName);
};

#endif // __OpenViBE_AcquisitionServer_CHeaderConfiguratorGlade_H__
