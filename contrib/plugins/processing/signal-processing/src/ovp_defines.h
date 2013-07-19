
#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

// @BEGIN inserm
#define OVP_ClassId_BoxAlgorithm_BandFrequencyAverage                                  OpenViBE::CIdentifier(0x95E4781E, 0x45576562)
#define OVP_ClassId_BoxAlgorithm_BandFrequencyAverageDesc                              OpenViBE::CIdentifier(0xB87877FC, 0xFB3591F2)

#define OVP_ClassId_BoxAlgorithm_Epoching                                              OpenViBE::CIdentifier(0x974D2B9C, 0x369D1065)
#define OVP_ClassId_BoxAlgorithm_EpochingDesc                                          OpenViBE::CIdentifier(0x617BF26E, 0xADDFBC4D)

#define OVP_ClassId_BoxAlgorithm_SteadyStateFrequencyComparison                        OpenViBE::CIdentifier(0xC13EFA6C, 0x7757834D)
#define OVP_ClassId_BoxAlgorithm_SteadyStateFrequencyComparisonDesc                    OpenViBE::CIdentifier(0x50DC8D3E, 0x22B235DB)
#define OVP_TypeId_ComparisonMethod                                                    OpenViBE::CIdentifier(0xCF7AE755, 0x134E71CB)
#define OVP_TypeId_ComparisonMethod_Ratio                                              OpenViBE::CIdentifier(0xE81FAD81, 0xB320B8EA)
#define OVP_TypeId_ComparisonMethod_Substraction                                       OpenViBE::CIdentifier(0x2444FC59, 0x40DAB3AD)
#define OVP_TypeId_ComparisonMethod_LateralityIndex                                    OpenViBE::CIdentifier(0x127A1836, 0xF608C3B5)
// @END inserm

// @BEGIN inserm-gpl
#define OVP_ClassId_SpectralAnalysis                                                    OpenViBE::CIdentifier(0x84218FF8, 0xA87E7995)
#define OVP_ClassId_SpectralAnalysisDesc                                                OpenViBE::CIdentifier(0x0051E63C, 0x68E83AD1)
#define OVP_TypeId_SpectralComponent                                                    OpenViBE::CIdentifier(0x764E148A, 0xC704D4F5)
#define OVP_TypeId_SpectralComponent_Amplitude                                          OpenViBE::CIdentifier(0x00000000, 0x00000001)
#define OVP_TypeId_SpectralComponent_Phase                                              OpenViBE::CIdentifier(0x00000000, 0x00000002)
#define OVP_TypeId_SpectralComponent_RealPart                                           OpenViBE::CIdentifier(0x00000000, 0x00000004)
#define OVP_TypeId_SpectralComponent_ImaginaryPart                                      OpenViBE::CIdentifier(0x00000000, 0x00000008)

#define OVP_TypeId_FilterMethod                                                         OpenViBE::CIdentifier(0x2F2C606C, 0x8512ED68)
#define OVP_TypeId_FilterMethod_Butterworth                                             OpenViBE::CIdentifier(0x00E4EADE, 0x9586A50E)
#define OVP_TypeId_FilterMethod_Chebychev                                               OpenViBE::CIdentifier(0x0C3F5301, 0x7397A7FB)
#define OVP_TypeId_FilterMethod_YuleWalker                                              OpenViBE::CIdentifier(0x7BF59C86, 0xB6D459BA)

#define OVP_TypeId_FilterType                                                           OpenViBE::CIdentifier(0xFA20178E, 0x4CBA62E9)
#define OVP_TypeId_FilterType_LowPass                                                   OpenViBE::CIdentifier(0x0C33A568, 0xE2BD46D2)
#define OVP_TypeId_FilterType_BandPass                                                  OpenViBE::CIdentifier(0x19E89DDB, 0xD1FF7FF5)
#define OVP_TypeId_FilterType_HighPass                                                  OpenViBE::CIdentifier(0x2F677195, 0x9F9F15B9)
#define OVP_TypeId_FilterType_BandStop                                                  OpenViBE::CIdentifier(0x41C34DD7, 0xF70E4043)

