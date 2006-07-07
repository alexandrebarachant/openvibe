#ifndef __OpenViBE_Defines_H__
#define __OpenViBE_Defines_H__

//___________________________________________________________________//
//                                                                   //
// Global Class Identifiers                                          //
//___________________________________________________________________//
//                                                                   //

#define OV_ClassIdentifier_Undefined                        OpenViBE::CIdentifier(0x00000000, 0x00000000)
#define OV_ClassIdentifier_Object                           OpenViBE::CIdentifier(0x327B23C6, 0x6B8B4567)
#define OV_ClassIdentifier_KernelLoader                     OpenViBE::CIdentifier(0x1930F64B, 0x6F689236)
#define OV_ClassIdentifier_KernelLoaderFinal                OpenViBE::CIdentifier(0x624A6E5B, 0x52228CEA)

//___________________________________________________________________//
//                                                                   //
// Kernel Class Identifiers                                          //
//___________________________________________________________________//
//                                                                   //

#define OV_ClassIdentifier_Box                              OpenViBE::CIdentifier(0x41209015, 0x1183237A)
#define OV_ClassIdentifier_Kernel                           OpenViBE::CIdentifier(0x255EEE43, 0x2B852543)
#define OV_ClassIdentifier_KernelDesc                       OpenViBE::CIdentifier(0x5410291E, 0x2FEF6B81)
#define OV_ClassIdentifier_KernelObject                     OpenViBE::CIdentifier(0x69BAE694, 0x163C8B2D)
#define OV_ClassIdentifier_KernelObjectFactory              OpenViBE::CIdentifier(0x04D88707, 0x21F2F456)
#define OV_ClassIdentifier_PluginModule                     OpenViBE::CIdentifier(0x74EAC344, 0x375C88EC)
#define OV_ClassIdentifier_Scenario                         OpenViBE::CIdentifier(0x6EED456C, 0x2FC6F03A)
#define OV_ClassIdentifier_PluginObjectLoadContext          OpenViBE::CIdentifier(0x624A6E5B, 0x52228CEA)
#define OV_ClassIdentifier_PluginObjectSaveContext          OpenViBE::CIdentifier(0x2A49A89B, 0x78843238)

//___________________________________________________________________//
//                                                                   //
// Plugin Class Identifiers                                          //
//___________________________________________________________________//
//                                                                   //

#define OV_ClassIdentifier_PluginObject                     OpenViBE::CIdentifier(0x1356510D, 0x6B6532DF)
#define OV_ClassIdentifier_PluginObjectDesc                 OpenViBE::CIdentifier(0x100E6855, 0x7C1CA212)
#define OV_ClassIdentifier_PluginObjectContext              OpenViBE::CIdentifier(0x1F8D3E14, 0x73A4DB1F)
#define OV_ClassIdentifier_BoxAlgorithm                     OpenViBE::CIdentifier(0x2A910204, 0x57E55735)
#define OV_ClassIdentifier_BoxAlgorithmDesc                 OpenViBE::CIdentifier(0x2C4ECAD0, 0x19607084)
#define OV_ClassIdentifier_BoxAlgorithmContext              OpenViBE::CIdentifier(0x275F4BB8, 0x49E08D24)
#define OV_ClassIdentifier_BoxBehavior                      OpenViBE::CIdentifier(0x58AE3092, 0x0C3371A7)
#define OV_ClassIdentifier_BoxBehaviorDesc                  OpenViBE::CIdentifier(0x1B9B49DB, 0x6DA217E7)
#define OV_ClassIdentifier_BoxBehaviorContext               OpenViBE::CIdentifier(0x4E1709E5, 0x38AADA6C)
// #define OV_ClassIdentifier_ScenarioImporter                 OpenViBE::CIdentifier(0x6BF28385, 0x77F8D9A1)
// #define OV_ClassIdentifier_ScenarioImporterDesc             OpenViBE::CIdentifier(0x05146C29, 0x283B5BB8)
// #define OV_ClassIdentifier_ScenarioExporter                 OpenViBE::CIdentifier(0x6C54A6C9, 0x174982CD)
// #define OV_ClassIdentifier_ScenarioExporterDesc             OpenViBE::CIdentifier(0x13B1BD52, 0x06AB7557)
#define OV_ClassIdentifier_BoxContext                       OpenViBE::CIdentifier(0x0B5DADFD, 0x6CC63125)
#define OV_ClassIdentifier_BoxContext_Input                 OpenViBE::CIdentifier(0x330306DD, 0x74A95F98)
#define OV_ClassIdentifier_BoxContext_InputChunk            OpenViBE::CIdentifier(0x4331378D, 0x5A1E0A69)
#define OV_ClassIdentifier_BoxContext_Output                OpenViBE::CIdentifier(0x34BBFD1D, 0x3132DA6F)
#define OV_ClassIdentifier_BoxContext_OutputChunk           OpenViBE::CIdentifier(0x691473DE, 0x2EE37ABD)
#define OV_ClassIdentifier_BoxContext_ConnectorBase         OpenViBE::CIdentifier(0x544A003E, 0x6DCBA5F6)
#define OV_ClassIdentifier_BoxProto                         OpenViBE::CIdentifier(0x169ED1C1, 0x639C5957)
// #define OV_ClassIdentifier_KernelContext                    OpenViBE::CIdentifier(0x1F8D3E14, 0x73A4DB1F)

