#ifndef __OpenViBE_Kernel_Algorithm_IAlgorithmProto_H__
#define __OpenViBE_Kernel_Algorithm_IAlgorithmProto_H__

#include "../ovIConfigurable.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class OV_API IAlgorithmProto : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::boolean addInputParameter(
				const OpenViBE::CIdentifier& rInputParameterIdentifier,
				const OpenViBE::CString& sInputName,
				const OpenViBE::Kernel::EParameterType eParameterType)=0;

			virtual OpenViBE::boolean addOutputParameter(
				const OpenViBE::CIdentifier& rOutputParameterIdentifier,
				const OpenViBE::CString& sOutputName,
				const OpenViBE::Kernel::EParameterType eParameterType)=0;

			virtual OpenViBE::boolean addInputTrigger(
				const OpenViBE::CIdentifier& rInputTriggerIdentifier,
				const OpenViBE::CString& rInputTriggerName)=0;

			virtual OpenViBE::boolean addOutputTrigger(
				const OpenViBE::CIdentifier& rOutputTriggerIdentifier,
				const OpenViBE::CString& rOutputTriggerName)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Algorithm_AlgorithmProto)
		};
	};
};

#endif // __OpenViBE_Kernel_Algorithm_IAlgorithmProto_H__
