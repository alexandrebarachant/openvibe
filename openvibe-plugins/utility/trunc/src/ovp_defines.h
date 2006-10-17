#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

//___________________________________________________________________//
//                                                                   //
// Plugin Object Descriptor Class Identifiers                        //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_EEGStreamWriterGDFDesc                  OpenViBE::CIdentifier(0x375BF8EC, 0x023ACEEB)

/*
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x02817C77, 0x77FE3D6A)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x1E8AAB1A, 0x085D72F6)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x4EDEE8BD, 0x134E6DB4)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x3817D2AC, 0x505D4A31)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x6766D3CC, 0x64C05DA5)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x23A5755C, 0x1DF00578)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x247D3951, 0x25A9EBDA)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x2FF17CC9, 0x21BAC866)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x22637C65, 0x056B6E6D)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x5810005D, 0x4CAFB07F)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x35A6B0CF, 0x20DB9D67)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x1DC2525B, 0x6F2379C7)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x1338B4B7, 0x5C5E561E)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x11058F72, 0x1AEF2155)
*/

//___________________________________________________________________//
//                                                                   //
// Plugin Object Class Identifiers                                   //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_EEGStreamWriterGDF                     OpenViBE::CIdentifier(0x1E7B2155, 0x107289CE)

/*
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x78EA86B0, 0x2933E255)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x0B1D880D, 0x02A17229)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x61927968, 0x6C20387F)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x0873D0DD, 0x0CF4ACA6)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x2A4714D8, 0x4A61C013)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x71304D40, 0x56D5E67C)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x44FA2707, 0x44E3CBFF)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x092A9D05, 0x728816C2)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x24E248DB, 0x451B3B5E)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x5098E408, 0x2C7F7E68)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x77020839, 0x061A5EC4)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x4C525120, 0x3FE541A0)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x5D10C4FA, 0x5D88B479)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x2F3477B3, 0x4AFC10F0)
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
