#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

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
// Algorithms parameter/trigger identifier                           //
//___________________________________________________________________//
//                                                                   //

// EBML base decoder
#define OVP_EBMLDecoder_EBMLMemoryBuffer_InParameterId                                  OpenViBE::CIdentifier(0x2F98EA3C, 0xFB0BE096)
#define OVP_EBMLDecoder_GotHeader_TriggerId                                             OpenViBE::CIdentifier(0x815234BF, 0xAABAE5F2)
#define OVP_EBMLDecoder_GotBuffer_TriggerId                                             OpenViBE::CIdentifier(0xAA2738BF, 0xF7FE9FC3)
#define OVP_EBMLDecoder_GotEnd_TriggerId                                                OpenViBE::CIdentifier(0xC4AA114C, 0x628C2D77)

// Acquisition decoder
#define OVP_AcquisitionDecoder_ExperimentId_OutParameterId                              OpenViBE::CIdentifier(0xC1B135A6, 0x3FEB7790)
#define OVP_AcquisitionDecoder_SubjectAge_OutParameterId                                OpenViBE::CIdentifier(0x02305F0E, 0xBE576533)
#define OVP_AcquisitionDecoder_SubjectGender_OutParameterId                             OpenViBE::CIdentifier(0x980FE40F, 0xB9E4A021)
#define OVP_AcquisitionDecoder_SamplingRate_OutParameterId                              OpenViBE::CIdentifier(0xF9414765, 0x2A689150)
#define OVP_AcquisitionDecoder_ChannelLocation_OutParameterId                           OpenViBE::CIdentifier(0xEDD46293, 0x67747212)
#define OVP_AcquisitionDecoder_Signal_OutParameterId                                    OpenViBE::CIdentifier(0x4FCC8D5E, 0x5A47117D)
#define OVP_AcquisitionDecoder_StimulationSet_OutParameterId                            OpenViBE::CIdentifier(0xC3049A33, 0x890DC535)

// Experiment information decoder
#define OVP_ExperimentInformationDecoder_ExperimentIdentifier_OutParameterId            OpenViBE::CIdentifier(0x40259641, 0x478C73DE)
#define OVP_ExperimentInformationDecoder_ExperimentDate_OutParameterId                  OpenViBE::CIdentifier(0xBC0266A2, 0x9C2935F1)
#define OVP_ExperimentInformationDecoder_SubjectIdentifier_OutParameterId               OpenViBE::CIdentifier(0x97C5D20D, 0x203E65B3)
#define OVP_ExperimentInformationDecoder_SubjectName_OutParameterId                     OpenViBE::CIdentifier(0x3D3826EA, 0xE8883815)
#define OVP_ExperimentInformationDecoder_SubjectAge_OutParameterId                      OpenViBE::CIdentifier(0xC36C6B08, 0x5227380A)
#define OVP_ExperimentInformationDecoder_SubjectGender_OutParameterId                   OpenViBE::CIdentifier(0x7D5059E8, 0xE4D8B38D)
#define OVP_ExperimentInformationDecoder_LaboratoryIdentifier_OutParameterId            OpenViBE::CIdentifier(0xE761D3D4, 0x44BA1EBF)
#define OVP_ExperimentInformationDecoder_LaboratoryName_OutParameterId                  OpenViBE::CIdentifier(0x5CA80FA5, 0x774F01CB)
#define OVP_ExperimentInformationDecoder_TechnicianIdentifier_OutParameterId            OpenViBE::CIdentifier(0xC8ECFBBC, 0x0DCDA310)
#define OVP_ExperimentInformationDecoder_TechnicianName_OutParameterId                  OpenViBE::CIdentifier(0xB8A94B68, 0x389393D9)

// Signal decoder
#define OVP_SignalDecoder_SamplingRate_OutParameterId                                   OpenViBE::CIdentifier(0x363D8D79, 0xEEFB912C)

// Spectrum decoder
#define OVP_SpectrumDecoder_FrequencyBandMinMax_OutParameterId                          OpenViBE::CIdentifier(0x375E55E9, 0x9B3666F6)

// Stimulation decoder
#define OVP_StimulationDecoder_StimulationSet_OutParameterId                            OpenViBE::CIdentifier(0xF46D0C19, 0x47306BEA)

// Streamed matrix decoder
#define OVP_StreamedMatrixDecoder_Matrix_OutParameterId                                 OpenViBE::CIdentifier(0x79EF3123, 0x35E3EA4D)

// EBML base encoder
#define OVP_EBMLEncoder_MemoryBuffer_OutParameterId                                     OpenViBE::CIdentifier(0xA3D8B171, 0xF8734734)
#define OVP_EBMLEncoder_WriteHeader_TriggerId                                           OpenViBE::CIdentifier(0x878EAF60, 0xF9D5303F)
#define OVP_EBMLEncoder_WriteBuffer_TriggerId                                           OpenViBE::CIdentifier(0x1B7076FD, 0x449BC70A)
#define OVP_EBMLEncoder_WriteEnd_TriggerId                                              OpenViBE::CIdentifier(0x3FC23508, 0x806753D8)
#define OVP_EBMLEncoder_MemoryBufferUpdated_TriggerId                                   OpenViBE::CIdentifier(0xD46C7462, 0xD3407E5F)

