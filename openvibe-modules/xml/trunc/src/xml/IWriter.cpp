#include "IWriter.h"

#include <stack>
#include <string>

using namespace XML;
using namespace std;

namespace XML
{

	class CWriter : public IWriter
	{
	public:
		CWriter(IWriterCallback& rWriterCallback);

		virtual boolean openChild(const char* sName);
		virtual boolean setChildData(const char* sData);
		virtual boolean setAttribute(const char* sAttributeName, const char* sAttributeValue);
		virtual boolean closeChild(void);
		virtual void release(void);

	protected:
		IWriterCallback& m_rWriterCallback;
		stack<string> m_vNodes;
		boolean m_bHasChild;
		boolean m_bHasData;
		boolean m_bHasClosedOpeningNode;
	};
};

CWriter::CWriter(IWriterCallback& rWriterCallback)
	:m_rWriterCallback(rWriterCallback)
	,m_bHasChild(false)
	,m_bHasData(false)
	,m_bHasClosedOpeningNode(true)
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

	if(!m_bHasClosedOpeningNode)
	{
		m_rWriterCallback.write(">");
		m_bHasClosedOpeningNode=true;
	}

	string l_sIndent(m_vNodes.size(), '\t');
	string l_sResult=(m_vNodes.size()!=0?string("\n"):string(""))+l_sIndent+string("<")+string(sName);
	m_rWriterCallback.write(l_sResult.c_str());
	m_vNodes.push(sName);
	m_bHasChild=false;
	m_bHasData=false;
	m_bHasClosedOpeningNode=false;
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

	if(!m_bHasClosedOpeningNode)
	{
		m_rWriterCallback.write(">");
		m_bHasClosedOpeningNode=true;
	}

	m_rWriterCallback.write(sData);
	m_bHasChild=false;
	m_bHasData=true;
	return true;
}

boolean CWriter::setAttribute(const char* sAttributeName, const char* sAttributeValue)
{
	if(sAttributeName==NULL)
	{
		return false;
	}

	if(sAttributeValue==NULL)
	{
		return false;
	}

	if(m_bHasChild)
	{
		return false;
	}

	if(m_bHasData)
	{
		return false;
	}

	if(m_bHasClosedOpeningNode)
	{
		return false;
	}

	string l_sResult=string(" ")+string(sAttributeName)+string("=\"")+string(sAttributeValue)+string("\"");
	m_rWriterCallback.write(l_sResult.c_str());
	return true;
}

boolean CWriter::closeChild(void)
{
	if(m_vNodes.size()==0)
	{
		return false;
	}

	if(!m_bHasClosedOpeningNode)
	{
		m_rWriterCallback.write(">");
		m_bHasClosedOpeningNode=true;
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
