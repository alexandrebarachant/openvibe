#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

//___________________________________________________________________//
//                                                                   //
// Plugin Object Descriptor Class Identifiers                        //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_SignalDisplay                           OpenViBE::CIdentifier(0x0055BE5F, 0x087BDD12)
#define OVP_ClassId_GrazVisualization                       OpenViBE::CIdentifier(0x00DD290D, 0x5F142820)
#define OVP_ClassId_FrequencySpectrumDisplay                OpenViBE::CIdentifier(0x0090540A, 0x6FB8C769)

/*
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x004C0EA4, 0x713EC6D9)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00DF1F9E, 0x50EB7093)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x007527BD, 0x77FB529D)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00D5E055, 0x56B9FA8B)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0071B371, 0x613E802D)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00F88F03, 0x22F0E059)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x007C40F1, 0x51E88A0F)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00E367BB, 0x2A8A278D)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00D31E69, 0x2E7BC03E)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0032C666, 0x4726C03A)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x000E4389, 0x1D51DBE9)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0086386C, 0x101AEB77)
*/

//___________________________________________________________________//
//                                                                   //
// Plugin Object Class Identifiers                                   //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_SignalDisplayDesc                       OpenViBE::CIdentifier(0x00C4F2D5, 0x58810276)
#define OVP_ClassId_GrazVisualizationDesc                   OpenViBE::CIdentifier(0x00F1955D, 0x38813A6A)
#define OVP_ClassId_FrequencySpectrumDisplayDesc            OpenViBE::CIdentifier(0x0045E1B7, 0x49F62560)

/*
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00116B40, 0x69E1B00D)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x005AA737, 0x3CF063F3)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x006B709F, 0x067F5678)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x001F32C5, 0x17AA8A9E)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00C6741E, 0x6D26AF5D)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00EA22D5, 0x74042A68)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0060EA95, 0x5F5C33C3)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00FF084E, 0x2F7FDC61)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00852E71, 0x07997AF5)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x008F8B86, 0x2A89912F)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00254FB1, 0x16EBACFF)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00981681, 0x5ED034D9)
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