#define OVP_TypeId_WindowMethod                                                         OpenViBE::CIdentifier(0x0A430FE4, 0x4F318280)
#define OVP_TypeId_WindowMethod_Hamming                                                 OpenViBE::CIdentifier(0x3A9FF7F1, 0x54E79D67)
#define OVP_TypeId_WindowMethod_Hanning                                                 OpenViBE::CIdentifier(0x660DA3E7, 0x7BD87719)
#define OVP_TypeId_WindowMethod_Hann                                                    OpenViBE::CIdentifier(0x9BBBDC65, 0xFDFEF7A4)
#define OVP_TypeId_WindowMethod_Blackman                                                OpenViBE::CIdentifier(0x9D5937A4, 0xE43A9E3D)
#define OVP_TypeId_WindowMethod_Triangular                                              OpenViBE::CIdentifier(0xE652A852, 0xFE3CBC46)
#define OVP_TypeId_WindowMethod_SquareRoot                                              OpenViBE::CIdentifier(0xFE80BE5C, 0xAE59A27A)

#define OVP_TypeId_FrequencyCutOffRatio                                                 OpenViBE::CIdentifier(0x709FC9DF, 0x30A2CB2A)
#define OVP_TypeId_FrequencyCutOffRatio_14                                              OpenViBE::CIdentifier(0x1647FA20, 0xF8312286)
#define OVP_TypeId_FrequencyCutOffRatio_13                                              OpenViBE::CIdentifier(0x228279C0, 0x434B9E77)
#define OVP_TypeId_FrequencyCutOffRatio_12                                              OpenViBE::CIdentifier(0x4D216BC9, 0xCA817C88)

#define OVP_TypeId_MinMax                                                               OpenViBE::CIdentifier(0x4263AC45, 0x0AF5E07E)
#define OVP_TypeId_MinMax_Min                                                           OpenViBE::CIdentifier(0x0DB9C7B9, 0x44C8C59D)
#define OVP_TypeId_MinMax_Max                                                           OpenViBE::CIdentifier(0x14711334, 0xE8F32071)

#define OVP_ClassId_Algorithm_DetectingMinMax                                           OpenViBE::CIdentifier(0x46C14A64, 0xE00541DD)
#define OVP_ClassId_Algorithm_DetectingMinMaxDesc                                       OpenViBE::CIdentifier(0x5B194CDA, 0x54E6DEC7)
#define OVP_Algorithm_DetectingMinMax_InputParameterId_SignalMatrix                     OpenViBE::CIdentifier(0x9CA3B6BB, 0x6E24A3E3)
#define OVP_Algorithm_DetectingMinMax_InputParameterId_TimeWindowStart                  OpenViBE::CIdentifier(0xB3DED659, 0xD8A85CFA)
#define OVP_Algorithm_DetectingMinMax_InputParameterId_TimeWindowEnd                    OpenViBE::CIdentifier(0x9F55A091, 0xA042E9C0)
#define OVP_Algorithm_DetectingMinMax_InputParameterId_SamplingFrequency                OpenViBE::CIdentifier(0x8519915D, 0xB6BE506D)
#define OVP_Algorithm_DetectingMinMax_OutputParameterId_SignalMatrix                    OpenViBE::CIdentifier(0x853F2DE5, 0x628237CE)
#define OVP_Algorithm_DetectingMinMax_InputTriggerId_Initialize                         OpenViBE::CIdentifier(0x6B43B69D, 0xDA1EAE30)
#define OVP_Algorithm_DetectingMinMax_InputTriggerId_DetectsMin                         OpenViBE::CIdentifier(0xFCB3CFC2, 0x980E3085)
#define OVP_Algorithm_DetectingMinMax_InputTriggerId_DetectsMax                         OpenViBE::CIdentifier(0x24926194, 0x086E6C2A)

#define OVP_ClassId_Box_DetectingMinMaxBoxAlgorithm                                     OpenViBE::CIdentifier(0xD647A2C4, 0xD4833160)
#define OVP_ClassId_Box_DetectingMinMaxBoxAlgorithmDesc                                 OpenViBE::CIdentifier(0xEF9E296A, 0x10285AE1)

