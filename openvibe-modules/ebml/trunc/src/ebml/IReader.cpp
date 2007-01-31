#include "IReader.h"

#include <string.h>

using namespace EBML;

// ________________________________________________________________________________________________________________
//

inline unsigned long getCodedSizeLength(unsigned char* pBuffer)
{
	unsigned long l_ulCodedSizeLength;
	if((*pBuffer)>>7)
		l_ulCodedSizeLength=1;
	else if((*pBuffer)>>6)
		l_ulCodedSizeLength=2;
	else if((*pBuffer)>>5)
		l_ulCodedSizeLength=3;
	else if((*pBuffer)>>4)
		l_ulCodedSizeLength=4;
	else if((*pBuffer)>>3)
		l_ulCodedSizeLength=5;
	else if((*pBuffer)>>2)
		l_ulCodedSizeLength=6;
	else if((*pBuffer)>>1)
		l_ulCodedSizeLength=7;
	else
		l_ulCodedSizeLength=8;
	return l_ulCodedSizeLength;
}

inline uint64 getValue(unsigned char* pBuffer, unsigned long ulBufferLength=0)
{
	uint64 l_uiResult=0;
	unsigned long l_ulCodedSizeLength=getCodedSizeLength(pBuffer);
	unsigned long i;
	for(i=0; i<l_ulCodedSizeLength; i++)
	{
		l_uiResult=(l_uiResult<<8)+(pBuffer[i]);
		l_uiResult&=~(8-l_ulCodedSizeLength-i*8<8 && 8-l_ulCodedSizeLength-i*8>=0?(1<<(8-l_ulCodedSizeLength-i*8)):0);
	}
	return l_uiResult;
}

// ________________________________________________________________________________________________________________
//

namespace EBML
{
	class CReaderNode
	{
	public:
		CReaderNode(const CIdentifier& rIdentifier, CReaderNode* pParentNode);

	private:
		CReaderNode(void);

	public:
		CReaderNode* m_pParentNode;
		CIdentifier m_oIdentifier;
		uint64 m_ui64ContentSize;
		uint64 m_ui64ReadContentSize;
		unsigned char* m_pBuffer;
	};
};

CReaderNode::CReaderNode(const CIdentifier& rIdentifier, CReaderNode* pParentNode)
	:m_pParentNode(pParentNode)
	,m_oIdentifier(rIdentifier)
	,m_ui64ContentSize(0)
	,m_ui64ReadContentSize(0)
	,m_pBuffer(NULL)
{
}

// ________________________________________________________________________________________________________________
//

namespace EBML
{
	class CReader : virtual public IReader
	{
	public:
		CReader(IReaderCallback& rReaderCallback);
		virtual ~CReader(void);

		virtual boolean processData(const void* pBuffer, const uint64 ui64BufferSize);

		virtual void release(void);

	protected:
		enum Status
		{
			FillingIdentifier,
			FillingContentSize,
			FillingContent,
		};

		IReaderCallback& m_rReaderCallback;
		CReaderNode* m_pCurrentNode;
		uint64 m_ui64PendingSize;
		uint64 m_ui64PendingCount;
		unsigned char* m_pPending;
		Status m_eStatus;
		Status m_eLastStatus;
		CIdentifier m_oCurrentIdentifier;
		uint64 m_ui64CurrentContentSize;
	};
};

// ________________________________________________________________________________________________________________
//

#define _Debug_ 0

CReader::CReader(IReaderCallback& rReaderCallback)
	:m_rReaderCallback(rReaderCallback)
	,m_pCurrentNode(NULL)
	,m_ui64PendingSize(0)
	,m_ui64PendingCount(0)
	,m_pPending(NULL)
	,m_eStatus(FillingIdentifier)
	,m_oCurrentIdentifier(0)
	,m_ui64CurrentContentSize(0)
{
}

CReader::~CReader(void)
{
	delete [] m_pPending;
	while (m_pCurrentNode)
	{
		CReaderNode* l_pParentNode=m_pCurrentNode->m_pParentNode;
		delete m_pCurrentNode;
		m_pCurrentNode=l_pParentNode;
	}
}

