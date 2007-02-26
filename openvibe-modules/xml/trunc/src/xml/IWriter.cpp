#include "IWriter.h"

#include <stack>
#include <string>

using namespace XML;
using namespace std;

namespace XML
{

	class CWriter : virtual public IWriter
	{
	public:
		CWriter(IWriterCallback& rWriterCallback);

		virtual XML::boolean openChild(const char* sName);
		virtual XML::boolean setChildData(const char* sData);
		virtual XML::boolean closeChild(void);
		virtual void release(void);

	protected:
		IWriterCallback& m_rWriterCallback;
		stack<string> m_vNodes;
		boolean m_bHasChild;
		boolean m_bHasData;
	};
};

CWriter::CWriter(IWriterCallback& rWriterCallback)
	:m_rWriterCallback(rWriterCallback)
	,m_bHasChild(false)
	,m_bHasData(false)
{
}

boolean CWriter::openChild(const char* sName)
{
	if(sName==NULL)
	{
		return false;
	}

	if(m_bHasData)
	{
		return false;
	}

	string l_sIndent(m_vNodes.size(), '\t');
	string l_sResult=(m_vNodes.size()!=0?string("\n"):string(""))+l_sIndent+string("<")+string(sName)+string(">");
	m_rWriterCallback.write(l_sResult.c_str());
	m_vNodes.push(sName);
	m_bHasChild=false;
	m_bHasData=false;
	return true;
}

boolean CWriter::setChildData(const char* sData)
{
	if(sData==NULL)
	{
		return false;
	}

	if(m_bHasChild)
	{
		return false;
	}

	m_rWriterCallback.write(sData);
	m_bHasChild=false;
	m_bHasData=true;
	return true;
}

boolean CWriter::closeChild(void)
{
	if(m_vNodes.size()==0)
	{
		return false;
	}

	string l_sIndent(m_vNodes.size()-1, '\t');
	string l_sResult=((m_bHasData||(!m_bHasData&&!m_bHasChild))?string(""):string("\n")+l_sIndent)+string("</")+m_vNodes.top()+string(">");
	m_rWriterCallback.write(l_sResult.c_str());
	m_vNodes.pop();
	m_bHasChild=true;
	m_bHasData=false;
	return true;
}

void CWriter::release(void)
{
	while(m_vNodes.size())
	{
		closeChild();
	}
	delete this;
}

XML_API XML::IWriter* XML::createWriter(IWriterCallback& rWriterCallback)
{
	return new CWriter(rWriterCallback);
}
