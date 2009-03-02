#include "ovpCCommonAverageReference.h"

using namespace OpenViBE;
using namespace OpenViBEPlugins;
using namespace OpenViBEToolkit;
using namespace OpenViBEPlugins::SignalProcessing;
using namespace OpenViBE::Kernel;

CCommonAverageReference::CCommonAverageReference(void) :
  m_pSignalReader(NULL),
  m_pSignalReaderCallBack(NULL),
  m_oSignalReaderCallBackProxy(*this,
			       &CCommonAverageReference::setChannelCount,
			       &CCommonAverageReference::setChannelName,
			       &CCommonAverageReference::setSampleCountPerBuffer,
			       &CCommonAverageReference::setSamplingRate,
			       &CCommonAverageReference::setSampleBuffer),
  m_ui64LastChunkStartTime(0),
  m_ui64LastChunkEndTime(0),
  m_pSignalOutputWriter(NULL),
  m_oSignalOutputWriterCallbackProxy(*this,
				     &CCommonAverageReference::appendSignalOutputChunkData),
  m_pSignalOutputWriterHelper(NULL),
  m_ui32ChannelCount(0),
  m_ui32SamplePerBufferCount(0)
{
}

void CCommonAverageReference::release(void)
{
  delete this;
}

boolean CCommonAverageReference::initialize(void)
{
  m_pSignalReaderCallBack = createBoxAlgorithmSignalInputReaderCallback(m_oSignalReaderCallBackProxy);
  m_pSignalReader = EBML::createReader(*m_pSignalReaderCallBack);
  m_pSignalOutputWriter = EBML::createWriter(m_oSignalOutputWriterCallbackProxy);
  m_pSignalOutputWriterHelper = createBoxAlgorithmSignalOutputWriter();
  m_pInputBuffer = NULL;
  m_pOutputBuffer = NULL;
  return true;
}

boolean CCommonAverageReference::uninitialize(void)
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

boolean CCommonAverageReference::processInput(uint32 ui32InputIndex)
{
  getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
  return true;
}

boolean CCommonAverageReference::process(void)
{
  IBoxIO * l_pDynamicContext = getBoxAlgorithmContext()->getDynamicBoxContext();
  uint32 l_ui32InputChunkCount = l_pDynamicContext->getInputChunkCount(0);
  for(uint32 i = 0; i < l_ui32InputChunkCount; i++)
    {
      uint64 l_ui64ChunkSize = 0;
      const uint8 * l_pChunkBuffer = NULL;
      if(l_pDynamicContext->getInputChunk(0, i, m_ui64LastChunkStartTime, m_ui64LastChunkEndTime, l_ui64ChunkSize, l_pChunkBuffer))
	{
	  m_pSignalReader->processData(l_pChunkBuffer, l_ui64ChunkSize);
	  l_pDynamicContext->markInputAsDeprecated(0,i);

	  if(m_pInputBuffer == NULL) //dealing with the signal header
	    {
	      //l_pDynamicContext->appendOutputChunkData(0, l_pChunkBuffer, l_ui64ChunkSize);
	      m_pSignalOutputWriterHelper->writeHeader(*m_pSignalOutputWriter);
	      m_pOutputBuffer = new float64[m_ui32SamplePerBufferCount*m_ui32ChannelCount];
	      m_pSignalOutputWriterHelper->setSampleBuffer(m_pOutputBuffer);
	    }
	  else
	    { //doing the processing
	      for(uint32 j = 0; j < m_ui32SamplePerBufferCount; j++)
		{
		  float64 l_f64Average = 0.0;
		  for(uint32 k = 0; k < m_ui32ChannelCount; k++)
		    {
		      l_f64Average += m_pInputBuffer[k*m_ui32SamplePerBufferCount + j];
		    }
		  l_f64Average /= m_ui32ChannelCount;
		  getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Info << "Average: " << l_f64Average << "\n";

		  for(uint32 k = 0; k < m_ui32ChannelCount; k++)
		    {
		      m_pOutputBuffer[k*m_ui32SamplePerBufferCount + j] = m_pInputBuffer[k*m_ui32SamplePerBufferCount + j] - l_f64Average;
		    }
		}
	      m_pSignalOutputWriterHelper->writeBuffer(*m_pSignalOutputWriter);
	    }
	  l_pDynamicContext->markOutputAsReadyToSend(0,m_ui64LastChunkStartTime, m_ui64LastChunkEndTime);
	}
      else
	{
	  //I should insult the user...
	}
    }
  return true;
}

void CCommonAverageReference::setChannelCount(const uint32 ui32ChannelCount)
{
  m_ui32ChannelCount = ui32ChannelCount;
  m_pSignalOutputWriterHelper->setChannelCount(ui32ChannelCount);
}

void CCommonAverageReference::setChannelName(const uint32 ui32ChannelIndex, const char* sChannelName)
{
  m_pSignalOutputWriterHelper->setChannelName(ui32ChannelIndex, sChannelName);
}

void CCommonAverageReference::setSampleCountPerBuffer(const uint32 ui32SampleCountPerBuffer)
{
  m_ui32SamplePerBufferCount = ui32SampleCountPerBuffer;
  m_pSignalOutputWriterHelper->setSampleCountPerBuffer(ui32SampleCountPerBuffer);
}

void CCommonAverageReference::setSamplingRate(const uint32 ui32SamplingFrequency)
{
  m_pSignalOutputWriterHelper->setSamplingRate(ui32SamplingFrequency);
}

void CCommonAverageReference::setSampleBuffer(const float64* pBuffer)
{
  m_pInputBuffer = pBuffer;
}

