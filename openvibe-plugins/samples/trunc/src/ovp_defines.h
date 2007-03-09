#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

//___________________________________________________________________//
//                                                                   //
// Plugin Object Descriptor Class Identifiers                        //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_BoxAlgorithmProducerDesc    OpenViBE::CIdentifier(0x10C620AE, 0x211274FF)
#define OVP_ClassId_BoxAlgorithmConsumerDesc    OpenViBE::CIdentifier(0x191F4A6F, 0x07EB1BF2)
#define OVP_ClassId_ScenarioExporterXMLDesc     OpenViBE::CIdentifier(0x5B2B5C67, 0x78EA7539)
#define OVP_ClassId_ScenarioImporterXMLDesc     OpenViBE::CIdentifier(0x28DC180C, 0x4CCA7314)
#define OVP_ClassId_EBMLStreamSpyDesc           OpenViBE::CIdentifier(0x354A6864, 0x06BC570C)
#define OVP_ClassId_SinusSignalGeneratorDesc    OpenViBE::CIdentifier(0x2633AFA2, 0x6974E32F)
#define OVP_ClassId_IdentityDesc                OpenViBE::CIdentifier(0x54743810, 0x6A1A88CC)
#define OVP_ClassId_WidgetDesc                  OpenViBE::CIdentifier(0x400B3B8D, 0x427A7CE7)

/*
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x61ED4780, 0x758C3DD3)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x4D3676B8, 0x01ED7594)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x6B5A33C9, 0x7EB13AD6)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x50AEA682, 0x22ADFFA0)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x7CA6CD07, 0x626CBEEA)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x042893AF, 0x00BC2448)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x60C4EF84, 0x10F4B044)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x401CF892, 0x055B4108)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x3A2B0FA9, 0x49C37676)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x336DBB3F, 0x766D9860)
*/

//___________________________________________________________________//
//                                                                   //
// Plugin Object Class Identifiers                                   //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_BoxAlgorithmProducer        OpenViBE::CIdentifier(0x77A308C8, 0x711D8E57)
#define OVP_ClassId_BoxAlgorithmConsumer        OpenViBE::CIdentifier(0x271A6EEC, 0x7B1D847E)
#define OVP_ClassId_ScenarioExporterXML         OpenViBE::CIdentifier(0x77075B3B, 0x3D632492)
#define OVP_ClassId_ScenarioImporterXML         OpenViBE::CIdentifier(0x440BF3AC, 0x2D960300)
#define OVP_ClassId_EBMLStreamSpy               OpenViBE::CIdentifier(0x0ED76695, 0x01A69CC3)
#define OVP_ClassId_SinusSignalGenerator        OpenViBE::CIdentifier(0x7E33BDB8, 0x68194A4A)
#define OVP_ClassId_Identity                    OpenViBE::CIdentifier(0x5DFFE431, 0x35215C50)
#define OVP_ClassId_Widget                      OpenViBE::CIdentifier(0x5DAFB0BA, 0x77E3BB0F)

/*
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x6B00CEA0, 0x23617BB8)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x69D67D52, 0x7FA5E735)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x391AB48F, 0x0EB03ECD)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x151EAEF6, 0x460AA4F6)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x45FAE21C, 0x2FA256C3)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x3C2AB7F9, 0x35950591)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x57F2869C, 0x0BA2BA14)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x27F70837, 0x0C9B17CE)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x42D47B30, 0x76B122E5)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x187DC075, 0x44F46655)
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
