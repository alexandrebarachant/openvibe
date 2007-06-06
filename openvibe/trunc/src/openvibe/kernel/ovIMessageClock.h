#ifndef __OpenViBE_Kernel_IMessageClock_H__
#define __OpenViBE_Kernel_IMessageClock_H__

#include "ovIMessage.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class OV_API IMessageClock : virtual public OpenViBE::Kernel::IMessage
		{
		public:

			_IsDerivedFromClass_(OpenViBE::Kernel::IMessage, OV_ClassId_Kernel_MessageClock)

		};
	};
};

#endif // __OpenViBE_Kernel_IMessageClock_H__
