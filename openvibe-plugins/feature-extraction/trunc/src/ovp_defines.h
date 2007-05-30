#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

//___________________________________________________________________//
//                                                                   //
// Plugin Object Descriptor Class Identifiers                        //
//___________________________________________________________________//
//                                                                   //

/*
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00B5B638, 0x25821BAF)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00E4DF63, 0x143A5A89)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00F83C33, 0x52EAD8FE)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x003EA213, 0x20D139DC)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00CDDA36, 0x5EC58508)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00BC4CC2, 0x4EEE2284)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x0057BEC4, 0x60CCE317)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x003ADCA7, 0x5EC85138)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00AFE878, 0x5BE884F8)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x0070C3FC, 0x6A0B1E81)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x0045C08C, 0x50B86150)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00FC84F0, 0x399A2056)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x008DC4E3, 0x05C0CE39)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00D86009, 0x24823F85)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00AA3DED, 0x2848196D)
*/

//___________________________________________________________________//
//                                                                   //
// Plugin Object Class Identifiers                                   //
//___________________________________________________________________//
//                                                                   //

/*
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00682417, 0x453635F9)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00D95B50, 0x49B23044)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x009C9CF4, 0x510916B5)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x006469FD, 0x2558AD85)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00B08439, 0x25C1DEDE)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x002316F2, 0x40FA3254)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00F37963, 0x57FAF438)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00D6415C, 0x36058C09)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x001EC742, 0x2B758702)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00FE36DF, 0x5CC1BA53)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00AB8BC1, 0x0822825D)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00B817C0, 0x5D65C261)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x006894FC, 0x4E83FCD9)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x0020682D, 0x33C28137)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x002E38F5, 0x54BA8B14)
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
