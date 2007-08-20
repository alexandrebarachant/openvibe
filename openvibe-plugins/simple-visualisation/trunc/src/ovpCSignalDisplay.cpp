#include "ovpCSignalDisplay.h"

#include <math.h>
#include <iostream>


using namespace OpenViBE;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;

using namespace OpenViBEToolkit;

using namespace std;

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{

		/**
		* Constructor
		*/
		CSignalDisplay::CSignalDisplay(void) : 
			m_pReader(NULL),
			m_pStreamedMatrixReaderCallBack(NULL),
			m_pSignalDisplayView(NULL),
			m_pBufferDatabase(NULL)
		{
			
		}
		
		
		OpenViBE::boolean CSignalDisplay::initialize()
		{
			
			//initializes the ebml input
			m_pStreamedMatrixReaderCallBack = createBoxAlgorithmStreamedMatrixInputReaderCallback(*this);
			m_pReader=EBML::createReader(*m_pStreamedMatrixReaderCallBack);

			m_pBufferDatabase = new CBufferDatabase(*this);
			m_pSignalDisplayView = new CSignalDisplayView(*m_pBufferDatabase);
			m_pBufferDatabase->setDrawable(m_pSignalDisplayView);
			
			return true;
		}
		
		
		OpenViBE::boolean CSignalDisplay::uninitialize()
		{
			//release the ebml reader
			releaseBoxAlgorithmStreamedMatrixInputReaderCallback(m_pStreamedMatrixReaderCallBack);

			m_pReader->release();
			m_pReader=NULL;
			
			delete m_pSignalDisplayView;
			delete m_pBufferDatabase;

			return true;
		}
		
		
		OpenViBE::boolean CSignalDisplay::processInput(OpenViBE::uint32 ui32InputIndex)
		{
			getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
			return true;
		}
		
		
		OpenViBE::boolean CSignalDisplay::process()
		{
			IDynamicBoxContext* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();

			for(uint32 i=0; i<l_pDynamicBoxContext->getInputChunkCount(0); i++)
			{
				uint64 l_ui64ChunkSize;
				const uint8* l_pChunkBuffer=NULL;

				if(l_pDynamicBoxContext->getInputChunk(0, i, m_ui64StartTime, m_ui64EndTime, l_ui64ChunkSize, l_pChunkBuffer))
				{
					m_pReader->processData(l_pChunkBuffer, l_ui64ChunkSize);
					l_pDynamicBoxContext->markInputAsDeprecated(0, i);
				}
			}
			
			return true;
		}


		void CSignalDisplay::setMatrixDimmensionCount(const OpenViBE::uint32 ui32DimmensionCount)
		{
			m_pBufferDatabase->setMatrixDimmensionCount(ui32DimmensionCount);
		}
		
		void CSignalDisplay::setMatrixDimmensionSize(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionSize)
		{
			m_pBufferDatabase->setMatrixDimmensionSize(ui32DimmensionIndex, ui32DimmensionSize);
		}
		
		void CSignalDisplay::setMatrixDimmensionLabel(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel)
		{
			m_pBufferDatabase->setMatrixDimmensionLabel(ui32DimmensionIndex, ui32DimmensionEntryIndex, sDimmensionLabel);
		}
		
		void CSignalDisplay::setMatrixBuffer(const OpenViBE::float64* pBuffer)
		{
			m_pBufferDatabase->setMatrixBuffer(pBuffer, m_ui64StartTime, m_ui64EndTime);
		}


	};
};

