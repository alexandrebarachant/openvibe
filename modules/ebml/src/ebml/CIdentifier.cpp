#include "CIdentifier.h"

using namespace EBML;

CIdentifier::CIdentifier(void)
	:m_ui64Identifier(0)
{
}

CIdentifier::CIdentifier(const uint64 ui64Identifier)
	:m_ui64Identifier(ui64Identifier)
{
}

CIdentifier::CIdentifier(const CIdentifier& rIdentifier)
	:m_ui64Identifier(rIdentifier.m_ui64Identifier)
{
}

const CIdentifier& CIdentifier::operator=(const CIdentifier& rIdentifier)
{
	m_ui64Identifier=rIdentifier.m_ui64Identifier;
	return *this;
}

EBML_API boolean EBML::operator==(const CIdentifier& rIdentifier1, const CIdentifier& rIdentifier2)
{
	return rIdentifier1.m_ui64Identifier==rIdentifier2.m_ui64Identifier;
}

EBML_API boolean EBML::operator!=(const CIdentifier& rIdentifier1, const CIdentifier& rIdentifier2)
{
	return rIdentifier1.m_ui64Identifier!=rIdentifier2.m_ui64Identifier;
}

EBML_API boolean EBML::operator<=(const CIdentifier& rIdentifier1, const CIdentifier& rIdentifier2)
{
	return rIdentifier1.m_ui64Identifier<=rIdentifier2.m_ui64Identifier;
}

EBML_API boolean EBML::operator>=(const CIdentifier& rIdentifier1, const CIdentifier& rIdentifier2)
{
	return rIdentifier1.m_ui64Identifier>=rIdentifier2.m_ui64Identifier;
}

EBML_API boolean EBML::operator<(const CIdentifier& rIdentifier1, const CIdentifier& rIdentifier2)
{
	return rIdentifier1.m_ui64Identifier<rIdentifier2.m_ui64Identifier;
}

EBML_API boolean EBML::operator>(const CIdentifier& rIdentifier1, const CIdentifier& rIdentifier2)
{
	return rIdentifier1.m_ui64Identifier>rIdentifier2.m_ui64Identifier;
}

CIdentifier::operator uint64(void) const
{
	return m_ui64Identifier;
}
