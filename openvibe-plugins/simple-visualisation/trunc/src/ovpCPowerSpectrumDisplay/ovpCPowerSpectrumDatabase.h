#ifndef __OpenViBEPlugins_SimpleVisualisation_CPowerSpectrumDatabase_H__
#define __OpenViBEPlugins_SimpleVisualisation_CPowerSpectrumDatabase_H__

#include "../ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <ebml/IReader.h>

#include <vector>
#include <deque>
#include <queue>
#include <string>
#include <cfloat>
#include <iostream>

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		class CSignalDisplayDrawable;

		/**
		* This class is used to store information about the incoming spectrum stream. It can request a CSignalDisplayDrawable
		* object to redraw himself in case of some changes in its data.
		*/
		class CPowerSpectrumDatabase
		{
		public:
			CPowerSpectrumDatabase(
				OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>& oPlugin);
			~CPowerSpectrumDatabase();

			//IBoxAlgorithmSpectrumInputReaderCallback::ICallback callbacks
			void setChannelCount(
				const OpenViBE::uint32 ui32ChannelCount);
			void setChannelName(
				const OpenViBE::uint32 ui32ChannelIndex,
				const char* sChannelName);
			void setFrequencyBandCount(
				const OpenViBE::uint32 ui32FrequencyBandCount);
			void setFrequencyBandName(
				const OpenViBE::uint32 ui32FrequencyBandIndex,
				const char* sFrequencyBandName);
			void setFrequencyBandStart(
				const OpenViBE::uint32 ui32FrequencyBandIndex,
				const OpenViBE::float64 f64FrequencyBandStart);
			void setFrequencyBandStop(
				const OpenViBE::uint32 ui32FrequencyBandIndex,
				const OpenViBE::float64 f64FrequencyBandStop);
			void setBuffer(
				const OpenViBE::float64* pBuffer);

			//COPIED FROM BUFFERDATABASE

			/**
			* Sets the drawable object to update.
			* \param pDrawable drawable object to update.
			*/
			void setDrawable(
				CSignalDisplayDrawable * pDrawable);

			//! Returns the min/max values of the last buffer arrived for the given channel
			void getLastBufferChannelMinMaxValue(
				OpenViBE::uint32 ui32Channel,
				OpenViBE::float64& f64Min, OpenViBE::float64& f64Max);

			//! Returns the min/max values of the last buffer arrived  (all channels taken into account)
			void getLastBufferMinMaxValue(
				OpenViBE::float64& f64Min, OpenViBE::float64& f64Max);

			OpenViBE::float64* getLastBufferChannelPointer(
				OpenViBE::uint32 ui32Channel);

		public:
			std::vector<std::string> m_pChannelLabels;
			std::vector<std::string> m_pFrequencyBandLabels;
			std::vector<std::pair<OpenViBE::float64, OpenViBE::float64> > m_pFrequencyBands;
			std::vector<std::pair<OpenViBE::float64, OpenViBE::float64> > m_oMinMaxValues; //last buffer's per-channel min/max vals
			OpenViBE::float64 m_f64MinValue, m_f64MaxValue; //last buffer's global min/max vals
			OpenViBE::float64* m_pBuffer;

			//COPIED FROM BUFFERDATABASE
			OpenViBE::boolean m_bFirstBufferReceived;

			//! Sampling frequency of the incoming stream
			OpenViBE::uint32 m_ui32SamplingFrequency;

			//! double-linked list of pointers to the samples buffers of the current time window
			std::deque<OpenViBE::float64 *> m_oSampleBuffers;

			//! Number of buffers to display at the same time
			OpenViBE::uint64 m_ui64NumberOfBufferToDisplay;

			//! double-linked list of the start times of the current buffers
			std::deque<OpenViBE::uint64> m_oStartTime;

			OpenViBE::uint64 m_ui64BufferDuration;

			//! Pointer to the drawable object to update (if needed)
			CSignalDisplayDrawable* m_pDrawable;

			OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>& m_oParentPlugin;
		};
	}

}

#endif
