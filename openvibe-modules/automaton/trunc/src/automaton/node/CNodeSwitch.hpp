#ifndef __Automaton_CNodeSwitch_HPP__
#define __Automaton_CNodeSwitch_HPP__

#include "../INode.h"
#include "TNode.inl"
#include "../IAutomatonContext.h"

#include "../CIdentifier.h"

#include <string>
#include <vector>
#include <iostream>

#include <stdlib.h>
#include <time.h>

namespace Automaton
{
	class ISwitchConditionFunctor
	{
		public:
			virtual Automaton::uint64 operator()(IAutomatonContext*) = 0;
			virtual ~ISwitchConditionFunctor(){}
	};

	class CSwitchConditionProbability : virtual public ISwitchConditionFunctor
	{
		std::vector<Automaton::uint64> m_oProbability;

		public:
			CSwitchConditionProbability(std::vector<Automaton::uint64>& oBaseProbability)
			{
				Automaton::uint64 l_ui64Sum = 0;
				for(Automaton::uint64 i=0 ; i<oBaseProbability.size() ; i++)
				{
					l_ui64Sum += oBaseProbability[(size_t)i];
					m_oProbability.push_back(l_ui64Sum);
				}
			}

			virtual Automaton::uint64 operator()(IAutomatonContext*)
			{

				Automaton::uint64 l_ui64Max = m_oProbability.back();
				Automaton::uint64 l_ui64Number = rand() % l_ui64Max;
				Automaton::boolean l_bFound = false;

				uint64 i;
				for(i=0 ; i<m_oProbability.size() && !l_bFound; i++)
				{
					if(l_ui64Number<m_oProbability[(size_t)i])
					{
						l_bFound=true;
					}
				}

				return i-1;
			}

			virtual ~CSwitchConditionProbability(){}
	};

	class CNodeSwitch : virtual public TNode<INode>
	{
		protected:
			ISwitchConditionFunctor * m_pCondition;

			Automaton::boolean m_bContinue;
			Automaton::uint64 m_ui64BranchNumber;

		public:
			CNodeSwitch() : m_pCondition(NULL), m_bContinue(false), m_ui64BranchNumber(0) {}

			virtual ~CNodeSwitch(){ delete m_pCondition; }

			virtual Automaton::boolean process(IAutomatonContext*);
			virtual Automaton::CIdentifier getSuccessor(IAutomatonContext*);

			virtual void setSwitchCondition(const std::string& oCondition, std::vector<Automaton::CIdentifier>& oParameter, Automaton::IAutomatonContext* pContext);
	};


};

#endif

