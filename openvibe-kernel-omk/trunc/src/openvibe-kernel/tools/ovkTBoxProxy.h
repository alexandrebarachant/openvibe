#ifndef __OpenViBEKernel_Tools_TBoxProxy_H__
#define __OpenViBEKernel_Tools_TBoxProxy_H__

#include <openvibe/ov_all.h>

#include "../ovk_base.h"

namespace OpenViBE
{
	namespace Tools
	{
		template <class CParentBox>
		class TBoxProxy : virtual public CParentBox
		{
		public:

			TBoxProxy(const OpenViBE::Kernel::IKernelContext& rKernelContext, const OpenViBE::Kernel::IBox& rBox);
			TBoxProxy(const OpenViBE::Kernel::IKernelContext& rKernelContext, OpenViBE::Kernel::IBox& rBox);

			virtual OpenViBE::boolean getCanModifyIdentifierFlag(void);
			virtual OpenViBE::boolean getCanModifyAlgorithmClassIdentifierFlag(void);
			virtual OpenViBE::boolean getCanModifyProcessingUnitIdentifierFlag(void);
			virtual OpenViBE::boolean getCanModifyNameFlag(void);
			virtual OpenViBE::boolean getCanModifyInputFlag(void);
			virtual OpenViBE::boolean getCanModifyOutputFlag(void);
			virtual OpenViBE::boolean getCanModifySettingFlag(void);
			virtual OpenViBE::boolean getCanModifyAttributeFlag(void);

			virtual OpenViBE::boolean setCanModifyIdentifierFlag(OpenViBE::boolean bValue);
			virtual OpenViBE::boolean setCanModifyAlgorithmClassIdentifierFlag(OpenViBE::boolean bValue);
			virtual OpenViBE::boolean setCanModifyProcessingUnitIdentifierFlag(OpenViBE::boolean bValue);
			virtual OpenViBE::boolean setCanModifyNameFlag(OpenViBE::boolean bValue);
			virtual OpenViBE::boolean setCanModifyInputFlag(OpenViBE::boolean bValue);
			virtual OpenViBE::boolean setCanModifyOutputFlag(OpenViBE::boolean bValue);
			virtual OpenViBE::boolean setCanModifySettingFlag(OpenViBE::boolean bValue);
			virtual OpenViBE::boolean setCanModifyAttributeFlag(OpenViBE::boolean bValue);

			virtual OpenViBE::CIdentifier getIdentifier(void) const;
			virtual OpenViBE::CString getName(void) const;
			virtual OpenViBE::CIdentifier getAlgorithmClassIdentifier(void) const;
			virtual OpenViBE::CIdentifier getProcessingUnitIdentifier(void) const;
			virtual OpenViBE::boolean setIdentifier(const OpenViBE::CIdentifier& rIdentifier);
			virtual OpenViBE::boolean setName(const OpenViBE::CString& sName);
			virtual OpenViBE::boolean setAlgorithmClassIdentifier(const OpenViBE::CIdentifier& rAlgorithmClassIdentifier);
			virtual OpenViBE::boolean setProcessingUnitIdentifier(const OpenViBE::CIdentifier& rProcessingUnitIdentifier);

			virtual OpenViBE::boolean initializeFromAlgorithmClassIdentifier(const OpenViBE::CIdentifier& rAlgorithmClassIdentifier);
			virtual OpenViBE::boolean initializeFromExistingBox(const OpenViBE::Kernel::IBox& rExisitingBox);

