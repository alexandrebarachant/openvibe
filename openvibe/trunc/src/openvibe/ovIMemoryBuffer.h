#ifndef __OpenViBE_IMemoryBuffer_H__
#define __OpenViBE_IMemoryBuffer_H__

#include "ovIObject.h"

namespace OpenViBE
{
	class OV_API IMemoryBuffer : virtual public OpenViBE::IObject
	{
	public:

		virtual OpenViBE::boolean setSize(const OpenViBE::uint64 ui64Size, const OpenViBE::boolean bDiscard)=0;
		virtual OpenViBE::uint64 getSize(void) const=0;
		virtual OpenViBE::uint8* getDirectPointer(void)=0;
		virtual const OpenViBE::uint8* getDirectPointer(void) const=0;

	protected:

		_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassId_MemoryBuffer);
	};
};

#endif // __OpenViBE_IMemoryBuffer_H__
