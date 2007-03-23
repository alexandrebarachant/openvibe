#include "ovkCBuffer.h"

#include <system/Memory.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Kernel::Player;
#define boolean OpenViBE::boolean

CBuffer::CBuffer(void)
	:m_pBuffer(NULL)
	,m_ui64BufferSize(0)
	,m_ui64AllocatedSize(0)
{
}

CBuffer::CBuffer(
	const CBuffer& rBuffer)
	:m_pBuffer(NULL)
	,m_ui64BufferSize(0)
	,m_ui64AllocatedSize(0)
{
	if(rBuffer.m_ui64BufferSize)
	{
		m_pBuffer=new uint8[static_cast<size_t>(rBuffer.m_ui64BufferSize)]; // $$$
		System::Memory::copy(m_pBuffer, rBuffer.m_pBuffer, rBuffer.m_ui64BufferSize);
		m_ui64BufferSize=rBuffer.m_ui64BufferSize;
		m_ui64AllocatedSize=rBuffer.m_ui64BufferSize;
	}
}

CBuffer::CBuffer(
	const uint8* pBuffer,
	const uint64 ui64BufferSize)
	:m_pBuffer(NULL)
	,m_ui64BufferSize(0)
	,m_ui64AllocatedSize(0)
{
	if(ui64BufferSize)
	{
		m_pBuffer=new uint8[static_cast<size_t>(ui64BufferSize)]; // $$$
		System::Memory::copy(m_pBuffer, pBuffer, ui64BufferSize);
		m_ui64BufferSize=ui64BufferSize;
		m_ui64AllocatedSize=ui64BufferSize;
	}
}

const CBuffer& CBuffer::operator=(
	const CBuffer& rBuffer)
{
	if(rBuffer.m_ui64BufferSize>m_ui64AllocatedSize)
	{
		delete [] m_pBuffer;
		m_pBuffer=new uint8[static_cast<size_t>(rBuffer.m_ui64BufferSize)]; // $$$
		m_ui64AllocatedSize=rBuffer.m_ui64BufferSize;
	}
	System::Memory::copy(m_pBuffer, rBuffer.m_pBuffer, rBuffer.m_ui64BufferSize);
	m_ui64BufferSize=rBuffer.m_ui64BufferSize;
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

uint64 CBuffer::getSize(void) const
{
	return m_ui64BufferSize;
}

boolean CBuffer::setSize(
	const uint64 ui64Size,
	const boolean bDiscard)
{
	if(ui64Size>m_ui64AllocatedSize)
	{
		uint8* l_pSavedBuffer=m_pBuffer;
		m_pBuffer=new uint8[static_cast<size_t>(ui64Size)]; // $$$
		if(!bDiscard)
		{
			System::Memory::copy(m_pBuffer, l_pSavedBuffer, m_ui64BufferSize);
		}
		delete [] l_pSavedBuffer;
		m_ui64AllocatedSize=ui64Size;
	}
	m_ui64BufferSize=ui64Size;
	return true;
}

boolean CBuffer::appendOutputChunkData(
	const uint8* pBuffer,
	const uint64 ui64BufferSize)
{
	uint64 l_ui64LastBufferSize=m_ui64BufferSize;
	if(!setSize(l_ui64LastBufferSize+ui64BufferSize, false))
	{
		return false;
	}
	System::Memory::copy(m_pBuffer+l_ui64LastBufferSize, pBuffer, ui64BufferSize);
	return true;
}
