#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

#include <ov_common_defines.h>

//___________________________________________________________________//
//                                                                   //
// Plugin identifiers                                                //
//___________________________________________________________________//
//                                                                   //
#define OVP_ClassId_BoxAlgorithm_Python                     OpenViBE::CIdentifier(0x5DC4F669, 0xD3FD4D64)
#define OVP_ClassId_BoxAlgorithm_PythonDesc                 OpenViBE::CIdentifier(0x404B6FFD, 0x12BDD429)
//___________________________________________________________________//
//                                                                   //
// Gloabal defines                                                   //
//___________________________________________________________________//
//                                                                   //

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines
 #include "ovp_global_defines.h"
#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines


#endif // __OpenViBEPlugins_Defines_H__
