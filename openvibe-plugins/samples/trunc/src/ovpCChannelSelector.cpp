#include "ovpCChannelSelector.h"

#include <iostream>
#include <sstream>

using namespace OpenViBEToolkit;

using namespace std;

namespace OpenViBEPlugins
{
	namespace Samples
	{
		
		void CChannelSelector::setChannelCount(const OpenViBE::uint32 ui32ChannelCount)
		{
			//gets the channel count
			m_pSignalDescription->m_ui32ChannelCount = ui32ChannelCount;
		}
		
		
		void CChannelSelector::setChannelName(const OpenViBE::uint32 ui32ChannelIndex, const char* sChannelName)
		{
			//copy the channel name into another buffer
			const char  * l_pChannelNameTemp = sChannelName;
					
			//If the channels are selected by name
			if(!m_bSelectionbyIndex)
			{
				//tries to find the name of the current channel in the selected channel list
				//if found, stores the index in the map
				for(int i=0 ; i<m_vSelectedChannelNames.size() ; i++)
				{
					if(m_vSelectedChannelNames[i].compare(0, m_vSelectedChannelNames[i].length(), l_pChannelNameTemp, m_vSelectedChannelNames[i].length()) == 0)
					{
						m_oSelectedIndex[m_vSelectedChannelNames[i]]=ui32ChannelIndex;
					}
				}
			}
			//if they are selected by index
			else
			{
				//tries to find the index of the current channel in the selected indexes list
				//if found, stores the index in the map
				for(int i=0 ; i<m_vSelectedChannelIndexes.size() ; i++)
				{
					if(m_vSelectedChannelIndexes[i] == ui32ChannelIndex)
					{
						m_oSelectedIndex[l_pChannelNameTemp]=ui32ChannelIndex;
					}
				}
			}

			//if we've just read the last channel label
			if(ui32ChannelIndex == m_pSignalDescription->m_ui32ChannelCount-1)
			{
				//we've read all the channel information
				m_bCurrentDimension = true;

				//the new channel count is the number of selected channels found in the input stream
				m_pSignalDescription->m_ui32ChannelCount = m_oSelectedIndex.size();

				//check if we have at least one channel
				if(m_pSignalDescription->m_ui32ChannelCount == 0)
				{
					cout<<"0 channels selected!"<<endl;
					//stop output in this case
				}
				else
				{
					//lists the channels that were found
					m_pSignalDescription->m_pChannelName.resize(m_pSignalDescription->m_ui32ChannelCount);
					OpenViBE::uint32 l_ui32CurrentIndex  = 0;
					
					//If the channels are selected by name
					if(!m_bSelectionbyIndex)
					{
						//by selecting the channels names like that, we adopt the same order for the channels than 
						//the one used by the user in the configuration box
						for(int i=0 ; i<m_vSelectedChannelNames.size() ; i++)
						{
							if(m_oSelectedIndex.count(m_vSelectedChannelNames[i]) != 0)
							{
								m_pSignalDescription->m_pChannelName[l_ui32CurrentIndex]=m_vSelectedChannelNames[i];
								l_ui32CurrentIndex++;
							}
						}
					}
					//If the channels are selected by index
					else
					{
						std::map<string,OpenViBE::uint32>::iterator iter;
								
						for(int i=0 ; i<m_vSelectedChannelIndexes.size() ; i++)
						{
							for(iter=m_oSelectedIndex.begin(); iter!=m_oSelectedIndex.end() ; iter++)
							{
								if(iter->second == m_vSelectedChannelIndexes[i])
								{
									m_pSignalDescription->m_pChannelName[l_ui32CurrentIndex]=iter->first;
									l_ui32CurrentIndex++;
								}
							}
						}
					}
				}
			}
		}
		
		
		
