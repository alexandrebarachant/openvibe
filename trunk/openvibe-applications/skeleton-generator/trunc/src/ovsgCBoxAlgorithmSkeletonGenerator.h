#ifndef __OpenViBESkeletonGenerator_CBoxAlgorithmSkeletonGenerator_H__
#define __OpenViBESkeletonGenerator_CBoxAlgorithmSkeletonGenerator_H__

#include "ovsgCSkeletonGenerator.h"
#include <map>
#include <vector>

namespace OpenViBESkeletonGenerator
{
	class CBoxAlgorithmSkeletonGenerator : public CSkeletonGenerator
	{
		public:

			CBoxAlgorithmSkeletonGenerator(OpenViBE::Kernel::IKernelContext & rKernelContext,::GtkBuilder * pBuilderInterface);
			virtual ~CBoxAlgorithmSkeletonGenerator(void);

			void initialize(void);
			OpenViBE::boolean save(OpenViBE::CString sFileName);
			OpenViBE::boolean load(OpenViBE::CString sFileName);
			void getCurrentParameters(void);
		
			// Box Description
			OpenViBE::CString              m_sName;
			OpenViBE::CString              m_sVersion;
			OpenViBE::CString              m_sClassName;
			OpenViBE::CString              m_sCategory;
			OpenViBE::CString              m_sShortDescription;
			OpenViBE::CString              m_sDetailedDescription;
			OpenViBE::int32                m_i32GtkStockItemIndex;
			OpenViBE::CString              m_sGtkStockItemName;

			struct IOSStruct{
				OpenViBE::CString _name;
				OpenViBE::CString _type;
				OpenViBE::CString _typeId;
				OpenViBE::CString _defaultValue;
			};

			// Inputs
			OpenViBE::boolean              m_bCanModifyInputs;
			OpenViBE::boolean              m_bCanAddInputs;
			std::vector<IOSStruct>         m_vInputs;
			// Outputs
			OpenViBE::boolean              m_bCanModifyOutputs;
			OpenViBE::boolean              m_bCanAddOutputs;
			std::vector<IOSStruct>         m_vOutputs;
			// Settings
			OpenViBE::boolean              m_bCanModifySettings;
			OpenViBE::boolean              m_bCanAddSettings;
			std::vector<IOSStruct>         m_vSettings;

			//Algorithms
			std::vector<OpenViBE::CString> m_vAlgorithms; // the algorithm selected by user
			std::map <OpenViBE::CString, OpenViBE::CString> m_mAlgorithmHeaderDeclaration; //the map between algorithm and corresponding header declaration (all variables algo/input/output).
			std::map <OpenViBE::CString, OpenViBE::CString> m_mAlgorithmInitialisation;//the map between algorithm and corresponding initialisation
			std::map <OpenViBE::CString, OpenViBE::CString> m_mAlgorithmInitialisation_ReferenceTargets;//the map between algorithm and corresponding initialisation of ref targets
			std::map <OpenViBE::CString, OpenViBE::CString> m_mAlgorithmUninitialisation;//the map between algorithm and corresponding uninitialisation
			
			

			void buttonCheckCB(void);
			void buttonOkCB(void);
			void buttonTooltipCB(::GtkButton* pButton);
			void buttonExitCB(void);

			void buttonAddInputCB(void);
			void buttonRemoveInputCB(void);
			void buttonAddOutputCB(void);
			void buttonRemoveOutputCB(void);
			void buttonAddSettingCB(void);
			void buttonRemoveSettingCB(void);
			void buttonAddAlgorithmCB(void);
			void buttonRemoveAlgorithmCB(void);
			void algorithmSelectedCB(OpenViBE::int32 i32IndexSelected);

		private:

			typedef enum
			{
				WidgetName_NameVersion,
				WidgetName_Description,
				WidgetName_Icon,
				WidgetName_Inputs,
				WidgetName_Inputs_CanModify,
				WidgetName_Inputs_CanAddRemove,
				WidgetName_Outputs,
				WidgetName_Outputs_CanModify,
				WidgetName_Outputs_CanAddRemove,
				WidgetName_Settings,
				WidgetName_Settings_CanModify,
				WidgetName_Settings_CanAddRemove,
				WidgetName_TargetDirectory,
				WidgetName_ClassName,
				WidgetName_Category,
				WidgetName_Algorithm
			} EWidgetName;

			std::map < ::GtkButton*, EWidgetName > m_vWidgetName;

			OpenViBE::CString getRandomIdentifierString(void);

			std::vector<OpenViBE::CString> m_vParameterType_EnumTypeCorrespondance;

	};

	class CDummyAlgoProto : public OpenViBE::Kernel::IAlgorithmProto
	{
	public:
		std::map<OpenViBE::CString, OpenViBE::Kernel::EParameterType> m_vInputs;
		std::map<OpenViBE::CString, OpenViBE::Kernel::EParameterType> m_vOutputs;
		std::vector<OpenViBE::CString> m_vInputTriggers;
		std::vector<OpenViBE::CString> m_vOutputTriggers;
	public:
		OpenViBE::boolean addInputParameter(
			const OpenViBE::CIdentifier& rInputParameterIdentifier,
			const OpenViBE::CString& sInputName,
			const OpenViBE::Kernel::EParameterType eParameterType,
			const OpenViBE::CIdentifier& rSubTypeIdentifier=OV_UndefinedIdentifier);
			
		OpenViBE::boolean addOutputParameter(
			const OpenViBE::CIdentifier& rOutputParameterIdentifier,
			const OpenViBE::CString& sOutputName,
			const OpenViBE::Kernel::EParameterType eParameterType,
			const OpenViBE::CIdentifier& rSubTypeIdentifier=OV_UndefinedIdentifier);
			
		OpenViBE::boolean addInputTrigger(
			const OpenViBE::CIdentifier& rInputTriggerIdentifier,
			const OpenViBE::CString& rInputTriggerName);
			
		OpenViBE::boolean addOutputTrigger(
			const OpenViBE::CIdentifier& rOutputTriggerIdentifier,
			const OpenViBE::CString& rOutputTriggerName);

		OpenViBE::CIdentifier getClassIdentifier(void) const {return OV_UndefinedIdentifier;}
	};
}

#endif //__OpenViBESkeletonGenerator_CBoxAlgorithmSkeletonGenerator_H__
