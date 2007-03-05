#ifndef __OpenViBEKernel_Kernel_CDynamicBoxContext_H__
#define __OpenViBEKernel_Kernel_CDynamicBoxContext_H__

#include "ovkTPluginObjectContext.h"
#include "simulated-objects/ovkPsSimulatedBox.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class CDynamicBoxContext : virtual public OpenViBE::Kernel::TPluginObjectContext<OpenViBE::Plugins::IDynamicBoxContext>
		{
		public:

			CDynamicBoxContext(const OpenViBE::Kernel::IKernelContext& rKernelContext, ::PsSimulatedBox* pSimulatedBox);

			virtual OpenViBE::uint32 getInputChunkCount(
				const OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean getInputChunk(
				const OpenViBE::uint32 ui32InputIndex,
				const OpenViBE::uint32 ui32ChunkIndex,
				OpenViBE::uint64& rStartTime,
				OpenViBE::uint64& rEndTime,
				OpenViBE::uint64& rChunkSize,
				const OpenViBE::uint8*& rpChunkBuffer);
			virtual OpenViBE::boolean markInputAsDeprecated(
				const OpenViBE::uint32 ui32InputIndex,
				const OpenViBE::uint32 ui32ChunkIndex);

			virtual OpenViBE::uint64 getOutputChunkSize(
				const OpenViBE::uint32 ui32OutputIndex);
			virtual OpenViBE::boolean setOutputChunkSize(
				const OpenViBE::uint32 ui32OutputIndex,
				const OpenViBE::uint64 ui64Size,
				const OpenViBE::boolean bDiscard=true);
			virtual OpenViBE::uint8* getOutputChunkBuffer(
				const OpenViBE::uint32 ui32OutputIndex);
			virtual OpenViBE::boolean appendOutputChunkData(
				const OpenViBE::uint32 ui32OutputIndex,
				const OpenViBE::uint8* pBuffer,
				const OpenViBE::uint64 ui64BufferSize);
			virtual OpenViBE::boolean markOutputAsReadyToSend(
				const OpenViBE::uint32 ui32OutputIndex,
				const OpenViBE::uint64 ui64StartTime,
				const OpenViBE::uint64 ui64EndTime);

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TPluginObjectContext<OpenViBE::Plugins::IDynamicBoxContext>, OVK_ClassId_Kernel_DynamicBoxContext)

		protected:

			::PsSimulatedBox* m_pSimulatedBox;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_CDynamicBoxContext_H__
