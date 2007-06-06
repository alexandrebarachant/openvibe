#include "ovpCXMLStimulationScenarioPlayer.h"

#include <fstream>
#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Stimulation;

using namespace OpenViBEToolkit;

using namespace std;

namespace OpenViBEPlugins
{
	namespace Stimulation
	{
		void CXMLStimulationScenarioPlayer::writeStimulationOutput(const void* pBuffer, const EBML::uint64 ui64BufferSize)
		{
			appendOutputChunkData<0>(pBuffer, ui64BufferSize);
		}


		void CXMLStimulationScenarioPlayer::setStimulationCount(const OpenViBE::uint32 ui32StimulationCount)
		{
			/* TODO nothing? */
		}

		void CXMLStimulationScenarioPlayer::setStimulation(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint64 ui64StimulationIdentifier, const OpenViBE::uint64 ui64StimulationDate)
		{
			m_oStimulationReceived.push_back(pair<uint64, uint64>(ui64StimulationIdentifier, ui64StimulationDate));
		}


		OpenViBE::boolean CXMLStimulationScenarioPlayer::readAutomaton(OpenViBE::CString& oFilename)
		{
			ifstream l_oFile((const char*)oFilename);

			size_t l_iFileSize = 0;
			l_oFile.seekg(0, ios::end);
			l_iFileSize = l_oFile.tellg();
			l_oFile.seekg(0, ios::beg);

			char * l_pXmlBuffer = new char[l_iFileSize];
			l_oFile.read(l_pXmlBuffer, l_iFileSize);

			while(m_pXMLAutomatonReader->processData(l_pXmlBuffer, l_iFileSize))
			{
			}
			
			l_oFile.close();

			return true;
		}

		CXMLStimulationScenarioPlayer::CXMLStimulationScenarioPlayer(void) :
			m_pReader(NULL),
			m_pStimulationReaderCallBack(NULL),
			m_pWriter(NULL),
			m_pOutputWriterCallbackProxy(NULL),
			m_pStimulationOutputWriterHelper(NULL),
			m_pXMLAutomatonReader(NULL),
			m_pAutomatonController(NULL),
			m_pAutomatonContext(NULL),
			m_bEndOfAutomaton(false),
			m_ui64PreviousActivationTime(0)
		{	
		}

		OpenViBE::boolean CXMLStimulationScenarioPlayer::initialize()
		{
			m_bEndOfAutomaton = false;

			m_pStimulationReaderCallBack=createBoxAlgorithmStimulationInputReaderCallback(*this);
			m_pReader = EBML::createReader(*m_pStimulationReaderCallBack);
		
			m_pOutputWriterCallbackProxy = new EBML::TWriterCallbackProxy1<OpenViBEPlugins::Stimulation::CXMLStimulationScenarioPlayer>(*this, &CXMLStimulationScenarioPlayer::writeStimulationOutput);

			m_pWriter=EBML::createWriter(*m_pOutputWriterCallbackProxy);

			m_pStimulationOutputWriterHelper=createBoxAlgorithmStimulationOutputWriter();


			const IStaticBoxContext* l_pBoxContext=getBoxAlgorithmContext()->getStaticBoxContext();
			OpenViBE::CString l_sFileName;

			m_pXMLAutomatonReader = Automaton::createXMLAutomatonReader();
		
			// Parses box settings to find input file's name
			l_pBoxContext->getSettingValue(0, l_sFileName);

			readAutomaton(l_sFileName);

			m_pAutomatonController = m_pXMLAutomatonReader->getAutomatonController();

			m_pAutomatonContext = m_pAutomatonController->getAutomatonContext();

			//we don't need the XML reader anymore
			releaseXMLAutomatonReader(m_pXMLAutomatonReader);

			m_pStimulationOutputWriterHelper->writeHeader(*m_pWriter);
			getBoxAlgorithmContext()->getDynamicBoxContext()->markOutputAsReadyToSend(0, 0, 0);
			
			return true;
		}

