#include "CXMLParameterReader.hpp"

using namespace std;
using namespace Automaton;


EParsingStatus CXMLParameterReader::openChild(const char* sName, const char** sAttributeName, const char** sAttributeValue, uint64 ui64AttributeCount, IAutomatonContext* pContext)
{
	m_oParameterType = getAttributeValue(sAttributeName, sAttributeValue, ui64AttributeCount, "class");

	if(m_oParameterType == "IValue" || m_oParameterType == "IVariable")
	{
		m_eStatus = ParsingStatus_Parameter;

	}
	else
	{
		cout<<"Couldn't find valid class attribute in parameter node"<<endl;
		m_eStatus = ParsingStatus_Error;
	}

	return m_eStatus;
}

EParsingStatus CXMLParameterReader::processChildData(const char* sData, IAutomatonContext* pContext)
{
	if(m_oParameterType=="IValue")
	{
		//for now, uint64 value
		istringstream l_oStringData(sData);
		uint64 l_ui64ReadValue;

		while(l_oStringData>>l_ui64ReadValue)
		{
			m_oParameter.push_back(pContext->addParameter(NULL,l_ui64ReadValue));
		}
	}
	else if(m_oParameterType=="IVariable")
	{
		CIdentifier l_oVariableIdentifier = pContext->getVariableIdentifier(sData);

		if(l_oVariableIdentifier == CIdentifier(0))
		{
			cout<<"Variable used wasn't declared! Incorrect variable : "<<sData<<endl;
			m_eStatus = ParsingStatus_Error;
		}
		else
		{
			m_oParameter.push_back(l_oVariableIdentifier);
		}
	}

	return m_eStatus;
}

EParsingStatus CXMLParameterReader::closeChild(IAutomatonContext* pContext)
{
	return ParsingStatus_Property;
}


