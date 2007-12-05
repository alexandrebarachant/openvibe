#ifndef __OpenViBE_Kernel_IConfigurable_H__
#define __OpenViBE_Kernel_IConfigurable_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		enum EParameterType
		{
			ParameterType_None,
			ParameterType_Integer,
			ParameterType_UInteger,
			ParameterType_Boolean,
			ParameterType_Float,
			ParameterType_String,
			ParameterType_Identifier,
			ParameterType_Matrix,
			ParameterType_StimulationSet,
			ParameterType_MemoryBuffer,
			ParameterType_Object,
			ParameterType_Pointer,
		};

		class IParameter;
		class OV_API IConfigurable : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::CIdentifier getNextParameterIdentifier(const OpenViBE::CIdentifier& rPreviousIdentifier) const=0;

			virtual OpenViBE::Kernel::IParameter* getParameter(const OpenViBE::CIdentifier& rParameterIdentifier)=0;
			virtual OpenViBE::boolean setParameter(const OpenViBE::CIdentifier& rParameterIdentifier, OpenViBE::Kernel::IParameter& rpParameter)=0;
			virtual OpenViBE::Kernel::IParameter* createParameter(const OpenViBE::CIdentifier& rParameterIdentifier, const OpenViBE::Kernel::EParameterType eParameterType)=0;
			virtual OpenViBE::boolean removeParameter(const OpenViBE::CIdentifier& rParameterIdentifier)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Configurable);
		};
	};
};

#endif // __OpenViBE_Kernel_IConfigurable_H__
