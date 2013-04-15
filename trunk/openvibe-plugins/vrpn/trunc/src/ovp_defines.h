#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

//___________________________________________________________________//
//                                                                   //
// Plugin Object Descriptor Class Identifiers                        //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_VRPNAnalogServerDesc                             OpenViBE::CIdentifier(0xF54B8E03, 0xAAFF15C6)
#define OVP_ClassId_VRPNButtonServerDesc                             OpenViBE::CIdentifier(0xBC86F256, 0x002495EF)

//___________________________________________________________________//
//                                                                   //
// Plugin Object Class Identifiers                                   //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_VRPNAnalogServer                                 OpenViBE::CIdentifier(0x0DDC3A7E, 0x6F6E6401)
#define OVP_ClassId_VRPNButtonServer                                 OpenViBE::CIdentifier(0x0E382E6F, 0x5BE1F00C)

//___________________________________________________________________//
//                                                                   //
// Global defines                                                   //
//___________________________________________________________________//
//                                                                   //

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines
 #include "ovp_global_defines.h"
#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines


#endif // __OpenViBEPlugins_Defines_H__
