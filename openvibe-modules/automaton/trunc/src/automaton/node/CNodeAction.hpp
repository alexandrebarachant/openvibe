#ifndef __Automaton_CNodeAction_H__
#define __Automaton_CNodeAction_H__

#include "../INode.h"
#include "TNode.inl"
#include "../CIdentifier.h"
#include "../IAutomatonContext.h"

#include <vector>
#include <string>
#include <algorithm>

#include <iostream>

namespace Automaton
{
	class IActionFunctor
	{
		public:
			virtual Automaton::boolean operator()(IAutomatonContext*) = 0;
			virtual ~IActionFunctor(){}
	};

	class CSendEvent : virtual public IActionFunctor
	{
		private:
			Automaton::CIdentifier m_oEvent;

		public:
			CSendEvent::CSendEvent(Automaton::uint64 ui64Event)
				: m_oEvent(ui64Event) {}

			virtual ~CSendEvent(){}

			virtual Automaton::boolean operator()(IAutomatonContext* pContext)
			{
#ifdef DEBUG
				std::cout<<"<<<<<<<<<<<<<<sending event : "<<std::dec<<(uint64)m_oEvent<<std::endl;
#endif
				pContext->sendEvent(m_oEvent);
				
				return true;
			}
	};

	class CWaitEvent : virtual public IActionFunctor
	{
		private:
			Automaton::CIdentifier m_oEvent;

		public:
			CWaitEvent::CWaitEvent(Automaton::uint64 ui64Event)
				: m_oEvent(ui64Event) {}

			virtual ~CWaitEvent(){}

			virtual Automaton::boolean operator()(IAutomatonContext* pContext)
			{
#ifdef DEBUG
				std::cout<<"waiting for event : "<<std::dec<<(uint64)m_oEvent<<std::endl;
#endif
				const CIdentifier* l_pReceivedEvents = pContext->getReceivedEvents();

				if(l_pReceivedEvents)
				{
					const CIdentifier* l_pEnd = l_pReceivedEvents + pContext->getReceivedEventsCount();
					return std::find(l_pReceivedEvents, l_pEnd, m_oEvent) != l_pEnd;
				}
				return false;
			}
	};

	class CWaitTime : virtual public IActionFunctor
	{
		private:
			Automaton::uint64 m_ui64TimeLength;
			Automaton::uint64 m_ui64EndTime;
			Automaton::boolean m_bTimeSet;
		public:
			CWaitTime::CWaitTime(Automaton::uint64 ui64TimeLength) : 
	       			m_bTimeSet(false) 
			{
				uint64 l_ui64Temp = ((uint64)(ui64TimeLength%1000)<<32);

				//converts it to 32:32 format 
				m_ui64TimeLength = (( ui64TimeLength/1000) << 32) +
						(l_ui64Temp/1000);

			}

			//TODO constructor with two arguments, to randomize between two value the wait duration

			virtual ~CWaitTime(){}

			virtual Automaton::boolean operator()(IAutomatonContext* pContext)
			{
				if(!m_bTimeSet)
				{
					m_ui64EndTime = pContext->getCurrentTime() + m_ui64TimeLength;
					m_bTimeSet = true;
				}
				else
				{
					if(pContext->getCurrentTime() >= m_ui64EndTime)
					{
						//the next time we go through this node, the timer will be reset
						m_bTimeSet=false;

						return true;
					}
				}

				return false;
			}

	};

	class CNodeAction : public virtual TNode<INode>
	{
		IActionFunctor * m_pAction; 
	
		public:
			CNodeAction() : m_pAction(NULL) {}
		
			virtual ~CNodeAction(){ delete m_pAction; }

			virtual Automaton::boolean process(IAutomatonContext * pContext);	

			virtual Automaton::CIdentifier getSuccessor(IAutomatonContext * pContext)
			{
				if(TNode<INode>::m_oSuccessor.size() == 0)
				{
					return Automaton_No_Successor;
				}
				else
				{
					return TNode<INode>::m_oSuccessor[0];
				}
			}

			virtual Automaton::boolean setAction(const std::string& oAction, const std::vector<Automaton::CIdentifier>& oParameter, IAutomatonContext * pContext);

	};

};

#endif

