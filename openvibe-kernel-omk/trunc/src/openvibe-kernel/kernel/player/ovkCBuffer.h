#ifndef __OpenViBEKernel_Kernel_Player_CBuffer_H__
#define __OpenViBEKernel_Kernel_Player_CBuffer_H__

#include "../../ovk_base.h"

#include <openvibe/ov_all.h>

namespace OpenViBE
{
	namespace Kernel
	{
		namespace Player
		{
			class CBuffer
			{
			public:

				CBuffer(void);
				CBuffer(const CBuffer& rBuffer);
				CBuffer(const OpenViBE::uint8* pBuffer, const OpenViBE::uint32 ui32BufferSize);
				virtual ~CBuffer(void);

				const CBuffer& operator=(const CBuffer& rBuffer);

				OpenViBE::uint8* getDirectPointer(void);
				const OpenViBE::uint8* getDirectPointer(void) const;
				OpenViBE::uint32 getSize(void) const;

				OpenViBE::boolean setSize(
					OpenViBE::uint32 ui32Size,
					OpenViBE::boolean bDiscard=true);

			protected:

				OpenViBE::uint8* m_pBuffer;
				OpenViBE::uint32 m_ui32BufferSize;
				OpenViBE::uint32 m_ui32AllocatedSize;
			};
		};
	};
};

#endif // __OpenViBEKernel_Kernel_Player_CBuffer_H__
