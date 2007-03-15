#include <stdio.h>

#include "ebml/defines.h"
#include "ebml/IWriter.h"
#include "ebml/IWriterHelper.h"

class CWriterCallBack : public EBML::IWriterCallBack
{
public:
	CWriterCallBack(char* filename)
	{
		f=fopen(filename, "wb");
	}

	virtual ~CWriterCallBack(void)
	{
		if(f) fclose(f);
	}

	virtual void write(const void* pBuffer, const EBML::uint64 ui64BufferSize)
	{
		if(f) fwrite(pBuffer, ui64BufferSize, 1, f);
	}

	FILE* f;
};

int main(int argc, char** argv)
{
	if(argc<2)
	{
		printf("syntax : %s fileout.ebml\n", argv[0]);
		return -1;
	}

	CWriterCallBack cb(argv[1]);
	EBML::IWriter* l_pWriter=EBML::createWriter(cb);
	EBML::IWriterHelper* l_pWriterHelper=EBML::createWriterHelper();

	l_pWriterHelper->connect(l_pWriter);

	l_pWriterHelper->openChild(EBML_Identifier_Header);

		l_pWriterHelper->openChild(EBML_Identifier_DocType);
		l_pWriterHelper->setASCIIStringAsChildData("matroska");
		l_pWriterHelper->closeChild();

		l_pWriterHelper->openChild(0xffff);

			l_pWriterHelper->openChild(EBML_Identifier_DocTypeVersion);
			l_pWriterHelper->setUIntegerAsChildData(1);
			l_pWriterHelper->closeChild();

			l_pWriterHelper->openChild(EBML_Identifier_DocTypeReadVersion);
			l_pWriterHelper->setSIntegerAsChildData(655356);
			l_pWriterHelper->closeChild();

		l_pWriterHelper->closeChild();

		l_pWriterHelper->openChild(0x1234);
		l_pWriterHelper->setUIntegerAsChildData(0);
		l_pWriterHelper->closeChild();

		l_pWriterHelper->openChild(0xffffffffffffffffLL);
		l_pWriterHelper->setUIntegerAsChildData(0xff000000ff000000LL);
		l_pWriterHelper->closeChild();

	l_pWriterHelper->closeChild();

	l_pWriterHelper->release();

	return 0;
}
