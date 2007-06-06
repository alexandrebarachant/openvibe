#include "CNodeSwitch.hpp"

using namespace Automaton;
using namespace std;

Automaton::boolean CNodeSwitch::process(IAutomatonContext* pContext)
{
	m_ui64BranchNumber = (*m_pCondition)(pContext);
	return true;
}

Automaton::CIdentifier CNodeSwitch::getSuccessor(IAutomatonContext* pContext)
{
	if(m_bContinue)
	{
#ifdef DEBUG
		cout<<"Quit switch"<<endl;
#endif
		m_bContinue = false;
		return TNode<INode>::m_oSuccessor[TNode<INode>::m_oSuccessor.size()-1];
	}
	else
	{
#ifdef DEBUG
		cout<<"Enter branch"<<m_ui64BranchNumber<<endl;
#endif
		m_bContinue = true;
		return TNode<INode>::m_oSuccessor[(size_t)m_ui64BranchNumber];
	}

}

void CNodeSwitch::setSwitchCondition(const std::string& oCondition, std::vector<Automaton::CIdentifier>& oParameter, Automaton::IAutomatonContext * pContext)
{
	std::vector<Automaton::uint64> l_oProbability;
	l_oProbability.resize(oParameter.size());

	for(uint32 i=0 ; i<oParameter.size() ; i++)
	{
		l_oProbability[i] = pContext->getParameterValue(oParameter[i]);
	}

	if(oCondition == "Probability")
	{
		m_pCondition = new CSwitchConditionProbability(l_oProbability);
	}
	else
	{

	}
}

