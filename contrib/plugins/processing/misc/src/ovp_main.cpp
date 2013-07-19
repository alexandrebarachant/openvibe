
#include <vector>
#include <openvibe/ov_all.h>
#include "ovp_defines.h"

#include "box-algorithms/ovpCMouseControl.h"							// inserm
#include "box-algorithms/ovpCBoxAlgorithmSoundPlayer.h"					// multiple authors

OVP_Declare_Begin();

// @BEGIN inserm
	OVP_Declare_New(OpenViBEPlugins::Tools::CMouseControlDesc);
// @END inserm

	OVP_Declare_New(OpenViBEPlugins::Stimulation::CBoxAlgorithmSoundPlayerDesc);		// multiple authors
		 
OVP_Declare_End();
