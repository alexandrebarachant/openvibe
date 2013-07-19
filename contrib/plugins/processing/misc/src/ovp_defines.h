
#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

// @BEGIN inserm
#define OVP_ClassId_MouseControl                                                       OpenViBE::CIdentifier(0xDA4B4EEB, 0x64FC6A16)
#define OVP_ClassId_MouseControlDesc                                                   OpenViBE::CIdentifier(0xB6B65C98, 0xA756ED0E)
// @END inserm

// @BEGIN multiple authors
#define OVP_ClassId_BoxAlgorithm_SoundPlayer                  OpenViBE::CIdentifier(0x18D06E9F, 0x68D43C23)
#define OVP_ClassId_BoxAlgorithm_SoundPlayerDesc              OpenViBE::CIdentifier(0x246E5EC4, 0x127D21AA)
// @END multiple authors

//___________________________________________________________________//
//                                                                   //
// Global defines                                                   //
//___________________________________________________________________//
//                                                                   //

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines
 #include "ovp_global_defines.h"
#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines


#endif // __OpenViBEPlugins_Defines_H__
