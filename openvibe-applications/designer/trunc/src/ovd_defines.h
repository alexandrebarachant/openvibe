#ifndef __OpenViBEDesigner_defines_H__
#define __OpenViBEDesigner_defines_H__

#define OVD_AttributeId_XBoxCenterPosition                  OpenViBE::CIdentifier(0x207C9054, 0x3C841B63)
#define OVD_AttributeId_YBoxCenterPosition                  OpenViBE::CIdentifier(0x1FA7A38F, 0x54EDBE0B)
#define OVD_AttributeId_XLinkSourcePosition                 OpenViBE::CIdentifier(0x358AE8B5, 0x0F8BACD1)
#define OVD_AttributeId_YLinkSourcePosition                 OpenViBE::CIdentifier(0x1B32C44C, 0x1905E0E9)
#define OVD_AttributeId_XLinkTargetPosition                 OpenViBE::CIdentifier(0x6267B5C5, 0x676E3E42)
#define OVD_AttributeId_YLinkTargetPosition                 OpenViBE::CIdentifier(0x3F0A3B27, 0x570913D2)
/*
#define OVD_AttributeId_                                    OpenViBE::CIdentifier(0x7B814CCA, 0x271DF6DD)
#define OVD_AttributeId_                                    OpenViBE::CIdentifier(0x4C90D4AD, 0x7A2554EC)
#define OVD_AttributeId_                                    OpenViBE::CIdentifier(0x73CF34A9, 0x3D95DA4A)
#define OVD_AttributeId_                                    OpenViBE::CIdentifier(0x2D9272EB, 0x2E2B1F52)
#define OVD_AttributeId_                                    OpenViBE::CIdentifier(0x58C3F253, 0x3C6968F9)
#define OVD_AttributeId_                                    OpenViBE::CIdentifier(0x67FCCF68, 0x2EB30C69)
#define OVD_AttributeId_                                    OpenViBE::CIdentifier(0x1ADEF355, 0x4F3669DA)
#define OVD_AttributeId_                                    OpenViBE::CIdentifier(0x5D72CF63, 0x6B05F549)
#define OVD_AttributeId_                                    OpenViBE::CIdentifier(0x1AB61306, 0x2A30C51C)
*/

namespace OpenViBEDesigner
{
	enum
	{
		BoxAlgorithm_StringName,
		BoxAlgorithm_StringShortDescription,
		BoxAlgorithm_StringIdentifier,
		BoxAlgorithm_StringStockIcon,
		BoxAlgorithm_BooleanIsPlugin,
	};

	enum
	{
		Color_BackgroundPlayerStarted,
		Color_BoxBackground,
		Color_BoxBackgroundSelected,
		Color_BoxBorder,
		Color_BoxBorderSelected,
		Color_BoxInputBackground,
		Color_BoxInputBorder,
		Color_BoxOutputBackground,
		Color_BoxOutputBorder,
		Color_BoxSettingBackground,
		Color_BoxSettingBorder,
		Color_Link,
		Color_LinkSelected,
		Color_SelectionArea,
		Color_SelectionAreaBorder,
	};

	enum
	{
		Connector_None,
		Connector_Input,
		Connector_Output,
		Connector_Setting,
		Connector_Link,
	};

	enum
	{
		Mode_None,
		Mode_Selection,
		Mode_SelectionAdd,
		Mode_MoveScenario,
		Mode_MoveSelection,
		Mode_Connect,
		Mode_EditSettings,
	};
};

#endif // __OpenViBEDesigner_defines_H__
