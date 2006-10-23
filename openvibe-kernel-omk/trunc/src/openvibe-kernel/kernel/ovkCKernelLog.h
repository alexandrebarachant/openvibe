#ifndef __OpenViBEKernel_Kernel_CKernelLog_h__
#define __OpenViBEKernel_Kernel_CKernelLog_h__

#include "ovkTKernelObject.h"

#include <vector>

namespace OpenViBE
{
	namespace Kernel
	{
		class CKernelLog : virtual public OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IKernelLog>
		{
		public:

			CKernelLog(OpenViBE::Kernel::IKernel& rKernel);
			virtual ~CKernelLog(void);

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

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IKernelLog>, OVK_ClassId_Kernel_KernelLog);

		protected:

			template <class T> IKernelLog& logForEach(T tValue)
			{
				std::vector<OpenViBE::Kernel::IKernelLog*>::iterator i;
				for(i=m_vImplementation.begin(); i!=m_vImplementation.end(); i++)
				{
					(*i)->log(tValue);
				}
				return *this;
			}

		protected:

			std::vector<OpenViBE::Kernel::IKernelLog*> m_vImplementation;
			OpenViBE::Kernel::TKernelLogLevel m_oLogLevel;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_CKernelLog_h__
