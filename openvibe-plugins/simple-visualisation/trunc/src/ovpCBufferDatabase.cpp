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
		//FIXME : use info from signalstream
		static const unsigned int s_nbChannels = 72 + 23;

		static string s_channelLabels[s_nbChannels] = {
			//72 channels from INSERM's elec.dat
			"O1",
			"O2",
			"T5",
			"P3",
			"Pz",
			"P4",
			"T6",
			"T3",
			"C3",
			"Cz",
			"C4",
			"T4",
			"F7",
			"F3",
 			"Fz",
 			"F4",
 			"F8",
			"Fp1",
 			"Fp2",
			"P5",
			"P1",
			"P2",
			"P6",
			"C5",
			"C1",
			"C2",
			"C6",
			"F5",
			"F1",
			"F2",
			"F6",
			"M1",
			"M2",
			"Oz",
			"POz",
			"CPz",
			"FCz",
			"FFz",
			"Fpz",
			"FC1",
			"FC2",
			"CP1",
			"CP2",
			"FC3",
			"FC4",
			"CP3",
			"CP4",
			"FT3",
			"FT4",
			"TP3",
			"TP4",
			"P'1",
			"P'2",
			"Nz",
			"I1",
			"Iz",
			"I2",
			"FF3",
			"TP5",
			"OT5",
			"IT1",
			"IT2",
			"IP1",
			"IPz",
			"IP2",
			"M'1",
			"M'2",
			"TP7",
			"TP8",
			"Nez",
			"F3z",
			"F4z",
			//23 channels from http://www.biosemi.com/download/Cap_coords_all.xls
			"AF3",
			"AF4",
			"AF7",
			"AF8",
			"AFz",
			"CP5",
			"CP6",
			"F9",
			"F10",
			"FC5",
			"FC6",
			"FT7",
			"FT8",
			/*"FT9", x*/
			/*"FT10",x*/
			/*"O9",  x*/
			/*"O10"  x*/
			"P7",
			"P8",
			"P9",
			"P10",
			"PO3",
			"PO4",
			"PO7",
			"PO8",
			/*"PO9", x*/
			/*"PO10",x*/
			"T7",
			"T8"
			/*"TP9", x*/
			/*"TP10",x*/
		};

		static float64 s_channelCoords[s_nbChannels*2] = {
			//72 channels from INSERM's elec.dat
/*"O1"*/			90.0, 252.0,
/*"O2"*/ 			90.0, 288.0,
/*"T5"*/ 			90.0, 216.0,
/*"P3"*/ 			63.1, 223.1,
/*"Pz"*/ 			45.0, 270.0,
/*"P4"*/ 			63.1, 316.9,
/*"T6"*/ 			90.0, 324.0,
/*"T3"*/ 			90.0, 180.0,
/*"C3"*/ 			45.0, 180.0,
/*"Cz"*/ 			0.0,  0.0,
/*"C4"*/			45.0, 0.0,
/*"T4"*/			90.0,	0.0,
/*"F7"*/			90.0, 144.0,
/*"F3"*/			63.1, 136.9,
/*"Fz"*/			45.0, 90.0,
/*"F4"*/			63.1, 43.1,
/*"F8"*/			90.0, 36.0,
/*"Fp1"*/			90.0, 108.0,
/*"Fp2"*/			90.0, 72.0,
/*"P5"*/			76.5, 219.3,
/*"P1"*/			51.7, 243.7,
/*"P2"*/ 			51.7, 296.3,
/*"P6"*/ 			76.5, 320.7,
/*"C5"*/ 			67.5, 180.0,
/*"C1"*/ 			22.5, 180.0,
/*"C2"*/ 			22.5, 0.0,
/*"C6"*/ 			67.5, 0.0,
/*"F5"*/ 			76.5, 140.7,
/*"F1"*/ 			51.7, 116.3,
/*"F2"*/ 			51.7, 63.7,
/*"F6"*/ 			76.5, 39.3,
/*"M1"*/ 			120.0, 215.6,
/*"M2"*/ 			120.0, 324.4,
/*"Oz"*/ 			90.0, 270.0,
/*"POz"*/			67.5, 270.0,
/*"CPz"*/			22.5, 270.0,
/*"FCz"*/			22.5, 90.0,
/*"FFz"*/			67.5, 90.0,
/*"Fpz"*/			90.0, 90.0,
/*"FC1"*/			31.5, 136.9,
/*"FC2"*/			31.5, 43.1,
/*"CP1"*/			31.5, 223.1,
/*"CP2"*/			31.5, 316.9,
/*"FC3"*/			52.1, 155.8,
/*"FC4"*/			52.1, 24.2,
/*"CP3"*/			52.1, 204.2,
/*"CP4"*/			52.1, 335.8,
/*"FT3"*/			75.6, 159.7,
/*"FT4"*/			75.6, 20.3,
/*"TP3"*/			75.6, 200.3,
/*"TP4"*/			75.6, 339.7,
/*"P'1"*/			52.0, 241.3,
/*"P'2"*/			52.0, 298.7,
/*"Nz"*/ 			112.5, 90.0,
/*"I1"*/ 			120.0, 234.4,
/*"Iz"*/ 			112.5, 270.0,
/*"I2"*/			120.0, 305.6,
/*"FF3"*/			33.2, 135.6,
/*"TP5"*/			66.5, 201.2,
/*"OT5"*/			77.4, 241.8,
/*"IT1"*/			102.6, 241.8,
/*"IT2"*/			102.6, 298.2,
/*"IP1"*/			87.6, 247.0,
/*"IPz"*/			78.8, 270.0,
/*"IP2"*/			87.6, 293.0,
/*"M'1"*/			105.4, 203.0,
/*"M'2"*/			105.4, 337.0,
/*"TP7"*/			90.0, 198.0,
/*"TP8"*/			90.0, 342.0,
/*"Nez"*/			118.1, 90.0,
/*"F3z"*/			63.4, 113.0,
/*"F4z"*/			63.4, 67.0,

//23 channels from http://www.biosemi.com/download/Cap_coords_all.xls

/*"AF3"  -74, -65,*/ 74, 115,
/*"AF4"  74, 65, */ 74, 65,
/*"AF7" -92, -54,*/ 92, 126,
/*"AF8" 92, 54,*/ 92, 54,
/*"AFz" 69, 90,*/ 69, 90,
/*"CP5" -72, 21,*/ 72, 21,
/*"CP6" 72, -21,*/ 72, 159,
/*"F9" -82.8, -56.25,*/ 82.8, 123.75,
/*"F10"-92, -56.25*/ 92, 123.75,
/*"FC5" -72, -21,*/ 72, 159,
/*"FC6" 72, 21,*/ 72, 21,
/*"FT7" -92, -18,*/ 92, 162,
/*"FT8" 92, 18,*/ 92, 18,
/*"FT9", x*/
/*"FT10",x*/
/*"O9",  x*/
/*"O10"  x*/
/*"P7" -92, 36, */ 92, 36,
/*"P8"  92, -36,*/ 92, 144,
/*"P9" -115, 40*/ 115, 40,
/*"P10" 115, -40,*/ 115, 120,
/*"PO3" -74, 65,*/ 74, 65,
/*"PO4" 74, -65,*/ 74, 115,
/*"PO7" -92, 54,*/ 92, 54,
/*"PO8" 92, -54,*/ 92, 126,
/*"PO9", x*/
/*"PO10",x*/
/*"T7" -92, 0*/ 92, 180,
/*"T8" 92, 0*/ 92, 0
/*"TP9", x*/
/*"TP10",x*/
		};

		CBufferDatabase::CBufferDatabase(OpenViBEToolkit::TBoxAlgorithm<Plugins::IBoxAlgorithm>& oPlugin)
			:
			m_i64NbElectrodes(0)
			,m_bFirstBufferReceived(false)
			,m_ui32SamplingFrequency(0)
			,m_ui64NumberOfBufferToDisplay(2)
			,m_f64MaximumValue(-DBL_MAX)
			,m_f64MinimumValue(+DBL_MAX)
			,m_f64TotalDuration(10000) // 10 seconds
			,m_ui64BufferDuration(0)
			,m_pDrawable(NULL)
			,m_oParentPlugin(oPlugin)
			,m_bError(false)
			,m_bRedrawOnNewData(true)
		{
			m_pDimmensionSizes[0] = m_pDimmensionSizes[1] = 0;
		}

		CBufferDatabase::~CBufferDatabase()
		{
			//FIXME : isn't there a leak here???
			//delete all the remaining buffers
			for(unsigned int i=0 ; i<m_oSampleBuffers.size() ; i++)
			{
				delete[] m_oSampleBuffers.front();
				m_oSampleBuffers.pop_front();
			}

		}

		boolean CBufferDatabase::isFirstBufferReceived()
		{
			return m_bFirstBufferReceived;
		}

		boolean CBufferDatabase::adjustNumberOfDisplayedBuffers(float64 f64NumberOfSecondsToDisplay)
		{
			boolean l_bNumberOfBufferToDisplayChanged = false;

			if(f64NumberOfSecondsToDisplay>0)
			{
				m_f64TotalDuration=f64NumberOfSecondsToDisplay;
			}

			uint64 l_ui64NewNumberOfBufferToDisplay =  static_cast<uint64>(ceil( (m_f64TotalDuration*m_ui32SamplingFrequency) / m_pDimmensionSizes[1]));

			//displays at least one buffer
			l_ui64NewNumberOfBufferToDisplay = (l_ui64NewNumberOfBufferToDisplay == 0) ? 1 : l_ui64NewNumberOfBufferToDisplay;
			if(l_ui64NewNumberOfBufferToDisplay != m_ui64NumberOfBufferToDisplay || f64NumberOfSecondsToDisplay<=0)
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
					m_oEndTime.pop_front();

					//suppress the corresponding minmax values
					for(uint32 c=0 ; c<m_pDimmensionSizes[0] ; c++)
					{
						m_oLocalMinMaxValue[c].pop_front();
					}
				}
			}

			return l_bNumberOfBufferToDisplayChanged;
		}

		uint64 CBufferDatabase::getChannelCount()
		{
			return m_pDimmensionSizes[0];
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
				m_i64NbElectrodes = ui32DimmensionSize;

				//resize min/max values vector
				m_oLocalMinMaxValue.resize((unsigned int)m_i64NbElectrodes);

				//resize electrode coords matrix
				m_oElectrodesCoords.setDimensionCount(1);
				m_oElectrodesCoords.setDimensionSize(0, (uint32)(3*m_i64NbElectrodes));

				//resize electrode spherical coords matrix
				m_oElectrodesSphericalCoords.setDimensionCount(1);
				m_oElectrodesSphericalCoords.setDimensionSize(0, (uint32)(2*m_i64NbElectrodes));

				//resize electrode labels vector
				m_oElectrodesLabels.resize((uint32)m_i64NbElectrodes);
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

			if(m_bFirstBufferReceived == false)
			{
				//make sure channel coordinates are filled
				//TODO : add coordinates info to streamed matrix stream header!!
				computeChannelCoords();

				m_ui64BufferDuration = ui64EndTime - ui64StartTime;

				//test if it is equal to zero : Error
				if(m_ui64BufferDuration == 0)
				{
					m_oParentPlugin.getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Warning << "Error : buffer start time and end time are equal : " << ui64StartTime << "\n";

					m_bError = true;
					return;
				}

				//computes the sampling frequency
				m_ui32SamplingFrequency = (uint32)((float64)(((uint64)1<<32)/(m_ui64BufferDuration)) * m_pDimmensionSizes[1]);

				//computes the number of buffer necessary to display the interval
				adjustNumberOfDisplayedBuffers(-1);

				m_pDrawable->init();

				m_bFirstBufferReceived = true;
			}

			float64 * l_pBufferToWrite;

			//Adds the starting time to the list
			m_oStartTime.push_back(ui64StartTime);
			m_oEndTime.push_back(ui64EndTime);

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
				m_oEndTime.pop_front();

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
			if(m_bRedrawOnNewData)
			{
				m_pDrawable->redraw();
			}
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

		uint64 CBufferDatabase::getElectrodeCount()
		{
			return s_nbChannels;
		}

		boolean CBufferDatabase::getElectrodePosition(const uint32 ui32ElectrodeIndex, float64* pElectrodePosition)
		{
			if(ui32ElectrodeIndex < s_nbChannels)
			{
				double teta = s_channelCoords[ui32ElectrodeIndex*2] * 3.1415926535 / 180.0;
				double phi  = s_channelCoords[ui32ElectrodeIndex*2+1] * 3.1415926535 / 180.0;
				*pElectrodePosition = sin(teta) * cos(phi);
				*(pElectrodePosition+1) = sin(teta) * sin(phi);
				*(pElectrodePosition+2) = cos(teta);
				return true;
			}

			return false;
		}

		boolean CBufferDatabase::getElectrodePosition(const CString& rElectrodeLabel, float64* pElectrodePosition)
		{
			for(unsigned int i=0; i<s_nbChannels; i++)
			{
				if(strcmp(rElectrodeLabel.toASCIIString(), s_channelLabels[i].c_str()) == 0)
				{
					double teta = s_channelCoords[i*2] * 3.1415926535 / 180.0;
					double phi  = s_channelCoords[i*2+1] * 3.1415926535 / 180.0;
					*pElectrodePosition = sin(teta) * cos(phi);
					*(pElectrodePosition+1) = sin(teta) * sin(phi);
					*(pElectrodePosition+2) = cos(teta);
					return true;
				}
			}

			return false;
		}

		boolean CBufferDatabase::getElectrodeLabel(const uint32 ui32ElectrodeIndex, CString& rElectrodeLabel)
		{
			if(ui32ElectrodeIndex >= s_nbChannels)
			{
				return false;
			}
			rElectrodeLabel = s_channelLabels[ui32ElectrodeIndex].c_str();
			return true;
		}

		boolean CBufferDatabase::getChannelPosition(const uint32 ui32ChannelIndex, float64*& pElectrodePosition)
		{
			if(ui32ChannelIndex >= 0 && ui32ChannelIndex < m_oElectrodesCoords.getDimensionSize(0))
			{
				pElectrodePosition = m_oElectrodesCoords.getBuffer() + 3*ui32ChannelIndex;
				return true;
			}

			return false;
		}

		boolean CBufferDatabase::getChannelSphericalCoordinates(const uint32 ui32ChannelIndex, float64& rTheta, float64& rPhi)
		{
			if(ui32ChannelIndex >= 0 && ui32ChannelIndex < m_oElectrodesSphericalCoords.getDimensionSize(0))
			{
				rTheta = *(m_oElectrodesSphericalCoords.getBuffer() + 2*ui32ChannelIndex);
				rPhi = *(m_oElectrodesSphericalCoords.getBuffer() + 2*ui32ChannelIndex+1);
				return true;
			}

			return false;
		}

		boolean CBufferDatabase::getChannelLabel(const uint32 ui32ChannelIndex, CString& rElectrodeLabel)
		{
			if(ui32ChannelIndex >= 0 && ui32ChannelIndex < m_oElectrodesLabels.size())
			{
				rElectrodeLabel = m_oElectrodesLabels[ui32ChannelIndex];
				return true;
			}

			return false;
		}

		void CBufferDatabase::setStimulationCount(const uint32 ui32StimulationCount)
		{
		}

		void CBufferDatabase::setStimulation(const uint32 ui32StimulationIndex, const uint64 ui64StimulationIdentifier, const uint64 ui64StimulationDate)
		{
			// m_oParentPlugin.getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Trace << "Received new stimulation id:" << ui64StimulationIdentifier << " date:" << ui64StimulationDate << "\n";

			m_oStimulations.push_back(std::pair<uint64, uint64>(ui64StimulationDate, ui64StimulationIdentifier));

			if(m_oStartTime.size()!=0)
			{
				std::deque<std::pair<uint64, uint64> >::iterator i;
				for(i=m_oStimulations.begin(); i!=m_oStimulations.end() && (*i).first<m_oStartTime.front(); )
				{
					i++;
					m_oStimulations.pop_front();
				}
			}
		}

		void CBufferDatabase::setRedrawOnNewData(boolean bSet)
		{
			m_bRedrawOnNewData = bSet;
		}

		boolean CBufferDatabase::computeChannelCoords()
		{
			boolean res = true;

			for(uint32 i=0; i<m_pDimmensionSizes[0]; i++)
			{
				//trim string
				uint32 firstNonWhitespaceChar = 0;
				for(; firstNonWhitespaceChar < m_pDimmesionLabels[0][i].size(); firstNonWhitespaceChar++)
				{
					if(!isspace(m_pDimmesionLabels[0][i][firstNonWhitespaceChar]))
					{
						break;
					}
				}

				uint32 lastNonWhitespaceChar = 0;
				if(m_pDimmesionLabels[0][i].size() > 0)
				{
					for(lastNonWhitespaceChar = m_pDimmesionLabels[0][i].size()-1; lastNonWhitespaceChar >= 0; lastNonWhitespaceChar--)
				{
					if(!isspace(m_pDimmesionLabels[0][i][lastNonWhitespaceChar]))
					{
						break;
					}
				}
				}

				boolean l_bLabelRecognized = false;

				if(firstNonWhitespaceChar < lastNonWhitespaceChar)
				{
					std::string l_oChannelLabel(m_pDimmesionLabels[0][i].substr(firstNonWhitespaceChar, lastNonWhitespaceChar-firstNonWhitespaceChar+1));

					for(unsigned int j=0; j<s_nbChannels; j++)
					{
						//look for a similar string in database
						if(strcmp(l_oChannelLabel.c_str(), s_channelLabels[j].c_str()) == 0)
						{
							l_bLabelRecognized = true;
							double teta = s_channelCoords[j*2] * 3.1415926535 / 180.0;
							double phi  = s_channelCoords[j*2+1] * 3.1415926535 / 180.0;
							*(m_oElectrodesCoords.getBuffer()+3*i) = sin(teta) * cos(phi);
							*(m_oElectrodesCoords.getBuffer()+3*i+1) = sin(teta) * sin(phi);
							*(m_oElectrodesCoords.getBuffer()+3*i+2) = cos(teta);

							*(m_oElectrodesSphericalCoords.getBuffer()+2*i) = s_channelCoords[j*2];
							*(m_oElectrodesSphericalCoords.getBuffer()+2*i+1) = s_channelCoords[j*2+1];

							//copy label
							m_oElectrodesLabels[i] = s_channelLabels[j].c_str();

							break;
						}
					}
				}

				//unrecognized electrode!
				if(l_bLabelRecognized == false)
				{
					m_oParentPlugin.getLogManager() << LogLevel_Trace
						<< "Unrecognized electrode name (index=" << (uint32)i
						<< ", name=" << m_pDimmesionLabels[0][i].c_str()
						<< ")!\n";
					res = false;
				}
			}

			m_oParentPlugin.getLogManager() << LogLevel_Trace << "Electrodes list : " ;

			for(uint32 i=0; i<m_pDimmensionSizes[0]; i++)
			{
				m_oParentPlugin.getLogManager() << (uint32)i << ":" << m_oElectrodesLabels[i];
				if(i<m_pDimmensionSizes[0]-1)
				{
					m_oParentPlugin.getLogManager() << ", ";
				}
				else
				{
					m_oParentPlugin.getLogManager() << "\n";
				}
			}

			return res;
		}
	}
}
