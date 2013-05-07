#include "ovp_defines.h"

#include "algorithms/ovpCAlgorithmSphericalSplineInterpolation.h"
#include "algorithms/ovpCAlgorithmLevelMeasure.h"

//Presentation
#include "box-algorithms/ovpCGrazVisualization.h"
#include "box-algorithms/ovpCBoxAlgorithmP300SpellerVisualisation.h"
#include "box-algorithms/ovpCBoxAlgorithmP300MagicCardVisualisation.h"
#include "box-algorithms/ovpCBoxAlgorithmP300IdentifierCardVisualisation.h"
#include "box-algorithms/ovpCDisplayCueImage.h"

//2D plugins
#include "box-algorithms/ovpCSignalDisplay.h"
#include "box-algorithms/ovpCTimeFrequencyMapDisplay.h"
#include "box-algorithms/ovpCPowerSpectrumDisplay.h"
#include "box-algorithms/ovpCTopographicMap2DDisplay.h"
#include "box-algorithms/ovpCBoxAlgorithmLevelMeasure.h"
#include "box-algorithms/ovpCBoxAlgorithmClassifierAccuracyMeasure.h"
#include "box-algorithms/ovpCBoxAlgorithmMatrixDisplay.h"
//3D plugins
#include "box-algorithms/ovpCSimple3DDisplay.h"
#include "box-algorithms/ovpCTopographicMap3DDisplay.h"
#include "box-algorithms/ovpCVoxelDisplay.h"

OVP_Declare_Begin()

	rPluginModuleContext.getTypeManager().registerEnumerationType (OVP_TypeId_SphericalLinearInterpolationType, "Spherical linear interpolation type");
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_SphericalLinearInterpolationType, "Spline (potentials)", OVP_TypeId_SphericalLinearInterpolationType_Spline);
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_SphericalLinearInterpolationType, "Spline laplacian (currents)",  OVP_TypeId_SphericalLinearInterpolationType_Laplacian);

	rPluginModuleContext.getTypeManager().registerEnumerationType (OVP_TypeId_SignalDisplayMode, "Signal display mode");
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_SignalDisplayMode, "Scroll", OVP_TypeId_SignalDisplayMode_Scroll.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_SignalDisplayMode, "Scan",  OVP_TypeId_SignalDisplayMode_Scan.toUInteger());

	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CAlgorithmLevelMeasureDesc)
	OVP_Declare_New(OpenViBEPlugins::Test::CAlgorithmSphericalSplineInterpolationDesc)

	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CGrazVisualizationDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CBoxAlgorithmP300SpellerVisualisationDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CBoxAlgorithmP300MagicCardVisualisationDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CDisplayCueImageDesc)

	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CSignalDisplayDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CTimeFrequencyMapDisplayDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CPowerSpectrumDisplayDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CTopographicMap2DDisplayDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CBoxAlgorithmLevelMeasureDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CBoxAlgorithmClassifierAccuracyMeasureDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CBoxAlgorithmMatrixDisplayDesc)

	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CSimple3DDisplayDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CTopographicMap3DDisplayDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CVoxelDisplayDesc)
	OVP_Declare_New(OpenViBEPlugins::SimpleVisualisation::CBoxAlgorithmP300IdentifierCardVisualisationDesc)

OVP_Declare_End()
