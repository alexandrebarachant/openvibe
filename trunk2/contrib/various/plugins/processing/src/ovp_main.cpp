
#include "box-algorithms/ovpCBoxAlgorithmSoundPlayer.h"
#include "box-algorithms/ovpCBoxAlgorithmPython.h"

OVP_Declare_Begin()

	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmSoundPlayerDesc);

#if defined TARGET_HAS_ThirdPartyPython
	OVP_Declare_New(OpenViBEPlugins::Python::CBoxAlgorithmPythonDesc);
#endif

OVP_Declare_End()
