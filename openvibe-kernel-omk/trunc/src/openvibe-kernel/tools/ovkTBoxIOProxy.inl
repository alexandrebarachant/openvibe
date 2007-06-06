#include "ovkTBoxIOProxy.h"

namespace OpenViBE {
namespace Tools {

//___________________________________________________________________//
//                                                                   //

template <class CParentBoxIO>
TBoxIOProxy<CParentBoxIO>::TBoxIOProxy(const Kernel::IKernelContext& rKernelContext, const Kernel::IBoxIO& rBoxIO)
	:CParentBoxIO(rKernelContext)
	,m_pBoxIO(NULL)
	,m_bCanModify(false)
	,m_bCanModifyInput(false)
	,m_bCanModifyOutput(false)
{
	m_pBoxIO=const_cast<Kernel::IBoxIO*>(&rBoxIO);
}

template <class CParentBoxIO>
TBoxIOProxy<CParentBoxIO>::TBoxIOProxy(const Kernel::IKernelContext& rKernelContext, Kernel::IBoxIO& rBoxIO)
	:CParentBoxIO(rKernelContext)
	,m_pBoxIO(&rBoxIO)
	,m_bCanModify(true)
	,m_bCanModifyInput(true)
	,m_bCanModifyOutput(true)
{
}

//___________________________________________________________________//
//                                                                   //

template <class CParentBoxIO>
boolean TBoxIOProxy<CParentBoxIO>::getCanModifyInputFlag(void) const
{
	return m_bCanModifyInput;
}

template <class CParentBoxIO>
boolean TBoxIOProxy<CParentBoxIO>::getCanModifyOutputFlag(void) const
{
	return m_bCanModifyOutput;
}

//___________________________________________________________________//
//                                                                   //

template <class CParentBoxIO>
boolean TBoxIOProxy<CParentBoxIO>::setCanModifyInputFlag(boolean bValue)
{
	if(!m_bCanModify)
	{
		return false;
	}
	m_bCanModifyInput=bValue;
	return true;
}

template <class CParentBoxIO>
boolean TBoxIOProxy<CParentBoxIO>::setCanModifyOutputFlag(boolean bValue)
{
	if(!m_bCanModify)
	{
		return false;
	}
	m_bCanModifyOutput=bValue;
	return true;
}

//___________________________________________________________________//
//                                                                   //

template <class CParentBoxIO>
uint32 TBoxIOProxy<CParentBoxIO>::getInputChunkCount(
	const uint32 ui32InputIndex) const
{
	return m_pBoxIO->getInputChunkCount(ui32InputIndex);
}

template <class CParentBoxIO>
boolean TBoxIOProxy<CParentBoxIO>::getInputChunk(
	const uint32 ui32InputIndex,
	const uint32 ui32ChunkIndex,
	uint64& rStartTime,
	uint64& rEndTime,
	uint64& rChunkSize,
	const uint8*& rpChunkBuffer) const
{
	return m_pBoxIO->getInputChunk(ui32InputIndex, ui32ChunkIndex, rStartTime, rEndTime, rChunkSize, rpChunkBuffer);
}

template <class CParentBoxIO>
boolean TBoxIOProxy<CParentBoxIO>::markInputAsDeprecated(
	const uint32 ui32InputIndex,
	const uint32 ui32ChunkIndex)
{
	return m_pBoxIO->markInputAsDeprecated(ui32InputIndex, ui32ChunkIndex);
}

//___________________________________________________________________//
//                                                                   //

template <class CParentBoxIO>
uint64 TBoxIOProxy<CParentBoxIO>::getOutputChunkSize(
	const uint32 ui32OutputIndex) const
{
	return m_pBoxIO->getOutputChunkSize(ui32OutputIndex);
}

template <class CParentBoxIO>
boolean TBoxIOProxy<CParentBoxIO>::setOutputChunkSize(
	const uint32 ui32OutputIndex,
	const uint64 ui64Size,
	const boolean bDiscard)
{
	return m_pBoxIO->setOutputChunkSize(ui32OutputIndex, ui64Size, bDiscard);
}

template <class CParentBoxIO>
uint8* TBoxIOProxy<CParentBoxIO>::getOutputChunkBuffer(
	const uint32 ui32OutputIndex)
{
	return m_pBoxIO->getOutputChunkBuffer(ui32OutputIndex);
}

template <class CParentBoxIO>
boolean TBoxIOProxy<CParentBoxIO>::appendOutputChunkData(
	const uint32 ui32OutputIndex,
	const uint8* pBuffer,
	const uint64 ui64BufferSize)
{
	return m_pBoxIO->appendOutputChunkData(ui32OutputIndex, pBuffer, ui64BufferSize);
}

template <class CParentBoxIO>
boolean TBoxIOProxy<CParentBoxIO>::markOutputAsReadyToSend(
	const uint32 ui32OutputIndex,
	const uint64 ui64StartTime,
	const uint64 ui64EndTime)
{
	return m_pBoxIO->markOutputAsReadyToSend(ui32OutputIndex, ui64StartTime, ui64EndTime);
}

//___________________________________________________________________//
//                                                                   //

};
};
