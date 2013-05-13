#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

//___________________________________________________________________//
//                                                                   //
//                                                                   //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_MouseControl                            OpenViBE::CIdentifier(0xDA4B4EEB, 0x64FC6A16)
#define OVP_ClassId_MouseControlDesc                        OpenViBE::CIdentifier(0xB6B65C98, 0xA756ED0E)

#define OVP_ClassId_BoxAlgorithm_LatencyEvaluation          OpenViBE::CIdentifier(0x0AD11EC1, 0x7EF3690B)
#define OVP_ClassId_BoxAlgorithm_LatencyEvaluationDesc      OpenViBE::CIdentifier(0x5DB56A54, 0x5380262B)

//___________________________________________________________________//
//                                                                   //
// Global defines                                                   //
//___________________________________________________________________//
//                                                                   //

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines
 #include "ovp_global_defines.h"
#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines

//___________________________________________________________________//


#endif // __OpenViBEPlugins_Defines_H__
