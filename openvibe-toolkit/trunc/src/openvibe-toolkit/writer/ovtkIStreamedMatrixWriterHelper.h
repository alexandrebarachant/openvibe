#ifndef __OpenViBEToolkit_Writer_IStreamedMatrixWriterHelper_H__
#define __OpenViBEToolkit_Writer_IStreamedMatrixWriterHelper_H__

#include "ovtkIBoxAlgorithmOutputWriter.h"

namespace OpenViBEToolkit
{
	class OVTK_API IStreamedMatrixWriterHelper : virtual public OpenViBEToolkit::IBoxAlgorithmOutputWriter
	{
	public:

		virtual OpenViBE::boolean setDimmensionCount(const OpenViBE::uint32 ui32DimmensionCount)=0;
		virtual OpenViBE::boolean setDimmensionSize(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionSize)=0;
		virtual OpenViBE::boolean setDimmensionLabel(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel)=0;
		virtual OpenViBE::boolean setBuffer(const OpenViBE::float64* pBuffer)=0;

		virtual OpenViBE::boolean writeHeader(EBML::IWriter& rWriter)=0;
		virtual OpenViBE::boolean writeBuffer(EBML::IWriter& rWriter)=0;

		_IsDerivedFromClass_(OpenViBEToolkit::IBoxAlgorithmOutputWriter, OVTK_ClassId_);
	};

	extern OVTK_API OpenViBEToolkit::IStreamedMatrixWriterHelper* createStreamedMatrixWriterHelper(void);
	extern OVTK_API void releaseStreamedMatrixWriterHelper(OpenViBEToolkit::IStreamedMatrixWriterHelper* pOutputWriter);
};

#endif // __OpenViBEToolkit_Writer_IStreamedMatrixWriterHelper_H__
