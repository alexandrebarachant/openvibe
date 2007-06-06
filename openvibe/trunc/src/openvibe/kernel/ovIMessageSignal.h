#ifndef __OpenViBE_Kernel_IMessageSignal_H__
#define __OpenViBE_Kernel_IMessageSignal_H__

#include "ovIMessage.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class OV_API IMessageSignal : virtual public OpenViBE::Kernel::IMessage
		{
		public:

			_IsDerivedFromClass_(OpenViBE::Kernel::IMessage, OV_ClassId_Kernel_MessageSignal)

		};
	};
};

#endif // __OpenViBE_Kernel_IMessageSignal_H__
