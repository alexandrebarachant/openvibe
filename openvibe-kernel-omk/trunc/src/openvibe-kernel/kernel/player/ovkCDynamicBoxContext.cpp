#include "ovkCDynamicBoxContext.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
#define boolean OpenViBE::boolean

//___________________________________________________________________//
//                                                                   //

CDynamicBoxContext::CDynamicBoxContext(::PsSimulatedBox* pSimulatedBox)
	:m_pSimulatedBox(pSimulatedBox)
{
}

//___________________________________________________________________//
//                                                                   //

uint32 CDynamicBoxContext::getInputChunkCount(
	const uint32 ui32InputIndex)
{
	assert(m_pSimulatedBox);
	return m_pSimulatedBox->getInputChunkCount(ui32InputIndex);
}

boolean CDynamicBoxContext::getInputChunk(
	const uint32 ui32InputIndex,
	const uint32 ui32ChunkIndex,
	uint64& rStartTime,
	uint64& rEndTime,
	uint64& rChunkSize,
	const uint8*& rpChunkBuffer)
{
	assert(m_pSimulatedBox);
	return m_pSimulatedBox->getInputChunk(ui32InputIndex, ui32ChunkIndex, rStartTime, rEndTime, rChunkSize, rpChunkBuffer);
}

boolean CDynamicBoxContext::markInputAsDeprecated(
	const uint32 ui32InputIndex,
	const uint32 ui32ChunkIndex)
{
	assert(m_pSimulatedBox);
	return m_pSimulatedBox->markInputAsDeprecated(ui32InputIndex, ui32ChunkIndex);
}

//___________________________________________________________________//
//                                                                   //

uint64 CDynamicBoxContext::getOutputChunkSize(
	const uint32 ui32OutputIndex)
{
	assert(m_pSimulatedBox);
	return m_pSimulatedBox->getOutputChunkSize(ui32OutputIndex);
}

boolean CDynamicBoxContext::setOutputChunkSize(
	const uint32 ui32OutputIndex,
	const uint64 ui64Size,
	const boolean bDiscard)
{
	assert(m_pSimulatedBox);
	return m_pSimulatedBox->setOutputChunkSize(ui32OutputIndex, ui64Size, bDiscard);
}

uint8* CDynamicBoxContext::getOutputChunkBuffer(
	const uint32 ui32OutputIndex)
{
	assert(m_pSimulatedBox);
	return m_pSimulatedBox->getOutputChunkBuffer(ui32OutputIndex);
}

boolean CDynamicBoxContext::appendOutputChunkData(
	const OpenViBE::uint32 ui32OutputIndex,
	const OpenViBE::uint8* pBuffer,
	const OpenViBE::uint64 ui64BufferSize)
{
	assert(m_pSimulatedBox);
	return m_pSimulatedBox->appendOutputChunkData(ui32OutputIndex, pBuffer, ui64BufferSize);
}

boolean CDynamicBoxContext::markOutputAsReadyToSend(
	const uint32 ui32OutputIndex,
	const uint64 ui64EndTime)
{
	assert(m_pSimulatedBox);
	return m_pSimulatedBox->markOutputAsReadyToSend(ui32OutputIndex, ui64EndTime);
}

//___________________________________________________________________//
//                                                                   //
