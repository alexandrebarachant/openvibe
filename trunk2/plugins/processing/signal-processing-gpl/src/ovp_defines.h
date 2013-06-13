
#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

// %s/OVP_\([a-zA-Z0-9]*\)_\([a-zA-Z0-9]*\)_\(In\|Out\)ParameterId/OVP_Algorithm_\1_\3putParameterId_\2/g
// %s/OVP_\([a-zA-Z0-9]*\)_\([a-zA-Z0-9]*\)_\(In\|Out\)TriggerId/OVP_Algorithm_\1_\3putTriggerId_\2/g

//___________________________________________________________________//
//                                                                   //
// Plugin Object Descriptor Class Identifiers                        //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_Algorithm_AcquisitionDecoderDesc                                    OpenViBE::CIdentifier(0xA01599B0, 0x7F51631A)
#define OVP_ClassId_Algorithm_EBMLBaseDecoderDesc                                       OpenViBE::CIdentifier(0x4F701AC9, 0xDFBE912E)
#define OVP_ClassId_Algorithm_ExperimentInformationDecoderDesc                          OpenViBE::CIdentifier(0x0F37CA61, 0x8A77F44E)
#define OVP_ClassId_Algorithm_FeatureVectorDecoderDesc                                  OpenViBE::CIdentifier(0xAB0AE561, 0xF181E34F)
#define OVP_ClassId_Algorithm_SignalDecoderDesc                                         OpenViBE::CIdentifier(0xF1547D89, 0x49FFD0C2)
#define OVP_ClassId_Algorithm_SpectrumDecoderDesc                                       OpenViBE::CIdentifier(0x54D18EE8, 0x5DBD913A)
#define OVP_ClassId_Algorithm_StimulationDecoderDesc                                    OpenViBE::CIdentifier(0x391A615B, 0x71CD888A)
#define OVP_ClassId_Algorithm_StreamedMatrixDecoderDesc                                 OpenViBE::CIdentifier(0x384529D5, 0xD8E0A728)

#define OVP_ClassId_Algorithm_EBMLBaseEncoderDesc                                       OpenViBE::CIdentifier(0x47A9E701, 0x7C57BF3C)
#define OVP_ClassId_Algorithm_ExperimentInformationEncoderDesc                          OpenViBE::CIdentifier(0x8CC2C754, 0x61665FDA)
#define OVP_ClassId_Algorithm_FeatureVectorEncoderDesc                                  OpenViBE::CIdentifier(0xC249527B, 0x89EE1996)
#define OVP_ClassId_Algorithm_SignalEncoderDesc                                         OpenViBE::CIdentifier(0x90AC1E0F, 0x01518200)
#define OVP_ClassId_Algorithm_SpectrumEncoderDesc                                       OpenViBE::CIdentifier(0xD6182973, 0x122CE114)
#define OVP_ClassId_Algorithm_StimulationEncoderDesc                                    OpenViBE::CIdentifier(0x9B994B50, 0x52C3F06A)
#define OVP_ClassId_Algorithm_StreamedMatrixEncoderDesc                                 OpenViBE::CIdentifier(0xEEEFE060, 0x646EE8AB)

#define OVP_ClassId_Algorithm_Desc                                                      OpenViBE::CIdentifier(0x8F323B37, 0xAD9C560F)

#define OVP_ClassId_Box_DecoderAlgorithmTestDesc                                        OpenViBE::CIdentifier(0xE5176EB9, 0xD6E47D7F)
#define OVP_ClassId_Box_EncoderAlgorithmTestDesc                                        OpenViBE::CIdentifier(0x95E27325, 0x6893A519)

#define OVP_ClassId_Box_Desc                                                            OpenViBE::CIdentifier(0xD318C958, 0xBCA9A176)

