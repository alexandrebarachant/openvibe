#ifndef __OpenViBEDesigner_defines_H__
#define __OpenViBEDesigner_defines_H__

//Attributes of visualisation window :
#define OVD_AttributeId_VisualisationWindow_Width           OpenViBE::CIdentifier(0x7B814CCA, 0x271DF6DD)
#define OVD_AttributeId_VisualisationWindow_Height          OpenViBE::CIdentifier(0x4C90D4AD, 0x7A2554EC)

//Attributes of visualisation paned :
#define OVD_AttributeId_VisualisationWidget_DividerPosition         OpenViBE::CIdentifier(0x54E45F5B, 0x76C036E2)
#define OVD_AttributeId_VisualisationWidget_MaxDividerPosition      OpenViBE::CIdentifier(0x237E56D2, 0x10CD68AE)

namespace OpenViBEDesigner
{

	typedef enum
	{
		CommandLineFlag_None             =0x00000000,
		CommandLineFlag_Open             =0x00000001,
		CommandLineFlag_Play             =0x00000002,
		CommandLineFlag_PlayFast         =0x00000004,
		CommandLineFlag_NoGui            =0x00000008,
		CommandLineFlag_NoCheckColorDepth=0x00000010,
		CommandLineFlag_NoManageSession  =0x00000020,
		CommandLineFlag_Define           =0x00000040,
	} ECommandLineFlag;

	enum
	{
		ContextMenu_SelectionCopy,
		ContextMenu_SelectionCut,
		ContextMenu_SelectionPaste,
		ContextMenu_SelectionDelete,

		ContextMenu_BoxRename,
		ContextMenu_BoxDelete,
		ContextMenu_BoxAddInput,
		ContextMenu_BoxEditInput,
		ContextMenu_BoxRemoveInput,
		ContextMenu_BoxAddOutput,
		ContextMenu_BoxEditOutput,
		ContextMenu_BoxRemoveOutput,
		ContextMenu_BoxAddSetting,
		ContextMenu_BoxRemoveSetting,
		ContextMenu_BoxEditSetting,
		ContextMenu_BoxConfigure,
		ContextMenu_BoxAbout,

		ContextMenu_ScenarioAbout,
	};

	enum
	{
		Resource_StringName,
		Resource_StringShortDescription,
		Resource_StringIdentifier,
		Resource_StringStockIcon,
		Resource_StringColor,
		Resource_BooleanIsPlugin,
		Resource_BooleanIsUnstable,
	};

	enum
	{
		Color_BackgroundPlayerStarted,
		Color_BoxBackground,
		Color_BoxBackgroundSelected,
		Color_BoxBackgroundMissing,
		Color_BoxBackgroundDeprecated,
		Color_BoxBackgroundUnstable,
		Color_BoxBackgroundNeedsUpdate,
		Color_BoxBorder,
		Color_BoxBorderSelected,
		Color_BoxInputBackground,
		Color_BoxInputBorder,
		Color_BoxOutputBackground,
		Color_BoxOutputBorder,
		Color_BoxSettingBackground,
		Color_BoxSettingBorder,
		Color_CommentBackground,
		Color_CommentBackgroundSelected,
		Color_CommentBorder,
		Color_CommentBorderSelected,
		Color_Link,
		Color_LinkSelected,
		Color_LinkDownCast,
		Color_LinkUpCast,
		Color_LinkInvalid,
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

//___________________________________________________________________//
//                                                                   //
// Global defines                                                   //
//___________________________________________________________________//
//                                                                   //

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines
 #include "ovp_global_defines.h"
#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines



#endif // __OpenViBEDesigner_defines_H__
