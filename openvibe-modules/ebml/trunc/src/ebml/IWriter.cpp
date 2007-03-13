#include "IWriter.h"

#include <vector>

using namespace EBML;
using namespace std;

// ________________________________________________________________________________________________________________
//

inline unsigned long getCodedSizeLength(const uint64 uiValue)
{
	unsigned long l_ulCodedSizeLength=0;
	if(uiValue<0x7f)
		l_ulCodedSizeLength=1;
	else if(uiValue<0x3fff)
		l_ulCodedSizeLength=2;
	else if(uiValue<0x1fffff)
		l_ulCodedSizeLength=3;
	else if(uiValue<0x0fffffff)
		l_ulCodedSizeLength=4;
/*
	else if(uiValue<0xffffffffff)
		l_ulCodedSizeLength=5;
	else if(uiValue<0xffffffffffff)
		l_ulCodedSizeLength=6;
	else if(uiValue<0xffffffffffffff)
		l_ulCodedSizeLength=7;
*/
	else
		l_ulCodedSizeLength=8;

	return l_ulCodedSizeLength;
}

#if 0
inline unsigned long getCodedSizeLength(const int64 iValue)
{
	unsigned long l_ulCodedSizeLength=0;
	if(iValue>-0x40 && iValue<0x40)
		l_ulCodedSizeLength=1;
	else if(iValue>-0x2000 && iValue<0x2000)
		l_ulCodedSizeLength=2;
	else if(iValue>-0x100000 && iValue<0x100000)
		l_ulCodedSizeLength=3;
	else if(iValue>-0x08000000 && iValue<0x08000000)
		l_ulCodedSizeLength=4;
/*
	else if(iValue>-0x0800000000 && iValue<0x0800000000)
		l_ulCodedSizeLength=5;
	else if(iValue>-0x080000000000 && iValue<0x080000000000)
		l_ulCodedSizeLength=6;
	else if(iValue>-0x08000000000000 && iValue<0x08000000000000)
		l_ulCodedSizeLength=7;
*/
	else
		l_ulCodedSizeLength=8;

	return l_ulCodedSizeLength;
}
#endif

inline boolean getCodedBuffer(const uint64 uiValue, unsigned char* pBuffer, uint64* pBufferLength)
{
	unsigned long i;
	unsigned long l_ulSize=getCodedSizeLength(uiValue);

	if(l_ulSize>*pBufferLength)
	{
		return false;
	}

	for(i=0; i<l_ulSize; i++)
	{
		unsigned long l_ulByteShift=l_ulSize-i-1;
		unsigned long l_ulByte=(unsigned char)((uiValue>>(l_ulByteShift*8))&0xff);
		l_ulByte|=(8-l_ulSize-i*8<8 && 8-l_ulSize-i*8>=0?(1<<(8-l_ulSize-i*8)):0);
		pBuffer[i]=(unsigned char)l_ulByte;
	}

	*pBufferLength=l_ulSize;
	return true;
}

#if 0
inline boolean getCodedBuffer(const int64 iValue, unsigned char* pBuffer, uint64* pBufferLength)
{
	if(iValue>-0x40 && iValue<0x40)
		iValue+=0x3f;
	else if(iValue>-0x2000 && iValue<0x2000)
		iValue+=0x1fff;
	else if(iValue>-0x100000 && iValue<0x100000)
		iValue+=0x0fffff;
	else if(iValue>-0x08000000 && iValue<0x08000000)
		iValue+=0x07ffffff;
/*
	else if(iValue>-0x0800000000 && iValue<0x0800000000)
		iValue+=0x07ffffffff;
	else if(iValue>-0x080000000000 && iValue<0x080000000000)
		iValue+=0x07ffffffffff;
	else if(iValue>-0x08000000000000 && iValue<0x08000000000000)
		iValue+=0x07ffffffffffff;
	else // if(iValue>-0x0800000000000000 && iValue<0x0800000000000000)
		iValue+=0x07ffffffffffffff;
*/

	return getCodedBuffer((uint64)iValue, pBuffer, pBufferLength);
}
#endif

// ________________________________________________________________________________________________________________
//

namespace EBML
{
	class CWriterNode
	{
	public:
		CWriterNode(const CIdentifier& rIdentifier, CWriterNode* pParentNode);
		virtual ~CWriterNode(void);
		void process(IWriterCallback& rWriterCallback);

	protected:
		uint64 getTotalContentSize(boolean bCountIdentifierAndSize);

	private:
		CWriterNode(void);

	public:
		CIdentifier m_oIdentifier;
		CWriterNode* m_pParentNode;
		uint64 m_ui64BufferLength;
		unsigned char* m_pBuffer;
		boolean m_bBuffered;
		vector<CWriterNode*> m_vChildren;
	};
};

// ________________________________________________________________________________________________________________
//

CWriterNode::CWriterNode(const CIdentifier& rIdentifier, EBML::CWriterNode* pParentNode)
	:m_oIdentifier(rIdentifier)
	,m_pParentNode(pParentNode)
	,m_ui64BufferLength(0)
	,m_pBuffer(NULL)
	,m_bBuffered(false)
{
}

CWriterNode::~CWriterNode(void)
{
	vector<CWriterNode*>::iterator i;
	for(i=m_vChildren.begin(); i!=m_vChildren.end(); i++)
	{
		delete (*i);
	}

	delete [] m_pBuffer;
}

