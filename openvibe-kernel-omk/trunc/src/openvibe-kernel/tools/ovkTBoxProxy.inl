#include "ovkTBoxProxy.h"

namespace OpenViBE {
namespace Tools {

//___________________________________________________________________//
//                                                                   //

template <class CParentBox>
TBoxProxy<CParentBox>::TBoxProxy(const Kernel::IKernelContext& rKernelContext, const Kernel::IBox& rBox)
	:CParentBox(rKernelContext)
	,m_pBox(NULL)
	,m_bCanModify(false)
	,m_bCanModifyIdentifier(false)
	,m_bCanModifyAlgorithmClassIdentifier(false)
	,m_bCanModifyProcessingUnitIdentifier(false)
	,m_bCanModifyName(false)
	,m_bCanModifyInput(false)
	,m_bCanModifyOutput(false)
	,m_bCanModifySetting(false)
	,m_bCanModifyAttribute(false)
{
	m_pBox=const_cast<Kernel::IBox*>(&rBox);
}

template <class CParentBox>
TBoxProxy<CParentBox>::TBoxProxy(const Kernel::IKernelContext& rKernelContext, Kernel::IBox& rBox)
	:CParentBox(rKernelContext)
	,m_pBox(&rBox)
	,m_bCanModify(true)
	,m_bCanModifyIdentifier(true)
	,m_bCanModifyAlgorithmClassIdentifier(true)
	,m_bCanModifyProcessingUnitIdentifier(true)
	,m_bCanModifyName(true)
	,m_bCanModifyInput(true)
	,m_bCanModifyOutput(true)
	,m_bCanModifySetting(true)
	,m_bCanModifyAttribute(true)
{
}

//___________________________________________________________________//
//                                                                   //

template <class CParentBox>
boolean TBoxProxy<CParentBox>::getCanModifyIdentifierFlag(void)
{
	return m_bCanModifyIdentifier;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::getCanModifyAlgorithmClassIdentifierFlag(void)
{
	return m_bCanModifyAlgorithmClassIdentifier;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::getCanModifyProcessingUnitIdentifierFlag(void)
{
	return m_bCanModifyProcessingUnitIdentifier;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::getCanModifyNameFlag(void)
{
	return m_bCanModifyName;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::getCanModifyInputFlag(void)
{
	return m_bCanModifyInput;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::getCanModifyOutputFlag(void)
{
	return m_bCanModifyOutput;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::getCanModifySettingFlag(void)
{
	return m_bCanModifySetting;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::getCanModifyAttributeFlag(void)
{
	return m_bCanModifyAttribute;
}

//___________________________________________________________________//
//                                                                   //

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setCanModifyIdentifierFlag(boolean bValue)
{
	if(m_bCanModify)
	{
		return false;
	}
	m_bCanModifyIdentifier=bValue;
	return true;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setCanModifyAlgorithmClassIdentifierFlag(boolean bValue)
{
	if(m_bCanModify)
	{
		return false;
	}
	m_bCanModifyAlgorithmClassIdentifier=bValue;
	return true;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setCanModifyProcessingUnitIdentifierFlag(boolean bValue)
{
	if(m_bCanModify)
	{
		return false;
	}
	m_bCanModifyProcessingUnitIdentifier=bValue;
	return true;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setCanModifyNameFlag(boolean bValue)
{
	if(m_bCanModify)
	{
		return false;
	}
	m_bCanModifyName=bValue;
	return true;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setCanModifyInputFlag(boolean bValue)
{
	if(m_bCanModify)
	{
		return false;
	}
	m_bCanModifyInput=bValue;
	return true;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setCanModifyOutputFlag(boolean bValue)
{
	if(m_bCanModify)
	{
		return false;
	}
	m_bCanModifyOutput=bValue;
	return true;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setCanModifySettingFlag(boolean bValue)
{
	if(m_bCanModify)
	{
		return false;
	}
	m_bCanModifySetting=bValue;
	return true;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setCanModifyAttributeFlag(boolean bValue)
{
	if(m_bCanModify)
	{
		return false;
	}
	m_bCanModifyAttribute=bValue;
	return true;
}

//___________________________________________________________________//
//                                                                   //

template <class CParentBox>
CIdentifier TBoxProxy<CParentBox>::getIdentifier(void) const
{
	return m_pBox->getIdentifier();
}

template <class CParentBox>
CString TBoxProxy<CParentBox>::getName(void) const
{
	return m_pBox->getName();
}

template <class CParentBox>
CIdentifier TBoxProxy<CParentBox>::getAlgorithmClassIdentifier(void) const
{
	return m_pBox->getAlgorithmClassIdentifier();
}

template <class CParentBox>
CIdentifier TBoxProxy<CParentBox>::getProcessingUnitIdentifier(void) const
{
	return m_pBox->getProcessingUnitIdentifier();
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setIdentifier(
	const CIdentifier& rIdentifier)
{
	return (m_bCanModify&&m_bCanModifyIdentifier)?m_pBox->setIdentifier(rIdentifier):false;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setName(
	const CString& sName)
{
	return (m_bCanModify&&m_bCanModifyName)?m_pBox->setName(sName):false;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setAlgorithmClassIdentifier(
	const CIdentifier& rAlgorithmClassIdentifier)
{
	return (m_bCanModify&&m_bCanModifyAlgorithmClassIdentifier)?m_pBox->setAlgorithmClassIdentifier(rAlgorithmClassIdentifier):false;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setProcessingUnitIdentifier(
	const CIdentifier& rProcessingUnitIdentifier)
{
	return (m_bCanModify&&m_bCanModifyProcessingUnitIdentifier)?m_pBox->setAlgorithmClassIdentifier(rProcessingUnitIdentifier):false;
}

//___________________________________________________________________//
//                                                                   //

template <class CParentBox>
boolean TBoxProxy<CParentBox>::initializeFromAlgorithmClassIdentifier(const CIdentifier& rAlgorithmClassIdentifier)
{
	return (m_bCanModify
		&&m_bCanModifyAlgorithmClassIdentifier
		&&m_bCanModifyName
		&&m_bCanModifyInput
		&&m_bCanModifyOutput
		&&m_bCanModifySetting
		&&m_bCanModifyAttribute)?m_pBox->initializeFromAlgorithmClassIdentifier(rAlgorithmClassIdentifier):false;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::initializeFromExistingBox(const Kernel::IBox& rExisitingBox)
{
	return (m_bCanModify
		&&m_bCanModifyAlgorithmClassIdentifier
		&&m_bCanModifyName
		&&m_bCanModifyInput
		&&m_bCanModifyOutput
		&&m_bCanModifySetting
		&&m_bCanModifyAttribute)?m_pBox->initializeFromExistingBox(rExisitingBox):false;
}

//___________________________________________________________________//
//                                                                   //

template <class CParentBox>
boolean TBoxProxy<CParentBox>::addInput(
	const CString& sName,
	const CIdentifier& rTypeIdentifier)
{
	return (m_bCanModify&&m_bCanModifyInput)?m_pBox->addInput(sName, rTypeIdentifier):false;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::removeInput(
	const uint32 ui32InputIndex)
{
	return (m_bCanModify&&m_bCanModifyInput)?m_pBox->removeInput(ui32InputIndex):false;
}

template <class CParentBox>
uint32 TBoxProxy<CParentBox>::getInputCount(void) const
{
	return m_pBox->getInputCount();
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::getInputType(
	const uint32 ui32InputIndex,
	CIdentifier& rTypeIdentifier) const
{
	return m_pBox->getInputType(ui32InputIndex, rTypeIdentifier);
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::getInputName(
	const uint32 ui32InputIndex,
	CString& rName) const
{
	return m_pBox->getInputName(ui32InputIndex, rName);
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setInputType(
	const uint32 ui32InputIndex,
	const CIdentifier& rTypeIdentifier)
{
	return (m_bCanModify&&m_bCanModifyInput)?m_pBox->setInputType(ui32InputIndex, rTypeIdentifier):false;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setInputName(
	const uint32 ui32InputIndex,
	const CString& rName)
{
	return (m_bCanModify&&m_bCanModifyInput)?m_pBox->setInputName(ui32InputIndex, rName):false;
}

//___________________________________________________________________//
//                                                                   //

template <class CParentBox>
boolean TBoxProxy<CParentBox>::addOutput(
	const CString& sName,
	const CIdentifier& rTypeIdentifier)
{
	return (m_bCanModify&&m_bCanModifyOutput)?m_pBox->addOutput(sName, rTypeIdentifier):false;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::removeOutput(
	const uint32 ui32OutputIndex)
{
	return (m_bCanModify&&m_bCanModifyOutput)?m_pBox->removeOutput(ui32OutputIndex):false;
}

template <class CParentBox>
uint32 TBoxProxy<CParentBox>::getOutputCount(void) const
{
	return m_pBox->getOutputCount();
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::getOutputType(
	const uint32 ui32OutputIndex,
	CIdentifier& rTypeIdentifier) const
{
	return m_pBox->getOutputType(ui32OutputIndex, rTypeIdentifier);
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::getOutputName(
	const uint32 ui32OutputIndex,
	CString& rName) const
{
	return m_pBox->getOutputName(ui32OutputIndex, rName);
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setOutputType(
	const uint32 ui32OutputIndex,
	const CIdentifier& rTypeIdentifier)
{
	return (m_bCanModify&&m_bCanModifyOutput)?setOutputType(ui32OutputIndex, rTypeIdentifier):false;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setOutputName(
	const uint32 ui32OutputIndex,
	const CString& rName)
{
	return (m_bCanModify&&m_bCanModifyOutput)?setOutputName(ui32OutputIndex, rName):false;
}

//___________________________________________________________________//
//                                                                   //

template <class CParentBox>
boolean TBoxProxy<CParentBox>::addSetting(
	const CString& sName,
	const CIdentifier& rTypeIdentifier,
	const CString& sDefaultValue)
{
	return (m_bCanModify&&m_bCanModifySetting)?m_pBox->addSetting(sName, rTypeIdentifier, sDefaultValue):false;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::removeSetting(
	const uint32 ui32Index)
{
	return (m_bCanModify&&m_bCanModifySetting)?m_pBox->removeSetting(ui32Index):false;
}

template <class CParentBox>
uint32 TBoxProxy<CParentBox>::getSettingCount(void) const
{
	return m_pBox->getSettingCount();
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::getSettingType(
	const uint32 ui32SettingIndex,
	CIdentifier& rTypeIdentifier) const
{
	return m_pBox->getSettingType(ui32SettingIndex, rTypeIdentifier);
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::getSettingName(
	const uint32 ui32SettingIndex,
	CString& rName) const
{
	return m_pBox->getSettingName(ui32SettingIndex, rName);
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::getSettingDefaultValue(
	const uint32 ui32SettingIndex,
	CString& rDefaultValue) const
{
	return m_pBox->getSettingDefaultValue(ui32SettingIndex, rDefaultValue);
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::getSettingValue(
	const uint32 ui32SettingIndex,
	CString& rValue) const
{
	return m_pBox->getSettingValue(ui32SettingIndex, rValue);
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setSettingType(
	const uint32 ui32SettingIndex,
	const CIdentifier& rTypeIdentifier)
{
	return (m_bCanModify&&m_bCanModifySetting)?m_pBox->setSettingType(ui32SettingIndex, rTypeIdentifier):false;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setSettingName(
	const uint32 ui32SettingIndex,
	const CString& rName)
{
	return (m_bCanModify&&m_bCanModifySetting)?m_pBox->setSettingName(ui32SettingIndex, rName):false;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setSettingDefaultValue(
	const uint32 ui32SettingIndex,
	const CString& rDefaultValue)
{
	return  (m_bCanModify&&m_bCanModifySetting)?m_pBox->setSettingDefaultValue(ui32SettingIndex, rDefaultValue):false;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setSettingValue(
	const uint32 ui32SettingIndex,
	const CString& rValue)
{
	return  (m_bCanModify&&m_bCanModifySetting)?m_pBox->setSettingValue(ui32SettingIndex, rValue):false;
}

//___________________________________________________________________//
//                                                                   //

template <class CParentBox>
boolean TBoxProxy<CParentBox>::addAttribute(const CIdentifier& rAttributeIdentifier, const CString& sAttributeValue)
{
	return (m_bCanModify&&m_bCanModifyAttribute)?m_pBox->addAttribute(rAttributeIdentifier, sAttributeValue):false;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::removeAttribute(const CIdentifier& rAttributeIdentifier)
{
	return (m_bCanModify&&m_bCanModifyAttribute)?m_pBox->removeAttribute(rAttributeIdentifier):false;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::removeAllAttributes(void)
{
	return (m_bCanModify&&m_bCanModifyAttribute)?m_pBox->removeAllAttributes():false;
}

template <class CParentBox>
CString TBoxProxy<CParentBox>::getAttributeValue(const CIdentifier& rAttributeIdentifier) const
{
	return m_pBox->getAttributeValue(rAttributeIdentifier);
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::setAttributeValue(const CIdentifier& rAttributeIdentifier, const CString& sAttributeValue)
{
	return (m_bCanModify&&m_bCanModifyAttribute)?m_pBox->setAttributeValue(rAttributeIdentifier, sAttributeValue):false;
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::hasAttribute(const CIdentifier& rAttributeIdentifier) const
{
	return m_pBox->hasAttribute(rAttributeIdentifier);
}

template <class CParentBox>
boolean TBoxProxy<CParentBox>::hasAttributes(void) const
{
	return m_pBox->hasAttributes();
}

#if 0
template <class CParentBox>
boolean TBoxProxy<CParentBox>::enumerateAttributes(Kernel::IAttributable::IAttributeEnum& rCallback) const
{
	return (m_bCanModify&&m_bCanModifyAttribute)?m_pBox->enumerateAttributes(rCallback):false;
}
#endif

template <class CParentBox>
CIdentifier TBoxProxy<CParentBox>::getNextAttributeIdentifier(const CIdentifier& rPreviousIdentifier) const
{
	return m_pBox->getNextAttributeIdentifier(rPreviousIdentifier);
}

//___________________________________________________________________//
//                                                                   //

};
};
