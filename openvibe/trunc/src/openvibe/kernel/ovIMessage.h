#ifndef __OpenViBE_Kernel_IMessage_H__
#define __OpenViBE_Kernel_IMessage_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class OV_API IMessage : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::CIdentifier getIdentifier(void) const=0;
			virtual OpenViBE::uint64 getTime(void) const=0;

			virtual OpenViBE::boolean setIdentifier(
				const OpenViBE::CIdentifier& rIdentifier)=0;
			virtual OpenViBE::boolean setTime(
				const OpenViBE::uint64 ui64Time)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Message)

		};
	};
};

#endif // __OpenViBE_Kernel_IMessage_H__
