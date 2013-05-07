
#include "ovp_defines.h"

#include "box-algorithms/ovpCBoxAlgorithmStreamedMatrixMultiplexer.h"
#include "box-algorithms/ovpCBoxAlgorithmSignalMerger.h"
#include "box-algorithms/ovpCBoxAlgorithmStreamedMatrixSwitch.h"

#include <vector>

OVP_Declare_Begin()
	OVP_Declare_New(OpenViBEPlugins::Streaming::CBoxAlgorithmStreamedMatrixMultiplexerDesc);
	OVP_Declare_New(OpenViBEPlugins::Streaming::CBoxAlgorithmSignalMergerDesc);
	OVP_Declare_New(OpenViBEPlugins::Streaming::CBoxAlgorithmStreamedMatrixSwitchDesc);
OVP_Declare_End()
