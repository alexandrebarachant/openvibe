#ifndef __OpenViBE_Kernel_IMessageEvent_H__
#define __OpenViBE_Kernel_IMessageEvent_H__

#include "ovIMessage.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class OV_API IMessageEvent : virtual public OpenViBE::Kernel::IMessage
		{
		public:

			_IsDerivedFromClass_(OpenViBE::Kernel::IMessage, OV_ClassId_Kernel_MessageEvent)

		};
	};
};

#endif // __OpenViBE_Kernel_IMessageEvent_H__
