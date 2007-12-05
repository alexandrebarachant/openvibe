#ifndef __OpenViBE_CMatrix_H__
#define __OpenViBE_CMatrix_H__

#include "ovIMatrix.h"

namespace OpenViBE
{
	class OV_API CMatrix : virtual public OpenViBE::IMatrix
	{
	public:

		CMatrix(void);
		virtual ~CMatrix(void);

		virtual const OpenViBE::uint32 getDimensionCount(void) const;
		virtual const OpenViBE::uint32 getDimensionSize(const OpenViBE::uint32 ui32DimensionIndex) const;
		virtual const char* getDimensionLabel(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionEntryIndex) const;
		virtual const OpenViBE::float64* getBuffer(void) const;
		virtual const OpenViBE::uint32 getBufferElementCount(void) const;

		virtual OpenViBE::boolean setDimensionCount(const OpenViBE::uint32 ui32DimensionCount);
		virtual OpenViBE::boolean setDimensionSize(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionSize);
		virtual OpenViBE::boolean setDimensionLabel(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionEntryIndex, const char* sDimensionLabel);
		virtual OpenViBE::float64* getBuffer(void);

		_IsDerivedFromClass_Final_(OpenViBE::IMatrix, OV_ClassId_MatrixBridge);

	private:

		OpenViBE::IMatrix* m_pMatrixImpl;
	};
};

#endif // __OpenViBE_CMatrix_H__
