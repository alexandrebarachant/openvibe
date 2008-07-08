#include "ovp_defines.h"

#include "algorithms/ovpCAlgorithmSphericalSplineInterpolation.h"
#include "algorithms/simple-visualisation/ovpCAlgorithmLevelMeasure.h"

//2D plugins
#include "ovpCSignalDisplay.h"
#include "ovpCGrazVisualization.h"
#include "ovpCFrequencySpectrumDisplay.h"
#include "ovpCPowerSpectrumDisplay.h"
#include "ovpCTopographicMap2DDisplay.h"
#include "box-algorithms/simple-visualisation/ovpCBoxAlgorithmLevelMeasure.h"
//3D plugins
#include "ovpCSimple3DDisplay.h"
#include "ovpCTopographicMap3DDisplay.h"
#include "ovpCVoxelDisplay.h"

OVP_Declare_Begin()

	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CAlgorithmLevelMeasureDesc)
	OVP_Declare_New(OpenViBEPlugins::Test::CAlgorithmSphericalSplineInterpolationDesc)

	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CSignalDisplayDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CGrazVisualizationDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CFrequencySpectrumDisplayDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CPowerSpectrumDisplayDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CTopographicMap2DDisplayDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CBoxAlgorithmLevelMeasureDesc)

	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CSimple3DDisplayDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CTopographicMap3DDisplayDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CVoxelDisplayDesc)

OVP_Declare_End()
