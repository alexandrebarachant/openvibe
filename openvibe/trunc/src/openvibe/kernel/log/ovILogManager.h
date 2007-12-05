#ifndef __OpenViBE_Kernel_Log_ILogManager_h__
#define __OpenViBE_Kernel_Log_ILogManager_h__

#include "ovILogListener.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class OV_API ILogManager : virtual public OpenViBE::Kernel::ILogListener
		{
		public:

			virtual OpenViBE::boolean addListener(OpenViBE::Kernel::ILogListener* pListener)=0;
			virtual OpenViBE::boolean removeListener(OpenViBE::Kernel::ILogListener* pListener)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Log_LogManager);
		};

		template <class T> OpenViBE::Kernel::ILogManager& operator << (OpenViBE::Kernel::ILogManager& rLogManager, const T& rObject)
		{
			rLogManager.log(rObject);
			return rLogManager;
		}
	};
};

#endif // __OpenViBE_Kernel_Log_ILogManager_h__
