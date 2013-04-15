#ifndef __OpenViBEApplication_Defines_H__
#define __OpenViBEApplication_Defines_H__

//___________________________________________________________________//
//                                                                   //
// Operating System identification                                   //
//___________________________________________________________________//
//                                                                   //

// #define OVA_OS_Linux
// #define OVA_OS_Windows
// #define OVA_OS_MacOS
// #define OVA_OS_

#if defined TARGET_OS_Windows
 #define OVA_OS_Windows
#elif defined TARGET_OS_Linux
 #define OVA_OS_Linux
#elif defined TARGET_OS_MacOS
 #define OVA_OS_MacOS
#else
 #warning "No target operating system defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Hardware Architecture identification                              //
//___________________________________________________________________//
//                                                                   //

// #define OVA_ARCHITECTURE_i386
// #define OVA_ARCHITECTURE_

#if defined TARGET_ARCHITECTURE_i386
 #define OVA_ARCHITECTURE_i386
#else
 #warning "No target architecture defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Build type identification                                         //
//___________________________________________________________________//
//                                                                   //

// #define OVK_BUILDTYPE_Debug
// #define OVK_BUILDTYPE_Release

#if defined TARGET_BUILDTYPE_Debug
 #define OVA_BUILDTYPE_Debug
#elif defined TARGET_BUILDTYPE_Release
 #define OVA_BUILDTYPE_Release
#else
 #warning "No build type defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Compiler software identification                                //
//___________________________________________________________________//
//                                                                   //

// #define OVA_COMPILER_GCC
// #define OVA_COMPILER_VisualStudio
// #define OVA_COMPILER_

#if defined TARGET_COMPILER_GCC
 #define OVA_COMPILER_GCC
#elif defined TARGET_COMPILER_VisualStudio
 #define OVA_COMPILER_VisualStudio
#else
 #warning "No target compiler defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// API Definition                                                    //
//___________________________________________________________________//
//                                                                   //

// Taken from
// - http://people.redhat.com/drepper/dsohowto.pdf
// - http://www.nedprod.com/programs/gccvisibility.html
#if defined OVA_Shared
 #if defined OVA_OS_Windows
  #define OVA_API_Export __declspec(dllexport)
  #define OVA_API_Import __declspec(dllimport)
 #elif defined OVA_OS_Linux
  #define OVA_API_Export __attribute__((visibility("default")))
  #define OVA_API_Import __attribute__((visibility("default")))
 #else
  #define OVA_API_Export
  #define OVA_API_Import
 #endif
#else
 #define OVA_API_Export
 #define OVA_API_Import
#endif

#if defined OVA_Exports
 #define OVA_API OVA_API_Export
#else
 #define OVA_API OVA_API_Import
#endif

//___________________________________________________________________//
//                                                                   //
// NULL Definition                                                   //
//___________________________________________________________________//
//                                                                   //

#ifndef NULL
#define NULL 0
#endif

#endif // __OpenViBEApplication_Defines_H__