//___________________________________________________________________//
//                                                                   //
// Unused Class Identifiers                                          //
//___________________________________________________________________//
//                                                                   //

/*
#define OV_ClassIdentifier_PluginInformation                OpenViBE::CIdentifier(0x1AFF3D5A, 0x77F610EE)
#define OV_ClassIdentifier_BoxInputProto                    OpenViBE::CIdentifier(0x39F81801, 0x2491BF81)
#define OV_ClassIdentifier_BoxOutputProto                   OpenViBE::CIdentifier(0x1649669C, 0x25B89AD7)
#define OV_ClassIdentifier_BoxSettingProto                  OpenViBE::CIdentifier(0x652D2F0B, 0x5D5E0714)

#define OV_ClassIdentifier_                                 OpenViBE::CIdentifier(0x38F6E10D, 0x6BA2721A)
#define OV_ClassIdentifier_                                 OpenViBE::CIdentifier(0x690ED3AC, 0x2D0702F0)
#define OV_ClassIdentifier_                                 OpenViBE::CIdentifier(0x110AAB4F, 0x157CBEF1)
#define OV_ClassIdentifier_                                 OpenViBE::CIdentifier(0x790A8CC5, 0x1439C776)
#define OV_ClassIdentifier_                                 OpenViBE::CIdentifier(0x64DCCE4E, 0x530CA785)
#define OV_ClassIdentifier_                                 OpenViBE::CIdentifier(0x3681EC07, 0x337DE51F)
#define OV_ClassIdentifier_                                 OpenViBE::CIdentifier(0x22990B50, 0x571010D4)
#define OV_ClassIdentifier_                                 OpenViBE::CIdentifier(0x67F31911, 0x3FA45C02)
#define OV_ClassIdentifier_                                 OpenViBE::CIdentifier(0x007DEEF9, 0x2F3E95C6)
#define OV_ClassIdentifier_                                 OpenViBE::CIdentifier(0x512A166F, 0x5C3EF83F)
*/

//___________________________________________________________________//
//                                                                   //
// Parameter Types                                                   //
//___________________________________________________________________//
//                                                                   //

#define OV_ParameterType_8BitsInteger                       OpenViBE::CIdentifier(0x4860ADC2, 0x7CCE64EB)
#define OV_ParameterType_16BitsInteger                      OpenViBE::CIdentifier(0x453AB1E8, 0x553CE76B)
#define OV_ParameterType_32BitsInteger                      OpenViBE::CIdentifier(0x780E5791, 0x067858DB)
#define OV_ParameterType_64BitsInteger                      OpenViBE::CIdentifier(0x58220E1F, 0x162AD73B)
#define OV_ParameterType_32BitsFloat                        OpenViBE::CIdentifier(0x40880ACD, 0x467E1705)
#define OV_ParameterType_64BitsFloat                        OpenViBE::CIdentifier(0x5C335AEE, 0x28F1F987)
#define OV_ParameterType_String                             OpenViBE::CIdentifier(0x731D6800, 0x77F99E1D)
/*
#define OV_ParameterType_   OpenViBE::CIdentifier(0x731D6800, 0x77F99E1D)
#define OV_ParameterType_   OpenViBE::CIdentifier(0x76F16D33, 0x0BBDFD09)
#define OV_ParameterType_   OpenViBE::CIdentifier(0x52F5C932, 0x7323C657)
#define OV_ParameterType_   OpenViBE::CIdentifier(0x04C00D59, 0x3EC2590C)
*/

//___________________________________________________________________//
//                                                                   //
// Operating System identification                                   //
//___________________________________________________________________//
//                                                                   //

#define OV_OS_Linux
// #define OV_OS_Windows
// #define OV_OS_MacOS
// #define OV_OS_

//___________________________________________________________________//
//                                                                   //
// Hardware Architecture identification                              //
//___________________________________________________________________//
//                                                                   //

#define OV_ARCHITECTURE_i386
// #define OV_ARCHITECTURE_

//___________________________________________________________________//
//                                                                   //
// Compilator software identification                                //
//___________________________________________________________________//
//                                                                   //

#define OV_COMPILATOR_GCC
// #define OV_COMPILATOR_VisualStudio
// #define OV_COMPILATOR_

//___________________________________________________________________//
//                                                                   //
// API Definitioni                                                   //
//___________________________________________________________________//
//                                                                   //

#ifdef OV_OS_Windows
 #ifdef OV_Exports
  #define OV_API __declspec(dllexport)
 #else
  #define OV_API __declspec(dllimport)
 #endif
#else
  #define OV_API
#endif

#endif // __OpenViBE_Defines_H__
