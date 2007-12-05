#ifndef __OpenViBE_IMatrix_H__
#define __OpenViBE_IMatrix_H__

#include "ovIObject.h"

namespace OpenViBE
{
	class OV_API IMatrix : virtual public OpenViBE::IObject
	{
	public:

		virtual const OpenViBE::uint32 getDimensionCount(void) const=0;
		virtual const OpenViBE::uint32 getDimensionSize(const OpenViBE::uint32 ui32DimensionIndex) const=0;
		virtual const char* getDimensionLabel(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionEntryIndex) const=0;
		virtual const OpenViBE::float64* getBuffer(void) const=0;
		virtual const OpenViBE::uint32 getBufferElementCount(void) const=0;

		virtual OpenViBE::boolean setDimensionCount(const OpenViBE::uint32 ui32DimensionCount)=0;
		virtual OpenViBE::boolean setDimensionSize(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionSize)=0;
		virtual OpenViBE::boolean setDimensionLabel(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionEntryIndex, const char* sDimensionLabel)=0;
		virtual OpenViBE::float64* getBuffer(void)=0;

		_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassId_Matrix);
	};
};

#endif // __OpenViBE_IMatrix_H__
