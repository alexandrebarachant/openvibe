#ifndef __OpenViBE_Kernel_Algorithm_IAlgorithmContext_H__
#define __OpenViBE_Kernel_Algorithm_IAlgorithmContext_H__

#include "../ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IParameter;

		/**
		 * \class IAlgorithmContext
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2007-11-06
		 * \brief Exectution context for algorithm objects
		 * \sa OpenViBE::Plugins::IAlgorithm
		 * \sa OpenViBE::Plugins::IAlgorithmDesc
		 */
		class OV_API IAlgorithmContext : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::Kernel::ILogManager& getLogManager(void)=0;

			virtual OpenViBE::CIdentifier getNextInputParameterIdentifier(
				const OpenViBE::CIdentifier& rPreviousInputParameterIdentifier) const=0;
			virtual OpenViBE::Kernel::IParameter* getInputParameter(
				const OpenViBE::CIdentifier& rInputParameterIdentifier)=0;

			virtual OpenViBE::CIdentifier getNextOutputParameterIdentifier(
				const OpenViBE::CIdentifier& rPreviousOutputParameterIdentifier) const=0;
			virtual OpenViBE::Kernel::IParameter* getOutputParameter(
				const OpenViBE::CIdentifier& rOutputParameterIdentifier)=0;

			virtual OpenViBE::boolean isInputTriggerActive(
				const OpenViBE::CIdentifier& rInputTriggerIdentifier) const=0;

			virtual OpenViBE::boolean activateOutputTrigger(
				const OpenViBE::CIdentifier& rOutputTriggerIdentifier,
				const OpenViBE::boolean bTriggerState)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Algorithm_AlgorithmContext)
		};
	};
};

#endif // __OpenViBE_Kernel_Algorithm_IAlgorithmContext_H__
