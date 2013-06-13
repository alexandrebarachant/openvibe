#ifndef __OpenViBE_Kernel_Server_IServerExtensionDynamicContext_H__
#define __OpenViBE_Kernel_Server_IServerExtensionDynamicContext_H__

#include "../ovIKernelObject.h"

namespace OpenViBE
{
	class IStimulationSet;

	namespace Kernel
	{

		/**
		  * \class IServerExtensionDynamicContext
		  * \author Jozef Legeny (Inria)
		  * \date 2013-06-12
		  * \brief Dynamic context of a Server Extension object
		  * \ingroup Group_Server
		  * \ingroup Group_Kernel
		  * \ingroup Group_Extend
		  *
		  * This class contains accessors to data structures made available by
		  * the driver running in the Acquisition Server during the acquisition.
		  *
		  * \sa OpenViBE::Plugins::IServerExtension
		  */

		class OV_API IServerExtensionDynamicContext : public OpenViBE::Kernel::IKernelObject
		{
		public:
			virtual OpenViBE::uint32 getChannelCount() = 0;

			virtual OpenViBE::uint32 getSampleCountPerSentBlock() = 0;

			virtual OpenViBE::float64* getBuffer() = 0;

			virtual OpenViBE::uint64 getStimulationSetStartTime() = 0;

			virtual OpenViBE::uint64 getStimulationSetEndTime() = 0;

			virtual OpenViBE::IStimulationSet& getStimulationSet() = 0;
		};
	}
}

#endif // __OpenViBE_Kernel_Server_IServerExtensionDynamicContext_H__
