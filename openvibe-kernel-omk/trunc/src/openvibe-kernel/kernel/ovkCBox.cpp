#include "ovkCBox.h"

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;

//___________________________________________________________________//
//                                                                   //

CBox::CBox(const IKernelContext& rKernelContext)
	:TKernelObject<TAttributable<IBox> >(rKernelContext)
	,m_oIdentifier(OV_UndefinedIdentifier)
	,m_oAlgorithmClassIdentifier(OV_UndefinedIdentifier)
	,m_sName("unnamed")
{
}

//___________________________________________________________________//
//                                                                   //

const CIdentifier& CBox::getIdentifier(void) const
{
	return m_oIdentifier;
}

const CString& CBox::getName(void) const
{
	return m_sName;
}

const CIdentifier& CBox::getAlgorithmClassIdentifier(void) const
{
	return m_oAlgorithmClassIdentifier;
}

boolean CBox::setIdentifier(
	const CIdentifier& rIdentifier)
{
	m_oIdentifier=rIdentifier;
	return true;
}

boolean CBox::setName(
	const CString& sName)
{
	m_sName=sName;
	return true;
}

boolean CBox::setAlgorithmClassIdentifier(
	const CIdentifier& rIdentifier)
{
	m_oAlgorithmClassIdentifier=rIdentifier;
	return true;
}

//___________________________________________________________________//
//                                                                   //

boolean CBox::addInput(
	const CString& sName,
	const CIdentifier& rTypeIdentifier)
{
	CInput i;
	i.m_sName=sName;
	i.m_oTypeIdentifier=rTypeIdentifier;
	m_vInput.push_back(i);
	return true;
}

boolean CBox::removeInput(
	const uint32 ui32InputIndex)
{
	uint32 i=0;
	vector<CInput>::iterator it=m_vInput.begin();
	for(i=0; i<ui32InputIndex && it!=m_vInput.end(); i++)
	{
		it++;
	}
	if(it==m_vInput.end())
	{
		return false;
	}
	it=m_vInput.erase(it);
	return true;
}

uint32 CBox::getInputCount(void) const
{
	return m_vInput.size();
}

boolean CBox::getInputType(
	const uint32 ui32InputIndex,
	CIdentifier& rTypeIdentifier) const
{
	if(ui32InputIndex>=m_vInput.size())
	{
		return false;
	}
	rTypeIdentifier=m_vInput[ui32InputIndex].m_oTypeIdentifier;
	return true;
}

boolean CBox::getInputName(
	const uint32 ui32InputIndex,
	CString& rName) const
{
	if(ui32InputIndex>=m_vInput.size())
	{
		return false;
	}
	rName=m_vInput[ui32InputIndex].m_sName;
	return true;
}

boolean CBox::setInputType(
	const uint32 ui32InputIndex,
	const CIdentifier& rTypeIdentifier)
{
	if(ui32InputIndex>=m_vInput.size())
	{
		return false;
	}
	m_vInput[ui32InputIndex].m_oTypeIdentifier=rTypeIdentifier;
	return true;
}

boolean CBox::setInputName(
	const uint32 ui32InputIndex,
	const CString& rName)
{
	if(ui32InputIndex>=m_vInput.size())
	{
		return false;
	}
	m_vInput[ui32InputIndex].m_sName=rName;
	return true;
}

//___________________________________________________________________//
//                                                                   //

boolean CBox::addOutput(
	const CString& sName,
	const CIdentifier& rTypeIdentifier)
{
	COutput o;
	o.m_sName=sName;
	o.m_oTypeIdentifier=rTypeIdentifier;
	m_vOutput.push_back(o);
	return true;
}

boolean CBox::removeOutput(
	const uint32 ui32OutputIndex)
{
	uint32 i=0;
	vector<COutput>::iterator it=m_vOutput.begin();
	for(i=0; i<ui32OutputIndex && it!=m_vOutput.end(); i++)
	{
		it++;
	}
	if(it==m_vOutput.end())
	{
		return false;
	}
	it=m_vOutput.erase(it);
	return true;
}

uint32 CBox::getOutputCount(void) const
{
	return m_vOutput.size();
}

boolean CBox::getOutputType(
	const uint32 ui32OutputIndex,
	CIdentifier& rTypeIdentifier) const
{
	if(ui32OutputIndex>=m_vOutput.size())
	{
		return false;
	}
	rTypeIdentifier=m_vOutput[ui32OutputIndex].m_oTypeIdentifier;
	return true;
}