		OpenViBE::boolean CXMLStimulationScenarioPlayer::uninitialize()
		{
			releaseBoxAlgorithmStimulationInputReaderCallback(m_pStimulationReaderCallBack);
		
			delete m_pOutputWriterCallbackProxy;
			m_pOutputWriterCallbackProxy= NULL;
			m_pWriter->release();
			m_pWriter = NULL;
			releaseBoxAlgorithmStimulationOutputWriter(m_pStimulationOutputWriterHelper);
			m_pStimulationOutputWriterHelper=NULL;

			releaseAutomatonController(m_pAutomatonController);
			m_pAutomatonController=NULL;

			releaseAutomatonContext(m_pAutomatonContext);
			m_pAutomatonContext=NULL;

			return true;
		}
		
		OpenViBE::boolean CXMLStimulationScenarioPlayer::processInput(OpenViBE::uint32 ui32InputIndex)
		{
			IDynamicBoxContext* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();

			for(uint32 i=0; i<l_pDynamicBoxContext->getInputChunkCount(ui32InputIndex); i++)
			{
				uint64 l_ui64StartTime;
				uint64 l_ui64EndTime;
				uint64 l_ui64ChunkSize;
				const uint8* l_pChunkBuffer=NULL;

				if(l_pDynamicBoxContext->getInputChunk(ui32InputIndex, i, l_ui64StartTime, l_ui64EndTime, l_ui64ChunkSize, l_pChunkBuffer))
				{
					m_pReader->processData(l_pChunkBuffer, l_ui64ChunkSize);
				}
				l_pDynamicBoxContext->markInputAsDeprecated(ui32InputIndex, i);
			}

			getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

			return true;
		}

		OpenViBE::boolean CXMLStimulationScenarioPlayer::processClock(OpenViBE::CMessageClock &rMessageClock)
		{
//			cout<<"Current Time : "<<hex<<rMessageClock.getTime()<<endl;
			
			uint64 l_ui64CurrentTime = rMessageClock.getTime();

			IDynamicBoxContext * l_pDynamicBoxContext = getBoxAlgorithmContext()->getDynamicBoxContext();

			if(!m_bEndOfAutomaton)
			{
				//actualize context
				
				m_pAutomatonContext->setCurrentTime(rMessageClock.getTime());

				for(size_t i=0 ; i<m_oStimulationReceived.size() ; i++)
				{
					Automaton::CIdentifier l_oStimulationIdentifier = m_oStimulationReceived[i].first;
					m_pAutomatonContext->addReceivedEvent(l_oStimulationIdentifier);
				}

				//process	
				m_bEndOfAutomaton = m_pAutomatonController->process();

				const Automaton::CIdentifier * l_pSentEvents = m_pAutomatonContext->getSentEvents();
			
				//set the number of stimulation to send	
				m_pStimulationOutputWriterHelper->setStimulationCount(m_pAutomatonContext->getSentEventsCount());

				//if there were stimulations
				if(l_pSentEvents)
				{
					//adds em
					for(uint32 i = 0 ; i<m_pAutomatonContext->getSentEventsCount() ; i++)
					{
						/* TODO check Stimulation date */
						m_pStimulationOutputWriterHelper->setStimulation(i, l_pSentEvents[i], l_ui64CurrentTime);
					}

				}

				m_pStimulationOutputWriterHelper->writeBuffer(*m_pWriter);

				/* TODO check Stimulation date */
				l_pDynamicBoxContext->markOutputAsReadyToSend(0, m_ui64PreviousActivationTime, l_ui64CurrentTime);

				m_pAutomatonContext->clearSentEvents();


				//TODO clear all events?? or just used ones?
				m_pAutomatonContext->clearReceivedEvents();
			}
			
			m_ui64PreviousActivationTime = l_ui64CurrentTime;

			getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

			return true;
		}
		
		OpenViBE::boolean CXMLStimulationScenarioPlayer::process()
		{

		//	l_pDynamicBoxContext->markOutputAsReadyToSend(0, 0, 0);

			return true;
		}

	};

};

