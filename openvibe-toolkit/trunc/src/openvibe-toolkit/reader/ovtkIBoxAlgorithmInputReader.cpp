#include "ovtkIBoxAlgorithmInputReader.h"

#include <ebml/IReader.h>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;
using namespace OpenViBEToolkit;

// ________________________________________________________________________________________________________________
//

namespace OpenViBEToolkit
{
	class CBoxAlgorithmInputReader : virtual public IBoxAlgorithmInputReader
	{
	public:

		CBoxAlgorithmInputReader(uint32 ui32InputIndex, EBML::IReaderCallback& rReaderCallback);
		virtual ~CBoxAlgorithmInputReader(void);

		virtual boolean getCurrentChunkInfo(IBoxAlgorithmContext& rBoxAlgorithmContext, uint64& rStartTime, uint64& rEndTime);
		virtual boolean parseCurrentChunk(IBoxAlgorithmContext& rBoxAlgorithmContext);

		_IsDerivedFromClass_Final_(IBoxAlgorithmInputReader, OVTK_ClassId_);

	protected:

		virtual boolean getCurrentChunk(IBoxAlgorithmContext& rBoxAlgorithmContext, uint64& rStartTime, uint64& rEndTime, uint64& rChunkSize, const uint8*& rpChunkBuffer, boolean bMarkAsDeprecated);

	protected:

		uint32 m_ui32InputIndex;
		EBML::IReader* m_pReader;
	};
};

// ________________________________________________________________________________________________________________
//

CBoxAlgorithmInputReader::CBoxAlgorithmInputReader(
	uint32 ui32InputIndex,
	EBML::IReaderCallback& rReaderCallback)
	:m_ui32InputIndex(ui32InputIndex)
{
	m_pReader=EBML::createReader(rReaderCallback);
}

CBoxAlgorithmInputReader::~CBoxAlgorithmInputReader(void)
{
	m_pReader->release();
	m_pReader=NULL;
}

// ________________________________________________________________________________________________________________
//

boolean CBoxAlgorithmInputReader::getCurrentChunkInfo(
	IBoxAlgorithmContext& rBoxAlgorithmContext,
	uint64& rStartTime,
	uint64& rEndTime)
{
	uint64 l_ui64StartTime;
	uint64 l_ui64EndTime;
	uint64 l_ui64ChunkSize;
	const uint8* l_pChunkBuffer;
	if(!getCurrentChunk(rBoxAlgorithmContext, l_ui64StartTime, l_ui64EndTime, l_ui64ChunkSize, l_pChunkBuffer, false))
	{
		return false;
	}

	rStartTime=l_ui64StartTime;
	rEndTime=l_ui64EndTime;
	return true;
}

boolean CBoxAlgorithmInputReader::parseCurrentChunk(IBoxAlgorithmContext& rBoxAlgorithmContext)
{
	uint64 l_ui64StartTime;
	uint64 l_ui64EndTime;
	uint64 l_ui64ChunkSize;
	const uint8* l_pChunkBuffer;
	if(!getCurrentChunk(rBoxAlgorithmContext, l_ui64StartTime, l_ui64EndTime, l_ui64ChunkSize, l_pChunkBuffer, true))
	{
		return false;
	}

	return m_pReader->processData(l_pChunkBuffer, l_ui64ChunkSize);
}

// ________________________________________________________________________________________________________________
//

boolean CBoxAlgorithmInputReader::getCurrentChunk(
	IBoxAlgorithmContext& rBoxAlgorithmContext,
	uint64& rStartTime,
	uint64& rEndTime,
	uint64& rChunkSize,
	const uint8*& rpChunkBuffer,
	boolean bMarkAsDeprecated)
{
	IDynamicBoxContext* l_pDynamicBoxContext=rBoxAlgorithmContext.getDynamicBoxContext();
	if(!l_pDynamicBoxContext)
	{
		return false;
	}

	if(l_pDynamicBoxContext->getInputChunkCount(m_ui32InputIndex)==0)
	{
		return false;
	}

	uint64 l_ui64StartTime;
	uint64 l_ui64EndTime;
	uint64 l_ui64ChunkSize;
	const uint8* l_pChunkBuffer;
	if(!l_pDynamicBoxContext->getInputChunk(m_ui32InputIndex, 0, l_ui64StartTime, l_ui64EndTime, l_ui64ChunkSize, l_pChunkBuffer))
	{
		return false;
	}

	if(bMarkAsDeprecated && ! l_pDynamicBoxContext->markInputAsDeprecated(m_ui32InputIndex, 0))
	{
		return false;
	}

	rStartTime=l_ui64StartTime;
	rEndTime=l_ui64EndTime;
	rChunkSize=l_ui64ChunkSize;
	rpChunkBuffer=l_pChunkBuffer;
	return true;
}

// ________________________________________________________________________________________________________________
//

IBoxAlgorithmInputReader* OpenViBEToolkit::createBoxAlgorithmInputReader(uint32 ui32InputIndex, EBML::IReaderCallback& rReaderCallback)
{
	return new CBoxAlgorithmInputReader(ui32InputIndex, rReaderCallback);
}

void OpenViBEToolkit::releaseBoxAlgorithmInputReader(IBoxAlgorithmInputReader* pBoxAlgorithmInputReader)
{
	delete pBoxAlgorithmInputReader;
}

// ________________________________________________________________________________________________________________
//
