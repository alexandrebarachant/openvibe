#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

//___________________________________________________________________//
//                                                                   //
// Plugin Object Descriptor Class Identifiers                        //
//___________________________________________________________________//
//                                                                   //

/*
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00D846C8, 0x264AACC9)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00E51ACD, 0x284CA2CF)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00A2F2C5, 0x60ABEF76)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00DA6C88, 0x313CD876)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00C1E15D, 0x163B3E74)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00D18899, 0x65FF7879)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x0048DC97, 0x02E101FF)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x0078DA99, 0x2DECF736)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x007D2ABF, 0x510A67D5)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00438478, 0x2079A52F)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00CCBD90, 0x33C8F2E9)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00ED5D0E, 0x57DF5460)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x001BEF08, 0x607F816F)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00F44D6A, 0x6A3E3BC3)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00E879DC, 0x3C3F15F2)
*/

//___________________________________________________________________//
//                                                                   //
// Plugin Object Class Identifiers                                   //
//___________________________________________________________________//
//                                                                   //

/*
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00646136, 0x44853D76)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00D317B9, 0x6324C3FF)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00DBC95A, 0x35188A73)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00657E8F, 0x2206BA02)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00973603, 0x76A3945D)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00E55280, 0x2390DC48)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00410F52, 0x5624CEFB)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x007F1F83, 0x1E409F9C)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x008D96EE, 0x35962C30)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x003E4AFD, 0x1DE4EAE8)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00706731, 0x6852B86C)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x0006BD37, 0x64FB8469)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00E22396, 0x17B733CC)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x00CF3563, 0x34C6627E)
#define OV_ClassId_                                        OpenViBE::CIdentifier(0x008BB3E1, 0x32B973BE)

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
