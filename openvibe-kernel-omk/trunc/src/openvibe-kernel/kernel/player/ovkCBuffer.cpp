#include "ovkCBuffer.h"

#include <system/Memory.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Kernel::Player;

CBuffer::CBuffer(void)
	:m_pBuffer(NULL)
	,m_ui32BufferSize(0)
	,m_ui32AllocatedSize(0)
{
}

CBuffer::CBuffer(
	const CBuffer& rBuffer)
	:m_pBuffer(NULL)
	,m_ui32BufferSize(0)
	,m_ui32AllocatedSize(0)
{
	if(rBuffer.m_ui32BufferSize)
	{
		m_pBuffer=new uint8[rBuffer.m_ui32BufferSize];
		System::Memory::copy(m_pBuffer, rBuffer.m_pBuffer, rBuffer.m_ui32BufferSize);
		m_ui32BufferSize=rBuffer.m_ui32BufferSize;
		m_ui32AllocatedSize=rBuffer.m_ui32BufferSize;
	}
}

CBuffer::CBuffer(
	const uint8* pBuffer,
	const uint32 ui32BufferSize)
	:m_pBuffer(NULL)
	,m_ui32BufferSize(0)
	,m_ui32AllocatedSize(0)
{
	if(ui32BufferSize)
	{
		m_pBuffer=new uint8[ui32BufferSize];
		System::Memory::copy(m_pBuffer, pBuffer, ui32BufferSize);
		m_ui32BufferSize=ui32BufferSize;
		m_ui32AllocatedSize=ui32BufferSize;
	}
}

const CBuffer& CBuffer::operator=(
	const CBuffer& rBuffer)
{
	if(rBuffer.m_ui32BufferSize>m_ui32AllocatedSize)
	{
		delete [] m_pBuffer;
		m_pBuffer=new uint8[rBuffer.m_ui32BufferSize];
		m_ui32AllocatedSize=rBuffer.m_ui32BufferSize;
	}
	System::Memory::copy(m_pBuffer, rBuffer.m_pBuffer, rBuffer.m_ui32BufferSize);
	m_ui32BufferSize=rBuffer.m_ui32BufferSize;
	return *this;
}

CBuffer::~CBuffer(void)
{
	delete [] m_pBuffer;
}

uint8* CBuffer::getDirectPointer(void)
{
	return m_pBuffer;
}

const uint8* CBuffer::getDirectPointer(void) const
{
	return m_pBuffer;
}

uint32 CBuffer::getSize(void) const
{
	return m_ui32BufferSize;
}

boolean CBuffer::setSize(
	uint32 ui32Size,
	boolean bDiscard)
{
	if(ui32Size>m_ui32AllocatedSize)
	{
		uint8* l_pSavedBuffer=m_pBuffer;
		m_pBuffer=new uint8[ui32Size];
		if(!bDiscard)
		{
			System::Memory::copy(m_pBuffer, l_pSavedBuffer, m_ui32BufferSize);
		}
		delete [] l_pSavedBuffer;
		m_ui32AllocatedSize=ui32Size;
	}
	m_ui32BufferSize=ui32Size;
	return true;
}
