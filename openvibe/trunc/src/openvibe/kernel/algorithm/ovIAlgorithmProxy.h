#ifndef __OpenViBE_Kernel_Algorithm_IAlgorithmProxy_H__
#define __OpenViBE_Kernel_Algorithm_IAlgorithmProxy_H__

#include "../ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IParameter;

		class OV_API IAlgorithmProxy : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::CIdentifier getNextInputParameterIdentifier(
				const OpenViBE::CIdentifier& rPreviousInputParameterIdentifier) const=0;
			virtual OpenViBE::Kernel::IParameter* getInputParameter(
				const OpenViBE::CIdentifier& rInputParameterIdentifier)=0;

			virtual OpenViBE::CIdentifier getNextOutputParameterIdentifier(
				const OpenViBE::CIdentifier& rPreviousOutputParameterIdentifier) const=0;
			virtual OpenViBE::Kernel::IParameter* getOutputParameter(
				const OpenViBE::CIdentifier& rOutputParameterIdentifier)=0;

			virtual OpenViBE::boolean isOutputTriggerActive(
				const OpenViBE::CIdentifier& rOutputTriggerIdentifier) const=0;

			virtual OpenViBE::boolean activateInputTrigger(
				const OpenViBE::CIdentifier& rInputTriggerIdentifier,
				const OpenViBE::boolean bTriggerState)=0;

			virtual OpenViBE::boolean initialize(void)=0;
			virtual OpenViBE::boolean uninitialize(void)=0;
			virtual OpenViBE::boolean process(void)=0;
			virtual OpenViBE::boolean process(
				const OpenViBE::CIdentifier& rTriggerIdentifier)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Algorithm_AlgorithmProxy)
		};
	};
};

#endif // __OpenViBE_Kernel_Algorithm_IAlgorithmProxy_H__
