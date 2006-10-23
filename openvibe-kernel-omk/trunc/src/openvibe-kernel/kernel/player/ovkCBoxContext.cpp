#include "ovkCBoxContext.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

//___________________________________________________________________//
//                                                                   //

CBoxContext::CInput::CInput(::PsSimulatedBox& rSimulatedBox, const IBox& rBox, const uint32 ui32InputIndex)
	:m_rSimulatedBox(rSimulatedBox)
	,m_rBox(rBox)
	,m_ui32InputIndex(ui32InputIndex)
{
}

CString CBoxContext::CInput::getName(void)
{
	CString l_oResult;
	m_rBox.getInputName(m_ui32InputIndex, l_oResult);
	return l_oResult;
}

CIdentifier CBoxContext::CInput::getTypeIdentifier(void)
{
	CIdentifier l_oResult;
	m_rBox.getInputType(m_ui32InputIndex, l_oResult);
	return l_oResult;
}

uint32 CBoxContext::CInput::getChunkCount(void)
{
	return m_rSimulatedBox.getInputChunkCount(m_ui32InputIndex);
}

boolean CBoxContext::CInput::getChunk(
	const uint32 ui32Index,
	uint64& rStartTime,
	uint64& rEndTime,
	uint64& rChunkSize,
	const uint8*& rpChunkBuffer)
{
	return m_rSimulatedBox.getInputChunk(m_ui32InputIndex, ui32Index, rStartTime, rEndTime, rChunkSize, rpChunkBuffer);
}

boolean CBoxContext::CInput::markAsDeprecated(
	const uint32 ui32Index)
{
	return m_rSimulatedBox.markInputAsDeprecated(m_ui32InputIndex, ui32Index);
}

#if 0
boolean CBoxContext::CInput::getSource(
	CIdentifier& rBoxIdentifier,
	uint32& rBoxOutputIndex)
{
	return m_rSimulatedBox.getInputSource(m_ui32InputIndex, rBoxIdentifier, rBoxOutputIndex);
}
#endif

boolean CBoxContext::CInput::setInputIndex(
	const uint32 ui32InputIndex)
{
	m_ui32InputIndex=ui32InputIndex;
	return true;
}

//___________________________________________________________________//
//                                                                   //

CBoxContext::COutput::COutput(::PsSimulatedBox& rSimulatedBox, const IBox& rBox, const uint32 ui32OutputIndex)
	:m_rSimulatedBox(rSimulatedBox)
	,m_rBox(rBox)
	,m_ui32OutputIndex(ui32OutputIndex)
{
}

CString CBoxContext::COutput::getName(void)
{
	CString l_oResult;
	m_rBox.getOutputName(m_ui32OutputIndex, l_oResult);
	return l_oResult;
}

CIdentifier CBoxContext::COutput::getTypeIdentifier(void)
{
	CIdentifier l_oResult;
	m_rBox.getOutputType(m_ui32OutputIndex, l_oResult);
	return l_oResult;
}

uint64 CBoxContext::COutput::getChunkSize(void)
{
	return m_rSimulatedBox.getOutputChunkSize(m_ui32OutputIndex);
}

boolean CBoxContext::COutput::setChunkSize(
	const uint64 ui64Size,
	const boolean bDiscard)
{
	return m_rSimulatedBox.setOutputChunkSize(m_ui32OutputIndex, ui64Size, bDiscard);
}

uint8* CBoxContext::COutput::getChunkBuffer(void)
{
	return m_rSimulatedBox.getOutputChunkBuffer(m_ui32OutputIndex);
}

boolean CBoxContext::COutput::markAsReadyToSend(
	const uint64 ui64StartTime,
	const uint64 ui64EndTime)
{
	return m_rSimulatedBox.markOutputAsReadyToSend(m_ui32OutputIndex, ui64StartTime, ui64EndTime);
}

/*
uint32 CBoxContext::COutput::getTargetCount(void)
{
	return m_rSimulatedBox.getOutputTargetCount(m_ui32OutputIndex);
}

boolean CBoxContext::COutput::getTarget(
	const uint32 ui32TargetIndex,
	CIdentifier& rBoxIdentifier,
	uint32& rBoxInputIndex)
{
	return m_rSimulatedBox.getOutputTarget(m_ui32OutputIndex, ui32TargetIndex, rBoxIdentifier, rBoxInputIndex);
}
*/

boolean CBoxContext::COutput::setOutputIndex(
	const uint32 ui32OutputIndex)
{
	m_ui32OutputIndex=ui32OutputIndex;
	return true;
}

//___________________________________________________________________//
//                                                                   //

CBoxContext::CBoxContext(::PsSimulatedBox& rSimulatedBox, const IBox& rBox)
	:TPluginObjectContext<IBoxContext>()
	,m_rSimulatedBox(rSimulatedBox)
	,m_rBox(rBox)
	,m_oInput(rSimulatedBox, rBox, 0xffffffff)
	,m_oOutput(rSimulatedBox, rBox, 0xffffffff)
{
}

//___________________________________________________________________//
//                                                                   //

CIdentifier CBoxContext::getIdentifier(void) const
{
	return m_rBox.getIdentifier();
}

CString CBoxContext::getName(void) const
{
	return m_rBox.getName();
}

uint32 CBoxContext::getInputCount(void) const
{
	return m_rBox.getInputCount();
}

IBoxContext::IInput* CBoxContext::getInput(
	const uint32 ui32InputIndex)
{
	if(ui32InputIndex>=m_rBox.getInputCount())
	{
		return NULL;
	}
	m_oInput.setInputIndex(ui32InputIndex);
	return &m_oInput;
}

//___________________________________________________________________//
//                                                                   //

uint32 CBoxContext::getOutputCount(void) const
{
	return m_rBox.getOutputCount();
}

IBoxContext::IOutput* CBoxContext::getOutput(
	const uint32 ui32OutputIndex)
{
	if(ui32OutputIndex>=m_rBox.getOutputCount())
	{
		return NULL;
	}
	m_oOutput.setOutputIndex(ui32OutputIndex);
	return &m_oOutput;
}

//___________________________________________________________________//
//                                                                   //

uint32 CBoxContext::getSettingCount(void) const
{
	return m_rBox.getSettingCount();
}

boolean CBoxContext::getSettingType(
	const uint32 ui32SettingIndex,
	CIdentifier& rTypeIdentifier) const
{
	return m_rBox.getSettingType(ui32SettingIndex, rTypeIdentifier);
}

boolean CBoxContext::getSettingValue(
	const uint32 ui32SettingIndex,
	CString& rValue) const
{
	return m_rBox.getSettingValue(ui32SettingIndex, rValue);
}
