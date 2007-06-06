#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

//___________________________________________________________________//
//                                                                   //
// Plugin Object Descriptor Class Identifiers                        //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_ChannelSelectorDesc                     OpenViBE::CIdentifier(0x34893489, 0x44934897)
#define OVP_ClassId_SimpleDSPDesc                           OpenViBE::CIdentifier(0x00C44BFE, 0x76C9269E)
#define OVP_ClassId_SignalAverageDesc                       OpenViBE::CIdentifier(0x007CDCE9, 0x16034F77)

/*
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0033EAF8, 0x09C65E4E)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0039EF62, 0x11DA5967)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x000B30B6, 0x29E85D42)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x000738D9, 0x4A6648AE)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x000C8040, 0x7A4F177E)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00D3EBDE, 0x49B8EE8A)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00C4F2DD, 0x443045DD)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00B2E6E3, 0x23BD1C10)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0044C93B, 0x2229D6DA)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0006BFED, 0x0B591767)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00511ADC, 0x5ACD2C22)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00D572AD, 0x1E9BD83E)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x002D2682, 0x1D8678F0)
*/

//___________________________________________________________________//
//                                                                   //
// Plugin Object Class Identifiers                                   //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_ChannelSelector                         OpenViBE::CIdentifier(0x39484563, 0x46386889)
#define OVP_ClassId_SimpleDSP                               OpenViBE::CIdentifier(0x00E26FA1, 0x1DBAB1B2)
#define OVP_ClassId_SignalAverage                           OpenViBE::CIdentifier(0x00642C4D, 0x5DF7E50A)

/*
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x009C0CE3, 0x6BDF71C3)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x001B4E3D, 0x7FEE9CB1)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00D283A5, 0x40D76DD3)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00B82A89, 0x701DB82A)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0042232E, 0x6F5B5E21)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0023A01B, 0x60B31FEB)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00D2181F, 0x12AD12E0)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00F2E33C, 0x499FFC48)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0035AA12, 0x1A2AA4A4)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00F78154, 0x072DE682)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00F6B473, 0x45FCBAC3)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00654A7B, 0x6765A31A)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x007063D2, 0x5E9D2069)
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
