#include "ovpCBoxAlgorithmSignalConcatenation.h"

#include <system/Memory.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::FileIO;

using namespace std;

boolean CBoxAlgorithmSignalConcatenation::initialize(void)
{
	m_vSignalBuffers.resize(this->getStaticBoxContext().getInputCount() >> 1);
	m_vSignalBufferEndTimes.resize(this->getStaticBoxContext().getInputCount() >> 1);
	
	m_vStimulations.resize(this->getStaticBoxContext().getInputCount() >> 1);
	m_vStimulationSets.resize(this->getStaticBoxContext().getInputCount() >> 1);
	m_vStimulationBuffers.resize(this->getStaticBoxContext().getInputCount() >> 1);
	m_vStimulationBufferEndTimes.resize(this->getStaticBoxContext().getInputCount() >> 1);
	
	m_ui64TimeOut = FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 0);
	m_ui64TimeOut = m_ui64TimeOut << 32;
	this->getLogManager() << LogLevel_Info << "Timeout set to "<< time64(m_ui64TimeOut) <<".\n";
	for(uint32 i = 0; i < this->getStaticBoxContext().getInputCount(); i+=2)
	{
		m_vEndOfFileStimulations.push_back(FSettingValueAutoCast(*this->getBoxAlgorithmContext(), (i>>2)+1));
		m_vEndOfFileReached.push_back(false);
		m_vSignalFileEndTimes.push_back(0);
		
	}

	m_oStimulationDecoder.initialize(*this);
	m_oStimulationEncoder.initialize(*this);
	m_oStimulationEncoder.getInputStimulationSet().setReferenceTarget(m_oStimulationDecoder.getOutputStimulationSet());

	m_oSignalDecoder.initialize(*this);
	m_oSignalEncoder.initialize(*this);
	m_oSignalEncoder.getInputSamplingRate().setReferenceTarget(m_oSignalDecoder.getOutputSamplingRate());
	m_oSignalEncoder.getInputMatrix().setReferenceTarget(m_oSignalDecoder.getOutputMatrix());

	m_oTriggerEncoder.initialize(*this);
	
	m_ui32HeaderReceivedCount = 0;
	m_bHeaderSent = false;
	m_bEndSent = false;
	m_bStimHeaderSent = false;
	m_bConcatenationFinished = false;
	m_bResynchroDone = false;

	m_sState.ui32CurrentBufferIndex      = 0;
	m_sState.ui32CurrentChunkIndex       = 0;
	m_sState.ui32CurrentStimulationIndex = 0;
	m_sState.ui32CurrentStartTime        = 0;
	m_sState.ui32CurrentOffset           = 0;

	m_ui64TriggerDate = 0;
	m_ui64LastChunkStartTime = 0;
	m_ui64LastChunkEndTime = 0;

	return true;
}
/*******************************************************************************/

boolean CBoxAlgorithmSignalConcatenation::uninitialize(void)
{
	m_oStimulationDecoder.uninitialize();
	m_oStimulationEncoder.uninitialize();
	m_oSignalDecoder.uninitialize();
	m_oSignalEncoder.uninitialize();

	m_oTriggerEncoder.uninitialize();

	for(uint32 i = 0; i < m_vSignalBuffers.size(); i++)
	{
		for(uint32 j = 0; j < m_vSignalBuffers[i].size(); j++)
		{
			delete m_vSignalBuffers[i][j];
		}
	}
	for(uint32 i = 0; i < m_vStimulationBuffers.size(); i++)
	{
		for(uint32 j = 0; j < m_vStimulationBuffers[i].size(); j++)
		{
			delete m_vStimulationBuffers[i][j];
		}
	}
	for(uint32 i = 0; i < m_vStimulationSets.size(); i++)
	{
		for(uint32 j = 0; j < m_vStimulationSets[i].size(); j++)
		{
			delete m_vStimulationSets[i][j];
		}
	}
	return true;
}
/*******************************************************************************/