boolean CReader::processData(const void* pBuffer, const uint64 ui64BufferSize)
{
#if _Debug_
printf("Received %i byte(s) new buffer :", (int)ui64BufferSize);
for(int i=0; i<(int)ui64BufferSize && i<4; i++)
	printf("[%02X]", ((unsigned char*)pBuffer)[i]);
printf("...\n");
#endif

	if(!pBuffer || !ui64BufferSize)
	{
		return true;
	}

	unsigned char* l_pBuffer=(unsigned char*)pBuffer;
	uint64 l_ui64BufferSize=ui64BufferSize;
	boolean l_bFinished=false;
	while(!l_bFinished)
	{
		uint64 l_ui64ProcessedPendingBytes=0;
		uint64 l_ui64ProcessedBytes=0;
		m_eLastStatus=m_eStatus;

#if _Debug_
	if(m_ui64PendingCount)
	{
		printf("%i byte(s) pending : ", (int)m_ui64PendingCount);
		for(int i=0; i<(int)m_ui64PendingCount; i++)
			printf("[%02X]", m_pPending[i]);
		printf("\n");
	}
#endif

		// Processes data
		switch(m_eStatus)
		{
			case FillingIdentifier:
				{
					unsigned long l_ulCodedSizeLength=getCodedSizeLength(m_ui64PendingCount?m_pPending:l_pBuffer);
					if(l_ulCodedSizeLength>l_ui64BufferSize+m_ui64PendingCount)
					{
						l_bFinished=true;
					}
					else
					{
						unsigned char* l_pIdentifierBuffer=new unsigned char[l_ulCodedSizeLength];
						memcpy(l_pIdentifierBuffer, m_pPending, (size_t)(m_ui64PendingCount));
						memcpy(l_pIdentifierBuffer+m_ui64PendingCount, l_pBuffer, (size_t)(l_ulCodedSizeLength-m_ui64PendingCount));
						m_oCurrentIdentifier=getValue(l_pIdentifierBuffer);
						delete [] l_pIdentifierBuffer;
						l_ui64ProcessedPendingBytes=m_ui64PendingCount;
						l_ui64ProcessedBytes=l_ulCodedSizeLength;

						m_eStatus=FillingContentSize;
#if _Debug_
						printf("Found identifier 0x%08X - Changing status to FillingContentSize...\n", (int)m_oCurrentIdentifier);
#endif
					}
				}
				break;

			case FillingContentSize:
				{
					unsigned long l_ulCodedSizeLength=getCodedSizeLength(m_ui64PendingCount?m_pPending:l_pBuffer);
					if(l_ulCodedSizeLength>l_ui64BufferSize+m_ui64PendingCount)
					{
						l_bFinished=true;
					}
					else
					{
						unsigned char* l_pContentSizeBuffer=new unsigned char[l_ulCodedSizeLength];
						memcpy(l_pContentSizeBuffer, m_pPending, (size_t)(m_ui64PendingCount));
						memcpy(l_pContentSizeBuffer+m_ui64PendingCount, l_pBuffer, (size_t)(l_ulCodedSizeLength-m_ui64PendingCount));
						m_ui64CurrentContentSize=getValue(l_pContentSizeBuffer);
						delete [] l_pContentSizeBuffer;
						l_ui64ProcessedPendingBytes=m_ui64PendingCount;
						l_ui64ProcessedBytes=l_ulCodedSizeLength;

						if(m_rReaderCallback.isMasterChild(m_oCurrentIdentifier))
						{
							m_eStatus=FillingIdentifier;
#if _Debug_
							printf("Found content size %i of master node - Changing status to FillingIdentifier...\n", (int)m_ui64CurrentContentSize);
#endif
						}
						else
						{
							m_eStatus=FillingContent;
#if _Debug_
							printf("Found content size %i of *non* master node - Changing status to FillingContent...\n", (int)m_ui64CurrentContentSize);
#endif
						}
					}
				}
				break;

			case FillingContent:
				{
					if(m_pCurrentNode->m_ui64ContentSize==0)
					{
						m_eStatus=FillingIdentifier;
#if _Debug_
						printf("Finished with %i byte(s) content - Changing status to FillingIdentifier...\n", (int)m_pCurrentNode->m_ui64ContentSize);
#endif
						m_rReaderCallback.processChildData(NULL, 0);
					}
					else
					{
						if(m_pCurrentNode->m_ui64ContentSize-m_pCurrentNode->m_ui64ReadContentSize>l_ui64BufferSize)
						{
							memcpy(m_pCurrentNode->m_pBuffer+m_pCurrentNode->m_ui64ReadContentSize, l_pBuffer, (size_t)(l_ui64BufferSize));
							l_ui64ProcessedBytes=l_ui64BufferSize;
							l_bFinished=true;
						}
						else
						{
							memcpy(m_pCurrentNode->m_pBuffer+m_pCurrentNode->m_ui64ReadContentSize, l_pBuffer, (size_t)(m_pCurrentNode->m_ui64ContentSize-m_pCurrentNode->m_ui64ReadContentSize));
							l_ui64ProcessedBytes=m_pCurrentNode->m_ui64ContentSize-m_pCurrentNode->m_ui64ReadContentSize;

							m_eStatus=FillingIdentifier;
#if _Debug_
							printf("Finished with %i byte(s) content - Changing status to FillingIdentifier...\n", (int)m_pCurrentNode->m_ui64ContentSize);
#endif
							m_rReaderCallback.processChildData(m_pCurrentNode->m_pBuffer, m_pCurrentNode->m_ui64ContentSize);
						}
					}
				}
				break;
		}

		// Updates buffer pointer and size
		uint64 l_ui64ProcessedBytesInBuffer=l_ui64ProcessedBytes-l_ui64ProcessedPendingBytes;
		l_pBuffer+=l_ui64ProcessedBytesInBuffer;
		l_ui64BufferSize-=l_ui64ProcessedBytesInBuffer;
		m_ui64PendingCount-=l_ui64ProcessedPendingBytes;

		// Updates read size
		CReaderNode* l_pNode=m_pCurrentNode;
		while (l_pNode)
		{
			l_pNode->m_ui64ReadContentSize+=l_ui64ProcessedBytes;
			l_pNode=l_pNode->m_pParentNode;
		}

		// Creates new node when needed
		if(m_eStatus!=FillingContentSize && m_eLastStatus==FillingContentSize)
		{
			m_pCurrentNode=new CReaderNode(m_oCurrentIdentifier, m_pCurrentNode);
			m_pCurrentNode->m_ui64ContentSize=m_ui64CurrentContentSize;
			m_pCurrentNode->m_pBuffer=new unsigned char[(unsigned int)(m_ui64CurrentContentSize)];
			m_rReaderCallback.openChild(m_pCurrentNode->m_oIdentifier);
		}
		else
		{
			// Closes finished nodes
			while(m_pCurrentNode && m_pCurrentNode->m_ui64ContentSize == m_pCurrentNode->m_ui64ReadContentSize)
			{
				m_rReaderCallback.closeChild();
				CReaderNode* l_pParentNode=m_pCurrentNode->m_pParentNode;
				delete [] m_pCurrentNode->m_pBuffer;
				delete m_pCurrentNode;
				m_pCurrentNode=l_pParentNode;
			}
		}
	}

	// Updates pending data
	if(m_ui64PendingCount+l_ui64BufferSize>m_ui64PendingSize)
	{
		unsigned char* l_pPending=new unsigned char[(unsigned int)(m_ui64PendingCount+l_ui64BufferSize)];
		memcpy(l_pPending, m_pPending, (size_t)(m_ui64PendingCount));
		delete [] m_pPending;
		m_pPending=l_pPending;
		m_ui64PendingSize=m_ui64PendingCount+l_ui64BufferSize;
	}
	memcpy(m_pPending+m_ui64PendingCount, l_pBuffer, (size_t)(l_ui64BufferSize));
	m_ui64PendingCount+=l_ui64BufferSize;

#if _Debug_
printf("\n");
#endif

	return true;
}

void CReader::release(void)
{
	delete this;
}

// ________________________________________________________________________________________________________________
//

EBML_API IReader* EBML::createReader(IReaderCallback& rReaderCallback)
{
	return new CReader(rReaderCallback);
}
