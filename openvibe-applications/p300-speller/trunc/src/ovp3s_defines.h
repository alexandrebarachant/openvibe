#ifndef __OpenViBEP300Speller_defines_H__
#define __OpenViBEP300Speller_defines_H__

//___________________________________________________________________//
//                                                                   //
// Operating System identification                                   //
//___________________________________________________________________//
//                                                                   //

// #define OVP3S_OS_Linux
// #define OVP3S_OS_Windows
// #define OVP3S_OS_MacOS
// #define OVP3S_OS_

#if defined TARGET_OS_Windows
 #define OVP3S_OS_Windows
#elif defined TARGET_OS_Linux
 #define OVP3S_OS_Linux
#elif defined TARGET_OS_MacOS
 #define OVP3S_OS_MacOS
#else
 #warning "No target operating system defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Hardware Architecture identification                              //
//___________________________________________________________________//
//                                                                   //

// #define OVP3S_ARCHITECTURE_i386
// #define OVP3S_ARCHITECTURE_

#if defined TARGET_ARCHITECTURE_i386
 #define OVP3S_ARCHITECTURE_i386
#else
 #warning "No target architecture defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Compilator software identification                                //
//___________________________________________________________________//
//                                                                   //

// #define OVP3S_COMPILATOR_GCC
// #define OVP3S_COMPILATOR_VisualStudio
// #define OVP3S_COMPILATOR_

#if defined TARGET_COMPILATOR_GCC
 #define OVP3S_COMPILATOR_GCC
#elif defined TARGET_COMPILATOR_VisualStudio
 #define OVP3S_COMPILATOR_VisualStudio
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
#if defined OVP3S_Shared
 #if defined OVP3S_OS_Windows
  #define OVP3S_API_Export __declspec(dllexport)
  #define OVP3S_API_Import __declspec(dllimport)
 #elif defined OVP3S_OS_Linux
  #define OVP3S_API_Export __attribute__((visibility("default")))
  #define OVP3S_API_Import __attribute__((visibility("default")))
 #else
  #define OVP3S_API_Export
  #define OVP3S_API_Import
 #endif
#else
 #define OVP3S_API_Export
 #define OVP3S_API_Import
#endif

#if defined OVP3S_Exports
 #define OVP3S_API OVP3S_API_Export
#else
 #define OVP3S_API OVP3S_API_Import
#endif

//___________________________________________________________________//
//                                                                   //
// NULL Definition                                                   //
//___________________________________________________________________//
//                                                                   //

#ifndef NULL
#define NULL 0
#endif

#endif // __OpenViBEP300Speller_defines_H__
