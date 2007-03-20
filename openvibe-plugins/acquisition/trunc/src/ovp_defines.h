#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

//___________________________________________________________________//
//                                                                   //
// Plugin Object Descriptor Class Identifiers                        //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_GenericNetworkAcquisitionDesc          OpenViBE::CIdentifier(0x390236CA, 0x3255E115)
#define OVP_ClassId_StandardAcquisitionBehaviorDesc        OpenViBE::CIdentifier(0x0C1D9C21, 0x0D3CACBA)
#define OVP_ClassId_GDFReaderDesc                              OpenViBE::CIdentifier(0x2E2543C1, 0x47E3739E)

/*

#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x40DE7FE6, 0x1E27307A)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x28F9BFC0, 0x2117A59B)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x05E8507E, 0x60FDFD5B)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x37F5B974, 0x5D8E1280)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x3D9D23C8, 0x5680ADE4)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x7118E29E, 0x17C1A43D)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x4923FE5D, 0x73A416BC)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x4D3134B7, 0x4B86F63B)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x629F9699, 0x14F69B2E)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x7D62FB7C, 0x1C335D0E)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x01593515, 0x3867EF79)
*/

//___________________________________________________________________//
//                                                                   //
// Plugin Object Class Identifiers                                   //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_GenericNetworkAcquisition              OpenViBE::CIdentifier(0x5D5CA50E, 0x7486E783)
#define OVP_ClassId_StandardAcquisitionBehavior            OpenViBE::CIdentifier(0x4D7D74C3, 0x3B1158B5)
#define OVP_ClassId_GDFReader                              OpenViBE::CIdentifier(0x3EEB1264, 0x4EDFBD9A)

/*
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x3EEB1264, 0x4EDFBD9A)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x18B22B6A, 0x0232922F)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x4BD07B50, 0x321FF876)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x22BAFFF6, 0x30518825)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x6FEC5E06, 0x0F5B0ED5)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x182923E0, 0x5BF8C94B)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x53238821, 0x328673D9)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x2B19B195, 0x766F0496)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x67818436, 0x76FE6C9D)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x7D91294F, 0x717A7FFA)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x1D25AC8B, 0x5582C0C5)
#define OVP_ClassId_                                       OpenViBE::CIdentifier(0x354B920B, 0x1E3FE5B0)
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
