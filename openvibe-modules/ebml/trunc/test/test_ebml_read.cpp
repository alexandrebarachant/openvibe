#include <stdio.h>
#include <stdlib.h>

#include "ebml/IReader.h"
#include "ebml/IReaderHelper.h"

class CReaderCallBack : virtual public EBML::IReaderCallBack
{
public:
	CReaderCallBack(void)
		:m_iDepth(0)
		,m_pReaderHelper(NULL)
	{
		m_pReaderHelper=EBML::createReaderHelper();
	}

	virtual ~CReaderCallBack(void)
	{
		m_pReaderHelper->release();
	}

	virtual bool isMasterChild(const EBML::CIdentifier& rIdentifier)
	{
		if(rIdentifier==EBML_Identifier_Header) return true;
		if(rIdentifier==EBML::CIdentifier(0xffff)) return true;
		return false;
	}

	virtual void openChild(const EBML::CIdentifier& rIdentifier)
	{
		m_oCurrentIdentifier=rIdentifier;
		EBML::uint64 l_ui64Identifier=rIdentifier;
		for(int i=0; i<m_iDepth; i++) printf("   ");
		printf("Opening child node [0x%08X]\n", (int)l_ui64Identifier);
		m_iDepth++;
	}

	virtual void processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize)
	{
		for(int i=0; i<m_iDepth; i++) printf("   ");
		if(m_oCurrentIdentifier==EBML_Identifier_DocType)
			printf("Got doc type : [%s]\n", m_pReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize));
		else if(m_oCurrentIdentifier==EBML_Identifier_DocTypeVersion)
			printf("Got doc type version : [%i]\n", (int)m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize));
		else if(m_oCurrentIdentifier==EBML_Identifier_DocTypeReadVersion)
			printf("Got doc type read version : [%i]\n", (int)m_pReaderHelper->getSIntegerFromChildData(pBuffer, ui64BufferSize));
		else
			printf("Got %i data bytes\n", (int)ui64BufferSize);
	}

	virtual void closeChild(void)
	{
		m_iDepth--;
		for(int i=0; i<m_iDepth; i++) printf("   ");
		printf("Node closed\n");
	}

	int m_iDepth;
	EBML::IReaderHelper* m_pReaderHelper;
	EBML::CIdentifier m_oCurrentIdentifier;
};

int main(int argc, char** argv)
{
	if(argc<2)
	{
		printf("syntax : %s filein.ebml\n", argv[0]);
		return -1;
	}

	CReaderCallBack cb;
	EBML::IReader* l_pReader=EBML::createReader(cb);

	FILE* f=fopen("test.ebml", "rb");
	unsigned char c[13];
	int i=0;
	while(!feof(f))
	{
		i=fread(c, 1, sizeof(c), f);
		printf("\n --> reader has read %i bytes\n\n", i);
		l_pReader->processData(c, i);
	}
	fclose(f);

	return 0;
}
