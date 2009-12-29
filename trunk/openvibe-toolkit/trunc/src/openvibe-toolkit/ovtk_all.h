#ifndef __OpenViBEToolkit_All_H__
#define __OpenViBEToolkit_All_H__

//___________________________________________________________________//
//                                                                   //
// Basic includes                                                    //
//___________________________________________________________________//
//                                                                   //

#include "ovtk_defines.h"

//___________________________________________________________________//
//                                                                   //
// Tools                                                             //
//___________________________________________________________________//
//                                                                   //

#include "tools/ovtkColorGradient.h"
#include "tools/ovtkMatrix.h"
#include "tools/ovtkStimulationSet.h"

//___________________________________________________________________//
//                                                                   //
//                                                                   //
//___________________________________________________________________//
//                                                                   //

#include "ovtkIVector.h"
#include "ovtkIFeatureVector.h"
#include "ovtkIFeatureVectorSet.h"

#include "box-algorithms/ovtkTBoxAlgorithm.h"

#if defined TARGET_HAS_EBML
#include "box-algorithms/ovtkTTrainingBoxAlgorithm.h"
#endif

#include "algorithms/ovtkTAlgorithm.h"
#include "algorithms/classification/ovtkCAlgorithmClassifier.h"

#include "algorithms/scenario-io/ovtkCAlgorithmScenarioImporter.h"
#include "algorithms/scenario-io/ovtkCAlgorithmScenarioExporter.h"

//___________________________________________________________________//
//                                                                   //
//                                                                   //
//___________________________________________________________________//
//                                                                   //

#if defined TARGET_HAS_EBML
#include "deprecated/reader/ovtkIBoxAlgorithmEBMLInputReaderCallback.h"
#include "deprecated/reader/ovtkIBoxAlgorithmStreamedMatrixInputReaderCallback.h"
#include "deprecated/reader/ovtkIBoxAlgorithmSignalInputReaderCallback.h"
#include "deprecated/reader/ovtkIBoxAlgorithmSpectrumInputReaderCallback.h"
#include "deprecated/reader/ovtkIBoxAlgorithmFeatureVectorInputReaderCallback.h"
#include "deprecated/reader/ovtkIBoxAlgorithmStimulationInputReaderCallback.h"
#include "deprecated/reader/ovtkIBoxAlgorithmExperimentInformationInputReaderCallback.h"
#endif

//___________________________________________________________________//
//                                                                   //
//                                                                   //
//___________________________________________________________________//
//                                                                   //

#if defined TARGET_HAS_EBML
#include "deprecated/writer/ovtkIBoxAlgorithmEBMLOutputWriter.h"
#include "deprecated/writer/ovtkIBoxAlgorithmStreamedMatrixOutputWriter.h"
#include "deprecated/writer/ovtkIBoxAlgorithmSignalOutputWriter.h"
#include "deprecated/writer/ovtkIBoxAlgorithmSpectrumOutputWriter.h"
#include "deprecated/writer/ovtkIBoxAlgorithmFeatureVectorOutputWriter.h"
#include "deprecated/writer/ovtkIBoxAlgorithmStimulationOutputWriter.h"
#include "deprecated/writer/ovtkIBoxAlgorithmExperimentInformationOutputWriter.h"
#endif

//___________________________________________________________________//
//                                                                   //
//                                                                   //
//___________________________________________________________________//
//                                                                   //

namespace OpenViBEToolkit
{
	OVTK_API OpenViBE::boolean initialize(const OpenViBE::Kernel::IKernelContext& rKernelContext);
	OVTK_API OpenViBE::boolean uninitialize(const OpenViBE::Kernel::IKernelContext& rKernelContext);
};

#endif // __OpenViBEToolkit_All_H__
