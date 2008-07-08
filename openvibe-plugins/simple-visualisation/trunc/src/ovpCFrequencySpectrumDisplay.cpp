
#include "ovpCFrequencySpectrumDisplay.h"
using namespace OpenViBE;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;
using namespace OpenViBEToolkit;

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{

		CFrequencySpectrumDisplay::CFrequencySpectrumDisplay() :
			m_pReader(NULL),
			m_pStreamedMatrixReaderCallBack(NULL),
			m_pFrequencySpectrumDisplayView(NULL),
			m_pFrequencySpectrumDisplayDatabase(NULL)
		{
		}

		OpenViBE::boolean CFrequencySpectrumDisplay::initialize()
		{
			//initializes the ebml input
			m_pStreamedMatrixReaderCallBack = createBoxAlgorithmStreamedMatrixInputReaderCallback(*this);
			m_pReader=EBML::createReader(*m_pStreamedMatrixReaderCallBack);

			m_pFrequencySpectrumDisplayDatabase = new CBufferDatabase(*this);
			m_pFrequencySpectrumDisplayView = new CFrequencySpectrumDisplayView(*m_pFrequencySpectrumDisplayDatabase);
			m_pFrequencySpectrumDisplayDatabase->setDrawable(m_pFrequencySpectrumDisplayView);

			//parent visualisation box in visualisation tree
			::GtkWidget* l_pWidget=NULL;
			::GtkWidget* l_pToolbarWidget=NULL;
			dynamic_cast<CFrequencySpectrumDisplayView*>(m_pFrequencySpectrumDisplayView)->getWidgets(l_pWidget, l_pToolbarWidget);

			getBoxAlgorithmContext()->getVisualisationContext()->setWidget(l_pWidget);
			getBoxAlgorithmContext()->getVisualisationContext()->setToolbar(l_pToolbarWidget);

			return true;
		}

		OpenViBE::boolean CFrequencySpectrumDisplay::uninitialize()
		{
			//release the ebml reader
			releaseBoxAlgorithmStreamedMatrixInputReaderCallback(m_pStreamedMatrixReaderCallBack);

			m_pReader->release();
			m_pReader=NULL;

			delete m_pFrequencySpectrumDisplayView;
			delete m_pFrequencySpectrumDisplayDatabase;

			return true;
		}

		OpenViBE::boolean CFrequencySpectrumDisplay::processInput(OpenViBE::uint32 ui32InputIndex)
		{
			getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
			return true;
		}

		OpenViBE::boolean CFrequencySpectrumDisplay::process()
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

		void CFrequencySpectrumDisplay::setMatrixDimmensionCount(const OpenViBE::uint32 ui32DimmensionCount)
		{
			m_pFrequencySpectrumDisplayDatabase->setMatrixDimmensionCount(ui32DimmensionCount);
		}

		void CFrequencySpectrumDisplay::setMatrixDimmensionSize(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionSize)
		{
			m_pFrequencySpectrumDisplayDatabase->setMatrixDimmensionSize(ui32DimmensionIndex, ui32DimmensionSize);
		}

		void CFrequencySpectrumDisplay::setMatrixDimmensionLabel(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel)
		{
			m_pFrequencySpectrumDisplayDatabase->setMatrixDimmensionLabel(ui32DimmensionIndex, ui32DimmensionEntryIndex, sDimmensionLabel);
		}

		void CFrequencySpectrumDisplay::setMatrixBuffer(const OpenViBE::float64* pBuffer)
		{
			m_pFrequencySpectrumDisplayDatabase->setMatrixBuffer(pBuffer, m_ui64StartTime, m_ui64EndTime);
		}

	};
};

