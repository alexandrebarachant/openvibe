#include "ovkCBoxProto.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

CBoxProto::CBoxProto(const IKernelContext& rKernelContext, IBox& rBox)
	:TKernelObject<IBoxProto>(rKernelContext)
	,m_rBox(rBox)
{
}

uint32 CBoxProto::addInput(
	const CString& sName,
	const CIdentifier& rTypeIdentifier)
{
	return m_rBox.addInput(sName, rTypeIdentifier);
}

uint32 CBoxProto::addOutput(
	const CString& sName,
	const CIdentifier& rTypeIdentifier)
{
	return m_rBox.addOutput(sName, rTypeIdentifier);
}

uint32 CBoxProto::addSetting(
	const CString& sName,
	const CIdentifier& rTypeIdentifier,
	const CString& sDefaultValue)
{
	return m_rBox.addSetting(sName, rTypeIdentifier, sDefaultValue);
}

boolean CBoxProto::addFlag(
	const EBoxFlag eBoxFlag)
{
	switch (eBoxFlag)
	{
		case BoxFlag_CanAddInput: m_rBox.addAttribute(OV_AttributeId_Box_FlagCanAddInput, ""); break;
		case BoxFlag_CanModifyInput: m_rBox.addAttribute(OV_AttributeId_Box_FlagCanModifyInput, ""); break;
		case BoxFlag_CanAddOutput: m_rBox.addAttribute(OV_AttributeId_Box_FlagCanAddOutput, ""); break;
		case BoxFlag_CanModifyOutput: m_rBox.addAttribute(OV_AttributeId_Box_FlagCanModifyOutput, ""); break;
		case BoxFlag_CanAddSetting: m_rBox.addAttribute(OV_AttributeId_Box_FlagCanAddSetting, ""); break;
		case BoxFlag_CanModifySetting: m_rBox.addAttribute(OV_AttributeId_Box_FlagCanModifySetting, ""); break;
		case BoxFlag_IsDeprecated: m_rBox.addAttribute(OV_AttributeId_Box_FlagIsDeprecated, ""); break;
		default:
			return false;
	}
	return true;
}
