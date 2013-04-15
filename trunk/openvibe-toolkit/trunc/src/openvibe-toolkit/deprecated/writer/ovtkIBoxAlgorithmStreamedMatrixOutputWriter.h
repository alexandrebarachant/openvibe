#ifndef __OpenViBEToolkit_Writer_IBoxAlgorithmStreamedMatrixOutputWriter_H__
#define __OpenViBEToolkit_Writer_IBoxAlgorithmStreamedMatrixOutputWriter_H__

#include "ovtkIBoxAlgorithmEBMLOutputWriter.h"

namespace OpenViBEToolkit
{
	class OV_API IBoxAlgorithmStreamedMatrixOutputWriter : public OpenViBEToolkit::IBoxAlgorithmEBMLOutputWriter
	{
	public:

		virtual OpenViBE::boolean setDimensionCount(const OpenViBE::uint32 ui32DimensionCount)=0;
		virtual OpenViBE::boolean setDimensionSize(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionSize)=0;
		virtual OpenViBE::boolean setDimensionLabel(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionEntryIndex, const char* sDimensionLabel)=0;
		virtual OpenViBE::boolean setBuffer(const OpenViBE::float64* pBuffer)=0;

		virtual OpenViBE::boolean writeHeader(EBML::IWriter& rWriter)=0;
		virtual OpenViBE::boolean writeBuffer(EBML::IWriter& rWriter)=0;

		_IsDerivedFromClass_(OpenViBEToolkit::IBoxAlgorithmEBMLOutputWriter, OVTK_ClassId_);
	};

	extern OV_API OpenViBEToolkit::IBoxAlgorithmStreamedMatrixOutputWriter* createBoxAlgorithmStreamedMatrixOutputWriter(void);
	extern OV_API void releaseBoxAlgorithmStreamedMatrixOutputWriter(OpenViBEToolkit::IBoxAlgorithmStreamedMatrixOutputWriter* pOutputWriter);
};

#endif // __OpenViBEToolkit_Writer_IBoxAlgorithmStreamedMatrixOutputWriter_H__
