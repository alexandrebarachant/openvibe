#include "ovpCBufferDatabase.h"

#include <system/Memory.h>

#include <algorithm>

#include <math.h>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;
using namespace OpenViBE::Kernel;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;

using namespace OpenViBEToolkit;

using namespace std;

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{

		CBufferDatabase::CBufferDatabase(OpenViBEToolkit::TBoxAlgorithm<Plugins::IBoxAlgorithm>& oPlugin) :
			m_bFirstBufferReceived(true),
			m_ui32SamplingFrequency(0),
			m_ui64NumberOfBufferToDisplay(2),
			m_f64MaximumValue(-DBL_MAX),
			m_f64MinimumValue(+DBL_MAX),
			m_pDrawable(NULL),
			m_oParentPlugin(oPlugin),
			m_bError(false)
		{
		}

		CBufferDatabase::~CBufferDatabase()
		{
			//delete all the remaining buffers
			for(unsigned int i=0 ; i<m_oSampleBuffers.size() ; i++)
			{
				delete[] m_oSampleBuffers.front();
				m_oSampleBuffers.pop_front();
			}

		}

		boolean CBufferDatabase::adjustNumberOfDisplayedBuffers(float64 f64NumberOfMsToDisplay)
		{
			boolean l_bNumberOfBufferToDisplayChanged = false;

			uint64 l_ui64NewNumberOfBufferToDisplay =  static_cast<uint64>(ceil( (f64NumberOfMsToDisplay*m_ui32SamplingFrequency) / (m_pDimmensionSizes[1]*1000)));

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

					//suppress the corresponding minmax values
					for(uint32 c=0 ; c<m_pDimmensionSizes[0] ; c++)
					{
						m_oLocalMinMaxValue[c].pop_front();
					}
				}
			}

			return l_bNumberOfBufferToDisplayChanged;
		}

		float64 CBufferDatabase::getDisplayedTimeIntervalWidth()
		{
			return (m_ui64NumberOfBufferToDisplay * ((m_pDimmensionSizes[1]*1000.0) / m_ui32SamplingFrequency));
		}

		void CBufferDatabase::setMatrixDimmensionCount(const uint32 ui32DimmensionCount)
		{
			if(ui32DimmensionCount != 2)
			{

				m_oParentPlugin.getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Warning << "Error dimmension count isn't 2!\n";
			}
		}

		void CBufferDatabase::setMatrixDimmensionSize(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionSize)
		{
			m_pDimmensionSizes[ui32DimmensionIndex] = ui32DimmensionSize;
			m_pDimmesionLabels[ui32DimmensionIndex].resize(ui32DimmensionSize);

			if(ui32DimmensionIndex == 0)
			{
				m_oLocalMinMaxValue.resize(ui32DimmensionSize);
			}
		}

		void CBufferDatabase::setMatrixDimmensionLabel(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel)
		{
			m_pDimmesionLabels[ui32DimmensionIndex][ui32DimmensionEntryIndex] = sDimmensionLabel;
		}

		void CBufferDatabase::setMatrixBuffer(const float64* pBuffer, uint64 ui64StartTime, uint64 ui64EndTime)
		{
			//if an error has occured, do nothing
			if(m_bError)
			{
				return;
			}

			uint64 l_ui64NumberOfSamplesPerBuffer = m_pDimmensionSizes[0] * m_pDimmensionSizes[1];

			if(m_bFirstBufferReceived)
			{
				m_ui64BufferDuration = ui64EndTime - ui64StartTime;

				//test if it is equal to zero : Error
				if(m_ui64BufferDuration == 0)
				{
					m_oParentPlugin.getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Warning << "Error : buffer start time and end time are equal!\n";

					m_bError = true;
					return;
				}

				//computes the sampling frequency
				m_ui32SamplingFrequency = (uint32)((float64)(((uint64)1<<32)/(m_ui64BufferDuration)) * m_pDimmensionSizes[1]);

				//computes the number of buffer necessary to display a 10s interval
				adjustNumberOfDisplayedBuffers(10000);

				m_pDrawable->init();

				m_bFirstBufferReceived = false;
			}

			float64 * l_pBufferToWrite;

			//Adds the starting time to the list
			m_oStartTime.push_back(ui64StartTime);

			if(m_ui64NumberOfBufferToDisplay - m_oSampleBuffers.size() > 0)
			{
				////If the number of buffer to display has changed and is greater than before, create a new buffer
				l_pBufferToWrite = new float64[(size_t)l_ui64NumberOfSamplesPerBuffer];
			}
			else if (m_ui64NumberOfBufferToDisplay == m_oSampleBuffers.size())
			{
				//If it is the same than before, get the first buffer and removes it from the list
				l_pBufferToWrite = m_oSampleBuffers.front();
				m_oSampleBuffers.pop_front();

				//remove the corresponding starting time
				m_oStartTime.pop_front();

				//removes the corresponding minmax values
				for(uint32 c=0 ; c<m_pDimmensionSizes[0] ; c++)
				{
					m_oLocalMinMaxValue[c].pop_front();
				}
			}

			uint64 l_ui64CurrentSample = 0;
			//for each channel
			for(uint32 c=0 ; c<m_pDimmensionSizes[0] ; c++)
			{
				float64 l_f64LocalMin = DBL_MAX;
				float64 l_f64LocalMax = -DBL_MAX;

				//for each sample
				for(uint64 i=0 ; i<m_pDimmensionSizes[1] ; i++, l_ui64CurrentSample++)
				{
					//get channel local min/max
					if(pBuffer[l_ui64CurrentSample] < l_f64LocalMin)
					{
						l_f64LocalMin = pBuffer[l_ui64CurrentSample];
					}
					if(pBuffer[l_ui64CurrentSample] > l_f64LocalMax)
					{
						l_f64LocalMax = pBuffer[l_ui64CurrentSample];
					}
				}

				//adds the minmax pair to the corresponding channel's list
				m_oLocalMinMaxValue[c].push_back(pair<float64, float64>(l_f64LocalMin, l_f64LocalMax));

				if(l_f64LocalMax > m_f64MaximumValue)
				{
					m_f64MaximumValue = l_f64LocalMax;
				}
				if(l_f64LocalMin < m_f64MinimumValue)
				{
					m_f64MinimumValue = l_f64LocalMin;
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

		void CBufferDatabase::getDisplayedChannelLocalMinMaxValue(uint32 ui32Channel, float64& f64Min, float64& f64Max)
		{
			f64Min = +DBL_MAX;
			f64Max = -DBL_MAX;

			for(uint64 i=0 ; i<m_oLocalMinMaxValue[(size_t)ui32Channel].size() ; i++)
			{
				if(f64Min > m_oLocalMinMaxValue[(size_t)ui32Channel][(size_t)i].first)
				{
					f64Min = m_oLocalMinMaxValue[(size_t)ui32Channel][(size_t)i].first;
				}
				if(f64Max < m_oLocalMinMaxValue[(size_t)ui32Channel][(size_t)i].second)
				{
					f64Max = m_oLocalMinMaxValue[(size_t)ui32Channel][(size_t)i].second;
				}
			}
		}

		void CBufferDatabase::getDisplayedGlobalMinMaxValue(float64& f64Min, float64& f64Max)
		{
			for(uint32 c=0 ; c<m_oLocalMinMaxValue.size() ; c++)
			{
				for(uint64 i=0 ; i<m_oLocalMinMaxValue[(size_t)c].size() ; i++)
				{
					if(f64Min > m_oLocalMinMaxValue[(size_t)c][(size_t)i].first)
					{
						f64Min = m_oLocalMinMaxValue[(size_t)c][(size_t)i].first;
					}
					if(f64Max < m_oLocalMinMaxValue[(size_t)c][(size_t)i].second)
					{
						f64Max = m_oLocalMinMaxValue[(size_t)c][(size_t)i].second;
					}
				}
			}
		}

		void CBufferDatabase::setStimulationCount(const uint32 ui32StimulationCount)
		{
		}

		void CBufferDatabase::setStimulation(const uint32 ui32StimulationIndex, const uint64 ui64StimulationIdentifier, const uint64 ui64StimulationDate)
		{
			// m_oParentPlugin.getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Trace << "Received new stimulation id:" << ui64StimulationIdentifier << " date:" << ui64StimulationDate << "\n";

			m_oStimulations.push_back(std::pair<uint64, uint64>(ui64StimulationDate, ui64StimulationIdentifier));

			std::deque<std::pair<uint64, uint64> >::iterator i;
			for(i=m_oStimulations.begin(); i!=m_oStimulations.end() && i->first<m_oStartTime.front(); )
			{
				i++;
				m_oStimulations.pop_front();
			}
		}
	}
}
