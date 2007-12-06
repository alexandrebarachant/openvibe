#ifndef __OpenViBE_CMemoryBuffer_H__
#define __OpenViBE_CMemoryBuffer_H__

#include "ovIMemoryBuffer.h"

namespace OpenViBE
{
	class OV_API CMemoryBuffer : virtual public OpenViBE::IMemoryBuffer
	{
	public:

		CMemoryBuffer(void);
		virtual ~CMemoryBuffer(void);

		virtual OpenViBE::boolean setSize(const OpenViBE::uint64 ui64Size, const OpenViBE::boolean bDiscard);
		virtual OpenViBE::uint64 getSize(void) const;
		virtual OpenViBE::uint8* getDirectPointer(void);
		virtual const OpenViBE::uint8* getDirectPointer(void) const;

		_IsDerivedFromClass_Final_(OpenViBE::IMemoryBuffer, OV_ClassId_MemoryBufferBridge);

	protected:

		OpenViBE::IMemoryBuffer* m_pMemoryBufferImpl;
	};
};

#endif // __OpenViBE_CMemoryBuffer_H__