#define OVP_ClassId_Algorithm_Downsampling                                              OpenViBE::CIdentifier(0xBBBB4E18, 0x17695604)
#define OVP_ClassId_Algorithm_DownsamplingDesc                                          OpenViBE::CIdentifier(0xC08BA8C1, 0x3A3B6E26)
#define OVP_Algorithm_Downsampling_InputParameterId_SamplingFrequency                   OpenViBE::CIdentifier(0x7C510AFB, 0x4F2B9FB7)
#define OVP_Algorithm_Downsampling_InputParameterId_NewSamplingFrequency                OpenViBE::CIdentifier(0x8617E5FA, 0xC39CDBE7)
#define OVP_Algorithm_Downsampling_InputTriggerId_Initialize                            OpenViBE::CIdentifier(0x82D96F84, 0x9479A701)
#define OVP_Algorithm_Downsampling_InputTriggerId_Resample                              OpenViBE::CIdentifier(0x2A88AFF5, 0x79ECAEB3)
#define OVP_Algorithm_Downsampling_InputTriggerId_ResampleWithHistoric                  OpenViBE::CIdentifier(0xD5740B33, 0x3785C886)
#define OVP_Algorithm_Downsampling_InputParameterId_SignalMatrix                        OpenViBE::CIdentifier(0xBB09054A, 0xEF13B2C6)
#define OVP_Algorithm_Downsampling_OutputParameterId_SignalMatrix                       OpenViBE::CIdentifier(0x4B9BE135, 0x14C10757)

#define OVP_ClassId_Box_DownsamplingBoxAlgorithm                                        OpenViBE::CIdentifier(0x6755FD0F, 0xE4857EA8)
#define OVP_ClassId_Box_DownsamplingBoxAlgorithmDesc                                    OpenViBE::CIdentifier(0xC8A99636, 0x81EF1AAD)

#define OVP_ClassId_Algorithm_ComputeTemporalFilterCoefficients                         OpenViBE::CIdentifier(0x55BAD77B, 0x5D8563A7)
#define OVP_ClassId_Algorithm_ComputeTemporalFilterCoefficientsDesc                     OpenViBE::CIdentifier(0xD871BD98, 0x705ED068)
#define OVP_Algorithm_ComputeTemporalFilterCoefficients_InputParameterId_SamplingFrequency  OpenViBE::CIdentifier(0x25A9A0FF, 0x168F1B50)
#define OVP_Algorithm_ComputeTemporalFilterCoefficients_InputParameterId_FilterMethod       OpenViBE::CIdentifier(0xCFB7CDC9, 0x3EFF788E)
#define OVP_Algorithm_ComputeTemporalFilterCoefficients_InputParameterId_FilterType         OpenViBE::CIdentifier(0x1B7BCB2C, 0xE235A6E7)
#define OVP_Algorithm_ComputeTemporalFilterCoefficients_InputParameterId_FilterOrder        OpenViBE::CIdentifier(0x8DA1E555, 0x17E17828)
#define OVP_Algorithm_ComputeTemporalFilterCoefficients_InputParameterId_LowCutFrequency    OpenViBE::CIdentifier(0x3175B774, 0xA15AEEB2)
#define OVP_Algorithm_ComputeTemporalFilterCoefficients_InputParameterId_HighCutFrequency   OpenViBE::CIdentifier(0xE36387B7, 0xFB766612)
#define OVP_Algorithm_ComputeTemporalFilterCoefficients_InputParameterId_BandPassRipple     OpenViBE::CIdentifier(0xB1500ED4, 0x0E558759)
#define OVP_Algorithm_ComputeTemporalFilterCoefficients_OutputParameterId_Matrix            OpenViBE::CIdentifier(0xE5B2A753, 0x150500B4)
#define OVP_Algorithm_ComputeTemporalFilterCoefficients_InputTriggerId_Initialize           OpenViBE::CIdentifier(0x3D2CBA61, 0x3FCF0DAC)
#define OVP_Algorithm_ComputeTemporalFilterCoefficients_InputTriggerId_ComputeCoefficients  OpenViBE::CIdentifier(0x053A2C6E, 0x3A878825)