boolean CBoxAlgorithmSignalConcatenation::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}
/*******************************************************************************/

boolean CBoxAlgorithmSignalConcatenation::processClock(CMessageClock& rMessageClock)
{
	if(!m_bHeaderSent || m_bConcatenationFinished)
	{
		return true;
	}

	uint64 l_ui64CurrentPlayerTime = this->getPlayerContext().getCurrentTime();
	
	for(uint32 i = 0; i < m_vSignalFileEndTimes.size(); i++)
	{
		if(!m_vEndOfFileReached[i] && l_ui64CurrentPlayerTime > m_vSignalFileEndTimes[i] + m_ui64TimeOut)
		{
			m_vEndOfFileReached[i] = true;
			this->getLogManager() << LogLevel_Info << "File #" << i+1 << "/" << (this->getStaticBoxContext().getInputCount()/2) << " has timed out (effective end time: "<< time64(m_vSignalFileEndTimes[i]) <<").\n";
		}
	}

	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}
/*******************************************************************************/

boolean CBoxAlgorithmSignalConcatenation::process(void)
{
	
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	//SIGNAL INPUTS
	uint64 l_ui64SamplingFrequency;
	uint32 l_ui32ChannelCount;
	for(uint32 input = 0 ; input < l_rStaticBoxContext.getInputCount() ; input+=2)
	{
		if(! m_vEndOfFileReached[input>>1])
		{
			for(uint32 chunk = 0; chunk < l_rDynamicBoxContext.getInputChunkCount(input); chunk++)
			{
				if(m_ui32HeaderReceivedCount < l_rStaticBoxContext.getInputCount()>>1)
				{
					m_oSignalDecoder.decode(input,chunk);
					
					if(m_oSignalDecoder.isHeaderReceived())
					{
						m_ui32HeaderReceivedCount++;

						l_ui64SamplingFrequency = m_oSignalDecoder.getOutputSamplingRate();
						l_ui32ChannelCount = m_oSignalDecoder.getOutputMatrix()->getDimensionSize(0);
						m_ui32SampleCountPerBuffer = m_oSignalDecoder.getOutputMatrix()->getDimensionSize(1);
						if(input == 0)
						{
							this->getLogManager() << LogLevel_Info << "Common sampling rate is " << l_ui64SamplingFrequency << ", channel count is " << l_ui32ChannelCount << " and sample count per buffer is " << m_ui32SampleCountPerBuffer <<".\n";
							m_oSignalEncoder.encodeHeader(0);
							l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_rDynamicBoxContext.getInputChunkStartTime(input,chunk), l_rDynamicBoxContext.getInputChunkEndTime(input,chunk));
							m_bHeaderSent = true;
						}
						else
						{
							if(m_oSignalDecoder.getOutputSamplingRate() != l_ui64SamplingFrequency)
							{
								this->getLogManager() << LogLevel_Error << "File #" << (input>>1)+1 << "/" << (l_rStaticBoxContext.getInputCount()/2) << " has a different sampling rate ("<< m_oSignalDecoder.getOutputSamplingRate() <<"Hz) than previous file(s) ("<< l_ui64SamplingFrequency <<"Hz).\n";
								return false;
							}
							if(m_oSignalDecoder.getOutputMatrix()->getDimensionSize(0) != l_ui32ChannelCount)
							{
								this->getLogManager() << LogLevel_Error << "File #" << (input>>1)+1 << "/" << (l_rStaticBoxContext.getInputCount()/2) << " has a different channel count ("<< m_oSignalDecoder.getOutputMatrix()->getDimensionSize(0) <<") than previous file(s) ("<< l_ui32ChannelCount <<").\n";
								return false;
							}
							if(m_oSignalDecoder.getOutputMatrix()->getDimensionSize(1) != m_ui32SampleCountPerBuffer)
							{
								this->getLogManager() << LogLevel_Error << "File #" << (input>>1)+1 << "/" << (l_rStaticBoxContext.getInputCount()/2) << " has a different sample count per buffer ("<< m_oSignalDecoder.getOutputMatrix()->getDimensionSize(1) <<") than previous file(s) ("<< m_ui32SampleCountPerBuffer <<").\n";
								return false;
							}
						}
					}
				}
				else
				{
					if(m_vEndOfFileReached[input>>1])
					{
						//just discard it
						l_rDynamicBoxContext.markInputAsDeprecated(input, chunk);
					}
					else
					{
						IMemoryBuffer * l_pBuffer = new CMemoryBuffer();
						l_pBuffer->setSize(l_rDynamicBoxContext.getInputChunk(input,chunk)->getSize(),true);
						System::Memory::copy(
							l_pBuffer->getDirectPointer(),
							l_rDynamicBoxContext.getInputChunk(input,chunk)->getDirectPointer(),
							l_pBuffer->getSize());

						m_vSignalBuffers[input>>1].push_back(l_pBuffer);

						m_vSignalFileEndTimes[input>>1] = l_rDynamicBoxContext.getInputChunkEndTime(input,chunk);
						m_vSignalBufferEndTimes[input>>1].push_back(l_rDynamicBoxContext.getInputChunkEndTime(input,chunk));

						l_rDynamicBoxContext.markInputAsDeprecated(input, chunk);
					}
				}
			}
		}
	}

	//STIMULATION INPUTS
	for(uint32 input = 1 ; input < l_rStaticBoxContext.getInputCount() ; input+=2)
	{
		for(uint32 chunk = 0; chunk < l_rDynamicBoxContext.getInputChunkCount(input); chunk++)
		{
			m_oStimulationDecoder.decode(input,chunk);
			if(m_oStimulationDecoder.isHeaderReceived() && !m_bStimHeaderSent)
			{
				m_oStimulationEncoder.encodeHeader(1);
				l_rDynamicBoxContext.markOutputAsReadyToSend(1,l_rDynamicBoxContext.getInputChunkStartTime(input,chunk),l_rDynamicBoxContext.getInputChunkEndTime(input,chunk));
				m_oTriggerEncoder.encodeHeader(2);
				l_rDynamicBoxContext.markOutputAsReadyToSend(2,l_rDynamicBoxContext.getInputChunkStartTime(input,chunk),l_rDynamicBoxContext.getInputChunkEndTime(input,chunk));
				m_bStimHeaderSent = true;
			}
			if(m_oStimulationDecoder.isEndReceived() && !m_bEndSent)
			{
				m_oStimulationEncoder.encodeEnd(1);
				l_rDynamicBoxContext.markOutputAsReadyToSend(1,l_rDynamicBoxContext.getInputChunkStartTime(input,chunk),l_rDynamicBoxContext.getInputChunkEndTime(input,chunk));
				m_oTriggerEncoder.encodeEnd(2);
				l_rDynamicBoxContext.markOutputAsReadyToSend(2,l_rDynamicBoxContext.getInputChunkStartTime(input,chunk),l_rDynamicBoxContext.getInputChunkEndTime(input,chunk));
				m_oSignalEncoder.encodeEnd(0);
				l_rDynamicBoxContext.markOutputAsReadyToSend(0,l_rDynamicBoxContext.getInputChunkStartTime(input,chunk),l_rDynamicBoxContext.getInputChunkEndTime(input,chunk));
				m_bEndSent = true;
			}
			if(m_oStimulationDecoder.isBufferReceived())
			{
				//in any case, we store the memory buffer.
				IMemoryBuffer * l_pBuffer = new CMemoryBuffer();
				l_pBuffer->setSize(l_rDynamicBoxContext.getInputChunk(input,chunk)->getSize(),true);
				System::Memory::copy(
					l_pBuffer->getDirectPointer(),
					l_rDynamicBoxContext.getInputChunk(input,chunk)->getDirectPointer(),
					l_pBuffer->getSize());

				m_vStimulationBuffers[input>>1].push_back(l_pBuffer);
				m_vStimulationBufferEndTimes[input>>1].push_back(l_rDynamicBoxContext.getInputChunkEndTime(input,chunk));

				if(m_oStimulationDecoder.getOutputStimulationSet()->getStimulationCount() > 0)
				{
					IStimulationSet * l_pStimulationSet = new CStimulationSet();
				
					for(uint32 stim = 0; stim < m_oStimulationDecoder.getOutputStimulationSet()->getStimulationCount(); stim++)
					{
						if(m_oStimulationDecoder.getOutputStimulationSet()->getStimulationIdentifier(stim) == m_vEndOfFileStimulations[input>>1])
						{
							this->getLogManager() << LogLevel_Info << "File #" << (input>>1)+1 << "/" << (l_rStaticBoxContext.getInputCount()/2) << " is finished (end time: "<< time64(m_vSignalFileEndTimes[input>>1]) <<"). Any more signal chunk will be discarded.\n";
							m_vEndOfFileReached[input>>1] = true;
						}
					
						l_pStimulationSet->appendStimulation(
							m_oStimulationDecoder.getOutputStimulationSet()->getStimulationIdentifier(stim),
							m_oStimulationDecoder.getOutputStimulationSet()->getStimulationDate(stim),
							m_oStimulationDecoder.getOutputStimulationSet()->getStimulationDuration(stim));
						m_vStimulations[input>>1].push_back(make_pair(
							m_oStimulationDecoder.getOutputStimulationSet()->getStimulationIdentifier(stim),
							m_oStimulationDecoder.getOutputStimulationSet()->getStimulationDate(stim)));
					}

					m_vStimulationSets[input>>1].push_back(l_pStimulationSet);
				}
				else
				{
					m_vStimulationSets[input>>1].push_back(NULL);
				}
				
			}
		}
	}

	boolean l_bShouldConcatenate = true;
	for(uint32 i = 0; i < m_vEndOfFileReached.size(); i++)
	{
		l_bShouldConcatenate &= m_vEndOfFileReached[i];
	}

	if(l_bShouldConcatenate && !m_bConcatenationFinished)
	{
		if(!this->concate())
		{
			// concatenation not finished, we will resume on next process
			return true;
		}
		else
		{
			m_oTriggerEncoder.getInputStimulationSet()->appendStimulation(OVTK_StimulationId_EndOfFile, this->getPlayerContext().getCurrentTime(), 0);
			m_oTriggerEncoder.encodeBuffer(2);
			l_rDynamicBoxContext.markOutputAsReadyToSend(2,this->getPlayerContext().getCurrentTime(),this->getPlayerContext().getCurrentTime());
			m_bConcatenationFinished = true;
		}
	}

	return true;
}


