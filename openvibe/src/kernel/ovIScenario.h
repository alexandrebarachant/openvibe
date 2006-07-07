#ifndef __OpenViBE_Kernel_IKernel_H__
#define __OpenViBE_Kernel_IKernel_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IBox;

		class IScenario : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:
			virtual OpenViBE::boolean load(
				const std::string& sFileName)=0;
			virtual OpenViBE::boolean save(
				const std::string& sFileName)=0;

			virtual OpenViBE::boolean connect(
				const OpenViBE::CIdentifier& rSourceBoxIdentifier,
				const OpenViBE::uint32 ui32SourceBoxInputIndex,
				const OpenViBE::CIdentifier& rTargetBoxIdentifier,
				const OpenViBE::uint32 ui32TargetBoxOutputIndex,
				OpenViBE::CIdentifier& rLinkIdentifier)=0;
			virtual OpenViBE::boolean disconnect(
				const OpenViBE::CIdentifier& rSourceBoxIdentifier,
				const OpenViBE::uint32 ui32SourceBoxInputIndex,
				const OpenViBE::CIdentifier& rTargetBoxIdentifier,
				const OpenViBE::uint32 ui32TargetBoxOutputIndex)=0;
			virtual OpenViBE::boolean disconnect(
				const OpenViBE::CIdentifier& rLinkIdentifier)=0;

			virtual OpenViBE::uint32 getBoxCount(void)=0;
			virtual OpenViBE::boolean getBoxIdentifier(
				const OpenViBE::uint32 ui32Index,
				OpenViBE::CIdentifier& rBoxIdentifier)=0;
			virtual OpenViBE::Kernel::IBox* getBoxDetails(
				const OpenViBE::uint32 ui32Index)=0;

			virtual OpenViBE::uint32 getLinkCount(void)=0;
			virtual OpenViBE::boolean getLinkIdentifier(
				const OpenViBE::uint32 ui32Index,
				OpenViBE::CIdentifier& rLinkIdentifier)=0;
			virtual OpenViBE::boolean getLinkDetails(
				const OpenViBE::CIdentifier& rLinkIdentifier,
				OpenViBE::CIdentifier& rSourceBoxIdentifier,
				OpenViBE::uint32& ui32SourceBoxInputIndex,
				OpenViBE::CIdentifier& rTargetIdentifier,
				OpenViBE::uint32& ui32TargetBoxOutputIndex)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassIdentifier_Scenario)
		};
	};
};

#endif // __OpenViBE_Kernel_IKernel_H__
