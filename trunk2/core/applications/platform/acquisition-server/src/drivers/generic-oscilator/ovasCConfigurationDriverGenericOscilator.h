#ifndef __OpenViBE_AcquisitionServer_CConfigurationDriverGenericOscilator_H__
#define __OpenViBE_AcquisitionServer_CConfigurationDriverGenericOscilator_H__

#include "../ovasCConfigurationBuilder.h"
#include "../ovasIDriver.h"

#include <gtk/gtk.h>

namespace OpenViBEAcquisitionServer
{
	/**
	 * \class CConfigurationDriverGenericOscilator
	 * \author Jozef Legeny (Inria)
	 * \date 28 jan 2013
	 * \brief The CConfigurationDriverGenericOscilator handles the configuration dialog specific to the Generic Oscilator driver
	 *
	 * \sa CDriverGenericOscillator
	 */

	class CConfigurationDriverGenericOscilator : public OpenViBEAcquisitionServer::CConfigurationBuilder
	{
		public:
			CConfigurationDriverGenericOscilator(OpenViBEAcquisitionServer::IDriverContext& rDriverContext, const char* sGtkBuilderFileName, OpenViBE::boolean& rSendPeriodicStimulations);

			virtual OpenViBE::boolean preConfigure(void);
			virtual OpenViBE::boolean postConfigure(void);

		protected:

		OpenViBEAcquisitionServer::IDriverContext& m_rDriverContext;

		OpenViBE::boolean& m_rSendPeriodicStimulations;

	};
};

#endif // __OpenViBE_AcquisitionServer_CConfigurationDriverGenericOscilator_H__

