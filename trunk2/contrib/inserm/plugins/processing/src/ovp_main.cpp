
#include <ov_all.h>

#include "ovp_defines.h"

#include "box-algorithms/ovpCSteadyStateFrequencyComparison.h"
#include "box-algorithms/ovpCEpoching.h"
#include "box-algorithms/ovpCBandFrequencyAverage.h"
#include "box-algorithms/ovpCMouseControl.h"

OVP_Declare_Begin();

	rPluginModuleContext.getTypeManager().registerEnumerationType (OVP_TypeId_ComparisonMethod, "Comparison method");
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_ComparisonMethod, "Ratio",            OVP_TypeId_ComparisonMethod_Ratio.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_ComparisonMethod, "Substraction",     OVP_TypeId_ComparisonMethod_Substraction.toUInteger());
	rPluginModuleContext.getTypeManager().registerEnumerationEntry(OVP_TypeId_ComparisonMethod, "Laterality index", OVP_TypeId_ComparisonMethod_LateralityIndex.toUInteger());

	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CBandFrequencyAverageDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CEpochingDesc)
	OVP_Declare_New(OpenViBEPlugins::SignalProcessing::CSteadyStateFrequencyComparisonDesc)
	OVP_Declare_New(OpenViBEPlugins::Tools::CMouseControlDesc);

OVP_Declare_End();