		void CChannelSelector::setSampleCountPerBuffer(const OpenViBE::uint32 ui32SampleCountPerBuffer)
		{
			//Don't send anything if no channels are selected
			if(m_pSignalDescription->m_ui32ChannelCount!=0)
			{
				//gets the sample count
				m_pSignalDescription->m_ui32SampleCount = ui32SampleCountPerBuffer;
						
						
				//the matrix buffer hasn't been allocated yet, allocate it
				if(!m_pMatrixBuffer)
				{
					m_ui64MatrixBufferSize = m_pSignalDescription -> m_ui32SampleCount * m_pSignalDescription -> m_ui32ChannelCount;
	
					m_pMatrixBuffer = new EBML::float64[m_ui64MatrixBufferSize];
				}
						
				//we have everything needed to send the stream header
				m_pSignalDescription->m_bReadyToSend = true;
				
			}
		}
		
		
		void CChannelSelector::setSamplingRate(const OpenViBE::uint32 ui32SamplingFrequency)
		{
			m_pSignalDescription->m_ui32SamplingRate = ui32SamplingFrequency;
		}
		
		
		void CChannelSelector::setSampleBuffer(const OpenViBE::float64* pBuffer)
		{
			//Don't send anything if no channels are selected
			if(m_pSignalDescription->m_ui32ChannelCount!=0)
			{
				const EBML::uint8 * l_pSourceMatrix = reinterpret_cast<const EBML::uint8 *>(pBuffer);	
				OpenViBE::uint8 * l_pDestinationMatrix = reinterpret_cast<EBML::uint8 *>(m_pMatrixBuffer);
					
				//size of a sample block (for one channel) in bytes
				OpenViBE::uint64 l_ui64SampleBlockSize = m_pSignalDescription->m_ui32SampleCount * sizeof(EBML::float64);
	
				//for each selected channel found in the input stream
				for(int i=0 ; i<m_pSignalDescription->m_ui32ChannelCount ; i++)
				{
					//gets its "index" in the input stream's channel list
					OpenViBE::uint32 l_ui32CurrentChannelIndex = m_oSelectedIndex[m_pSignalDescription->m_pChannelName[i]];
						
					//copy the sample block of the current selected channel
					memcpy(l_pDestinationMatrix + (i*l_ui64SampleBlockSize), l_pSourceMatrix+(l_ui32CurrentChannelIndex*l_ui64SampleBlockSize), l_ui64SampleBlockSize);
	
				}
	
				//the matrix is ready to be sent
				m_bMatrixReadyToSend = true;
			}
		}
		

		void CChannelSelector::writeSignalOutput(const void* pBuffer, const EBML::uint64 ui64BufferSize)
		{
			appendOutputChunkData<0>(pBuffer, ui64BufferSize);
		}



		CChannelSelector::CChannelSelector(void)
			:m_bSignalDescriptionSent(false)
			,m_pSignalDescription(NULL)
			,m_pMatrixBuffer(NULL)
			,m_ui64MatrixBufferSize(0)
			,m_bMatrixReadyToSend(false)
			,m_bSelectionbyIndex(false)
			,m_pReader(NULL)
			,m_pSignalReaderCallBack(NULL)
			,m_bCurrentDimension(false)
			,m_pWriter(NULL)
			,m_oSignalOutputWriterCallbackProxy(*this)
		{
		}
		
		void CChannelSelector::release(void)
		{
		}
		
		
		OpenViBE::boolean CChannelSelector::initialize()
		{
			//reads the plugin settings
			OpenViBE::CString l_sSettings;
			getBoxAlgorithmContext()->getStaticBoxContext()->getSettingValue(1, l_sSettings);
			if(l_sSettings == OpenViBE::CString("S"))
			{
				m_bSelectionbyIndex = false;
			}
			else if(l_sSettings == OpenViBE::CString("I"))
			{
				m_bSelectionbyIndex = true;
			}
			else
			{
				cout<<"Selection mode needs to be either S or I!"<<endl
				<<"Using default mode : Selection by name"<<endl;
				m_bSelectionbyIndex = false;
			}

			//reads the plugin settings
			getBoxAlgorithmContext()->getStaticBoxContext()->getSettingValue(0, l_sSettings);
				
			istringstream l_sChannelList(static_cast<const char *>(l_sSettings));
			
			string l_sCurrentChannelId;
				
			//parses the configuration string
			while(!l_sChannelList.eof())
			{
				getline(l_sChannelList, l_sCurrentChannelId, OV_Value_EnumeratedStringSeparator);
				
				//if the channels are selected by their names
				if(!m_bSelectionbyIndex)
				{
					//We don't want empty strings (causes problems when comparing with input stream's channel's labels)
					if(l_sCurrentChannelId.length() != 0)
					{
						m_vSelectedChannelNames.push_back(l_sCurrentChannelId);
					}
				}
				else
				{
					m_vSelectedChannelIndexes.push_back(atol(l_sCurrentChannelId.c_str()));	
				}
				
			}
						
			
			//initialises the signal description structure
			m_pSignalDescription = new CSignalDescription();
			
			// Prepares EBML reader
			m_pSignalReaderCallBack = createBoxAlgorithmSignalInputReaderCallback(*this);
			m_pReader=EBML::createReader(*m_pSignalReaderCallBack);

			// Prepares EBML writer
			m_pSignalOutputWriterHelper=createBoxAlgorithmSignalOutputWriter();
			m_pWriter=EBML::createWriter(m_oSignalOutputWriterCallbackProxy);
				
			return true;
		}
		
