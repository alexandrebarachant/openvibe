#ifndef __OpenViBEToolkit_Writer_CBoxAlgorithmStreamedMatrixOutputWriterPart_INL__
#define __OpenViBEToolkit_Writer_CBoxAlgorithmStreamedMatrixOutputWriterPart_INL__

#include "ovtkIBoxAlgorithmEBMLOutputWriter.h"
#include "ovtkCBoxAlgorithmEBMLOutputWriterPartBase.inl"

#include <ebml/CWriterHelper.h>

#include <map>
#include <string>
#include <iostream>

namespace OpenViBEToolkit
{
	class CBoxAlgorithmStreamedMatrixOutputWriterPart : virtual public OpenViBEToolkit::CBoxAlgorithmEBMLOutputWriterPartBase
	{
	public:

		enum
		{
			Status_SendingNothing,
			Status_SendingHeader,
			Status_SendingBuffer,
		};

		CBoxAlgorithmStreamedMatrixOutputWriterPart(void);

		virtual OpenViBE::boolean setDimmensionCount(const OpenViBE::uint32 ui32DimmensionCount);
		virtual OpenViBE::boolean setDimmensionSize(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionSize);
		virtual OpenViBE::boolean setDimmensionLabel(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel);
		virtual OpenViBE::boolean setBuffer(const OpenViBE::float64* pBuffer);

		_IsDerivedFromClass_Final_(OpenViBEToolkit::CBoxAlgorithmEBMLOutputWriterPartBase, OVTK_ClassId_);

	protected:

		virtual OpenViBE::boolean writeHeaderStart(EBML::IWriter& rWriter);
		virtual OpenViBE::boolean writeHeaderStop(EBML::IWriter& rWriter);
		virtual OpenViBE::boolean writeBufferStart(EBML::IWriter& rWriter);
		virtual OpenViBE::boolean writeBufferStop(EBML::IWriter& rWriter);
		// virtual OpenViBE::boolean writeEndStart(EBML::IWriter& rWriter);
		// virtual OpenViBE::boolean writeEndStop(EBML::IWriter& rWriter);

	protected:

		const OpenViBE::float64* m_pMatrixBuffer;
		OpenViBE::uint64 m_ui64MatrixBufferSize;
		OpenViBE::uint32 m_ui32Status;
		OpenViBE::uint32 m_ui32DimmensionCount;
		std::map<OpenViBE::uint32, OpenViBE::uint32> m_vDimmensionSize;
		std::map<OpenViBE::uint32, std::map< OpenViBE::uint32, std::string > > m_vDimmensionLabel;

		EBML::CWriterHelper m_oWriterHelper;
	};
};

#endif // __OpenViBEToolkit_Writer_CBoxAlgorithmStreamedMatrixOutputWriterPart_INL__
