#ifndef __OpenViBEKernel_Tools_TBoxIOProxy_H__
#define __OpenViBEKernel_Tools_TBoxIOProxy_H__

#include <openvibe/ov_all.h>

#include "../ovk_base.h"

namespace OpenViBE
{
	namespace Tools
	{
		template <class CParentBoxIO>
		class TBoxIOProxy : virtual public CParentBoxIO
		{
		public:

			TBoxIOProxy(const OpenViBE::Kernel::IKernelContext& rKernelContext, const OpenViBE::Kernel::IBoxIO& rBoxIO);
			TBoxIOProxy(const OpenViBE::Kernel::IKernelContext& rKernelContext, OpenViBE::Kernel::IBoxIO& rBoxIO);

			virtual OpenViBE::boolean getCanModifyInputFlag(void) const;
			virtual OpenViBE::boolean getCanModifyOutputFlag(void) const;

			virtual OpenViBE::boolean setCanModifyInputFlag(OpenViBE::boolean bValue);
			virtual OpenViBE::boolean setCanModifyOutputFlag(OpenViBE::boolean bValue);

			virtual OpenViBE::uint32 getInputChunkCount(
				const OpenViBE::uint32 ui32InputIndex) const;
			virtual OpenViBE::boolean getInputChunk(
				const OpenViBE::uint32 ui32InputIndex,
				const OpenViBE::uint32 ui32ChunkIndex,
				OpenViBE::uint64& rStartTime,
				OpenViBE::uint64& rEndTime,
				OpenViBE::uint64& rChunkSize,
				const OpenViBE::uint8*& rpChunkBuffer) const;
			virtual OpenViBE::boolean markInputAsDeprecated(
				const OpenViBE::uint32 ui32InputIndex,
				const OpenViBE::uint32 ui32ChunkIndex);

			virtual OpenViBE::uint64 getOutputChunkSize(
				const OpenViBE::uint32 ui32OutputIndex) const;
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

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::IBoxIO, OVK_ClassId_Tools_BoxIOProxy)

		protected:

			OpenViBE::Kernel::IBoxIO* m_pBoxIO;
			OpenViBE::boolean m_bCanModify;
			OpenViBE::boolean m_bCanModifyInput;
			OpenViBE::boolean m_bCanModifyOutput;

		private:

			TBoxIOProxy(void);
			TBoxIOProxy(const TBoxIOProxy<CParentBoxIO>&);
		};
	};
};

#include "ovkTBoxIOProxy.inl"

#endif // __OpenViBEKernel_Tools_TBoxIOProxy_H__
