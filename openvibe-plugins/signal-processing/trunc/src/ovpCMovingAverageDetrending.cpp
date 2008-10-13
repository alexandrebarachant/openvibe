#include "ovpCMovingAverageDetrending.h"

using namespace OpenViBE;
using namespace OpenViBEPlugins;
using namespace OpenViBEToolkit;
using namespace OpenViBEPlugins::SignalProcessing;
using namespace OpenViBE::Kernel;
using namespace std;

CMovingAverageDetrending::CMovingAverageDetrending(void) : 
	m_pSignalReader(NULL),
	m_pSignalReaderCallBack(NULL),
	m_oSignalReaderCallBackProxy(*this,
		&CMovingAverageDetrending::setChannelCount, 
		&CMovingAverageDetrending::setChannelName,
		&CMovingAverageDetrending::setSampleCountPerBuffer,
		&CMovingAverageDetrending::setSamplingRate,
		&CMovingAverageDetrending::setSampleBuffer),
	m_ui64LastChunkStartTime(0),
	m_ui64LastChunkEndTime(0),
	m_pSignalOutputWriter(NULL),
	m_oSignalOutputWriterCallbackProxy(*this,
		&CMovingAverageDetrending::appendSignalOutputChunkData),
	m_pSignalOutputWriterHelper(NULL),
	m_ui32ChannelCount(0),
	m_ui32SamplePerBufferCount(0),
	m_ui32SamplingFrequency(0),
	m_ui32nbChunkInAverage(0)
{
}

void CMovingAverageDetrending::release(void)
{
	delete this;
}

boolean CMovingAverageDetrending::initialize(void)
{
	m_pSignalReaderCallBack = createBoxAlgorithmSignalInputReaderCallback(m_oSignalReaderCallBackProxy);
	m_pSignalReader = EBML::createReader(*m_pSignalReaderCallBack);
	m_pSignalOutputWriter = EBML::createWriter(m_oSignalOutputWriterCallbackProxy);
	m_pSignalOutputWriterHelper = createBoxAlgorithmSignalOutputWriter();
	m_pInputBuffer = NULL;
	m_pOutputBuffer = NULL;
	//making the queue empty
	//uint32 l_ui32sizeQueue = m_oaveragesQueue.size();
	//for(uint32 i = 0; i < l_ui32sizeQueue; i++)
	//{
	//	m_oaveragesQueue.pop();
	//}
	m_oaveragesQueue.clear();
	return true;
}

boolean CMovingAverageDetrending::uninitialize(void)
{
	releaseBoxAlgorithmSignalInputReaderCallback(m_pSignalReaderCallBack);
	m_pSignalReader->release();
	m_pSignalOutputWriter->release();
	releaseBoxAlgorithmSignalOutputWriter(m_pSignalOutputWriterHelper);
	m_pSignalReaderCallBack = NULL;
	m_pSignalReader = NULL;
	m_pSignalOutputWriter = NULL;
	m_pSignalOutputWriterHelper = NULL;
	delete[] m_pOutputBuffer;
	return true;
}

