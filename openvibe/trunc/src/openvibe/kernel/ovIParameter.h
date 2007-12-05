#ifndef __OpenViBE_Kernel_IParameter_H__
#define __OpenViBE_Kernel_IParameter_H__

#include "ovIKernelObject.h"
#include "ovIConfigurable.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class OV_API IParameter : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::uint64 getParameterSize(void) const                                        { return 0; }
			virtual OpenViBE::Kernel::EParameterType getType(void) const                                 { return ParameterType_None; }

			virtual OpenViBE::boolean clearReferenceTarget(void)                                         { return false; }

			virtual OpenViBE::boolean getReferenceTarget(OpenViBE::Kernel::IParameter*& value) const     { return false; }
			virtual OpenViBE::boolean setReferenceTarget(OpenViBE::Kernel::IParameter*  value)           { return false; }
			virtual OpenViBE::boolean getReferenceTarget(void* value) const                              { return false; }
			virtual OpenViBE::boolean setReferenceTarget(const void* value)                              { return false; }

			virtual OpenViBE::boolean getValue(void* value) const                                        { return false; }
			virtual OpenViBE::boolean setValue(const void* value)                                        { return false; }

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Parameter);
		};
	};
};

#endif // __OpenViBE_Kernel_IParameter_H__
