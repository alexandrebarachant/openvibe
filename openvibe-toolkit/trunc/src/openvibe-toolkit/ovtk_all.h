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

#include "tools/ovtkMatrix.h"
#include "tools/ovtkStimulationSet.h"

//___________________________________________________________________//
//                                                                   //
//                                                                   //
//___________________________________________________________________//
//                                                                   //

#include "ovtkTAlgorithm.h"
#include "ovtkTBoxAlgorithm.h"

#if defined TARGET_HAS_EBML
#include "ovtkTTrainingBoxAlgorithm.h"
#endif

//___________________________________________________________________//
//                                                                   //
//                                                                   //
//___________________________________________________________________//
//                                                                   //

#if defined TARGET_HAS_EBML
#include "reader/ovtkIBoxAlgorithmEBMLInputReaderCallback.h"
#include "reader/ovtkIBoxAlgorithmStreamedMatrixInputReaderCallback.h"
#include "reader/ovtkIBoxAlgorithmSignalInputReaderCallback.h"
#include "reader/ovtkIBoxAlgorithmSpectrumInputReaderCallback.h"
#include "reader/ovtkIBoxAlgorithmFeatureVectorInputReaderCallback.h"
#include "reader/ovtkIBoxAlgorithmStimulationInputReaderCallback.h"
#include "reader/ovtkIBoxAlgorithmExperimentInformationInputReaderCallback.h"
#endif

//___________________________________________________________________//
//                                                                   //
//                                                                   //
//___________________________________________________________________//
//                                                                   //

#if defined TARGET_HAS_EBML
#include "writer/ovtkIBoxAlgorithmEBMLOutputWriter.h"
#include "writer/ovtkIBoxAlgorithmStreamedMatrixOutputWriter.h"
#include "writer/ovtkIBoxAlgorithmSignalOutputWriter.h"
#include "writer/ovtkIBoxAlgorithmSpectrumOutputWriter.h"
#include "writer/ovtkIBoxAlgorithmFeatureVectorOutputWriter.h"
#include "writer/ovtkIBoxAlgorithmStimulationOutputWriter.h"
#include "writer/ovtkIBoxAlgorithmExperimentInformationOutputWriter.h"
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