void CWriterNode::process(IWriterCallback& rWriterCallback)
{
	unsigned char l_pIdentifier[8];
	unsigned char l_pContentSize[8];
	uint64 l_ui64ContentSizeLength=sizeof(l_pContentSize);
	uint64 l_ui64IdentifierLength=sizeof(l_pIdentifier);
	uint64 l_ui64ContentSize=getTotalContentSize(false);

	if(!getCodedBuffer(l_ui64ContentSize, l_pContentSize, &l_ui64ContentSizeLength))
	{
		// SHOULD NEVER HAPPEN
	}

	if(!getCodedBuffer(m_oIdentifier, l_pIdentifier, &l_ui64IdentifierLength))
	{
		// SHOULD NEVER HAPPEN
	}

	rWriterCallback.write(l_pIdentifier, l_ui64IdentifierLength);
	rWriterCallback.write(l_pContentSize, l_ui64ContentSizeLength);

	if(m_vChildren.size()==0)
	{
		rWriterCallback.write(m_pBuffer, m_ui64BufferLength);
	}
	else
	{
		vector<CWriterNode*>::iterator i;
		for(i=m_vChildren.begin(); i!=m_vChildren.end(); i++)
		{
			(*i)->process(rWriterCallback);
		}
	}
}

uint64 CWriterNode::getTotalContentSize(boolean bCountIdentifierAndSize)
{
	uint64 l_ui64ContentSize=0;
	if(m_vChildren.size()==0)
	{
		l_ui64ContentSize=m_ui64BufferLength;
	}
	else
	{
		vector<CWriterNode*>::iterator i;
		for(i=m_vChildren.begin(); i!=m_vChildren.end(); i++)
		{
			l_ui64ContentSize+=(*i)->getTotalContentSize(true);
		}
	}

	uint64 l_ui64Result=l_ui64ContentSize;
	if(bCountIdentifierAndSize)
	{
		l_ui64Result+=getCodedSizeLength(m_oIdentifier);
		l_ui64Result+=getCodedSizeLength(l_ui64ContentSize);
	}

	return l_ui64Result;
}

// ________________________________________________________________________________________________________________
//

namespace EBML
{
	class CWriter : virtual public IWriter
	{
	public:
		CWriter(IWriterCallback& rWriterCallback);

		virtual boolean openChild(const CIdentifier& rIdentifier);
		virtual boolean setChildData(const void* pBuffer, const uint64 ui64BufferSize);
		virtual boolean closeChild(void);

		virtual void release(void);

	protected:
		CWriterNode* m_pCurrentNode;
		IWriterCallback& m_rWriterCallback;

	private:
		CWriter(void);
	};
};

// ________________________________________________________________________________________________________________
//

CWriter::CWriter(IWriterCallback& rWriterCallback)
	:m_pCurrentNode(NULL)
	,m_rWriterCallback(rWriterCallback)
{
}

boolean CWriter::openChild(const CIdentifier& rIdentifier)
{
	if(m_pCurrentNode)
	{
		if(m_pCurrentNode->m_bBuffered)
		{
			return false;
		}
	}

	CWriterNode* pResult=new CWriterNode(rIdentifier, m_pCurrentNode);
	if(m_pCurrentNode)
	{
		m_pCurrentNode->m_vChildren.push_back(pResult);
	}
	m_pCurrentNode=pResult;
	return true;
}

boolean CWriter::setChildData(const void* pBuffer, const uint64 ui64BufferSize)
{
	if(!m_pCurrentNode)
	{
		return false;
	}

	if(m_pCurrentNode->m_vChildren.size()!=0)
	{
		return false;
	}

	unsigned char* l_pBufferCopy=NULL;
	if(ui64BufferSize)
	{
		if(!pBuffer)
		{
			return false;
		}
		l_pBufferCopy=new unsigned char[(unsigned int)(ui64BufferSize)];
		if(!l_pBufferCopy)
		{
			return false;
		}
		memcpy(l_pBufferCopy, pBuffer, (size_t)(ui64BufferSize));
	}

	m_pCurrentNode->m_ui64BufferLength=ui64BufferSize;
	m_pCurrentNode->m_pBuffer=l_pBufferCopy;
	m_pCurrentNode->m_bBuffered=true;
	return true;
}

boolean CWriter::closeChild(void)
{
	if(!m_pCurrentNode)
	{
		return false;
	}

	if((!m_pCurrentNode->m_bBuffered) && (!m_pCurrentNode->m_vChildren.size()))
	{
		m_pCurrentNode->m_ui64BufferLength=0;
		m_pCurrentNode->m_pBuffer=NULL;
		m_pCurrentNode->m_bBuffered=true;
	}

	CWriterNode* l_pParentNode=m_pCurrentNode->m_pParentNode;
	if(!l_pParentNode)
	{
		m_pCurrentNode->process(m_rWriterCallback);
		delete m_pCurrentNode;
	}

	m_pCurrentNode=l_pParentNode;
	return true;
}

void CWriter::release(void)
{
	while(m_pCurrentNode)
	{
		closeChild();
	}
	delete this;
}

// ________________________________________________________________________________________________________________
//

EBML_API IWriter* EBML::createWriter(IWriterCallback& rWriterCallback)
{
	return new CWriter(rWriterCallback);
}