boolean CMovingAverageDetrending::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CMovingAverageDetrending::process(void)
{  
	IBoxIO * l_pDynamicContext = getBoxAlgorithmContext()->getDynamicBoxContext();
	uint32 l_ui32InputChunkCount = l_pDynamicContext->getInputChunkCount(0);

	getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Info << "chunk count: " << l_ui32InputChunkCount << "\n";

	for(uint32 i = 0; i < l_ui32InputChunkCount; i++)
	{
		uint64 l_ui64ChunkSize = 0;
		const uint8 * l_pChunkBuffer = NULL;
		if(l_pDynamicContext->getInputChunk(0, i, m_ui64LastChunkStartTime, m_ui64LastChunkEndTime, l_ui64ChunkSize, l_pChunkBuffer))
		{
			m_pSignalReader->processData(l_pChunkBuffer, l_ui64ChunkSize);	       
			if(m_pInputBuffer == NULL) //dealing with the signal header
			{	      
				m_pSignalOutputWriterHelper->writeHeader(*m_pSignalOutputWriter);
				m_pOutputBuffer = new float64[m_ui32SamplePerBufferCount*m_ui32ChannelCount];				
				m_pSignalOutputWriterHelper->setSampleBuffer(m_pOutputBuffer);

				//getting the user ste moving average duration
				CString l_sMovingAverageDuration;		
				float64 l_f64MovingAverageDuration=0.25;
				getBoxAlgorithmContext()->getStaticBoxContext()->getSettingValue(0, l_sMovingAverageDuration);
				sscanf(l_sMovingAverageDuration, "%lf", &l_f64MovingAverageDuration);
				
				//computing the number of chunks within the duration (we will use a integer number of chunks)
				m_ui32nbChunkInAverage = (uint32)((l_f64MovingAverageDuration * m_ui32SamplingFrequency)/m_ui32SamplePerBufferCount);
				getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Info << "m_ui32nbChunkInAverage: " << m_ui32nbChunkInAverage << "\n";
			} 
			else 
			{ //doing the processing
				getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Info << "m_oaveragesQueue.size() at the beginning: " << (uint32)m_oaveragesQueue.size() << "\n";				

				//the average for each channel of this chunk
				vector<float64> l_ochunkAverages(m_ui32ChannelCount);
				for(uint32 k = 0; k < l_ochunkAverages.size(); k++)
				{
					l_ochunkAverages[k] = 0.0;
				}

				for(uint32 k = 0; k < m_ui32ChannelCount; k++)
				{
					for(uint32 j = 0; j < m_ui32SamplePerBufferCount; j++)
					{
						l_ochunkAverages[k] += m_pInputBuffer[k * m_ui32SamplePerBufferCount + j];
					}
					l_ochunkAverages[k] /= (float64)m_ui32SamplePerBufferCount;
				}

				getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Info << "l_ochunkAverages[0]: " << l_ochunkAverages[0] << "\n";

				//adding the averages in the queue
				m_oaveragesQueue.push_back(l_ochunkAverages);
				
				getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Info << "m_oaveragesQueue.size() after push_back: " << (uint32)m_oaveragesQueue.size() << "\n";

				//printing the queue for debug
				for(uint32 l = 0; l < m_oaveragesQueue.size(); l++)	
				{
					getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Info << m_oaveragesQueue[l][0] << " ";
				}
				getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Info << "\n";

		
				//the averages for each channel of the moving window
				vector<float64> l_oaverages(m_ui32ChannelCount);
				for(uint32 k = 0; k < l_oaverages.size(); k++)
				{
					l_oaverages[k] = 0.0;
				}

				//computing the average on the whole time window (i.e. the average of the chunk averages)
				for(uint32 k = 0; k < m_ui32ChannelCount; k++)
				{
					//deque< vector<float64> >::iterator queueIterator = m_oaveragesQueue.begin();
					//while(queueIterator != m_oaveragesQueue.end())
					for(uint32 l = 0; l < m_oaveragesQueue.size(); l++)						
					{					
						l_oaverages[k] += (m_oaveragesQueue[l])[k];
						//l_oaverages[k] += (*queueIterator)[k];
						//queueIterator++;
					}
					l_oaverages[k] /= (float64)m_oaveragesQueue.size();
				}

				getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Info << "l_oaverages[0]: " << l_oaverages[0] << "\n";

				getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Info << "m_oaveragesQueue: " << (uint32)m_oaveragesQueue.size() << "\n";

				//generating the output buffer
				for(uint32 k = 0; k < m_ui32ChannelCount; k++)
				{
					for(uint32 j = 0; j < m_ui32SamplePerBufferCount; j++)
					{
						m_pOutputBuffer[k * m_ui32SamplePerBufferCount + j] = m_pInputBuffer[k * m_ui32SamplePerBufferCount + j] - l_oaverages[k];
					}
				}

				//getting rid of the oldest chunk, if necessary
				if(m_oaveragesQueue.size() == m_ui32nbChunkInAverage)
				{
					getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Info << "removing a chunk !\n";
					m_oaveragesQueue.pop_front();
				}
	
				m_pSignalOutputWriterHelper->writeBuffer(*m_pSignalOutputWriter);
			}
			l_pDynamicContext->markOutputAsReadyToSend(0,m_ui64LastChunkStartTime, m_ui64LastChunkEndTime);
			l_pDynamicContext->markInputAsDeprecated(0,i);
		}
		else 
		{
			//I should insult the user...
		}      
	}
	return true;
}

void CMovingAverageDetrending::setChannelCount(const uint32 ui32ChannelCount)
{
	m_ui32ChannelCount = ui32ChannelCount;
	m_pSignalOutputWriterHelper->setChannelCount(ui32ChannelCount);
}

void CMovingAverageDetrending::setChannelName(const uint32 ui32ChannelIndex, const char* sChannelName)
{
	m_pSignalOutputWriterHelper->setChannelName(ui32ChannelIndex, sChannelName);
}

void CMovingAverageDetrending::setSampleCountPerBuffer(const uint32 ui32SampleCountPerBuffer)
{
	m_ui32SamplePerBufferCount = ui32SampleCountPerBuffer;
	m_pSignalOutputWriterHelper->setSampleCountPerBuffer(ui32SampleCountPerBuffer);
}

void CMovingAverageDetrending::setSamplingRate(const uint32 ui32SamplingFrequency)
{
	m_pSignalOutputWriterHelper->setSamplingRate(ui32SamplingFrequency);
	m_ui32SamplingFrequency = ui32SamplingFrequency;
}

void CMovingAverageDetrending::setSampleBuffer(const float64* pBuffer)
{
	m_pInputBuffer = pBuffer;  
}

