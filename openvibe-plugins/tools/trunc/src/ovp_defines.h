#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

//___________________________________________________________________//
//                                                                   //
// Plugin Object Descriptor Class Identifiers                        //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_EBMLStreamSpyDesc                       OpenViBE::CIdentifier(0x354A6864, 0x06BC570C)

/*
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00D16F71, 0x31B2F266)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0005CBFB, 0x1F0E96A4)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00B5A13B, 0x71F3E137)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0027B7A0, 0x2070BD9F)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00EEB84E, 0x5F6BC168)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00B8F4D7, 0x3D690A60)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00891C63, 0x7B36F6CA)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00C53C3C, 0x1F8ACD9B)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00C57D1A, 0x6F93767A)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0051D367, 0x564DBAE2)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00286473, 0x37A599DB)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x002C9EC5, 0x5072EE78)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x007859AB, 0x03174870)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x004601D2, 0x4E6AF279)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x008DF1FF, 0x39D92E32)
*/

//___________________________________________________________________//
//                                                                   //
// Plugin Object Class Identifiers                                   //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_EBMLStreamSpy                           OpenViBE::CIdentifier(0x0ED76695, 0x01A69CC3)

/*
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00CAB19C, 0x0D926E59)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00AD8C4D, 0x4FF68962)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00BC0FA8, 0x7056BB7C)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x003735D4, 0x745A195A)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00D99E2A, 0x41279854)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00B37566, 0x77E720E7)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00112883, 0x76F88249)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0014E37D, 0x714CCD45)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x002782C3, 0x1FFD7599)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x003F5DE5, 0x50FEA854)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x007E1187, 0x56898A61)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x002A4E8D, 0x3BF3B873)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0073F1E5, 0x2BB7271F)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00E5945D, 0x702EB756)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00BD8F5D, 0x635E7EB2)
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
