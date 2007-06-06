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
			m_pSignalReaderCallBack(NULL),
			m_pSignalDisplayView(NULL),
			m_pSignalDisplayDatabase(NULL)
		{
			
		}
		
		
		OpenViBE::boolean CSignalDisplay::initialize()
		{
			
			//initializes the ebml input
			m_pSignalReaderCallBack = createBoxAlgorithmSignalInputReaderCallback(*this);
			m_pReader=EBML::createReader(*m_pSignalReaderCallBack);

			m_pSignalDisplayDatabase = new CSignalDisplayDatabase();
			m_pSignalDisplayView = new CSignalDisplayView(*m_pSignalDisplayDatabase);
			m_pSignalDisplayDatabase->setDrawable(m_pSignalDisplayView);
			
			return true;
		}
		
		
		OpenViBE::boolean CSignalDisplay::uninitialize()
		{
			//release the ebml reader
			releaseBoxAlgorithmSignalInputReaderCallback(m_pSignalReaderCallBack);

			m_pReader->release();
			m_pReader=NULL;
			
			delete m_pSignalDisplayView;
			delete m_pSignalDisplayDatabase;

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
		
		
		
		void CSignalDisplay::setChannelCount(const OpenViBE::uint32 ui32ChannelCount)
		{
			m_pSignalDisplayDatabase->setChannelCount(ui32ChannelCount);
		}
		
		void CSignalDisplay::setChannelName(const OpenViBE::uint32 ui32ChannelIndex, const char* sChannelName)
		{
			m_pSignalDisplayDatabase->setChannelName(ui32ChannelIndex, sChannelName);
		}
		
		
		void CSignalDisplay::setSampleCountPerBuffer(const OpenViBE::uint32 ui32SampleCountPerBuffer)
		{
			m_pSignalDisplayDatabase->setSampleCountPerBuffer(ui32SampleCountPerBuffer);
		}
		
		
		void CSignalDisplay::setSamplingRate(const OpenViBE::uint32 ui32SamplingFrequency)
		{
			m_pSignalDisplayDatabase->setSamplingRate(ui32SamplingFrequency);
		}
		
		
		void CSignalDisplay::setSampleBuffer(const OpenViBE::float64* pBuffer)
		{
			m_pSignalDisplayDatabase->setSampleBuffer(pBuffer, m_ui64StartTime);
		}


	};
};

