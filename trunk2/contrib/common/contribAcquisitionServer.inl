/*
#include "gtec-gmobilabplus/src/ovasCDriverGTecGMobiLabPlus.h"
#include "openeeg-modulareeg/src/ovasCDriverOpenEEGModularEEG.h"
#include "field-trip-protocol/src/ovasCDriverFieldtrip.h"
#include "brainproducts-brainvisionrecorder/src/ovasCDriverBrainProductsBrainVisionRecorder.h"
#include "egi-ampserver/src/ovasCDriverEGIAmpServer.h"
*/

#include "ovasCPluginExternalStimulations.h"

#include "ovasCDriverBrainProductsBrainVisionRecorder.h"
#include "ovasCDriverCtfVsmMeg.h"
#include "ovasCDriverGTecGUSBamp.h"
#include "ovasCDriverFieldtrip.h"
#include "ovasCDriverMitsarEEG202A.h"
#include "ovasCDriverOpenALAudioCapture.h"
#include "ovasCDriverOpenEEGModularEEG.h"


namespace OpenViBEContributions {




	void initiateContributions(OpenViBEAcquisitionServer::CAcquisitionServerGUI* pGUI, OpenViBEAcquisitionServer::CAcquisitionServer* pAcquisitionServer, const OpenViBE::Kernel::IKernelContext& rKernelContext, std::vector<OpenViBEAcquisitionServer::IDriver*>* vDriver)
	{
		vDriver->push_back(new OpenViBEAcquisitionServer::CDriverBrainProductsBrainVisionRecorder(pAcquisitionServer->getDriverContext()));
		vDriver->push_back(new OpenViBEAcquisitionServer::CDriverCtfVsmMeg(pAcquisitionServer->getDriverContext()));
#if defined TARGET_HAS_ThirdPartyGUSBampCAPI
		vDriver->push_back(new OpenViBEAcquisitionServer::CDriverGTecGUSBamp(pAcquisitionServer->getDriverContext()));
#endif
		vDriver->push_back(new OpenViBEAcquisitionServer::CDriverFieldtrip(pAcquisitionServer->getDriverContext()));
#if defined TARGET_OS_Windows
		vDriver->push_back(new OpenViBEAcquisitionServer::CDriverMitsarEEG202A(pAcquisitionServer->getDriverContext()));
#endif

#if defined TARGET_HAS_ThirdPartyOpenAL
		vDriver->push_back(new OpenViBEAcquisitionServer::CDriverOpenALAudioCapture(pAcquisitionServer->getDriverContext()));
#endif

		vDriver->push_back(new OpenViBEAcquisitionServer::CDriverOpenEEGModularEEG(pAcquisitionServer->getDriverContext()));

		pGUI->registerPlugin(new OpenViBEAcquisitionServer::OpenViBEAcquisitionServerPlugins::CPluginExternalStimulations(rKernelContext));
	}

}
