#ifndef __OpenViBEKernel_Kernel_CLogManagerConsole_H__
#define __OpenViBEKernel_Kernel_CLogManagerConsole_H__

#include "ovkTKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class CLogManagerConsole : virtual public OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::ILogManager>
		{
		public:

			CLogManagerConsole(const OpenViBE::Kernel::IKernelContext& rKernelContext);

			virtual OpenViBE::boolean setMaximumLogLevel(const OpenViBE::uint32 ui32LogLevel);
			virtual OpenViBE::boolean setCurrentLogLevel(const OpenViBE::uint32 ui32LogLevel);

			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::uint64 ui64Value);
			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::uint32 ui32Value);
			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::uint16 ui16Value);
			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::uint8 ui8Value);

			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::int64 i64Value);
			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::int32 i32Value);
			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::int16 i16Value);
			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::int8 i8Value);

			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::float64 f64Value);
			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::float32 f32Value);

			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::CIdentifier& rValue);
			virtual OpenViBE::Kernel::ILogManager& log(const OpenViBE::CString& rValue);
			virtual OpenViBE::Kernel::ILogManager& log(const char* rValue);

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::ILogManager>, OVK_ClassId_Kernel_LogManagerConsole);

		protected:

			OpenViBE::uint32 m_ui32CurrentLogLevel;
			OpenViBE::uint32 m_ui32MaximumLogLevel;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_CLogManagerConsole_H__
