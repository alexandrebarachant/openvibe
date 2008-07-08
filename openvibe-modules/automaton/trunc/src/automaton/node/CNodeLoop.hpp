#ifndef __Automaton_CNodeFiniteIterationLoop_HPP__
#define __Automaton_CNodeFiniteIterationLoop_HPP__

#include "../INode.h"
#include "TNode.inl"
#include "TNodeLoop.inl"
#include "../IAutomatonContext.h"

#include "../CIdentifier.h"

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

namespace Automaton
{
	class ILoopConditionFunctor
	{
		public:
			virtual void reset() {}
			virtual Automaton::boolean operator()(IAutomatonContext*) = 0;
			virtual ~ILoopConditionFunctor(){}
	};

	class CLoopConditionInfinite : public ILoopConditionFunctor
	{
		public:
			virtual ~CLoopConditionInfinite() {}

			virtual Automaton::boolean operator()(IAutomatonContext*)
			{
				return false;
			}
	};

	class CLoopConditionFinite : public ILoopConditionFunctor
	{
		Automaton::uint64 m_ui64Iteration;
		Automaton::uint64 m_ui64BaseIteration;

		public:
			CLoopConditionFinite(Automaton::uint64 ui64Iteration)
				: m_ui64Iteration(ui64Iteration),
				m_ui64BaseIteration(ui64Iteration) {}

			virtual void reset(){ m_ui64Iteration = m_ui64BaseIteration; }

			virtual ~CLoopConditionFinite() {}

			virtual Automaton::boolean operator()(IAutomatonContext*)
			{
				return (m_ui64Iteration--==0);
			}

	};

	class CLoopConditionEvent : public ILoopConditionFunctor
	{
		Automaton::CIdentifier m_oEvent;

		public:
			CLoopConditionEvent(Automaton::uint64 ui64Event)
				: m_oEvent(ui64Event) {}

			virtual ~CLoopConditionEvent() {}

			virtual Automaton::boolean operator()(IAutomatonContext* pContext)
			{
				const CIdentifier* l_pReceivedEvents = pContext->getReceivedEvents();

				if(l_pReceivedEvents)
				{
					const CIdentifier* l_pEnd = l_pReceivedEvents + pContext->getReceivedEventsCount();
					return std::find(l_pReceivedEvents, l_pEnd, m_oEvent) != l_pEnd;
				}
				return false;
			}

	};

	class CNodeLoop : public TNodeLoop<INode>
	{
		protected:
			ILoopConditionFunctor * m_pLoopCondition;
			Automaton::boolean m_bComplete;

		public:
			CNodeLoop() : m_pLoopCondition(NULL), m_bComplete(false) {}

			virtual ~CNodeLoop(){ delete m_pLoopCondition; }

			virtual Automaton::boolean process(IAutomatonContext*);
			virtual Automaton::CIdentifier getSuccessor(IAutomatonContext*);

			virtual Automaton::boolean setLoopCondition(const std::string& oCondition, const std::vector<Automaton::CIdentifier>& oParameter, IAutomatonContext * pContext);

	};
};

#endif

