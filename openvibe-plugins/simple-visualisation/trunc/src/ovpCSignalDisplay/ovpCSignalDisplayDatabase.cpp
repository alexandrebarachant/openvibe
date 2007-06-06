#include "ovpCSignalDisplayDatabase.h"

#include <system/Memory.h>

#include <math.h>
#include <cfloat>

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

		CSignalDisplayDatabase::CSignalDisplayDatabase() :
			m_ui32ChannelCount(0),
			m_ui32SamplingFrequency(0),
			m_ui32SamplesPerBuffer(0),
			m_ui64NumberOfBufferToDisplay(2),
			m_f64MaximumValue(-DBL_MAX),
			m_f64MinimumValue(+DBL_MAX),
			m_pDrawable(NULL)
		{
			adjustNumberOfDisplayedBuffers(1000);
		}

		CSignalDisplayDatabase::~CSignalDisplayDatabase()
		{
			//delete all the remaining buffers
			for(unsigned int i=0 ; i<m_oSampleBuffers.size() ; i++)
			{
				delete[] m_oSampleBuffers.front();
				m_oSampleBuffers.pop_front();
			}

		}

		OpenViBE::boolean CSignalDisplayDatabase::adjustNumberOfDisplayedBuffers(OpenViBE::float64 f64NumberOfMsToDisplay)
		{
			OpenViBE::boolean l_bNumberOfBufferToDisplayChanged = false;

			OpenViBE::uint64 l_ui64NewNumberOfBufferToDisplay =  static_cast<uint64>(ceil( (f64NumberOfMsToDisplay*m_ui32SamplingFrequency) / (m_ui32SamplesPerBuffer*1000)));

			//displays at least one buffer
			l_ui64NewNumberOfBufferToDisplay = (l_ui64NewNumberOfBufferToDisplay == 0) ? 1 : l_ui64NewNumberOfBufferToDisplay;
			if(l_ui64NewNumberOfBufferToDisplay != m_ui64NumberOfBufferToDisplay)
			{
				m_ui64NumberOfBufferToDisplay = l_ui64NewNumberOfBufferToDisplay;
				l_bNumberOfBufferToDisplayChanged = true;

				//if the new number of buffer to display is greater than the previous one, destroy the useless buffers
				while(m_ui64NumberOfBufferToDisplay < m_oSampleBuffers.size())
				{
					delete[] m_oSampleBuffers.front();
					m_oSampleBuffers.pop_front();

					//suppress the corresponding starting time from the list
					m_oStartTime.pop_front();
				}
			}

			return l_bNumberOfBufferToDisplayChanged;
		}
		
		OpenViBE::float64 CSignalDisplayDatabase::getDisplayedTimeIntervalWidth()
		{
			return (m_ui64NumberOfBufferToDisplay * ((m_ui32SamplesPerBuffer*1000.0) / m_ui32SamplingFrequency));
		}

		void CSignalDisplayDatabase::setChannelCount(OpenViBE::uint32 ui32ChannelCount)
		{
			m_ui32ChannelCount = ui32ChannelCount;
			m_vChannelName.resize(ui32ChannelCount);
		}

		void CSignalDisplayDatabase::setSampleCountPerBuffer(OpenViBE::uint32 ui32SampleCountPerBuffer)
		{
			m_ui32SamplesPerBuffer = ui32SampleCountPerBuffer;

			//we have all the information about the incoming signal, so we can initialize the drawable object
			m_pDrawable->init();
		}

		void CSignalDisplayDatabase::setChannelName(OpenViBE::uint32 ui32ChannelIndex, const char * pLabel)
		{
			//adds the label to the list
			m_vChannelName[ui32ChannelIndex] = pLabel;
		}

		void CSignalDisplayDatabase::setSamplingRate(const OpenViBE::uint32 ui32SamplingFrequency)
		{
			m_ui32SamplingFrequency = ui32SamplingFrequency;
		}


		void CSignalDisplayDatabase::setSampleBuffer(const OpenViBE::float64* pBuffer, OpenViBE::uint64 ui64StartTime)
		{
			uint64 l_ui64NumberOfSamplesPerBuffer = m_ui32SamplesPerBuffer*m_ui32ChannelCount;

			float64 * l_pBufferToWrite;

			//Adds the starting time to the list
			m_oStartTime.push_back(ui64StartTime);


			if(m_ui64NumberOfBufferToDisplay - m_oSampleBuffers.size() > 0)
			{
				////If the number of buffer to display has changed and is greater than before, create a new buffer
				l_pBufferToWrite = new OpenViBE::float64[(size_t)l_ui64NumberOfSamplesPerBuffer];
			}
			else if (m_ui64NumberOfBufferToDisplay == m_oSampleBuffers.size())
			{
				//If it is the same than before, get the first buffer and removes it from the list
				l_pBufferToWrite = m_oSampleBuffers.front();
				m_oSampleBuffers.pop_front();

				//remove the corresponding starting time
				m_oStartTime.pop_front();
			}


			//find min and max values of the samples
			for(uint64 i = 0 ; i<l_ui64NumberOfSamplesPerBuffer ; i++)
			{
				if(pBuffer[i] > m_f64MaximumValue)
				{
					m_f64MaximumValue = pBuffer[i];
				}
				if(pBuffer[i] < m_f64MinimumValue)
				{
					m_f64MinimumValue = pBuffer[i];
				}
			}

			//copy the content of pBuffer into the selected buffer
			System::Memory::copy(l_pBufferToWrite,
			       pBuffer,
			       l_ui64NumberOfSamplesPerBuffer*sizeof(float64));

			//add the buffer at the end of the list
			m_oSampleBuffers.push_back(l_pBufferToWrite);

			//tells the drawable to redraw himself since the signal information has been updated
			m_pDrawable->redraw();
		}


	}
}