//___________________________________________________________________//
//                                                                   //
// Plugin Object Class Identifiers                                   //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_Algorithm_AcquisitionDecoder                                        OpenViBE::CIdentifier(0x1E0812B7, 0x3F686DD4)
#define OVP_ClassId_Algorithm_EBMLBaseDecoder                                           OpenViBE::CIdentifier(0xFD30C96D, 0x8245A8F8)
#define OVP_ClassId_Algorithm_ExperimentInformationDecoder                              OpenViBE::CIdentifier(0x6FA7D52B, 0x80E2ABD6)
#define OVP_ClassId_Algorithm_FeatureVectorDecoder                                      OpenViBE::CIdentifier(0xC2689ECC, 0x43B335C1)
#define OVP_ClassId_Algorithm_SignalDecoder                                             OpenViBE::CIdentifier(0x7237C149, 0x0CA66DA7)
#define OVP_ClassId_Algorithm_SpectrumDecoder                                           OpenViBE::CIdentifier(0x128202DB, 0x449FC7A6)
#define OVP_ClassId_Algorithm_StimulationDecoder                                        OpenViBE::CIdentifier(0xC8807F2B, 0x0813C5B1)
#define OVP_ClassId_Algorithm_StreamedMatrixDecoder                                     OpenViBE::CIdentifier(0x7359D0DB, 0x91784B21)

#define OVP_ClassId_Algorithm_EBMLBaseEncoder                                           OpenViBE::CIdentifier(0x4272C178, 0x3FE84927)
#define OVP_ClassId_Algorithm_ExperimentInformationEncoder                              OpenViBE::CIdentifier(0x56B354FE, 0xBF175468)
#define OVP_ClassId_Algorithm_FeatureVectorEncoder                                      OpenViBE::CIdentifier(0x7EBE049D, 0xF777A602)
#define OVP_ClassId_Algorithm_SignalEncoder                                             OpenViBE::CIdentifier(0xC488AD3C, 0xEB2E36BF)
#define OVP_ClassId_Algorithm_SpectrumEncoder                                           OpenViBE::CIdentifier(0xB3E252DB, 0xC3214498)
#define OVP_ClassId_Algorithm_StimulationEncoder                                        OpenViBE::CIdentifier(0x6E86F7D5, 0xA4668108)
#define OVP_ClassId_Algorithm_StreamedMatrixEncoder                                     OpenViBE::CIdentifier(0x5CB32C71, 0x576F00A6)

#define OVP_ClassId_                                                                    OpenViBE::CIdentifier(0xFAB0E905, 0x48AFBD19)

#define OVP_ClassId_Box_DecoderAlgorithmTest                                            OpenViBE::CIdentifier(0x3C2EF355, 0xFE495C3D)
#define OVP_ClassId_Box_EncoderAlgorithmTest                                            OpenViBE::CIdentifier(0x87D18C62, 0xF2DAF779)

#define OVP_ClassId_Box_                                                                OpenViBE::CIdentifier(0xFDB56F8A, 0xEE2596C6)

//___________________________________________________________________//
//                                                                   //
// Plugin Object Descriptor Class Identifiers                        //
//___________________________________________________________________//
//                                                                   //


//___________________________________________________________________//
//                                                                   //
// Plugin Object Class Identifiers                                   //
//___________________________________________________________________//
//                                                                   //


#define OVP_ClassId_Algorithm_ARBurgMethod                        			OpenViBE::CIdentifier(0x3EC6A165, 0x2823A034)
#define OVP_ClassId_Algorithm_ARBurgMethodDesc                    			OpenViBE::CIdentifier(0xD7234DFF, 0x55447A14)
#define OVP_Algorithm_ARBurgMethod_InputParameterId_Matrix       			OpenViBE::CIdentifier(0x36A69669, 0x3651271D)
#define OVP_Algorithm_ARBurgMethod_OutputParameterId_Matrix      			OpenViBE::CIdentifier(0x55EF8C81, 0x178A51B2)
#define OVP_Algorithm_ARBurgMethod_InputParameterId_UInteger     			OpenViBE::CIdentifier(0x33139BC1, 0x03D30D3B)
#define OVP_Algorithm_ARBurgMethod_InputTriggerId_Initialize 	 			OpenViBE::CIdentifier(0xC27B06C6, 0xB8EB5F8D)
#define OVP_Algorithm_ARBurgMethod_InputTriggerId_Process		     		OpenViBE::CIdentifier(0xBEEBBE84, 0x4F14F8F8)
#define OVP_Algorithm_ARBurgMethod_OutputTriggerId_ProcessDone           		OpenViBE::CIdentifier(0xA5AAD435, 0x9EC3DB80) 


//___________________________________________________________________//
//                                                                   //
// Global defines                                                   //
//___________________________________________________________________//
//                                                                   //

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines
 #include "ovp_global_defines.h"
#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines


#endif // __OpenViBEPlugins_Defines_H__
