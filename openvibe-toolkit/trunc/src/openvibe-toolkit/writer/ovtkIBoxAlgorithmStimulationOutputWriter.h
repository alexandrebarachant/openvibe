#ifndef __OpenViBEToolkit_Writer_IBoxAlgorithmStimulationOutputWriter_H__
#define __OpenViBEToolkit_Writer_IBoxAlgorithmStimulationOutputWriter_H__

#include "ovtkIBoxAlgorithmOutputWriter.h"

namespace OpenViBEToolkit
{
	class OVTK_API IBoxAlgorithmStimulationOutputWriter : virtual public OpenViBEToolkit::IBoxAlgorithmOutputWriter
	{
	public:

		virtual OpenViBE::boolean setStimulationCount(const OpenViBE::uint32 ui32StimulationCount)=0;
		virtual OpenViBE::boolean setStimulation(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint32 ui32StimulationIdentifier, const OpenViBE::uint32 ui32SampleIndex)=0;

		virtual OpenViBE::boolean writeHeader(EBML::IWriter& rWriter)=0;
		virtual OpenViBE::boolean writeBuffer(EBML::IWriter& rWriter)=0;

		_IsDerivedFromClass_(OpenViBEToolkit::IBoxAlgorithmOutputWriter, OVTK_ClassId_);
	};

	extern OVTK_API OpenViBEToolkit::IBoxAlgorithmStimulationOutputWriter* createBoxAlgorithmStimulationOutputWriter(void);
	extern OVTK_API void releaseBoxAlgorithmStimulationOutputWriter(OpenViBEToolkit::IBoxAlgorithmStimulationOutputWriter* pOutputWriter);
};

#endif // __OpenViBEToolkit_Writer_IBoxAlgorithmStimulationOutputWriter_H__
