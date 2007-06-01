#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>

#include "ebml/IReader.h"
#include "ebml/IReaderHelper.h"
#include "ebml/CReaderHelper.h"

using namespace std;

class CReaderCallBack : virtual public EBML::IReaderCallBack
{
public:
	CReaderCallBack(void)
		:m_iDepth(0)
	{
	}

	virtual ~CReaderCallBack(void)
	{
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
		for(int i=0; i<m_iDepth; i++) cout << "   ";
		cout << "Opening child node [0x" << setw(16) << setfill('0') << hex << l_ui64Identifier << dec << "]\n";
		m_iDepth++;
	}

	virtual void processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize)
	{
		for(int i=0; i<m_iDepth; i++) cout << "   ";
		if(m_oCurrentIdentifier==EBML_Identifier_DocType)
			cout << "Got doc type : [" << m_oReaderHelper.getASCIIStringFromChildData(pBuffer, ui64BufferSize) << "]\n";
		else if(m_oCurrentIdentifier==EBML_Identifier_DocTypeVersion)
			cout << "Got doc type version : [0x" << setw(16) << setfill('0') << hex << m_oReaderHelper.getUIntegerFromChildData(pBuffer, ui64BufferSize) << dec << "]\n";
		else if(m_oCurrentIdentifier==EBML_Identifier_DocTypeReadVersion)
			cout <<"Got doc type read version : [0x" << setw(16) << setfill('0') << hex << m_oReaderHelper.getUIntegerFromChildData(pBuffer, ui64BufferSize) << dec << "]\n";
		else if(m_oCurrentIdentifier==EBML::CIdentifier(0x1234))
			cout <<"Got doc type uinteger : [0x" << setw(16) << setfill('0') << hex << m_oReaderHelper.getUIntegerFromChildData(pBuffer, ui64BufferSize) << dec << "]\n";
		else if(m_oCurrentIdentifier==EBML::CIdentifier(0xffffffffffffffffLL))
			cout <<"Got doc type uinteger : [0x" << setw(16) << setfill('0') << hex << m_oReaderHelper.getUIntegerFromChildData(pBuffer, ui64BufferSize) << dec << "]\n";
		else
			cout << "Got " << ui64BufferSize << " data bytes\n";
	}

	virtual void closeChild(void)
	{
		m_iDepth--;
		for(int i=0; i<m_iDepth; i++) cout << "   ";
		cout << "Node closed\n";
	}

	int m_iDepth;
	EBML::CReaderHelper m_oReaderHelper;
	EBML::CIdentifier m_oCurrentIdentifier;
};

int main(int argc, char** argv)
{
	if(argc<2)
	{
		cout << "syntax : " << argv[0] << " filein.ebml\n";
		return -1;
	}

	CReaderCallBack cb;
	EBML::IReader* l_pReader=EBML::createReader(cb);

	FILE* f=fopen(argv[1], "rb");
	unsigned char c[13];
	int i=0;
	while(!feof(f))
	{
		i=fread(c, 1, sizeof(c), f);
		cout << "\n --> reader has read " << dec << i << " bytes\n\n";
		l_pReader->processData(c, i);
	}
	fclose(f);

	return 0;
}
