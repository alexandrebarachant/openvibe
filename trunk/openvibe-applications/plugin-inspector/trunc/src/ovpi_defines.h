#ifndef __OpenViBEPluginInspector_defines_H__
#define __OpenViBEPluginInspector_defines_H__

//___________________________________________________________________//
//                                                                   //
// Operating System identification                                   //
//___________________________________________________________________//
//                                                                   //

// #define OVPI_OS_Linux
// #define OVPI_OS_Windows
// #define OVPI_OS_MacOS
// #define OVPI_OS_

#if defined TARGET_OS_Windows
 #define OVPI_OS_Windows
#elif defined TARGET_OS_Linux
 #define OVPI_OS_Linux
#elif defined TARGET_OS_MacOS
 #define OVPI_OS_MacOS
#else
 #warning "No target operating system defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Hardware Architecture identification                              //
//___________________________________________________________________//
//                                                                   //

// #define OVPI_ARCHITECTURE_i386
// #define OVPI_ARCHITECTURE_

#if defined TARGET_ARCHITECTURE_i386
 #define OVPI_ARCHITECTURE_i386
#else
 #warning "No target architecture defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Compiler software identification                                //
//___________________________________________________________________//
//                                                                   //

// #define OVPI_COMPILER_GCC
// #define OVPI_COMPILER_VisualStudio
// #define OVPI_COMPILER_

#if defined TARGET_COMPILER_GCC
 #define OVPI_COMPILER_GCC
#elif defined TARGET_COMPILER_VisualStudio
 #define OVPI_COMPILER_VisualStudio
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
#if defined OVPI_Shared
 #if defined OVPI_OS_Windows
  #define OVPI_API_Export __declspec(dllexport)
  #define OVPI_API_Import __declspec(dllimport)
 #elif defined OVPI_OS_Linux
  #define OVPI_API_Export __attribute__((visibility("default")))
  #define OVPI_API_Import __attribute__((visibility("default")))
 #else
  #define OVPI_API_Export
  #define OVPI_API_Import
 #endif
#else
 #define OVPI_API_Export
 #define OVPI_API_Import
#endif

#if defined OVPI_Exports
 #define OVPI_API OVPI_API_Export
#else
 #define OVPI_API OVPI_API_Import
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
