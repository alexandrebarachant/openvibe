#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

//___________________________________________________________________//
//                                                                   //
//                                                                   //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_BoxAlgorithm_StimulationMultiplexer       OpenViBE::CIdentifier(0x07DB4EFA, 0x472B0938)
#define OVP_ClassId_BoxAlgorithm_StimulationMultiplexerDesc   OpenViBE::CIdentifier(0x79EF4E4D, 0x178F09E6)

#define OVP_ClassId_BoxAlgorithm_SoundPlayer                  OpenViBE::CIdentifier(0x18D06E9F, 0x68D43C23)
#define OVP_ClassId_BoxAlgorithm_SoundPlayerDesc              OpenViBE::CIdentifier(0x246E5EC4, 0x127D21AA)

#define OVP_ClassId_BoxAlgorithm_StimulationVoter                      OpenViBE::CIdentifier(0x2BBD61FC, 0x041A4EDB)
#define OVP_ClassId_BoxAlgorithm_StimulationVoterDesc                  OpenViBE::CIdentifier(0x1C36287C, 0x6F143FBF)
#define OVP_TypeId_Voting_ClearVotes                                   OpenViBE::CIdentifier(0x17AE30F8, 0x40B57661)
#define OVP_TypeId_Voting_ClearVotes_AfterOutput                       OpenViBE::CIdentifier(0x7FA81A20, 0x484023F9)
#define OVP_TypeId_Voting_ClearVotes_WhenExpires                       OpenViBE::CIdentifier(0x02766639, 0x00B155B4)
#define OVP_TypeId_Voting_OutputTime                                   OpenViBE::CIdentifier(0x48583E8F, 0x47F22462)
#define OVP_TypeId_Voting_OutputTime_Vote                              OpenViBE::CIdentifier(0x2F37507F, 0x00C06761)
#define OVP_TypeId_Voting_OutputTime_Winner                            OpenViBE::CIdentifier(0x72416689, 0x17673658)
#define OVP_TypeId_Voting_OutputTime_Last                              OpenViBE::CIdentifier(0x4F2830DB, 0x716C2930)
#define OVP_TypeId_Voting_RejectClass_CanWin                                 OpenViBE::CIdentifier(0x442F2F14, 0x7A17336C)
#define OVP_TypeId_Voting_RejectClass_CanWin_Yes                             OpenViBE::CIdentifier(0x40011974, 0x54BB3C71)
#define OVP_TypeId_Voting_RejectClass_CanWin_No                              OpenViBE::CIdentifier(0x275B746A, 0x480B302C)

//___________________________________________________________________//
//                                                                   //
// Plugin Object Descriptor Class Identifiers                        //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_XMLStimulationScenarioPlayerDesc          OpenViBE::CIdentifier(0x00D846C8, 0x264AACC9)
#define OVP_ClassId_KeyboardStimulatorDesc                    OpenViBE::CIdentifier(0x00E51ACD, 0x284CA2CF)
#define OVP_ClassId_BoxAlgorithm_P300IdentifierStimulatorDesc OpenViBE::CIdentifier(0x00F20CA,0x2A4EA9C3)
#define OVP_ClassId_SignChangeDetectorDesc                    OpenViBE::CIdentifier(0x798ACD86, 0xEF1287A4)

//___________________________________________________________________//
//                                                                   //
// Plugin Object Class Identifiers                                   //
//___________________________________________________________________//
//

#define OVP_ClassId_XMLStimulationScenarioPlayer              OpenViBE::CIdentifier(0x00646136, 0x44853D76)
#define OVP_ClassId_KeyboardStimulator                        OpenViBE::CIdentifier(0x00D317B9, 0x6324C3FF)
#define OVP_ClassId_BoxAlgorithm_P300IdentifierStimulator     OpenViBE::CIdentifier(0x00F27FDB,0x8203D1A5)
#define OVP_ClassId_SignChangeDetector                        OpenViBE::CIdentifier(0x4FA78CD, 0xAFE45DE7)



//___________________________________________________________________//
//                                                                   //
// Global defines                                                   //
//___________________________________________________________________//
//                                                                   //

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines
 #include "ovp_global_defines.h"
#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines


#endif // __OpenViBEPlugins_Defines_H__
