#include "ovp_defines.h"

#include "ovpCSignalDisplay.h"
#include "ovpCGrazVisualization.h"
#include "ovpCFrequencySpectrumDisplay.h"
#include "ovpCPowerSpectrumDisplay.h"

#include "algorithms/simple-visualisation/ovpCAlgorithmLevelMeasure.h"

#include "box-algorithms/simple-visualisation/ovpCBoxAlgorithmLevelMeasure.h"

OVP_Declare_Begin()
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CSignalDisplayDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CGrazVisualizationDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CFrequencySpectrumDisplayDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CPowerSpectrumDisplayDesc)

	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CBoxAlgorithmLevelMeasureDesc)

	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CAlgorithmLevelMeasureDesc)
OVP_Declare_End()
