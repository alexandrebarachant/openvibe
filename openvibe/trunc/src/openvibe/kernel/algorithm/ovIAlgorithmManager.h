#ifndef __OpenViBE_Kernel_Algorithm_IAlgorithmManager_H__
#define __OpenViBE_Kernel_Algorithm_IAlgorithmManager_H__

#include "../ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IAlgorithmProxy;

		class OV_API IAlgorithmManager : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::CIdentifier createAlgorithm(
				const OpenViBE::CIdentifier& rAlgorithmClassIdentifier)=0;
			virtual OpenViBE::boolean releaseAlgorithm(
				const OpenViBE::CIdentifier& rAlgorithmIdentifier)=0;
			virtual OpenViBE::boolean releaseAlgorithm(
				OpenViBE::Kernel::IAlgorithmProxy& rAlgorithm)=0;
			virtual OpenViBE::Kernel::IAlgorithmProxy& getAlgorithm(
				const OpenViBE::CIdentifier& rAlgorithmIdentifier)=0;
			virtual OpenViBE::CIdentifier getNextAlgorithmIdentifier(
				const OpenViBE::CIdentifier& rPreviousIdentifier) const=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Algorithm_AlgorithmManager);
		};
	};
};

#endif // __OpenViBE_Kernel_Algorithm_IAlgorithmManager_H__
