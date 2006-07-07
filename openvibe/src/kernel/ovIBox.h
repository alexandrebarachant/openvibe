#ifndef __OpenViBE_Kernel_IBox_H__
#define __OpenViBE_Kernel_IBox_H__

#include "../ovIObject.h"

namespace OpenViBE
{
	namespace Plugins
	{
		class IBoxAlgorithmDesc;
		class IBoxBehaviorDesc;
	};

	namespace Kernel
	{
		class IBox : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::boolean initialize(
				OpenViBE::Plugins::IBoxAlgorithmDesc& rBoxAlgorithmDesc,
				OpenViBE::Plugins::IBoxBehaviorDesc& rBoxBehaviorDesc)=0;

			virtual std::string getName(void)=0;
			virtual OpenViBE::boolean setName(
				const std::string& sName)=0;

			virtual OpenViBE::CIdentifier getIdentifier(void)=0;
			virtual OpenViBE::boolean setIdentifier(
				const OpenViBE::CIdentifier& rIdentifier)=0;
			virtual OpenViBE::CIdentifier getClassIdentifier(void)=0;
			virtual OpenViBE::CIdentifier getBehaviorClassIdentifier(void)=0;
			virtual OpenViBE::CIdentifier getAlgorithmClassIdentifier(void)=0;

			virtual OpenViBE::boolean addInput(
				const std::string& sName,
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::boolean bDiscrete)=0;
			virtual OpenViBE::boolean removeInput(
				const OpenViBE::uint32 ui32InputIndex)=0;
			virtual OpenViBE::uint32 getInputCount(void)=0;
			virtual OpenViBE::boolean getInputType(
				const OpenViBE::uint32 ui32InputIndex,
				OpenViBE::CIdentifier& rTypeIdentifier)=0;

			virtual OpenViBE::boolean addOutput(
				const std::string& sName,
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::boolean bDiscrete)=0;
			virtual OpenViBE::boolean removeOutput(
				const OpenViBE::uint32 ui32OutputIndex)=0;
			virtual OpenViBE::uint32 getOutputCount(void)=0;
			virtual OpenViBE::boolean getOutputType(
				const OpenViBE::uint32 ui32OutputIndex,
				OpenViBE::CIdentifier& rTypeIdentifier)=0;

			virtual OpenViBE::boolean addSetting(
				const std::string& sName,
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const std::string& sDefaultValue)=0;
			virtual OpenViBE::boolean removeSetting(
				const OpenViBE::uint32 ui32Index)=0;
			virtual OpenViBE::uint32 getSettingCount(void)=0;
			virtual OpenViBE::boolean getSettingType(
				const OpenViBE::uint32 ui32SettingIndex,
				OpenViBE::CIdentifier& rTypeIdentifier)=0;
			virtual OpenViBE::boolean getDefaultSettingValue(
				const OpenViBE::uint32 ui32SettingIndex,
				std::string& rValue)=0;
			virtual OpenViBE::boolean setDefaultSettingValue(
				const OpenViBE::uint32 ui32SettingIndex,
				const std::string& rValue)=0;
			virtual OpenViBE::boolean getSettingValue(
				const OpenViBE::uint32 ui32SettingIndex,
				std::string& rValue)=0;
			virtual OpenViBE::boolean setSettingValue(
				const OpenViBE::uint32 ui32SettingIndex,
				const std::string& rValue)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassIdentifier_Box)
		};
	};
};

#endif // __OpenViBE_Kernel_IBox_H__
