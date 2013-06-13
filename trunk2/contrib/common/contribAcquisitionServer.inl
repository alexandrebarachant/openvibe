/*
#include "gtec-gmobilabplus/ovasCDriverGTecGMobiLabPlus.h"
#include "openeeg-modulareeg/ovasCDriverOpenEEGModularEEG.h"
#include "field-trip-protocol/ovasCDriverFieldtrip.h"
#include "brainproducts-brainvisionrecorder/ovasCDriverBrainProductsBrainVisionRecorder.h"
#include "egi-ampserver/ovasCDriverEGIAmpServer.h"
#include "mitsarEEG202A/ovasCDriverMitsarEEG202A.h"
*/

#include "ovasCPluginExternalStimulations.h"
#include "ovasCDriverGTecGUSBamp.h"
#include "ovasCDriverCtfVsmMeg.h"
#include "ovasCDriverBrainProductsBrainVisionRecorder.h"


namespace OpenViBEContributions {




	void initiateContributions(OpenViBEAcquisitionServer::CAcquisitionServerGUI* pGUI, OpenViBEAcquisitionServer::CAcquisitionServer* pAcquisitionServer, const OpenViBE::Kernel::IKernelContext& rKernelContext, std::vector<OpenViBEAcquisitionServer::IDriver*>* vDriver)
	{
		vDriver->push_back(new OpenViBEAcquisitionServer::CDriverCtfVsmMeg(pAcquisitionServer->getDriverContext()));
		vDriver->push_back(new OpenViBEAcquisitionServer::CDriverBrainProductsBrainVisionRecorder(pAcquisitionServer->getDriverContext()));

		pGUI->registerPlugin(new OpenViBEAcquisitionServer::OpenViBEAcquisitionServerPlugins::CPluginExternalStimulations(rKernelContext));
	}

}
