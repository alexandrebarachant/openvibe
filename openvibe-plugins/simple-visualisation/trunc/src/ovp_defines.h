#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

//___________________________________________________________________//
//                                                                   //
// Plugin Object Descriptor Class Identifiers                        //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_SignalDisplay                           OpenViBE::CIdentifier(0x0055BE5F, 0x087BDD12)
#define OVP_ClassId_GrazVisualization                       OpenViBE::CIdentifier(0x00DD290D, 0x5F142820)
#define OVP_ClassId_FrequencySpectrumDisplay                OpenViBE::CIdentifier(0x0090540A, 0x6FB8C769)
#define OVP_ClassId_PowerSpectrumDisplay                    OpenViBE::CIdentifier(0x004C0EA4, 0x713EC6D9)

//___________________________________________________________________//
//                                                                   //
// Plugin Object Class Identifiers                                   //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_SignalDisplayDesc                       OpenViBE::CIdentifier(0x00C4F2D5, 0x58810276)
#define OVP_ClassId_GrazVisualizationDesc                   OpenViBE::CIdentifier(0x00F1955D, 0x38813A6A)
#define OVP_ClassId_FrequencySpectrumDisplayDesc            OpenViBE::CIdentifier(0x0045E1B7, 0x49F62560)
#define OVP_ClassId_PowerSpectrumDisplayDesc                OpenViBE::CIdentifier(0x00116B40, 0x69E1B00D)

//___________________________________________________________________//
//                                                                   //
// Some enumerations                                                 //
//___________________________________________________________________//
//                                                                   //

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		enum EDisplayMode
		{
			DisplayMode_Default,
			DisplayMode_ZoomIn,
			DisplayMode_ZoomOut,
			DisplayMode_BestFit,
			DisplayMode_Normal,
		};
	};
};

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
