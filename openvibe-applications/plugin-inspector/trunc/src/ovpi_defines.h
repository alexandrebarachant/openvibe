#ifndef __OpenViBEPluginInspector_defines_H__
#define __OpenViBEPluginInspector_defines_H__

//___________________________________________________________________//
//                                                                   //
// Operating System identification                                   //
//___________________________________________________________________//
//                                                                   //

// #define OVD_OS_Linux
// #define OVD_OS_Windows
// #define OVD_OS_MacOS
// #define OVD_OS_

#if defined TARGET_OS_Windows
 #define OVD_OS_Windows
#elif defined TARGET_OS_Linux
 #define OVD_OS_Linux
#elif defined TARGET_OS_MacOS
 #define OVD_OS_MacOS
#else
 #warning "No target operating system defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Hardware Architecture identification                              //
//___________________________________________________________________//
//                                                                   //

// #define OVD_ARCHITECTURE_i386
// #define OVD_ARCHITECTURE_

#if defined TARGET_ARCHITECTURE_i386
 #define OVD_ARCHITECTURE_i386
#else
 #warning "No target architecture defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Compilator software identification                                //
//___________________________________________________________________//
//                                                                   //

// #define OVD_COMPILATOR_GCC
// #define OVD_COMPILATOR_VisualStudio
// #define OVD_COMPILATOR_

#if defined TARGET_COMPILATOR_GCC
 #define OVD_COMPILATOR_GCC
#elif defined TARGET_COMPILATOR_VisualStudio
 #define OVD_COMPILATOR_VisualStudio
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
#if defined OVD_Shared
 #if defined OVD_OS_Windows
  #define OVD_API_Export __declspec(dllexport)
  #define OVD_API_Import __declspec(dllimport)
 #elif defined OVD_OS_Linux
  #define OVD_API_Export __attribute__((visibility("default")))
  #define OVD_API_Import __attribute__((visibility("default")))
 #else
  #define OVD_API_Export
  #define OVD_API_Import
 #endif
#else
 #define OVD_API_Export
 #define OVD_API_Import
#endif

#if defined OVD_Exports
 #define OVD_API OVD_API_Export
#else
 #define OVD_API OVD_API_Import
#endif

//___________________________________________________________________//
//                                                                   //
// NULL Definition                                                   //
//___________________________________________________________________//
//                                                                   //

#ifndef NULL
#define NULL 0
#endif

#endif // __OpenViBEPluginInspector_defines_H__
