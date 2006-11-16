#ifndef __OpenViBEKernel_Kernel_CLogManager_h__
#define __OpenViBEKernel_Kernel_CLogManager_h__

#include "ovkTKernelObject.h"

#include <vector>

namespace OpenViBE
{
	namespace Kernel
	{
		class CLogManager : virtual public OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::ILogManager>
		{
		public:

			CLogManager(const OpenViBE::Kernel::IKernelContext& rKernelContext);
			virtual ~CLogManager(void);

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

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::ILogManager>, OVK_ClassId_Kernel_LogManager);

		protected:

			template <class T> ILogManager& logForEach(T tValue)
			{
				std::vector<OpenViBE::Kernel::ILogManager*>::iterator i;
				for(i=m_vImplementation.begin(); i!=m_vImplementation.end(); i++)
				{
					(*i)->log(tValue);
				}
				return *this;
			}

		protected:

			std::vector<OpenViBE::Kernel::ILogManager*> m_vImplementation;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_CLogManager_h__
