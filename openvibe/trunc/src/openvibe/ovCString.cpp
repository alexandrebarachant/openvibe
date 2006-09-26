#include "ovCString.h"

#include <string>

using namespace std;
using namespace OpenViBE;

#define m_pImplementation ((string*)m_pSecretImplementation)
#define m_pImplementation2(r) ((string*)r.m_pSecretImplementation)

CString::CString(void)
{
	m_pSecretImplementation=new string();
}

CString::CString(const CString& rString)
{
	m_pSecretImplementation=new string(*m_pImplementation2(rString));
}

CString::CString(const char* pString)
{
	if(pString)
	{
		m_pSecretImplementation=new string(pString);
	}
	else
	{
		m_pSecretImplementation=new string();
	}
}

CString::~CString(void)
{
	delete m_pImplementation;
}

CString::operator const char* (void) const
{
	return m_pImplementation->c_str();
}

CString& CString::operator=(const CString& rString)
{
	*m_pImplementation=*m_pImplementation2(rString);
	return *this;
}

const CString OpenViBE::operator+(const OpenViBE::CString& rString1, const OpenViBE::CString& rString2)
{
	CString l_oResult;
	*m_pImplementation2(l_oResult)=*m_pImplementation2(rString1)+*m_pImplementation2(rString2);
	return l_oResult;
}
