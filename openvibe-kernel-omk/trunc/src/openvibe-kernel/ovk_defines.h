#ifndef __OpenViBEKernel_Defines_H__
#define __OpenViBEKernel_Defines_H__

//___________________________________________________________________//
//                                                                   //
// Global Class Identifiers                                          //
//___________________________________________________________________//
//                                                                   //

#define OVK_ClassId_KernelDesc                               OpenViBE::CIdentifier(0x02F2D050, 0x25CD9EE5)

//___________________________________________________________________//
//                                                                   //
// Kernel Class Identifiers                                          //
//___________________________________________________________________//
//                                                                   //

#define OVK_ClassId_Kernel_Kernel                            OpenViBE::CIdentifier(0x02F2D050, 0x25CD9EE5)
#define OVK_ClassId_Kernel_KernelObject                      OpenViBE::CIdentifier(0x672AE465, 0x60CB46D8)
#define OVK_ClassId_Kernel_KernelObjectFactory               OpenViBE::CIdentifier(0x7D380DFA, 0x1B33AE2F)
#define OVK_ClassId_Kernel_KernelContext                     OpenViBE::CIdentifier(0x72D4050C, 0x543DDAD8)
#define OVK_ClassId_Kernel_TypeManager                       OpenViBE::CIdentifier(0x11D27788, 0x0602030A)

#define OVK_ClassId_Kernel_Log_LogManager                    OpenViBE::CIdentifier(0x1FE5CF01, 0x0B2B3748)
#define OVK_ClassId_Kernel_Log_LogListenerConsole            OpenViBE::CIdentifier(0x1EEB056C, 0x373C17C7)
#define OVK_ClassId_Kernel_Log_LogListenerFile               OpenViBE::CIdentifier(0x6A7FCB0C, 0xC847976D)
#define OVK_ClassId_Kernel_Log_LogListenerDebug              OpenViBE::CIdentifier(0x45C1A72F, 0x647DE336)
#define OVK_ClassId_Kernel_Log_LogListenerNull               OpenViBE::CIdentifier(0x1341E7E6, 0x2232EB75)

#define OVK_ClassId_Kernel_Plugins_PluginManager             OpenViBE::CIdentifier(0x07DB32AD, 0x76FAD392)
#define OVK_ClassId_Kernel_Plugins_PluginModule              OpenViBE::CIdentifier(0x34883FDB, 0x78B6D1E6)
#define OVK_ClassId_Kernel_Plugins_PluginModuleContext       OpenViBE::CIdentifier(0x78071145, 0x308261A2)
#define OVK_ClassId_Kernel_Plugins_PluginObject              OpenViBE::CIdentifier(0x718F6C0B, 0x368B2E6C)
#define OVK_ClassId_Kernel_Plugins_PluginObjectDesc          OpenViBE::CIdentifier(0x65853ECE, 0x6E203E71)

#define OVK_ClassId_Kernel_Player_BoxAlgorithmContext        OpenViBE::CIdentifier(0x48078FB2, 0x1897AD5A)
#define OVK_ClassId_Kernel_Player_DynamicBoxContext          OpenViBE::CIdentifier(0x18FC58E0, 0x505E97CD)
#define OVK_ClassId_Kernel_Player_Message                    OpenViBE::CIdentifier(0x42A56E80, 0x289CBB09)
#define OVK_ClassId_Kernel_Player_MessageClock               OpenViBE::CIdentifier(0x3AA34472, 0x2B1F67CB)
#define OVK_ClassId_Kernel_Player_MessageEvent               OpenViBE::CIdentifier(0x036401EF, 0x31E90C38)
#define OVK_ClassId_Kernel_Player_MessageSignal              OpenViBE::CIdentifier(0x3BB1DDD9, 0x0B289F8D)
#define OVK_ClassId_Kernel_Player_Player                     OpenViBE::CIdentifier(0x7C8777FF, 0x52CE89C2)
#define OVK_ClassId_Kernel_Player_PlayerContext              OpenViBE::CIdentifier(0x76A544A0, 0x01F508ED)
#define OVK_ClassId_Kernel_Player_PlayerManager              OpenViBE::CIdentifier(0x4A46A96D, 0x6D4957C5)
#define OVK_ClassId_Kernel_Player_SimulatedBox               OpenViBE::CIdentifier(0x74D37E85, 0x73952B1D)
#define OVK_ClassId_Kernel_Player_StaticBoxContext           OpenViBE::CIdentifier(0x59B7AE07, 0x4A6FE704)

