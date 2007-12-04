#include "ovpCPowerSpectrumDatabase.h"
#include "../ovpCBufferDatabase.h"

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

		CPowerSpectrumDatabase::CPowerSpectrumDatabase(OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>& oPlugin) :
			m_f64MinValue(+DBL_MAX),
			m_f64MaxValue(-DBL_MAX),
			m_pBuffer(NULL),
			m_bFirstBufferReceived(false),
			m_ui32SamplingFrequency(0),
			m_ui64NumberOfBufferToDisplay(2),
			m_pDrawable(NULL),
			m_oParentPlugin(oPlugin)
		{
		}

		CPowerSpectrumDatabase::~CPowerSpectrumDatabase()
		{
			if(m_pBuffer)
				delete[] m_pBuffer;
		}

		void CPowerSpectrumDatabase::setChannelCount(const OpenViBE::uint32 ui32ChannelCount)
		{
			m_pChannelLabels.resize(ui32ChannelCount);
			m_oMinMaxValues.resize(ui32ChannelCount);
		}

		void CPowerSpectrumDatabase::setChannelName(const OpenViBE::uint32 ui32ChannelIndex, const char* sChannelName)
		{
			if(ui32ChannelIndex >= m_pChannelLabels.size())
				m_pChannelLabels.resize(ui32ChannelIndex+1);

			m_pChannelLabels[ui32ChannelIndex] = sChannelName;
		}

		void CPowerSpectrumDatabase::setFrequencyBandCount(const OpenViBE::uint32 ui32FrequencyBandCount)
		{
			m_pFrequencyBandLabels.resize(ui32FrequencyBandCount);
			m_pFrequencyBands.resize(ui32FrequencyBandCount);
		}

		void CPowerSpectrumDatabase::setFrequencyBandName(const OpenViBE::uint32 ui32FrequencyBandIndex, const char* sFrequencyBandName)
		{
			if(ui32FrequencyBandIndex >= m_pFrequencyBandLabels.size())
				m_pFrequencyBandLabels.resize(ui32FrequencyBandIndex+1);

			m_pFrequencyBandLabels[ui32FrequencyBandIndex] = sFrequencyBandName;
		}

		void CPowerSpectrumDatabase::setFrequencyBandStart(const OpenViBE::uint32 ui32FrequencyBandIndex, const OpenViBE::float64 f64FrequencyBandStart)
		{
			if(ui32FrequencyBandIndex >= m_pFrequencyBandLabels.size())
				m_pFrequencyBands.resize(ui32FrequencyBandIndex+1);

			m_pFrequencyBands[ui32FrequencyBandIndex].first = f64FrequencyBandStart;
		}

		void CPowerSpectrumDatabase::setFrequencyBandStop(const OpenViBE::uint32 ui32FrequencyBandIndex, const OpenViBE::float64 f64FrequencyBandStop)
		{
			if(ui32FrequencyBandIndex >= m_pFrequencyBandLabels.size())
				m_pFrequencyBands.resize(ui32FrequencyBandIndex+1);

			m_pFrequencyBands[ui32FrequencyBandIndex].second = f64FrequencyBandStop;
		}

		void CPowerSpectrumDatabase::setBuffer(const OpenViBE::float64* pBuffer)
		{
			//init some members when receiving first buffer
			if(m_bFirstBufferReceived == false)
			{
				m_pDrawable->init();

				m_pBuffer = new OpenViBE::float64[(size_t)m_pChannelLabels.size()*m_pFrequencyBandLabels.size()];

				m_bFirstBufferReceived = true;
			}

			System::Memory::copy(m_pBuffer, pBuffer, m_pChannelLabels.size()*m_pFrequencyBandLabels.size()*sizeof(float64));

			const OpenViBE::float64* pCurChannel = pBuffer;

			m_f64MinValue = DBL_MAX;
			m_f64MaxValue = -DBL_MAX;

			//for each channel
			for(uint32 c=0; c<m_pChannelLabels.size(); c++, pCurChannel += m_pFrequencyBands.size())
			{
				m_oMinMaxValues[c].first = DBL_MAX;
				m_oMinMaxValues[c].second = -DBL_MAX;

				//for each frequency band
				for(uint64 i=0; i<m_pFrequencyBands.size(); i++)
				{
					//update channel min/max values
					if(pCurChannel[i] < m_oMinMaxValues[c].first)
					{
						m_oMinMaxValues[c].first = pCurChannel[i];
					}
					else if(pCurChannel[i] > m_oMinMaxValues[c].second)
					{
						m_oMinMaxValues[c].second = pCurChannel[i];
					}
				}

				//update global min/max values
				if(m_oMinMaxValues[c].first < m_f64MinValue)
					m_f64MinValue = m_oMinMaxValues[c].first;
				if(m_oMinMaxValues[c].second > m_f64MaxValue)
					m_f64MaxValue = m_oMinMaxValues[c].second;
			}

			//tells the drawable to redraw himself since the signal information has been updated
			m_pDrawable->redraw();
		}

		void CPowerSpectrumDatabase::setDrawable(CSignalDisplayDrawable * pDrawable)
		{
			m_pDrawable=pDrawable;
		}

		//! Returns min/max values of last buffer received by a given channel
		void CPowerSpectrumDatabase::getLastBufferChannelMinMaxValue(OpenViBE::uint32 ui32Channel, OpenViBE::float64& f64Min, OpenViBE::float64& f64Max)
		{
			f64Min = m_oMinMaxValues[ui32Channel].first;
			f64Max = m_oMinMaxValues[ui32Channel].second;
		}

		//! Returns global min/max values of last buffer received
		void CPowerSpectrumDatabase::getLastBufferMinMaxValue(OpenViBE::float64& f64Min, OpenViBE::float64& f64Max)
		{
			f64Min = m_f64MinValue;
			f64Max = m_f64MaxValue;
		}

		OpenViBE::float64* CPowerSpectrumDatabase::getLastBufferChannelPointer(OpenViBE::uint32 ui32Channel)
		{
			return m_pBuffer + ui32Channel * m_pFrequencyBands.size();
		}
/*
		void CPowerSpectrumDatabase::getDisplayedChannelMinMaxValue(OpenViBE::uint32 ui32Channel, OpenViBE::float64& f64Min, OpenViBE::float64& f64Max)
		{
			f64Min = +DBL_MAX;
			f64Max = -DBL_MAX;

			for(uint64 i=0 ; i<m_oLocalMinMaxValue[ui32Channel].size() ; i++)
			{
				if(f64Min > m_oLocalMinMaxValue[ui32Channel][i].first)
				{
					f64Min = m_oLocalMinMaxValue[ui32Channel][i].first;
				}
				if(f64Max < m_oLocalMinMaxValue[ui32Channel][i].second)
				{
					f64Max = m_oLocalMinMaxValue[ui32Channel][i].second;
				}
			}
		}

		void CPowerSpectrumDatabase::getDisplayedGlobalMinMaxValue(OpenViBE::float64& f64Min, OpenViBE::float64& f64Max)
		{
			for(uint32 c=0 ; c<m_oLocalMinMaxValue.size() ; c++)
			{
				for(uint64 i=0 ; i<m_oLocalMinMaxValue[c].size() ; i++)
				{
					if(f64Min > m_oLocalMinMaxValue[c][i].first)
					{
						f64Min = m_oLocalMinMaxValue[c][i].first;
					}
					if(f64Max < m_oLocalMinMaxValue[c][i].second)
					{
						f64Max = m_oLocalMinMaxValue[c][i].second;
					}
				}
			}
		}
*/
	}
}