boolean CBoxAlgorithmSignalConcatenation::concate(void)
{
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();
	if(!m_bResynchroDone)
	{
		this->getLogManager() << LogLevel_Info << "Concatenation in progress...\n";
		
		
		this->getLogManager() << LogLevel_Trace << "Resynchronizing stimulations...\n";

		uint64 l_ui64Offset = m_vSignalFileEndTimes[0];		
		for(uint32 i = 1; i < m_vStimulations.size(); i++)
		{
			for(uint32 j = 0; j < m_vStimulations[i].size(); j++)
			{
				uint64 l_ui64SynchronizedDate = m_vStimulations[i][j].second + l_ui64Offset;
				//this->getLogManager() << LogLevel_Info << "Resynchronizing stim ["<<m_vStimulations[i][j].first<<"] from time ["<<m_vStimulations[i][j].second<<"] to ["<<l_ui64SynchronizedDate<<"]\n";
				m_vStimulations[i][j].second = l_ui64SynchronizedDate;
			}
			l_ui64Offset = l_ui64Offset + m_vSignalFileEndTimes[i];
		}
		
		this->getLogManager() << LogLevel_Trace << "Resynchronization finished.\n";
		m_bResynchroDone = true;
	}
	uint64 l_ui64CurrentEndTime = m_sState.ui32CurrentStartTime;
	uint64 l_ui64CurrentStartTime = m_sState.ui32CurrentStartTime;
	uint64 l_ui64Offset = m_sState.ui32CurrentOffset;
		
	for(uint32 i = m_sState.ui32CurrentBufferIndex; i < m_vSignalBuffers.size(); i++)
	{
		for(uint32 j = m_sState.ui32CurrentChunkIndex; j < m_vSignalBuffers[i].size(); j++)
		{
			l_ui64CurrentEndTime = m_vSignalBufferEndTimes[i][j] + l_ui64Offset;
			IMemoryBuffer * l_pBuffer = m_vSignalBuffers[i][j];
				
			//we check if some stims are in the current time scope
			IStimulationSet * l_pStimSet = m_oStimulationEncoder.getInputStimulationSet();
			l_pStimSet->clear();
			boolean l_bStimFound = false;
			for(uint32 s = m_sState.ui32CurrentStimulationIndex; s < m_vStimulations[i].size(); s++)
			{
				if(l_ui64CurrentStartTime <= m_vStimulations[i][s].second  && m_vStimulations[i][s].second < l_ui64CurrentEndTime)
				{
					l_pStimSet->appendStimulation(m_vStimulations[i][s].first, m_vStimulations[i][s].second, 0);
					//this->getLogManager() << LogLevel_Info << "Appending stim ["<<m_vStimulations[i][s].first<<"] with date ["<<m_vStimulations[i][s].second<<"]\n";
					m_vStimulations[i][s].second = -1; // should not be needed...
					l_bStimFound = true;

					m_sState.ui32CurrentBufferIndex      = i;
					m_sState.ui32CurrentChunkIndex       = j+1;
					m_sState.ui32CurrentStimulationIndex = s+1;
					m_sState.ui32CurrentStartTime        = l_ui64CurrentEndTime;
					m_sState.ui32CurrentOffset           = l_ui64Offset;
				}
			}
				
			// we write the signal memory buffer
			IMemoryBuffer* l_pOutputMemoryBuffer=l_rDynamicBoxContext.getOutputChunk(0);
			l_pOutputMemoryBuffer->setSize(l_pBuffer->getSize(), true);
			System::Memory::copy(
				l_pOutputMemoryBuffer->getDirectPointer(),
				l_pBuffer->getDirectPointer(),
				l_pBuffer->getSize());
			l_rDynamicBoxContext.markOutputAsReadyToSend(0,l_ui64CurrentStartTime,l_ui64CurrentEndTime);
			
			// then the stim memory buffer even if it is empty
			m_oStimulationEncoder.encodeBuffer(1);
			l_rDynamicBoxContext.markOutputAsReadyToSend(1,l_ui64CurrentStartTime,l_ui64CurrentEndTime);

			l_ui64CurrentStartTime = l_ui64CurrentEndTime;

			if(l_bStimFound)
			{
				// we stop the concatenation here, to let the kernel send the chunks.
				// we will resume at next process()
				// this behaviour is implemented to avoid the Generic Stream R/W bug (all stims written at the end of the concatenation file)
				return false;
			}
		}
		l_ui64Offset += m_vSignalFileEndTimes[i];
		l_ui64CurrentStartTime = l_ui64Offset;
		
		m_sState.ui32CurrentBufferIndex      = i+1;
		m_sState.ui32CurrentChunkIndex       = 0;
		m_sState.ui32CurrentStimulationIndex = 0;
		m_sState.ui32CurrentStartTime        = l_ui64Offset;
		m_sState.ui32CurrentOffset           = l_ui64Offset;
	}
		
	this->getLogManager() << LogLevel_Info << "Concatenation finished !\n";
	
	return true;

}
