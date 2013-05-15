/*
#include "gtec-gmobilabplus/ovasCDriverGTecGMobiLabPlus.h"
#include "gtec-gusbamp/ovasCDriverGTecGUSBamp.h"
#include "openeeg-modulareeg/ovasCDriverOpenEEGModularEEG.h"
#include "field-trip-protocol/ovasCDriverFieldtrip.h"
#include "brainproducts-brainvisionrecorder/ovasCDriverBrainProductsBrainVisionRecorder.h"
#include "ctfvsm-meg/ovasCDriverCtfVsmMeg.h"
#include "egi-ampserver/ovasCDriverEGIAmpServer.h"
#include "mitsarEEG202A/ovasCDriverMitsarEEG202A.h"
*/

#include "ovasCPluginExternalStimulations.h"


namespace OpenViBEContributions {

	void initiateContributions(OpenViBEAcquisitionServer::CAcquisitionServerGUI* pGUI, const OpenViBE::Kernel::IKernelContext& rKernelContext, std::vector<OpenViBEAcquisitionServer::IDriver*> vDriver)
	{
		pGUI->registerPlugin(new OpenViBEAcquisitionServer::OpenViBEAcquisitionServerPlugins::CPluginExternalStimulations(rKernelContext));
	}

}
