#include "CInputStreamFile.hpp"

using namespace Stream;

CInputStreamFile::CInputStreamFile(const char* sFilename)
	:m_sFilename(sFilename)
	,m_bIsOpened(false)
{
}

boolean CInputStreamFile::open(void)
{
	if(m_bIsOpened)
	{
		return false;
	}
	m_oFile.open(m_sFilename.c_str(), std::ios::binary);
	if(!m_oFile.is_open())
	{
		return false;
	}
	m_bIsOpened=true;
	return true;
}

boolean CInputStreamFile::isOpened(void)
{
	return m_bIsOpened;
}

boolean CInputStreamFile::close(void)
{
	if(!m_bIsOpened)
	{
		return false;
	}
	m_oFile.close();
	m_bIsOpened=false;
	return true;
}

uint64 CInputStreamFile::bufferize(uint8* pBuffer, uint64 ui64RequestedBufferSize)
{
	if(m_bIsOpened)
	{
		m_oFile.read(reinterpret_cast<char*>(pBuffer), ui64RequestedBufferSize);
		uint64 l_ui64Result=m_oFile.gcount();
		return l_ui64Result;
	}
	return 0;
}

IInputStream* Stream::createInputStreamFile(const char* sFilename)
{
	return new CInputStreamFile(sFilename);
}
