#include "ovkCConfigurationManager.h"

#include <stack>

#include <stdlib.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

CConfigurationManager::CConfigurationManager(const IKernelContext& rKernelContext)
	:TKernelObject<IConfigurationManager>(rKernelContext)
{
}

boolean CConfigurationManager::addConfigurationFromFile(
	const CString& rFileNameWildCard)
{
	// TODO
	return false;
}

// ----------------------------------------------------------------------------------------------------------------------------
//
CIdentifier CConfigurationManager::createConfigurationToken(
	const CString& rConfigurationTokenName,
	const CString& rConfigurationTokenValue)
{
	if(this->lookUpConfigurationTokenIdentifier(rConfigurationTokenName)!=OV_UndefinedIdentifier)
	{
		this->getLogManager() << LogLevel_Warning << "Configuration token name " << rConfigurationTokenName << " already exists\n";
		return false;
	}

	CIdentifier l_oIdentifier=this->getUnusedIdentifier();
	m_vConfigurationToken[l_oIdentifier].m_sConfigurationName=rConfigurationTokenName;
	m_vConfigurationToken[l_oIdentifier].m_sConfigurationValue=rConfigurationTokenValue;
	return l_oIdentifier;
}

boolean CConfigurationManager::releaseConfigurationToken(
	const CIdentifier& rConfigurationTokenIdentifier)
{
	std::map < CIdentifier, SConfigurationToken >::iterator itConfigurationToken=m_vConfigurationToken.find(rConfigurationTokenIdentifier);
	if(itConfigurationToken==m_vConfigurationToken.end())
	{
		this->getLogManager() << LogLevel_Warning << "Configuration token not found " << rConfigurationTokenIdentifier << "\n";
		return false;
	}
	m_vConfigurationToken.erase(itConfigurationToken);
	return true;
}

CIdentifier CConfigurationManager::getNextConfigurationTokenIdentifier(
	const CIdentifier& rPreviousConfigurationTokenIdentifier) const
{
	std::map < CIdentifier, SConfigurationToken >::const_iterator itConfigurationToken;

	if(rPreviousConfigurationTokenIdentifier==OV_UndefinedIdentifier)
	{
		itConfigurationToken=m_vConfigurationToken.begin();
	}
	else
	{
		itConfigurationToken=m_vConfigurationToken.find(rPreviousConfigurationTokenIdentifier);
		if(itConfigurationToken==m_vConfigurationToken.end())
		{
			return OV_UndefinedIdentifier;
		}
		itConfigurationToken++;
	}

	return itConfigurationToken!=m_vConfigurationToken.end()?itConfigurationToken->first:OV_UndefinedIdentifier;
}

// ----------------------------------------------------------------------------------------------------------------------------

CString CConfigurationManager::getConfigurationTokenName(
	const CIdentifier& rConfigurationTokenIdentifier) const
{
	std::map < CIdentifier, SConfigurationToken >::const_iterator itConfigurationToken=m_vConfigurationToken.find(rConfigurationTokenIdentifier);
	if(itConfigurationToken!=m_vConfigurationToken.end())
	{
		return itConfigurationToken->second.m_sConfigurationName;
	}
	return "";
}

CString CConfigurationManager::getConfigurationTokenValue(
	const CIdentifier& rConfigurationTokenIdentifier) const
{
	std::map < CIdentifier, SConfigurationToken >::const_iterator itConfigurationToken=m_vConfigurationToken.find(rConfigurationTokenIdentifier);
	if(itConfigurationToken!=m_vConfigurationToken.end())
	{
		return itConfigurationToken->second.m_sConfigurationValue;
	}
	return "";
}

// ----------------------------------------------------------------------------------------------------------------------------

boolean CConfigurationManager::setConfigurationTokenName(
	const CIdentifier& rConfigurationTokenIdentifier,
	const CString& rConfigurationTokenName)
{
	if(this->lookUpConfigurationTokenIdentifier(rConfigurationTokenName)!=OV_UndefinedIdentifier)
	{
		getLogManager() << LogLevel_Warning << "Configuration token " << rConfigurationTokenName << " already exists\n";
		return false;
	}

	std::map < CIdentifier, SConfigurationToken >::iterator itConfigurationToken=m_vConfigurationToken.find(rConfigurationTokenIdentifier);
	if(itConfigurationToken==m_vConfigurationToken.end())
	{
		getLogManager() << LogLevel_Warning << "Configuration token " << rConfigurationTokenIdentifier << " does not exist\n";
		return false;
	}

	itConfigurationToken->second.m_sConfigurationName=rConfigurationTokenName;
	return true;
}

boolean CConfigurationManager::setConfigurationTokenValue(
	const CIdentifier& rConfigurationTokenIdentifier,
	const CString& rConfigurationTokenValue)
{
	std::map < CIdentifier, SConfigurationToken >::iterator itConfigurationToken=m_vConfigurationToken.find(rConfigurationTokenIdentifier);
	if(itConfigurationToken==m_vConfigurationToken.end())
	{
		getLogManager() << LogLevel_Warning << "Configuration token " << rConfigurationTokenIdentifier << " does not exist\n";
		return false;
	}

	itConfigurationToken->second.m_sConfigurationValue=rConfigurationTokenValue;
	return true;
}

