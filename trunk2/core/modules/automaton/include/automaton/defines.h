#ifndef __Automaton_Defines_H__
#define __Automaton_Defines_H__

#include <ov_common_defines.h>

#define OV_APPEND_TO_NAMESPACE Automaton
#include <ov_common_types.h>
#undef OV_APPEND_TO_NAMESPACE


//___________________________________________________________________//
//                                                                   //
// Node type identifiers definitions                                 //
//___________________________________________________________________//
//                                                                   //


#define Automaton_Node_Identifier_Automaton         Automaton::CIdentifier(0x00000000)
#define Automaton_Node_Identifier_No_Node           Automaton::CIdentifier(0x00000001)
#define Automaton_Node_Identifier_Action            Automaton::CIdentifier(0x00000010)
#define Automaton_Node_Identifier_ActionSendEvent   Automaton::CIdentifier(0x00000011)
#define Automaton_Node_Identifier_ActionWaitEvent   Automaton::CIdentifier(0x00000012)
#define Automaton_Node_Identifier_ActionWaitTime    Automaton::CIdentifier(0x00000013)

#define Automaton_Node_Identifier_Loop              Automaton::CIdentifier(0x00000021)
#define Automaton_Node_Identifier_LoopFinite        Automaton::CIdentifier(0x00000022)
#define Automaton_Node_Identifier_LoopInfinite      Automaton::CIdentifier(0x00000023)
#define Automaton_Node_Identifier_LoopWaitEvent     Automaton::CIdentifier(0x00000024)

#define Automaton_Node_Identifier_Switch            Automaton::CIdentifier(0x00000031)
#define Automaton_Node_Identifier_SwitchProbabilist Automaton::CIdentifier(0x00000032)
#define Automaton_Node_Identifier_SwitchEvent       Automaton::CIdentifier(0x00000033)

#define Automaton_Node_Identifier_RandomOrder       Automaton::CIdentifier(0x00000041)

#define Automaton_No_Successor                      Automaton::CIdentifier(0xFFFFFFFF)


#endif // __Automaton_Defines_H__
