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
				CBuffer(const OpenViBE::uint8* pBuffer, const OpenViBE::uint64 ui64BufferSize);
				virtual ~CBuffer(void);

				const CBuffer& operator=(const CBuffer& rBuffer);

				OpenViBE::uint8* getDirectPointer(void);
				const OpenViBE::uint8* getDirectPointer(void) const;
				OpenViBE::uint64 getSize(void) const;

				OpenViBE::boolean setSize(
					OpenViBE::uint64 ui64Size,
					OpenViBE::boolean bDiscard=true);

			protected:

				OpenViBE::uint8* m_pBuffer;
				OpenViBE::uint64 m_ui64BufferSize;
				OpenViBE::uint64 m_ui64AllocatedSize;
			};
		};
	};
};

#endif // __OpenViBEKernel_Kernel_Player_CBuffer_H__