		OpenViBE::boolean CChannelSelector::uninitialize()
		{
			if(m_pMatrixBuffer)
			{
				delete[] m_pMatrixBuffer;
			}

			// Cleans up EBML writer
			m_pWriter->release();
			m_pWriter=NULL;

			releaseBoxAlgorithmSignalOutputWriter(m_pSignalOutputWriterHelper);
			m_pSignalOutputWriterHelper=NULL;

			// Cleans up EBML reader
			releaseBoxAlgorithmSignalInputReaderCallback(m_pSignalReaderCallBack);

			m_pReader->release();
			m_pReader=NULL;

			delete m_pSignalDescription;
			m_pSignalDescription = NULL;

			return true;
		}
		
		OpenViBE::boolean CChannelSelector::processInput( OpenViBE::uint32 ui32InputIndex)
		{
			getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
			return true;
		}
		
		OpenViBE::boolean CChannelSelector::process()
		{
			OpenViBE::Plugins::IStaticBoxContext* l_pStaticBoxContext=getBoxAlgorithmContext()->getStaticBoxContext();
			OpenViBE::Plugins::IDynamicBoxContext* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();
			

			//reset the OutPut chunk
			l_pDynamicBoxContext->setOutputChunkSize(0, 0);
			
			//Sends the signal description header
			if(!m_bSignalDescriptionSent)
			{
				if(m_pSignalDescription->m_bReadyToSend)
				{
					m_pSignalOutputWriterHelper->setSamplingRate(m_pSignalDescription->m_ui32SamplingRate);
					m_pSignalOutputWriterHelper->setChannelCount(m_pSignalDescription->m_ui32ChannelCount);
			

			
					for(int i=0 ; i<m_pSignalDescription->m_ui32ChannelCount ; i++)
					{
						m_pSignalOutputWriterHelper->setChannelName(i, m_pSignalDescription->m_pChannelName[i].c_str());
					}

					m_pSignalOutputWriterHelper->setSampleCount(m_pSignalDescription->m_ui32SampleCount);
					m_pSignalOutputWriterHelper->setSampleBuffer(m_pMatrixBuffer);
			
					m_pSignalOutputWriterHelper->writeHeader(*m_pWriter);

					m_bSignalDescriptionSent=true;
					l_pDynamicBoxContext->markOutputAsReadyToSend(0, m_ui64LastChunkStartTime, m_ui64LastChunkEndTime);
				}
			}
			
			
			//sends the current signal matrix if ready
			if(m_bMatrixReadyToSend)
			{
				m_pSignalOutputWriterHelper->writeBuffer(*m_pWriter);

				m_bMatrixReadyToSend = false;
				l_pDynamicBoxContext->markOutputAsReadyToSend(0, m_ui64LastChunkStartTime, m_ui64LastChunkEndTime);
			}
			
			
			// Process input data
			for(OpenViBE::uint32 i=0; i<l_pDynamicBoxContext->getInputChunkCount(0); i++)
			{
				
				OpenViBE::uint64 l_ui64ChunkSize;
				const OpenViBE::uint8* l_pBuffer;
				l_pDynamicBoxContext->getInputChunk(0, i, m_ui64LastChunkStartTime, m_ui64LastChunkEndTime, l_ui64ChunkSize, l_pBuffer);
				l_pDynamicBoxContext->markInputAsDeprecated(0, i);
				m_pReader->processData(l_pBuffer, l_ui64ChunkSize);
			}
			
			
			return true;
		}
		
	}
}
