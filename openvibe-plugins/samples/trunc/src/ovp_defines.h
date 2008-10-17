#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

#define OVP_ClassId_Algorithm_SampleClassifier              OpenViBE::CIdentifier(0x7E54E7C8, 0xFECC2806)
#define OVP_ClassId_Algorithm_SampleClassifierDesc          OpenViBE::CIdentifier(0x17E4233B, 0xA74CD097)

//___________________________________________________________________//
//                                                                   //
// Plugin Object Descriptor Class Identifiers                        //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_ScenarioExporterSVGDesc                 OpenViBE::CIdentifier(0xF90BB4FF, 0x219FF8BE)
#define OVP_ClassId_ScenarioExporterXMLDesc                 OpenViBE::CIdentifier(0x5B2B5C67, 0x78EA7539)
#define OVP_ClassId_ScenarioImporterXMLDesc                 OpenViBE::CIdentifier(0x28DC180C, 0x4CCA7314)

#define OVP_ClassId_CrashingBoxDesc                         OpenViBE::CIdentifier(0x009F54B9, 0x2B6A4922)
#define OVP_ClassId_BoxAlgorithmProducerDesc                OpenViBE::CIdentifier(0x10C620AE, 0x211274FF)
#define OVP_ClassId_BoxAlgorithmConsumerDesc                OpenViBE::CIdentifier(0x191F4A6F, 0x07EB1BF2)
#define OVP_ClassId_SinusSignalGeneratorDesc                OpenViBE::CIdentifier(0x2633AFA2, 0x6974E32F)
#define OVP_ClassId_TimeSignalGeneratorDesc                 OpenViBE::CIdentifier(0x57AD8655, 0x1966B4DC)
#define OVP_ClassId_IdentityDesc                            OpenViBE::CIdentifier(0x54743810, 0x6A1A88CC)
#define OVP_ClassId_LogDesc                                 OpenViBE::CIdentifier(0x00780136, 0x57633D46)
#define OVP_ClassId_TestDesc                                OpenViBE::CIdentifier(0x0024B879, 0x33C0E15F)
#define OVP_ClassId_BoxAlgorithm_ClockDesc                  OpenViBE::CIdentifier(0x754C233D, 0x37DF04A3)

#define OVP_ClassId_AlgorithmAdditionDesc                   OpenViBE::CIdentifier(0x842E0B85, 0xA59FABC1)
#define OVP_ClassId_BoxAlgorithmAdditionTestDesc            OpenViBE::CIdentifier(0xB33EC315, 0xF63BC0C5)

//___________________________________________________________________//
//                                                                   //
// Plugin Object Class Identifiers                                   //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_ScenarioExporterSVG                     OpenViBE::CIdentifier(0x389FB9DF, 0x848FDF29)
#define OVP_ClassId_ScenarioExporterXML                     OpenViBE::CIdentifier(0x77075B3B, 0x3D632492)
#define OVP_ClassId_ScenarioImporterXML                     OpenViBE::CIdentifier(0x440BF3AC, 0x2D960300)

#define OVP_ClassId_CrashingBox                             OpenViBE::CIdentifier(0x00DAFD60, 0x39A58819)
#define OVP_ClassId_BoxAlgorithmProducer                    OpenViBE::CIdentifier(0x77A308C8, 0x711D8E57)
#define OVP_ClassId_BoxAlgorithmConsumer                    OpenViBE::CIdentifier(0x271A6EEC, 0x7B1D847E)
#define OVP_ClassId_SinusSignalGenerator                    OpenViBE::CIdentifier(0x7E33BDB8, 0x68194A4A)
#define OVP_ClassId_TimeSignalGenerator                     OpenViBE::CIdentifier(0x28A5E7FF, 0x530095DE)
#define OVP_ClassId_Identity                                OpenViBE::CIdentifier(0x5DFFE431, 0x35215C50)
#define OVP_ClassId_Log                                     OpenViBE::CIdentifier(0x00BE3E25, 0x274F2075)
#define OVP_ClassId_Test                                    OpenViBE::CIdentifier(0x0053270F, 0x78887C71)
#define OVP_ClassId_BoxAlgorithm_Clock                      OpenViBE::CIdentifier(0x14CB4CFC, 0x6D064CB3)

#define OVP_ClassId_AlgorithmAddition                       OpenViBE::CIdentifier(0x75FCE50E, 0x8302FA91)
#define OVP_ClassId_BoxAlgorithmAdditionTest                OpenViBE::CIdentifier(0x534EB140, 0x15F41496)

//___________________________________________________________________//
//                                                                   //
// Gloabal defines                                                   //
//___________________________________________________________________//
//                                                                   //

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines
 #include "ovp_global_defines.h"
#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines

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
