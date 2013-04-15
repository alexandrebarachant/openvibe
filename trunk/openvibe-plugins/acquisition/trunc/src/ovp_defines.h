#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

//___________________________________________________________________//
//                                                                   //
// Box algorithms                                                    //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_BoxAlgorithm_AcquisitionClient          OpenViBE::CIdentifier(0x35D225CB, 0x3E6E3A5F)
#define OVP_ClassId_BoxAlgorithm_AcquisitionClientDesc      OpenViBE::CIdentifier(0x7D3061B9, 0x43565E8C)

#define OVP_ClassId_GenericNetworkAcquisition               OpenViBE::CIdentifier(0x5D5CA50E, 0x7486E783)
#define OVP_ClassId_GenericNetworkAcquisitionDesc           OpenViBE::CIdentifier(0x390236CA, 0x3255E115)

//___________________________________________________________________//
//                                                                   //
// Global defines                                                   //
//___________________________________________________________________//
//                                                                   //

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines
 #include "ovp_global_defines.h"
#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines



#endif // __OpenViBEPlugins_Defines_H__
