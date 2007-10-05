#ifndef __OpenViBEPlugins_SimpleVisualisation_CBufferDatabase_H__
#define __OpenViBEPlugins_SimpleVisualisation_CBufferDatabase_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <ebml/IReader.h>

#include <vector>
#include <deque>
#include <queue>
#include <string>
#include <cfloat>

//#include "ovpCSignalDisplayView.h"

#include <iostream>

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		class CSignalDisplayDrawable;

		/**
		* Abtract class of objects than can be updated by a CBufferDatabase
		*/
		class CSignalDisplayDrawable
		{
			public:

				virtual ~CSignalDisplayDrawable(){}
				virtual void init() = 0;
				virtual void redraw() = 0;
		};

		/**
		* This class is used to store information about the incoming signal stream. It can request a CSignalDisplayDrawable
		* object to redraw himself in case of some changes in its data.
		*/
		class CBufferDatabase
		{

			public:
				OpenViBE::uint64 m_pDimmensionSizes[2];
				std::vector<std::string> m_pDimmesionLabels[2];

				OpenViBE::boolean m_bFirstBufferReceived;

				//! Sampling frequency of the incoming stream
				OpenViBE::uint32 m_ui32SamplingFrequency;

				//! double-linked list of pointers to the samples buffers of the current time window
				std::deque<OpenViBE::float64 *> m_oSampleBuffers;

				//! stimulations to display. pair values are <date, stimcode>
				std::deque<std::pair<OpenViBE::uint64, OpenViBE::uint64> > m_oStimulations;

				//! Number of buffer to display at the same time
				OpenViBE::uint64 m_ui64NumberOfBufferToDisplay;

				//! The global maximum value of the signal (up to now)
				OpenViBE::float64 m_f64MaximumValue;

				//! The global minimum value of the signal (up to now)
				OpenViBE::float64 m_f64MinimumValue;

				//! double-linked list of the start times of the current buffers
				std::deque<OpenViBE::uint64> m_oStartTime;

				//! duration to display in seconds
				OpenViBE::float64 m_f64TotalDuration;

				OpenViBE::uint64 m_ui64BufferDuration;

				//! Pointer to the drawable object to update (if needed)
				CSignalDisplayDrawable * m_pDrawable;

				std::vector<std::deque<std::pair<OpenViBE::float64, OpenViBE::float64> > > m_oLocalMinMaxValue;

				OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>& m_oParentPlugin;

				OpenViBE::boolean m_bError;

			public:
				CBufferDatabase(OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>& oPlugin);
				~CBufferDatabase();

				/**
				* Sets the drawable object to update.
				* \param pDrawable drawable object to update.
				*/
				void setDrawable(CSignalDisplayDrawable * pDrawable) { m_pDrawable=pDrawable; }

				/**
				* Compute the number of buffers needed to display the signal for a certain time period.
				* \param f64NumberOfMsToDisplay the time window's width in seconds.
				*/
				OpenViBE::boolean adjustNumberOfDisplayedBuffers(OpenViBE::float64 f64NumberOfSecondsToDisplay);

				OpenViBE::float64 getDisplayedTimeIntervalWidth();

				//! Returns the min/max values currently displayed for the given channel
				void getDisplayedChannelLocalMinMaxValue(OpenViBE::uint32 ui32Channel, OpenViBE::float64& f64Min, OpenViBE::float64& f64Max);
				//! Returns the min/max values currently displayed (all channels taken into account)
				void getDisplayedGlobalMinMaxValue(OpenViBE::float64& f64Min, OpenViBE::float64& f64Max);

				//! Returns the min/max values of the last buffer arrived for the given channel
				void getLastBufferChannelLocalMinMaxValue(OpenViBE::uint32 ui32Channel, OpenViBE::float64& f64Min, OpenViBE::float64& f64Max)
				{
					f64Min = m_oLocalMinMaxValue[ui32Channel].back().first;
					f64Max = m_oLocalMinMaxValue[ui32Channel].back().second;
				}

				//! Returns the min/max values of the last buffer arrived  (all channels taken into account)
				void getLastBufferMinMaxValue(OpenViBE::float64& f64Min, OpenViBE::float64& f64Max)
				{
					f64Min = +DBL_MAX;
					f64Max = -DBL_MAX;

					for(OpenViBE::uint32 c=0 ; c<m_oLocalMinMaxValue.size() ; c++)
					{
						f64Min = (m_oLocalMinMaxValue[c].back().first < f64Min) ? m_oLocalMinMaxValue[c].back().first : f64Min;
						f64Max = (m_oLocalMinMaxValue[c].back().second > f64Max) ? m_oLocalMinMaxValue[c].back().second : f64Max;
					}
				}

				void setMatrixDimmensionCount(const OpenViBE::uint32 ui32DimmensionCount);
				void setMatrixDimmensionSize(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionSize);
				void setMatrixDimmensionLabel(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel);
				void setMatrixBuffer(const OpenViBE::float64* pBuffer, OpenViBE::uint64 ui64StartTime, OpenViBE::uint64 ui64EndTime);

				void setStimulationCount(const OpenViBE::uint32 ui32StimulationCount);
				void setStimulation(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint64 ui64StimulationIdentifier, const OpenViBE::uint64 ui64StimulationDate);

		};
	}

}

#endif
