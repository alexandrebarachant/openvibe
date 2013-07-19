#include "ovdTAttributeHandler.h"

#include <cstdlib>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBEDesigner;

TAttributeHandler::TAttributeHandler(IAttributable& rAttributable)
	:m_pConstAttributable(&rAttributable)
	,m_pAttributable(&rAttributable)
{
}

TAttributeHandler::TAttributeHandler(const IAttributable& rAttributable)
	:m_pConstAttributable(&rAttributable)
	,m_pAttributable(NULL)
{
}

boolean TAttributeHandler::removeAttribute(
	const CIdentifier& rAttributeIdentifier)
{
	if(!m_pAttributable)
	{
		return false;
	}
	return m_pAttributable->removeAttribute(rAttributeIdentifier);
}

boolean TAttributeHandler::removeAllAttributes(void)
{
	if(!m_pAttributable)
	{
		return false;
	}
	return m_pAttributable->removeAllAttributes();
}

boolean TAttributeHandler::hasAttribute(
	const CIdentifier& rAttributeIdentifier) const
{
	return m_pConstAttributable->hasAttribute(rAttributeIdentifier);
}

boolean TAttributeHandler::hasAttributes(void) const
{
	return m_pConstAttributable->hasAttributes();
}

namespace OpenViBEDesigner
{
	template <>
	boolean TAttributeHandler::addAttribute(
		const CIdentifier& rAttributeIdentifier,
		const int& rValue) const
	{
		if(!m_pAttributable)
		{
			return false;
		}
		char l_sValue[1024];
		sprintf(l_sValue, "%i", rValue);
		return m_pAttributable->addAttribute(rAttributeIdentifier, l_sValue);
	}

	template <>
	boolean TAttributeHandler::addAttribute(
		const CIdentifier& rAttributeIdentifier,
		const bool& rValue) const
	{
		if(!m_pAttributable)
		{
			return false;
		}

		return m_pAttributable->addAttribute(rAttributeIdentifier, (rValue ? "true" : "false"));
	}

	template <>
	int TAttributeHandler::getAttributeValue<int>(
		const CIdentifier& rAttributeIdentifier) const
	{
		return atoi(m_pConstAttributable->getAttributeValue(rAttributeIdentifier));
	}

	template <>
	bool TAttributeHandler::getAttributeValue<bool>(
		const CIdentifier& rAttributeIdentifier) const
	{
		bool retval = false;
		CString l_sAttributeValue(m_pConstAttributable->getAttributeValue(rAttributeIdentifier));
		if ( l_sAttributeValue==CString("true") )
		{
			retval = true;
		}

		return retval;
	}

	template <>
	boolean TAttributeHandler::setAttributeValue(
		const CIdentifier& rAttributeIdentifier,
		const int& rValue)
	{
		if(!m_pAttributable)
		{
			return false;
		}
		char l_sValue[1024];
		sprintf(l_sValue, "%i", rValue);
		return m_pAttributable->setAttributeValue(rAttributeIdentifier, l_sValue);
	}

	template <>
	boolean TAttributeHandler::setAttributeValue(
		const CIdentifier& rAttributeIdentifier,
		const bool& rValue)
	{
		if(!m_pAttributable)
		{
			return false;
		}

		return m_pAttributable->setAttributeValue(rAttributeIdentifier, (rValue ? "true" : "false"));
	}
};
