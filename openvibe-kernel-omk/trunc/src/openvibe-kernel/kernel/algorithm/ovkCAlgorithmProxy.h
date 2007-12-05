#ifndef __OpenViBEKernel_Kernel_CAlgorithmProxy_H__
#define __OpenViBEKernel_Kernel_CAlgorithmProxy_H__

#include "../ovkTKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class CAlgorithm;

		class CAlgorithmProxy : virtual public OpenViBE::Kernel::TKernelObject < OpenViBE::Kernel::IAlgorithmProxy >
		{
		public:

			CAlgorithmProxy(const OpenViBE::Kernel::IKernelContext& rKernelContext, OpenViBE::Kernel::CAlgorithm& rAlgorithm);

			virtual OpenViBE::CIdentifier getNextInputParameterIdentifier(
				const OpenViBE::CIdentifier& rPreviousInputParameterIdentifier) const;
			virtual OpenViBE::Kernel::IParameter* getInputParameter(
				const OpenViBE::CIdentifier& rInputParameterIdentifier);

			virtual OpenViBE::CIdentifier getNextOutputParameterIdentifier(
				const OpenViBE::CIdentifier& rPreviousOutputParameterIdentifier) const;
			virtual OpenViBE::Kernel::IParameter* getOutputParameter(
				const OpenViBE::CIdentifier& rOutputParameterIdentifier);

			virtual OpenViBE::boolean isOutputTriggerActive(
				const OpenViBE::CIdentifier& rOutputTriggerIdentifier) const;

			virtual OpenViBE::boolean activateInputTrigger(
				const OpenViBE::CIdentifier& rInputTriggerIdentifier,
				const OpenViBE::boolean bTriggerState);

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean process(void);
			virtual OpenViBE::boolean process(
				const OpenViBE::CIdentifier& rTriggerIdentifier);

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TKernelObject < OpenViBE::Kernel::IAlgorithmProxy >, OVK_ClassId_Kernel_Algorithm_AlgorithmProxy)

		protected:

			OpenViBE::Kernel::CAlgorithm& m_rAlgorithm;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_CAlgorithmProxy_H__
