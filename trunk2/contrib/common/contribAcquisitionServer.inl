/*
#include "gtec-gmobilabplus/src/ovasCDriverGTecGMobiLabPlus.h"
#include "openeeg-modulareeg/src/ovasCDriverOpenEEGModularEEG.h"
#include "field-trip-protocol/src/ovasCDriverFieldtrip.h"
#include "brainproducts-brainvisionrecorder/src/ovasCDriverBrainProductsBrainVisionRecorder.h"
#include "egi-ampserver/src/ovasCDriverEGIAmpServer.h"
*/

#include "ovasCPluginExternalStimulations.h"

#include "ovasCDriverGTecGUSBamp.h"
#include "ovasCDriverCtfVsmMeg.h"
#include "ovasCDriverBrainProductsBrainVisionRecorder.h"
#include "ovasCDriverMitsarEEG202A.h"


namespace OpenViBEContributions {




	void initiateContributions(OpenViBEAcquisitionServer::CAcquisitionServerGUI* pGUI, OpenViBEAcquisitionServer::CAcquisitionServer* pAcquisitionServer, const OpenViBE::Kernel::IKernelContext& rKernelContext, std::vector<OpenViBEAcquisitionServer::IDriver*>* vDriver)
	{
		vDriver->push_back(new OpenViBEAcquisitionServer::CDriverCtfVsmMeg(pAcquisitionServer->getDriverContext()));
		vDriver->push_back(new OpenViBEAcquisitionServer::CDriverBrainProductsBrainVisionRecorder(pAcquisitionServer->getDriverContext()));
#if defined TARGET_HAS_ThirdPartyGUSBampCAPI
		vDriver->push_back(new OpenViBEAcquisitionServer::CDriverGTecGUSBamp(pAcquisitionServer->getDriverContext()));
#endif
#if defined TARGET_OS_Windows
		vDriver->push_back(new OpenViBEAcquisitionServer::CDriverMitsarEEG202A(pAcquisitionServer->getDriverContext()));
#endif

		pGUI->registerPlugin(new OpenViBEAcquisitionServer::OpenViBEAcquisitionServerPlugins::CPluginExternalStimulations(rKernelContext));
	}

}