#define OVK_ClassId_Kernel_Scenario_Attributable             OpenViBE::CIdentifier(0x6F4071A4, 0x31183474)
#define OVK_ClassId_Kernel_Scenario_Box                      OpenViBE::CIdentifier(0x715A25DC, 0x5322E94C)
#define OVK_ClassId_Kernel_Scenario_BoxProto                 OpenViBE::CIdentifier(0x290EDACD, 0x6D08F87D)
#define OVK_ClassId_Kernel_Scenario_Link                     OpenViBE::CIdentifier(0x118EF337, 0x095F5A5E)
#define OVK_ClassId_Kernel_Scenario_ProcessingUnit           OpenViBE::CIdentifier(0x3C581F28, 0x5A5E9C6B)
#define OVK_ClassId_Kernel_Scenario_Scenario                 OpenViBE::CIdentifier(0x4215A6BF, 0x6F4E5F96)
#define OVK_ClassId_Kernel_Scenario_ScenarioManager          OpenViBE::CIdentifier(0x5BB96551, 0x133303F9)
#define OVK_ClassId_Kernel_Scenario_ScenarioExporterContext  OpenViBE::CIdentifier(0x78606B83, 0x5EB11EC9)
#define OVK_ClassId_Kernel_Scenario_ScenarioImporterContext  OpenViBE::CIdentifier(0x17DC0F51, 0x795A3A7E)

#define OVK_ClassId_Kernel_Visualisation_VisualisationContext OpenViBE::CIdentifier(0x2C416B59, 0x312B2571)
#define OVK_ClassId_Kernel_Visualisation_VisualisationManager OpenViBE::CIdentifier(0xDEF8F7C4, 0x3501D90B)
#define OVK_ClassId_Kernel_Visualisation_VisualisationWidget  OpenViBE::CIdentifier(0x08E926D6, 0xEFF6BB75)
#define OVK_ClassId_Kernel_Visualisation_VisualisationTree    OpenViBE::CIdentifier(0xE02A7E73, 0xB2927EC4)

//___________________________________________________________________//
//                                                                   //
// Tools Class Identifiers                                           //
//___________________________________________________________________//
//                                                                   //

#define OVK_ClassId_Tools_ScopeTester                        OpenViBE::CIdentifier(0x5978C732, 0x5903C7A2)
#define OVK_ClassId_Tools_KernelObjectFactoryHelper          OpenViBE::CIdentifier(0x6059EA7C, 0x081FDAB5)
#define OVK_ClassId_Tools_BoxProxy                           OpenViBE::CIdentifier(0x4DFDFCE4, 0x49EF078E)
#define OVK_ClassId_Tools_BoxIOProxy                         OpenViBE::CIdentifier(0x25476613, 0x331633DA)

//___________________________________________________________________//
//                                                                   //
// Operating System identification                                   //
//___________________________________________________________________//
//                                                                   //

// #define OVK_OS_Linux
// #define OVK_OS_Windows
// #define OVK_OS_MacOS
// #define OVK_OS_

#if defined TARGET_OS_Windows
 #define OVK_OS_Windows
#elif defined TARGET_OS_Linux
 #define OVK_OS_Linux
#elif defined TARGET_OS_MacOS
 #define OVK_OS_MacOS
#else
 #warning "No target operating system defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Hardware Architecture identification                              //
//___________________________________________________________________//
//                                                                   //

// #define OVK_ARCHITECTURE_i386
// #define OVK_ARCHITECTURE_

#if defined TARGET_ARCHITECTURE_i386
 #define OVK_ARCHITECTURE_i386
#else
 #warning "No target architecture defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Compilator software identification                                //
//___________________________________________________________________//
//                                                                   //

// #define OVK_COMPILATOR_GCC
// #define OVK_COMPILATOR_VisualStudio
// #define OVK_COMPILATOR_

#if defined TARGET_COMPILATOR_GCC
 #define OVK_COMPILATOR_GCC
#elif defined TARGET_COMPILATOR_VisualStudio
 #define OVK_COMPILATOR_VisualStudio
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
#if defined OVK_Shared
 #if defined OVK_OS_Windows
  #define OVK_API_Export __declspec(dllexport)
  #define OVK_API_Import __declspec(dllimport)
 #elif defined OVK_OS_Linux
  #define OVK_API_Export __attribute__((visibility("default")))
  #define OVK_API_Import __attribute__((visibility("default")))
 #else
  #define OVK_API_Export
  #define OVK_API_Import
 #endif
#else
 #define OVK_API_Export
 #define OVK_API_Import
#endif

#if defined OVK_Exports
 #define OVK_API OVK_API_Export
#else
 #define OVK_API OVK_API_Import
#endif

//___________________________________________________________________//
//                                                                   //
// NULL Definition                                                   //
//___________________________________________________________________//
//                                                                   //

#ifndef NULL
#define NULL 0
#endif

#endif // __OpenViBEKernel_Defines_H__
