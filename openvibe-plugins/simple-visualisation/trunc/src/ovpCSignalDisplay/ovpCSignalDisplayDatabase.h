#ifndef __OpenViBEPlugins_SimpleVisualisation_CSignalDisplayDatabase_H__
#define __OpenViBEPlugins_SimpleVisualisation_CSignalDisplayDatabase_H__

#include "../ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <ebml/IReader.h>

#include <vector>
#include <deque>
#include <queue>
#include <string>

#include "ovpCSignalDisplayView.h"

#include <iostream>

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		class CSignalDisplayDrawable;
		
		/**
		* This class is used to store information about the incoming signal stream. It can request a CSignalDisplayDrawable
		* object to redraw himself in case of some changes in its data.
		*/
		class CSignalDisplayDatabase 
		{
				
			public:
				//stream information
				//! Number of channels in the incoming stream
				OpenViBE::uint32 m_ui32ChannelCount;
				//! Sampling frequency of the incoming stream
				OpenViBE::uint32 m_ui32SamplingFrequency;
				//! Number of samples per channel per incoming stream buffer
				OpenViBE::uint32 m_ui32SamplesPerBuffer;
				//! Vector containing the channels' names
				std::vector<std::string> m_vChannelName;

				
				//! double-linked list of pointers to the samples buffers of the current time window
				std::deque<OpenViBE::float64 *> m_oSampleBuffers;
		
				//! Number of buffer to display at the same time
				OpenViBE::uint64 m_ui64NumberOfBufferToDisplay;
						
				//! The global maximum value of the signal (up to now)
				OpenViBE::float64 m_f64MaximumValue;
				
				//! The global minimum value of the signal (up to now)
				OpenViBE::float64 m_f64MinimumValue;
				
				//! double-linked list of the start times of the current buffers
				std::deque<OpenViBE::uint64> m_oStartTime;

				//! Pointer to the drawable object to update (if needed)
				CSignalDisplayDrawable * m_pDrawable;
					
			public:	
				CSignalDisplayDatabase();
				~CSignalDisplayDatabase();
		
				/**
				* Sets the drawable object to update.
				* \param pDrawable drawable object to update.
				*/
				void setDrawable(CSignalDisplayDrawable * pDrawable) { m_pDrawable=pDrawable; }
				
				/**
				* Compute the number of buffers needed to display the signal for a certain time period.
				* \param f64NumberOfMsToDisplay the time window's width in ms.
				*/
				OpenViBE::boolean adjustNumberOfDisplayedBuffers(OpenViBE::float64 f64NumberOfMsToDisplay);

				OpenViBE::float64 getDisplayedTimeIntervalWidth();


				/**
				* Sets the number of channels.
				* \param ui32ChannelCount Number of channels in the input stream.
				*/
				void setChannelCount(OpenViBE::uint32 ui32ChannelCount);
				
				/**
				 * Sets the sample count per channel per buffer.
				 * \param ui32SampleCount Number of samples per channel per buffer.
				 */
				void setSampleCountPerBuffer(OpenViBE::uint32 ui32SampleCount);
				
				/**
				 * Sets the name of a channel.
				 * \param ui32ChannelIndex Index of the channel.
				 * \param pLabel Name of the channel.
				 */
				void setChannelName(OpenViBE::uint32 ui32ChannelIndex, const char * pLabel);
				
				/**
				 * Sets the sampling frequency.
				 * \param ui32SamplingFrequency The signal's sampling frequency.
				 */
				void setSamplingRate(const OpenViBE::uint32 ui32SamplingFrequency);
				
				/**
				 * Adds an incoming sample buffer.
				 * \param pBuffer The new sample buffer.
				 * \param ui64StartTime The start time (in 32:32 seconds) of the buffer.
				 */
				void setSampleBuffer(const OpenViBE::float64* pBuffer, OpenViBE::uint64 ui64StartTime);

		};
	}
}

#endif
