#include "ovCIdentifier.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

using namespace OpenViBE;

CIdentifier::CIdentifier(void)
	:m_ui32Identifier1(0xffffffff)
	,m_ui32Identifier2(0xffffffff)
{
}

CIdentifier::CIdentifier(const uint32 ui32Identifier1, const uint32 ui32Identifier2)
	:m_ui32Identifier1(ui32Identifier1)
	,m_ui32Identifier2(ui32Identifier2)
{
}

CIdentifier::CIdentifier(const uint64 ui64Identifier)
	:m_ui32Identifier1((uint32)((ui64Identifier>>32)&0xffffffff))
	,m_ui32Identifier2((uint32)(ui64Identifier&0xffffffff))
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

OV_API boolean OpenViBE::operator==(const CIdentifier& rIdentifier1, const CIdentifier& rIdentifier2)
{
	return (rIdentifier1.m_ui32Identifier1==rIdentifier2.m_ui32Identifier1) && (rIdentifier1.m_ui32Identifier2==rIdentifier2.m_ui32Identifier2);
}

OV_API boolean OpenViBE::operator!=(const CIdentifier& rIdentifier1, const CIdentifier& rIdentifier2)
{
	return !(rIdentifier1==rIdentifier2);
}	

OV_API boolean OpenViBE::operator<(const CIdentifier& rIdentifier1, const CIdentifier& rIdentifier2)
{
	boolean l_bResult=false;
	if(rIdentifier1.m_ui32Identifier1<rIdentifier2.m_ui32Identifier1)
	{
		l_bResult=true;
	}
	else if(rIdentifier1.m_ui32Identifier1==rIdentifier2.m_ui32Identifier1)
	{
		if(rIdentifier1.m_ui32Identifier2<rIdentifier2.m_ui32Identifier2)
		{
			l_bResult=true;
		}
	}
	return l_bResult;
}

OV_API boolean OpenViBE::operator>(const CIdentifier& rIdentifier1, const CIdentifier& rIdentifier2)
{
	boolean l_bResult=false;
	if(rIdentifier1.m_ui32Identifier1>rIdentifier2.m_ui32Identifier1)
	{
		l_bResult=true;
	}
	else if(rIdentifier1.m_ui32Identifier1==rIdentifier2.m_ui32Identifier1)
	{
		if(rIdentifier1.m_ui32Identifier2>rIdentifier2.m_ui32Identifier2)
		{
			l_bResult=true;
		}
	}
	return l_bResult;
}

CString CIdentifier::toString(void) const
{
	char l_sBuffer[1024];
	int l_iIdentifier1=m_ui32Identifier1;
	int l_iIdentifier2=m_ui32Identifier2;
	sprintf(l_sBuffer, "(0x%08x, 0x%08x)", l_iIdentifier1, l_iIdentifier2);
	return CString(l_sBuffer);
}

boolean CIdentifier::fromString(const CString& rString)
{
	const char* l_sBuffer=rString;
	int l_iIdentifier1;
	int l_iIdentifier2;
	if(sscanf(l_sBuffer, "(0x%x, 0x%x)", &l_iIdentifier1, &l_iIdentifier2)!=2)
	{
		return false;
	}
	m_ui32Identifier1=l_iIdentifier1;
	m_ui32Identifier2=l_iIdentifier2;
	return true;
}
