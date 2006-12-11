#ifndef __OpenViBE_Kernel_ILogManager_h__
#define __OpenViBE_Kernel_ILogManager_h__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class OV_API ILogManager : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::boolean setMaximumLogLevel(const OpenViBE::uint32 ui32LogLevel)=0;
			virtual OpenViBE::boolean setCurrentLogLevel(const OpenViBE::uint32 ui32LogLevel)=0;

			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::uint64 ui64Value)=0;
			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::uint32 ui32Value)=0;
			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::uint16 ui16Value)=0;
			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::uint8 ui8Value)=0;

			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::int64 i64Value)=0;
			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::int32 i32Value)=0;
			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::int16 i16Value)=0;
			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::int8 i8Value)=0;

			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::float32 f32Value)=0;
			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::float64 f64Value)=0;

			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::CIdentifier& rValue)=0;
			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::CString& rValue)=0;
			virtual OpenViBE::Kernel::ILogManager& log(const char* rValue)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_LogManager);
		};

		template <class T> OpenViBE::Kernel::ILogManager& operator << (OpenViBE::Kernel::ILogManager& rLogManager, const T& rObject)
		{
			return rLogManager.log(rObject);
		}
	};
};

#endif // __OpenViBE_Kernel_ILogManager_h__
