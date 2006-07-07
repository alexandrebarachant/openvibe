#include "ovCIdentifier.h"

#include <math.h>
#include <string.h>

namespace OpenViBE
{

CIdentifier::CIdentifier(void)
{
	m_ui32Identifier1=((rand()<<16)+rand())&0xffffffff;
	m_ui32Identifier2=((rand()<<16)+rand())&0xffffffff;
}

CIdentifier::CIdentifier(const OpenViBE::uint32 ui32Identifier1, const OpenViBE::uint32 ui32Identifier2)
	:m_ui32Identifier1(ui32Identifier1)
	,m_ui32Identifier2(ui32Identifier2)
{
}

CIdentifier::CIdentifier(const CIdentifier& rIdentifier)
	:m_ui32Identifier1(rIdentifier.m_ui32Identifier1)
	,m_ui32Identifier2(rIdentifier.m_ui32Identifier2)
{
}

const CIdentifier& CIdentifier::operator=(const CIdentifier& rIdentifier)
{
	m_ui32Identifier1=rIdentifier.m_ui32Identifier1;
	m_ui32Identifier2=rIdentifier.m_ui32Identifier2;
	return *this;
}

bool operator==(const CIdentifier& rIdentifier1, const CIdentifier& rIdentifier2)
{
	return (rIdentifier1.m_ui32Identifier1==rIdentifier2.m_ui32Identifier1) && (rIdentifier1.m_ui32Identifier2==rIdentifier2.m_ui32Identifier2);
}

bool operator!=(const CIdentifier& rIdentifier1, const CIdentifier& rIdentifier2)
{
	return !(rIdentifier1==rIdentifier2);
}	

std::string CIdentifier::toString(void) const
{
	char l_sBuffer[1024];
	sprintf(l_sBuffer, "(0x%08x, 0x%08x)", static_cast<unsigned int>(m_ui32Identifier1), static_cast<unsigned int>(m_ui32Identifier2));
	return std::string(l_sBuffer);
}

};
