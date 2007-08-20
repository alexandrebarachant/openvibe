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
//                                                                   //
//___________________________________________________________________//
//                                                                   //

#include "ovtkTBoxAlgorithm.h"
// #include "ovtkTTrainingBoxAlgorithm.h"

//___________________________________________________________________//
//                                                                   //
//                                                                   //
//___________________________________________________________________//
//                                                                   //

#include "reader/ovtkIBoxAlgorithmEBMLInputReaderCallback.h"
#include "reader/ovtkIBoxAlgorithmStreamedMatrixInputReaderCallback.h"
#include "reader/ovtkIBoxAlgorithmSignalInputReaderCallback.h"
#include "reader/ovtkIBoxAlgorithmSpectrumInputReaderCallback.h"
#include "reader/ovtkIBoxAlgorithmFeatureVectorInputReaderCallback.h"
#include "reader/ovtkIBoxAlgorithmStimulationInputReaderCallback.h"
#include "reader/ovtkIBoxAlgorithmExperimentInformationInputReaderCallback.h"

//___________________________________________________________________//
//                                                                   //
//                                                                   //
//___________________________________________________________________//
//                                                                   //

#include "writer/ovtkIBoxAlgorithmEBMLOutputWriter.h"
#include "writer/ovtkIBoxAlgorithmStreamedMatrixOutputWriter.h"
#include "writer/ovtkIBoxAlgorithmSignalOutputWriter.h"
#include "writer/ovtkIBoxAlgorithmSpectrumOutputWriter.h"
#include "writer/ovtkIBoxAlgorithmFeatureVectorOutputWriter.h"
#include "writer/ovtkIBoxAlgorithmStimulationOutputWriter.h"
#include "writer/ovtkIBoxAlgorithmExperimentInformationOutputWriter.h"

#endif // __OpenViBEToolkit_All_H__