// ----------------------------------------------------------------------------------------------------------------------------

CIdentifier CConfigurationManager::lookUpConfigurationTokenIdentifier(
	const CString& rConfigurationTokenName) const
{
	std::map < CIdentifier, SConfigurationToken >::const_iterator itConfigurationToken=m_vConfigurationToken.begin();
	while(itConfigurationToken!=m_vConfigurationToken.end())
	{
		if(itConfigurationToken->second.m_sConfigurationName==rConfigurationTokenName)
		{
			return itConfigurationToken->first;
		}
		itConfigurationToken++;
	}
	return OV_UndefinedIdentifier;
}

// ----------------------------------------------------------------------------------------------------------------------------

CString CConfigurationManager::expandExpression(
	const CString& rExpression) const
{
	std::string l_sValue(rExpression.toASCIIString());
	std::string l_sResult;
	if(this->expand(l_sValue, l_sResult))
	{
		return l_sResult.c_str();
	}
	return l_sValue.c_str();
}

// ----------------------------------------------------------------------------------------------------------------------------

CIdentifier CConfigurationManager::getUnusedIdentifier(void) const
{
	uint64 l_ui64Identifier=(((uint64)rand())<<32)+((uint64)rand());
	CIdentifier l_oResult;
	std::map < CIdentifier, SConfigurationToken >::const_iterator i;
	do
	{
		l_ui64Identifier++;
		l_oResult=CIdentifier(l_ui64Identifier);
		i=m_vConfigurationToken.find(l_oResult);
	}
	while(i!=m_vConfigurationToken.end() || l_oResult==OV_UndefinedIdentifier);
	return l_oResult;
}

// ----------------------------------------------------------------------------------------------------------------------------

namespace
{
	enum ENodeType
	{
		NodeType_Value,
		NodeType_NamePrefix,
		NodeType_NamePostfix,
	};
};

boolean CConfigurationManager::expand(const std::string& sValue, std::string& sResult) const
{
	std::stack < std::pair < ENodeType, std::string > > l_vChildren;
	l_vChildren.push(std::make_pair(NodeType_Value, std::string()));

	std::string l_sPrefix;
	std::string l_sPostfix;
	std::string l_sValue;
	std::string l_sExpandedValue;

	for(std::string::size_type i=0; i<sValue.length(); i++)
	{
		switch(sValue[i])
		{
			case '$':
				l_vChildren.push(std::make_pair(NodeType_NamePrefix, std::string()));
				break;

			case '{':
				if(l_vChildren.top().first != NodeType_NamePrefix)
				{
					this->getLogManager() << LogLevel_Warning << "Could not expand token with syntax error while expanding " << CString(sValue.c_str()) << "\n";
					return false;
				}
				l_vChildren.push(std::make_pair(NodeType_NamePostfix, std::string()));
				break;

			case '}':
				if(l_vChildren.top().first != NodeType_NamePostfix)
				{
					this->getLogManager() << LogLevel_Warning << "Could not expand token with syntax error while expanding " << CString(sValue.c_str()) << "\n";
					return false;
				}

				l_sPostfix=l_vChildren.top().second;
				l_vChildren.pop();

				l_sPrefix=l_vChildren.top().second;
				l_vChildren.pop();

				if(l_sPrefix=="")
				{
					l_sValue=this->getConfigurationTokenValue(this->lookUpConfigurationTokenIdentifier(l_sPostfix.c_str()));
				}
				else if(l_sPrefix=="ENV")
				{
					char* l_sEnvValue=::getenv(l_sPostfix.c_str());
					l_sValue=(l_sEnvValue?l_sEnvValue:"");
				}
				else
				{
					this->getLogManager() << LogLevel_Warning << "Could not expand token with " << CString(l_sPrefix.c_str()) << " prefix while expanding " << CString(sValue.c_str()) << "\n";
					return false;
				}

				if(!this->expand(l_sValue, l_sExpandedValue))
				{
					this->getLogManager() << LogLevel_Warning << "Could not expand " << CString(l_sValue.c_str()) << " while expanding " << CString(sValue.c_str()) << "\n";
					return false;
				}
				l_vChildren.top().second += l_sExpandedValue;
				break;

			case '\\':
				i++;
				if(i>=sValue.length())
				{
					this->getLogManager() << LogLevel_Warning << "Could not expand token with unterminated string while expanding " << CString(sValue.c_str()) << "\n";
					return false;
				}

			default:
				l_vChildren.top().second+=sValue[i];
				break;
		}
	}

	if(l_vChildren.size()!=1)
	{
		this->getLogManager() << LogLevel_Warning << "Could not expand token with unterminated string while expanding " << CString(sValue.c_str()) << "\n";
		return false;
	}

	sResult=l_vChildren.top().second;

	return true;
}
