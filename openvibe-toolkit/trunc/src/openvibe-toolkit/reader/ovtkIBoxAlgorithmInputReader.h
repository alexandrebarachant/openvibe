#ifndef __OpenViBEToolkit_Reader_IBoxAlgorithmInputReader_H__
#define __OpenViBEToolkit_Reader_IBoxAlgorithmInputReader_H__

#include "../ovtkIObject.h"

namespace OpenViBEToolkit
{
	class OVTK_API IBoxAlgorithmInputReader : virtual public OpenViBEToolkit::IObject
	{
	public:

		virtual OpenViBE::boolean getCurrentChunkInfo(OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext, OpenViBE::uint64& rStartTime, OpenViBE::uint64& rEndTime)=0;
		virtual OpenViBE::boolean parseCurrentChunk(OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext)=0;

		_IsDerivedFromClass_(OpenViBEToolkit::IObject, OVTK_ClassId_);
	};

	extern OVTK_API OpenViBEToolkit::IBoxAlgorithmInputReader* createBoxAlgorithmInputReader(OpenViBE::uint32 ui32InputIndex, EBML::IReaderCallback& rCallback);
	extern OVTK_API void releaseBoxAlgorithmInputReader(OpenViBEToolkit::IBoxAlgorithmInputReader* pBoxAlgorithmInputReader);
};

#endif // __OpenViBEToolkit_Reader_IBoxAlgorithmInputReader_H__
