#ifndef __OpenViBE_Kernel_IKernel_H__
#define __OpenViBE_Kernel_IKernel_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IKernelObjectFactory;

		class IKernel : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual void release(void)=0;

			virtual OpenViBE::boolean loadScenario(
				const std::string& sFileName)=0;
			virtual OpenViBE::boolean saveScenario(
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

			virtual OpenViBE::Kernel::IKernelObject* createKernelObject(
				const OpenViBE::CIdentifier& rClassIdentifier)=0;
			virtual OpenViBE::boolean releaseKernelObject(
				OpenViBE::Kernel::IKernelObject*& rpKernelObject)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassIdentifier_Kernel)
		};
	};
};

#endif // __OpenViBE_Kernel_IKernel_H__
