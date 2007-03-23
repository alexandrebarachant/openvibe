#include "ovkCStaticBoxContext.h"

#include <assert.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
#define boolean OpenViBE::boolean

//___________________________________________________________________//
//                                                                   //

CStaticBoxContext::CStaticBoxContext(const IKernelContext& rKernelContext, const IBox* pBox)
	:TKernelObject<IStaticBoxContext>(rKernelContext)
	,TPluginObjectContext<IStaticBoxContext>(rKernelContext)
	,m_pBox(pBox)
{
}

//___________________________________________________________________//
//                                                                   //

CIdentifier CStaticBoxContext::getIdentifier(void) const
{
	assert(m_pBox);
	return m_pBox->getIdentifier();
}

CString CStaticBoxContext::getName(void) const
{
	assert(m_pBox);
	return m_pBox->getName();
}

//___________________________________________________________________//
//                                                                   //

uint32 CStaticBoxContext::getInputCount(void) const
{
	assert(m_pBox);
	return m_pBox->getInputCount();
}

boolean CStaticBoxContext::getInputType(
	const uint32 ui32InputIndex,
	CIdentifier& rTypeIdentifier) const
{
	assert(m_pBox);
	return m_pBox->getInputType(ui32InputIndex, rTypeIdentifier);
}

boolean CStaticBoxContext::getInputName(
	const uint32 ui32InputIndex,
	CString& rName) const
{
	assert(m_pBox);
	return m_pBox->getInputName(ui32InputIndex, rName);
}

//___________________________________________________________________//
//                                                                   //

uint32 CStaticBoxContext::getOutputCount(void) const
{
	assert(m_pBox);
	return m_pBox->getOutputCount();
}

boolean CStaticBoxContext::getOutputType(
	const uint32 ui32OutputIndex,
	CIdentifier& rTypeIdentifier) const
{
	assert(m_pBox);
	return m_pBox->getOutputType(ui32OutputIndex, rTypeIdentifier);
}

boolean CStaticBoxContext::getOutputName(
	const uint32 ui32OutputIndex,
	CString& rName) const
{
	assert(m_pBox);
	return m_pBox->getOutputName(ui32OutputIndex, rName);
}

//___________________________________________________________________//
//                                                                   //

uint32 CStaticBoxContext::getSettingCount(void) const
{
	assert(m_pBox);
	return m_pBox->getSettingCount();
}

boolean CStaticBoxContext::getSettingType(
	const uint32 ui32SettingIndex,
	CIdentifier& rTypeIdentifier) const
{
	assert(m_pBox);
	return m_pBox->getSettingType(ui32SettingIndex, rTypeIdentifier);
}

boolean CStaticBoxContext::getSettingValue(
	const uint32 ui32SettingIndex,
	CString& rValue) const
{
	assert(m_pBox);
	return m_pBox->getSettingValue(ui32SettingIndex, rValue);
}