// Experiment information encoder
#define OVP_ExperimentInformationEncoder_ExperimentIdentifier_InParameterId             OpenViBE::CIdentifier(0x40259641, 0x478C73DE)
#define OVP_ExperimentInformationEncoder_ExperimentDate_InParameterId                   OpenViBE::CIdentifier(0xBC0266A2, 0x9C2935F1)
#define OVP_ExperimentInformationEncoder_SubjectIdentifier_InParameterId                OpenViBE::CIdentifier(0x97C5D20D, 0x203E65B3)
#define OVP_ExperimentInformationEncoder_SubjectName_InParameterId                      OpenViBE::CIdentifier(0x3D3826EA, 0xE8883815)
#define OVP_ExperimentInformationEncoder_SubjectAge_InParameterId                       OpenViBE::CIdentifier(0xC36C6B08, 0x5227380A)
#define OVP_ExperimentInformationEncoder_SubjectGender_InParameterId                    OpenViBE::CIdentifier(0x7D5059E8, 0xE4D8B38D)
#define OVP_ExperimentInformationEncoder_LaboratoryIdentifier_InParameterId             OpenViBE::CIdentifier(0xE761D3D4, 0x44BA1EBF)
#define OVP_ExperimentInformationEncoder_LaboratoryName_InParameterId                   OpenViBE::CIdentifier(0x5CA80FA5, 0x774F01CB)
#define OVP_ExperimentInformationEncoder_TechnicianIdentifier_InParameterId             OpenViBE::CIdentifier(0xC8ECFBBC, 0x0DCDA310)
#define OVP_ExperimentInformationEncoder_TechnicianName_InParameterId                   OpenViBE::CIdentifier(0xB8A94B68, 0x389393D9)

// Signal encoder
#define OVP_SignalEncoder_SamplingRate_InParameterId                                    OpenViBE::CIdentifier(0x998710FF, 0x2C5CCA82)

// Spectrum encoder
#define OVP_SpectrumEncoder_FrequencyBandMinMax_InParameterId                           OpenViBE::CIdentifier(0xF5251300, 0x72520E28)

// Stimulation encoder
#define OVP_StimulationEncoder_StimulationSet_InParameterId                             OpenViBE::CIdentifier(0x8565254C, 0x3A49268E)

// Streamed matrix encoder
#define OVP_StreamedMatrixEncoder_Matrix_InParameterId                                  OpenViBE::CIdentifier(0xA3E9E5B0, 0xAE756303)

/*
#define OVP__ParameterId                                                                OpenViBE::CIdentifier(0xCB650D53, 0x5198A45E)
#define OVP__ParameterId                                                                OpenViBE::CIdentifier(0xDF81BB74, 0xAB43CD56)
*/

//___________________________________________________________________//
//                                                                   //
// Operating System identification                                   //
//___________________________________________________________________//
//                                                                   //

// #define OVP_OS_Linux
// #define OVP_OS_Windows
// #define OVP_OS_MacOS
// #define OVP_OS_

#if defined TARGET_OS_Windows
 #define OVP_OS_Windows
#elif defined TARGET_OS_Linux
 #define OVP_OS_Linux
#elif defined TARGET_OS_MacOS
 #define OVP_OS_MacOS
#else
 #warning "No target operating system defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Hardware Architecture identification                              //
//___________________________________________________________________//
//                                                                   //

// #define OVP_ARCHITECTURE_i386
// #define OVP_ARCHITECTURE_

#if defined TARGET_ARCHITECTURE_i386
 #define OVP_ARCHITECTURE_i386
#else
 #warning "No target architecture defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Compilator software identification                                //
//___________________________________________________________________//
//                                                                   //

// #define OVP_COMPILATOR_GCC
// #define OVP_COMPILATOR_VisualStudio
// #define OVP_COMPILATOR_

#if defined TARGET_COMPILATOR_GCC
 #define OVP_COMPILATOR_GCC
#elif defined TARGET_COMPILATOR_VisualStudio
 #define OVP_COMPILATOR_VisualStudio
#else
 #warning "No target compilator defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// API Definition                                                    //
//___________________________________________________________________//
//                                                                   //

// Taken from
// - http://people.redhat.com/drepper/dsohowto.pdf
// - http://www.nedprod.com/programs/gccvisibility.html
#if defined OVP_Shared
 #if defined OVP_OS_Windows
  #define OVP_API_Export __declspec(dllexport)
  #define OVP_API_Import __declspec(dllimport)
 #elif defined OVP_OS_Linux
  #define OVP_API_Export __attribute__((visibility("default")))
  #define OVP_API_Import __attribute__((visibility("default")))
 #else
  #define OVP_API_Export
  #define OVP_API_Import
 #endif
#else
 #define OVP_API_Export
 #define OVP_API_Import
#endif

#if defined OVP_Exports
 #define OVP_API OVP_API_Export
#else
 #define OVP_API OVP_API_Import
#endif

//___________________________________________________________________//
//                                                                   //
// NULL Definition                                                   //
//___________________________________________________________________//
//                                                                   //

#ifndef NULL
#define NULL 0
#endif

#endif // __OpenViBEPlugins_Defines_H__