#define OVP_ClassId_Algorithm_ApplyTemporalFilter                                       OpenViBE::CIdentifier(0x916B558F, 0xE501A2F4)
#define OVP_ClassId_Algorithm_ApplyTemporalFilterDesc                                   OpenViBE::CIdentifier(0xCCBD104B, 0xAC1C7D3E)
#define OVP_Algorithm_ApplyTemporalFilter_InputParameterId_FilterCoefficientsMatrix     OpenViBE::CIdentifier(0xD316C4E7, 0xE4E89FD3)
#define OVP_Algorithm_ApplyTemporalFilter_InputParameterId_SignalMatrix                 OpenViBE::CIdentifier(0xD5339105, 0x1D1293F0)
#define OVP_Algorithm_ApplyTemporalFilter_OutputParameterId_FilteredSignalMatrix        OpenViBE::CIdentifier(0x463276D1, 0xEAEE8AAD)
#define OVP_Algorithm_ApplyTemporalFilter_InputTriggerId_Initialize                     OpenViBE::CIdentifier(0x3DAE69C7, 0x7CFCBE2C)
#define OVP_Algorithm_ApplyTemporalFilter_InputTriggerId_ApplyFilter                    OpenViBE::CIdentifier(0xBC1F5655, 0x9807B400)
#define OVP_Algorithm_ApplyTemporalFilter_InputTriggerId_ApplyFilterWithHistoric        OpenViBE::CIdentifier(0xB7B7D546, 0x6000FF51)

#define OVP_ClassId_TemporalFilter                                                      OpenViBE::CIdentifier(0x4DEB9943, 0x7E638DC1)
#define OVP_ClassId_TemporalFilterDesc                                                  OpenViBE::CIdentifier(0x9EB3386D, 0x492C30A2)

#define OVP_ClassId_Box_TemporalFilterBoxAlgorithm                                      OpenViBE::CIdentifier(0xB4F9D042, 0x9D79F2E5)
#define OVP_ClassId_Box_TemporalFilterBoxAlgorithmDesc                                  OpenViBE::CIdentifier(0x7BF6BA62, 0xAF829A37)

#define OVP_ClassId_Algorithm_FeatureExtractionLda                                      OpenViBE::CIdentifier(0xD1AA6C72, 0x0432E1C5)
#define OVP_ClassId_Algorithm_FeatureExtractionLdaDesc                                  OpenViBE::CIdentifier(0x2109D46A, 0x9A45E1DC)
#define OVP_Algorithm_FeatureExtractionLda_InputParameterId_EpochTable                  OpenViBE::CIdentifier(0xA7BE9AE6, 0xD09FAD58)
#define OVP_Algorithm_FeatureExtractionLda_InputParameterId_Class1SamplesNumber         OpenViBE::CIdentifier(0x7F99FBC6, 0x748B183B)
#define OVP_Algorithm_FeatureExtractionLda_OutputParameterId_MatrixFirstClass           OpenViBE::CIdentifier(0x76F84603, 0x1F5D7A10)
#define OVP_Algorithm_FeatureExtractionLda_OutputParameterId_MatrixSecondClass          OpenViBE::CIdentifier(0x0AF9008A, 0xE7A7B2B7)
#define OVP_Algorithm_FeatureExtractionLda_OutputParameterId_MatrixEpoch                OpenViBE::CIdentifier(0xD4B473F6, 0xD5626A12)
#define OVP_Algorithm_FeatureExtractionLda_InputTriggerId_Initialize                    OpenViBE::CIdentifier(0x06176684, 0x5FA2C7C7)
#define OVP_Algorithm_FeatureExtractionLda_InputTriggerId_ExtractFeature                OpenViBE::CIdentifier(0x3B505F2F, 0x024B2C52)

#define OVP_ClassId_WindowingFunctions                                                  OpenViBE::CIdentifier(0x002034AE, 0x6509FD8F)
#define OVP_ClassId_WindowingFunctionsDesc                                              OpenViBE::CIdentifier(0x602CF89F, 0x65BA6DA0)

#define OVP_ClassId_SpectralAnalysisCospectra                                           OpenViBE::CIdentifier(0x0002F6AA, 0x418B260F)
#define OVP_ClassId_SpectralAnalysisCospectraDesc                                       OpenViBE::CIdentifier(0xBE7311CE, 0x99BB5123)

#define OVP_ClassId_FastICA                                                             OpenViBE::CIdentifier(0x00649B6E, 0x6C88CD17)
#define OVP_ClassId_FastICADesc                                                         OpenViBE::CIdentifier(0x00E9436C, 0x41C904CA)

// @END inserm-gpl

//___________________________________________________________________//
//                                                                   //
// Global defines                                                   //
//___________________________________________________________________//
//                                                                   //

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines
 #include "ovp_global_defines.h"
#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines


#endif // __OpenViBEPlugins_Defines_H__