			virtual OpenViBE::boolean addInput(const OpenViBE::CString& sName, const OpenViBE::CIdentifier& rTypeIdentifier);
			virtual OpenViBE::boolean removeInput(const OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::uint32 getInputCount(void) const;
			virtual OpenViBE::boolean getInputType(const OpenViBE::uint32 ui32InputIndex, OpenViBE::CIdentifier& rTypeIdentifier) const;
			virtual OpenViBE::boolean getInputName(const OpenViBE::uint32 ui32InputIndex, OpenViBE::CString& rName) const;
			virtual OpenViBE::boolean setInputType(const OpenViBE::uint32 ui32InputIndex, const OpenViBE::CIdentifier& rTypeIdentifier);
			virtual OpenViBE::boolean setInputName(const OpenViBE::uint32 ui32InputIndex, const OpenViBE::CString& rName);

			virtual OpenViBE::boolean addOutput(const OpenViBE::CString& sName, const OpenViBE::CIdentifier& rTypeIdentifier);
			virtual OpenViBE::boolean removeOutput(const OpenViBE::uint32 ui32OutputIndex);
			virtual OpenViBE::uint32 getOutputCount(void) const;
			virtual OpenViBE::boolean getOutputType(const OpenViBE::uint32 ui32OutputIndex, OpenViBE::CIdentifier& rTypeIdentifier) const;
			virtual OpenViBE::boolean getOutputName(const OpenViBE::uint32 ui32OutputIndex, OpenViBE::CString& rName) const;
			virtual OpenViBE::boolean setOutputType(const OpenViBE::uint32 ui32OutputIndex, const OpenViBE::CIdentifier& rTypeIdentifier);
			virtual OpenViBE::boolean setOutputName(const OpenViBE::uint32 ui32OutputIndex, const OpenViBE::CString& rName);

			virtual OpenViBE::boolean addSetting(const OpenViBE::CString& sName, const OpenViBE::CIdentifier& rTypeIdentifier, const OpenViBE::CString& sDefaultValue);
			virtual OpenViBE::boolean removeSetting(const OpenViBE::uint32 ui32Index);
			virtual OpenViBE::uint32 getSettingCount(void) const;
			virtual OpenViBE::boolean getSettingType(const OpenViBE::uint32 ui32SettingIndex, OpenViBE::CIdentifier& rTypeIdentifier) const;
			virtual OpenViBE::boolean getSettingName(const OpenViBE::uint32 ui32SettingIndex, OpenViBE::CString& rName) const;
			virtual OpenViBE::boolean getSettingDefaultValue(const OpenViBE::uint32 ui32SettingIndex, OpenViBE::CString& rDefaultValue) const;
			virtual OpenViBE::boolean getSettingValue(const OpenViBE::uint32 ui32SettingIndex, OpenViBE::CString& rValue) const;
			virtual OpenViBE::boolean setSettingType(const OpenViBE::uint32 ui32SettingIndex, const OpenViBE::CIdentifier& rTypeIdentifier);
			virtual OpenViBE::boolean setSettingName(const OpenViBE::uint32 ui32SettingIndex, const OpenViBE::CString& rName);
			virtual OpenViBE::boolean setSettingDefaultValue(const OpenViBE::uint32 ui32SettingIndex, const OpenViBE::CString& rDefaultValue);
			virtual OpenViBE::boolean setSettingValue(const OpenViBE::uint32 ui32SettingIndex, const OpenViBE::CString& rValue);

			virtual OpenViBE::boolean addAttribute(const OpenViBE::CIdentifier& rAttributeIdentifier, const OpenViBE::CString& sAttributeValue);
			virtual OpenViBE::boolean removeAttribute(const OpenViBE::CIdentifier& rAttributeIdentifier);
			virtual OpenViBE::boolean removeAllAttributes(void);
			virtual OpenViBE::CString getAttributeValue(const OpenViBE::CIdentifier& rAttributeIdentifier) const;
			virtual OpenViBE::boolean setAttributeValue(const OpenViBE::CIdentifier& rAttributeIdentifier, const OpenViBE::CString& sAttributeValue);
			virtual OpenViBE::boolean hasAttribute(const OpenViBE::CIdentifier& rAttributeIdentifier) const;
			virtual OpenViBE::boolean hasAttributes(void) const;
#if 0
			virtual OpenViBE::boolean enumerateAttributes(OpenViBE::Kernel::IAttributable::IAttributeEnum& rCallback) const;
#endif
			virtual OpenViBE::CIdentifier getNextAttributeIdentifier(const OpenViBE::CIdentifier& rPreviousIdentifier) const;

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::IBox, OVK_ClassId_Tools_BoxProxy);

		private:

			TBoxProxy(void);
			TBoxProxy(const TBoxProxy<CParentBox>&);

		protected:

			OpenViBE::Kernel::IBox* m_pBox;
			OpenViBE::boolean m_bCanModify;
			OpenViBE::boolean m_bCanModifyIdentifier;
			OpenViBE::boolean m_bCanModifyAlgorithmClassIdentifier;
			OpenViBE::boolean m_bCanModifyProcessingUnitIdentifier;
			OpenViBE::boolean m_bCanModifyName;
			OpenViBE::boolean m_bCanModifyInput;
			OpenViBE::boolean m_bCanModifyOutput;
			OpenViBE::boolean m_bCanModifySetting;
			OpenViBE::boolean m_bCanModifyAttribute;
		};
	};
};

#include "ovkTBoxProxy.inl"

#endif // __OpenViBEKernel_Tools_TBoxProxy_H__
