#include "ovtkIBoxAlgorithmStreamedMatrixOutputWriter.h"
#include "ovtkCBoxAlgorithmStreamedMatrixOutputWriterPart.inl"
#include "ovtkCBoxAlgorithmEBMLOutputWriterPart.inl"

namespace OpenViBEToolkit
{
	class CBoxAlgorithmStreamedMatrixOutputWriter : public OpenViBEToolkit::IBoxAlgorithmStreamedMatrixOutputWriter
	{
	public:

		CBoxAlgorithmStreamedMatrixOutputWriter(void);

		virtual OpenViBE::boolean setDimensionCount(const OpenViBE::uint32 ui32DimensionCount);
		virtual OpenViBE::boolean setDimensionSize(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionSize);
		virtual OpenViBE::boolean setDimensionLabel(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionEntryIndex, const char* sDimensionLabel);
		virtual OpenViBE::boolean setBuffer(const OpenViBE::float64* pBuffer);

		virtual OpenViBE::boolean writeHeader(EBML::IWriter& rWriter);
		virtual OpenViBE::boolean writeBuffer(EBML::IWriter& rWriter);
		virtual OpenViBE::boolean writeEnd(EBML::IWriter& rWriter);

		_IsDerivedFromClass_Final_(OpenViBEToolkit::IBoxAlgorithmStreamedMatrixOutputWriter, OVTK_ClassId_);

	protected:

		OpenViBEToolkit::CBoxAlgorithmEBMLOutputWriterPart m_oEBMLOutputWriterPart;
		OpenViBEToolkit::CBoxAlgorithmStreamedMatrixOutputWriterPart m_oStreamedMatrixOutputWriterPart;
	};

	CBoxAlgorithmStreamedMatrixOutputWriter::CBoxAlgorithmStreamedMatrixOutputWriter(void)
	{
		m_oEBMLOutputWriterPart.registerBoxAlgorithmEBMLOutputWriterPart(m_oStreamedMatrixOutputWriterPart);
	}

	OpenViBE::boolean CBoxAlgorithmStreamedMatrixOutputWriter::setDimensionCount(const OpenViBE::uint32 ui32DimensionCount)
	{
		return m_oStreamedMatrixOutputWriterPart.setDimensionCount(ui32DimensionCount);
	}

	OpenViBE::boolean CBoxAlgorithmStreamedMatrixOutputWriter::setDimensionSize(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionSize)
	{
		return m_oStreamedMatrixOutputWriterPart.setDimensionSize(ui32DimensionIndex, ui32DimensionSize);
	}

	OpenViBE::boolean CBoxAlgorithmStreamedMatrixOutputWriter::setDimensionLabel(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionEntryIndex, const char* sDimensionLabel)
	{
		return m_oStreamedMatrixOutputWriterPart.setDimensionLabel(ui32DimensionIndex, ui32DimensionEntryIndex, sDimensionLabel);
	}

	OpenViBE::boolean CBoxAlgorithmStreamedMatrixOutputWriter::setBuffer(const OpenViBE::float64* pBuffer)
	{
		return m_oStreamedMatrixOutputWriterPart.setBuffer(pBuffer);
	}

	OpenViBE::boolean CBoxAlgorithmStreamedMatrixOutputWriter::writeHeader(EBML::IWriter& rWriter)
	{
		return m_oEBMLOutputWriterPart.writeHeader(rWriter);
	}

	OpenViBE::boolean CBoxAlgorithmStreamedMatrixOutputWriter::writeBuffer(EBML::IWriter& rWriter)
	{
		return m_oEBMLOutputWriterPart.writeBuffer(rWriter);
	}

	OpenViBE::boolean CBoxAlgorithmStreamedMatrixOutputWriter::writeEnd(EBML::IWriter& rWriter)
	{
		return m_oEBMLOutputWriterPart.writeEnd(rWriter);
	}

	OpenViBEToolkit::IBoxAlgorithmStreamedMatrixOutputWriter* createBoxAlgorithmStreamedMatrixOutputWriter(void)
	{
		return new OpenViBEToolkit::CBoxAlgorithmStreamedMatrixOutputWriter();
	}

	void releaseBoxAlgorithmStreamedMatrixOutputWriter(OpenViBEToolkit::IBoxAlgorithmStreamedMatrixOutputWriter* pOutputWriter)
	{
		delete pOutputWriter;
	}
};