boolean CBox::getOutputName(
	const uint32 ui32OutputIndex,
	CString& rName) const
{
	if(ui32OutputIndex>=m_vOutput.size())
	{
		return false;
	}
	rName=m_vOutput[ui32OutputIndex].m_sName;
	return true;
}

boolean CBox::setOutputType(
	const uint32 ui32OutputIndex,
	const CIdentifier& rTypeIdentifier)
{
	if(ui32OutputIndex>=m_vOutput.size())
	{
		return false;
	}
	m_vOutput[ui32OutputIndex].m_oTypeIdentifier=rTypeIdentifier;
	return true;
}

boolean CBox::setOutputName(
	const uint32 ui32OutputIndex,
	const CString& rName)
{
	if(ui32OutputIndex>=m_vOutput.size())
	{
		return false;
	}
	m_vOutput[ui32OutputIndex].m_sName=rName;
	return true;
}

//___________________________________________________________________//
//                                                                   //

#include <iostream>

boolean CBox::addSetting(
	const CString& sName,
	const CIdentifier& rTypeIdentifier,
	const CString& sDefaultValue)
{
	CSetting s;
	s.m_sName=sName;
	s.m_oTypeIdentifier=rTypeIdentifier;
	s.m_sDefaultValue=sDefaultValue;
	s.m_sValue=sDefaultValue;
	m_vSetting.push_back(s);
	return true;
}

boolean CBox::removeSetting(
	const uint32 ui32SettingIndex)
{
	uint32 i=0;
	vector<CSetting>::iterator it=m_vSetting.begin();
	for(i=0; i<ui32SettingIndex && it!=m_vSetting.end(); i++)
	{
		it++;
	}
	if(it==m_vSetting.end())
	{
		return false;
	}
	it=m_vSetting.erase(it);
	return true;
}

uint32 CBox::getSettingCount(void) const
{
	return m_vSetting.size();
}

boolean CBox::getSettingType(
	const uint32 ui32SettingIndex,
	CIdentifier& rTypeIdentifier) const
{
	if(ui32SettingIndex>=m_vSetting.size())
	{
		return false;
	}
	rTypeIdentifier=m_vSetting[ui32SettingIndex].m_oTypeIdentifier;
	return true;
}

boolean CBox::getSettingName(
	const uint32 ui32SettingIndex,
	CString& rName) const
{
	if(ui32SettingIndex>=m_vSetting.size())
	{
		return false;
	}
	rName=m_vSetting[ui32SettingIndex].m_sName;
	return true;
}

boolean CBox::getSettingDefaultValue(
	const uint32 ui32SettingIndex,
	CString& rDefaultValue) const
{
	if(ui32SettingIndex>=m_vSetting.size())
	{
		return false;
	}
	rDefaultValue=m_vSetting[ui32SettingIndex].m_sDefaultValue;
	return true;
}

boolean CBox::getSettingValue(
	const uint32 ui32SettingIndex,
	CString& rValue) const
{
	if(ui32SettingIndex>=m_vSetting.size())
	{
		return false;
	}
	rValue=m_vSetting[ui32SettingIndex].m_sValue;
	return true;
}

boolean CBox::setSettingType(
	const uint32 ui32SettingIndex,
	const CIdentifier& rTypeIdentifier)
{
	if(ui32SettingIndex>=m_vSetting.size())
	{
		return false;
	}
	m_vSetting[ui32SettingIndex].m_oTypeIdentifier=rTypeIdentifier;
	return true;
}

boolean CBox::setSettingName(
	const uint32 ui32SettingIndex,
	const CString& rName)
{
	if(ui32SettingIndex>=m_vSetting.size())
	{
		return false;
	}
	m_vSetting[ui32SettingIndex].m_sName=rName;
	return true;
}

boolean CBox::setSettingDefaultValue(
	const uint32 ui32SettingIndex,
	const CString& rDefaultValue)
{
	if(ui32SettingIndex>=m_vSetting.size())
	{
		return false;
	}
	m_vSetting[ui32SettingIndex].m_sDefaultValue=rDefaultValue;
	return true;
}

boolean CBox::setSettingValue(
	const uint32 ui32SettingIndex,
	const CString& rValue)
{
	if(ui32SettingIndex>=m_vSetting.size())
	{
		return false;
	}
	m_vSetting[ui32SettingIndex].m_sValue=rValue;
	return true;
}
