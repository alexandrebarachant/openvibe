#ifndef OV_COMMON_DEFINES_H
#define OV_COMMON_DEFINES_H

//
// This file checks the presence of several defines that are supposed to 
// be set by the build system. In addition, it defines some additional ones.
//

//___________________________________________________________________//
//                                                                   //
// Operating System identification                                   //
//___________________________________________________________________//
//                                                                   //

#if !(defined(TARGET_OS_Windows) || defined(TARGET_OS_Linux) || defined(TARGET_OS_MacOS))
 #warning "No known target OS specified!"
#endif

//___________________________________________________________________//
//                                                                   //
// Build type identification                                         //
//___________________________________________________________________//
//                                                                   //

#if !(defined(TARGET_BUILDTYPE_Debug) || defined(TARGET_BUILDTYPE_Release))
 #warning "No known build type defined!"
#endif

//___________________________________________________________________//
//                                                                   //
// Hardware Architecture identification                              //
//___________________________________________________________________//
//                                                                   //

#if !defined(TARGET_ARCHITECTURE_i386)
 #warning "No i386 target architecture defined!"
#endif

//___________________________________________________________________//
//                                                                   //
// Compiler software identification                                  //
//___________________________________________________________________//
//                                                                   //

#if !(defined(TARGET_COMPILER_GCC) || defined(TARGET_COMPILER_VisualStudio))
 #warning "No known compiler defined!"
#endif


//___________________________________________________________________//
//                                                                   //
// API Definition                                                    //
//___________________________________________________________________//
//                                                                   //

// Taken from
// - http://people.redhat.com/drepper/dsohowto.pdf
// - http://www.nedprod.com/programs/gccvisibility.html
#if defined OV_Shared
 #if defined TARGET_OS_Windows
  #define OV_API_Export __declspec(dllexport)
  #define OV_API_Import __declspec(dllimport)
 #elif defined TARGET_OS_Linux
  #define OV_API_Export __attribute__((visibility("default")))
  #define OV_API_Import __attribute__((visibility("default")))
 #else
  #define OV_API_Export
  #define OV_API_Import
 #endif
#else
 #define OV_API_Export
 #define OV_API_Import
#endif

#if defined OV_Exports
 #define OV_API OV_API_Export
#else
 #define OV_API OV_API_Import
#endif

//___________________________________________________________________//
//                                                                   //
// NULL Definition                                                   //
//___________________________________________________________________//
//                                                                   //

#ifndef NULL
#define NULL 0
#endif


#endif

