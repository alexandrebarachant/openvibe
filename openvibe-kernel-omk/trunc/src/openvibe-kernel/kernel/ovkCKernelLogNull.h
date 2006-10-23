#ifndef __OpenViBEKernel_Kernel_CKernelLogNull_H__
#define __OpenViBEKernel_Kernel_CKernelLogNull_H__

#include "ovkTKernelObject.h"

#include <vector>

namespace OpenViBE
{
	namespace Kernel
	{
		class CKernelLogNull : virtual public OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IKernelLog>
		{
		public:

			CKernelLogNull(OpenViBE::Kernel::IKernel& rKernel);

			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::uint64 ui64Value);
			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::uint32 ui32Value);
			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::uint16 ui16Value);
			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::uint8 ui8Value);

			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::int64 i64Value);
			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::int32 i32Value);
			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::int16 i16Value);
			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::int8 i8Value);

			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::float64 f64Value);
			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::float32 f32Value);

			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::CIdentifier& rValue);
			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::CString& rValue);
			virtual OpenViBE::Kernel::IKernelLog& log(const char* rValue);

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IKernelLog>, OVK_ClassId_Kernel_KernelLogNull);
		};
	};
};

#endif // __OpenViBEKernel_Kernel_CKernelLogNull_H__
