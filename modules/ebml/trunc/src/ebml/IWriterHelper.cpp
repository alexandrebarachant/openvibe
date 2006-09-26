#include "IWriterHelper.h"
#include "IWriter.h"

#include <string.h>

using namespace EBML;

// ________________________________________________________________________________________________________________
//

namespace EBML
{
	class CWriterHelper : virtual public IWriterHelper
	{
	public:
		CWriterHelper(void);

		virtual boolean connect(IWriter* pWriter);
		virtual boolean disconnect(void);

		virtual boolean openChild(const CIdentifier& rIdentifier);
		virtual boolean closeChild(void);

		virtual boolean setSIntegerAsChildData(const int64 iValue);
		virtual boolean setUIntegerAsChildData(const uint64 uiValue);
		virtual boolean setFloat32AsChildData(const float32 fValue);
		virtual boolean setFloat64AsChildData(const float64 fValue);
		virtual boolean setBinaryAsChildData(const void* pBuffer, const uint64 ui64BufferLength);
		virtual boolean setASCIIStringAsChildData(const char* sValue);

		virtual void release(void);

	protected:
		IWriter* m_pWriter;
	};
};

// ________________________________________________________________________________________________________________
//

CWriterHelper::CWriterHelper(void)
	:m_pWriter(0)
{
}

// ________________________________________________________________________________________________________________
//

boolean CWriterHelper::connect(IWriter* pWriter)
{
	m_pWriter=pWriter;
	return m_pWriter?true:false;
}

boolean CWriterHelper::disconnect(void)
{
	if(!m_pWriter)
	{
		return false;
	}
	m_pWriter=0;
	return true;
}

// ________________________________________________________________________________________________________________
//

boolean CWriterHelper::openChild(const CIdentifier& rIdentifier)
{
	return m_pWriter?m_pWriter->openChild(rIdentifier):false;
}

boolean CWriterHelper::closeChild(void)
{
	return m_pWriter?m_pWriter->closeChild():false;
}

// ________________________________________________________________________________________________________________
//

boolean CWriterHelper::setSIntegerAsChildData(const int64 iValue)
{
	uint64 i;
	uint64 l_ui64BufferSize=0;
	unsigned char l_pBuffer[8];

	if(iValue==0)
		l_ui64BufferSize=0;
	else if(iValue>=-0x80 && iValue<=0x7f)
		l_ui64BufferSize=1;
	else if(iValue>=-0x8000 && iValue<=0x7fff)
		l_ui64BufferSize=2;
	else if(iValue>=-0x800000 && iValue<=0x7fffff)
		l_ui64BufferSize=3;
	else if(iValue>=-0x80000000 && iValue<=0x7fffffff)
		l_ui64BufferSize=4;
/*
	else if(iValue>=-0x8000000000 && iValue<=0x7fffffffff)
		l_ui64BufferSize=5;
	else if(iValue>=-0x800000000000 && iValue<=0x7fffffffffff)
		l_ui64BufferSize=6;
	else if(iValue>=-0x80000000000000 && iValue<=0x7fffffffffffff)
		l_ui64BufferSize=7;
*/
	else
		l_ui64BufferSize=8;

	for(i=0; i<l_ui64BufferSize; i++)
	{
		l_pBuffer[l_ui64BufferSize-i-1]=(unsigned char)((iValue>>(i*8))&0xff);
	}

	return m_pWriter->setChildData(l_pBuffer, l_ui64BufferSize);
}

boolean CWriterHelper::setUIntegerAsChildData(const uint64 uiValue)
{
	uint64 i;
	uint64 l_ui64BufferSize=0;
	unsigned char l_pBuffer[8];

	if(uiValue==0)
		l_ui64BufferSize=0;
	else if(uiValue<0x100)
		l_ui64BufferSize=1;
	else if(uiValue<0x10000)
		l_ui64BufferSize=2;
	else if(uiValue<0x1000000)
		l_ui64BufferSize=3;
/*
	else if(uiValue<0x100000000)
		l_ui64BufferSize=4;
	else if(uiValue<0x10000000000)
		l_ui64BufferSize=5;
	else if(uiValue<0x1000000000000)
		l_ui64BufferSize=6;
	else if(uiValue<0x100000000000000)
		l_ui64BufferSize=7;
*/
	else
		l_ui64BufferSize=8;

	for(i=0; i<l_ui64BufferSize; i++)
	{
		l_pBuffer[l_ui64BufferSize-i-1]=(unsigned char)((uiValue>>(i*8))&0xff);
	}
	return m_pWriter->setChildData(l_pBuffer, l_ui64BufferSize);
}

boolean CWriterHelper::setFloat32AsChildData(const float32 fValue)
{
	uint32* l_pValue=(uint32*)&fValue;
	uint64 i;
	uint64 l_ui64BufferSize=0;
	unsigned char l_pBuffer[8];

	l_ui64BufferSize=(fValue!=0?4:0);
	for(i=0; i<l_ui64BufferSize; i++)
	{
		l_pBuffer[l_ui64BufferSize-i-1]=(unsigned char)(((*l_pValue)>>i)&0xff);
	}
	return m_pWriter->setChildData(l_pBuffer, l_ui64BufferSize);
}

boolean CWriterHelper::setFloat64AsChildData(const float64 fValue)
{
	uint64* l_pValue=(uint64*)&fValue;
	uint64 i;
	uint64 l_ui64BufferSize=0;
	unsigned char l_pBuffer[8];

	l_ui64BufferSize=(fValue!=0?8:0);
	for(i=0; i<l_ui64BufferSize; i++)
	{
		l_pBuffer[l_ui64BufferSize-i-1]=(unsigned char)(((*l_pValue)>>i)&0xff);
	}
	return m_pWriter->setChildData(l_pBuffer, l_ui64BufferSize);
}

boolean CWriterHelper::setBinaryAsChildData(const void* pBuffer, const uint64 ui64BufferLength)
{
	return m_pWriter->setChildData(pBuffer, ui64BufferLength);
}

boolean CWriterHelper::setASCIIStringAsChildData(const char* sValue)
{
	return m_pWriter->setChildData(sValue, strlen(sValue));
}

// ________________________________________________________________________________________________________________
//

void CWriterHelper::release(void)
{
	delete this;
}

// ________________________________________________________________________________________________________________
//

EBML_API IWriterHelper* EBML::createWriterHelper(void)
{
	return new CWriterHelper();
}
