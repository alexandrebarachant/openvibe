#ifndef __OpenViBE_Kernel_IKernelLog_h__
#define __OpenViBE_Kernel_IKernelLog_h__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		typedef uint32 TKernelLogLevel;

		class IKernelLog : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::uint64 ui64Value)=0;
			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::uint32 ui32Value)=0;
			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::uint16 ui16Value)=0;
			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::uint8 ui8Value)=0;

			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::int64 i64Value)=0;
			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::int32 i32Value)=0;
			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::int16 i16Value)=0;
			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::int8 i8Value)=0;

			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::float32 f32Value)=0;
			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::float64 f64Value)=0;

			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::CIdentifier& rValue)=0;
			virtual OpenViBE::Kernel::IKernelLog& log(const OpenViBE::CString& rValue)=0;
			virtual OpenViBE::Kernel::IKernelLog& log(const char* rValue)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_KernelLog);
		};

		template <class T> OpenViBE::Kernel::IKernelLog& operator << (OpenViBE::Kernel::IKernelLog& rKernelLog, const T& rObject)
		{
			return rKernelLog.log(rObject);
		}
	};
};

#endif // __OpenViBE_Kernel_IKernelLog_h__
