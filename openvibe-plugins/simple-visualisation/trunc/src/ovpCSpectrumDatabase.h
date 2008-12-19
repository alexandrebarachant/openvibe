#ifndef __OpenViBEPlugins_SimpleVisualisation_CSpectrumDatabase_H__
#define __OpenViBEPlugins_SimpleVisualisation_CSpectrumDatabase_H__

#include "ovp_defines.h"
#include "ovpIStreamDatabase.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

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
		/**
		* This class is used to store information about the incoming spectrum stream. It can request a IStreamDisplayDrawable
		* object to redraw itself upon changes in its data.
		*/
		class CSpectrumDatabase : public IStreamDatabase
		{
		public:
			/**
			 * \brief Constructor
			 * \param
			 */
			CSpectrumDatabase(
				OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>& oPlugin);

			/**
			 * \brief Destructor
			 */
			virtual ~CSpectrumDatabase();

			/**
			 * \brief Set drawable object to update.
			 * \param pDrawable drawable object to update.
			 */
			void setDrawable(
				IStreamDisplayDrawable* pDrawable);

			/**
			 * \brief Set flag stating whether or not to redraw upon new data reception
			 * \param bRedrawOnNewData Redraw flag
			 */
			void setRedrawOnNewData(
				OpenViBE::boolean bRedrawOnNewData);

			/**
			 * \brief Determine whether first buffer has been received yet
			 * \return True if first buffer has been received already, false otherwise
			 */
			OpenViBE::boolean isFirstBufferReceived();

			/**
			 * \brief Set time scale
			 * Computes the maximum number of buffers that can be displayed simultaneously
			 * \param f64TimeScale Time window's width in seconds.
			 * \return True if buffer count changed, false otherwise
			 */
			OpenViBE::boolean setTimeScale(
				OpenViBE::float64 f64TimeScale);

			/**
			 * \brief Set displayed frequency range
			 * \param f64MinimumDisplayedFrequency Minimum frequency to display
			 * \param f64MaximumDisplayedFrequency Maximum frequency to display
			 */
			//TODO (if min/max computation should be restricted to this range)
			/*
			void setDisplayedFrequencyRange(
				OpenViBE::float64 f64MinimumDisplayedFrequency,
				OpenViBE::float64 f64MaximumDisplayedFrequency);*/

			/**
			 * \brief Decode a memory buffer using proxy
			 * \param pMemoryBuffer Memory buffer to decode
			 * \param ui64StartTime Start time of memory buffer
			 * \param ui64EndTime End time of memory buffer
			 * \return True if memory buffer could be properly decoded, false otherwise
			 */
			OpenViBE::boolean decodeMemoryBuffer(
				const OpenViBE::IMemoryBuffer* pMemoryBuffer,
				OpenViBE::uint64 ui64StartTime,
				OpenViBE::uint64 ui64EndTime);

			/**
			 * \brief Compute min/max values currently displayed for a given channel
			 * \param [in] ui32Channel Index of channel
			 * \param [out] f64Min Minimum displayed value for channel of interest
			 * \param [out] f64Max Maximum displayed value for channel of interest
			 * \return True if values could be computed, false otherwise
			 */
			OpenViBE::boolean getChannelMinMaxValues(
				OpenViBE::uint32 ui32Channel,
				OpenViBE::float64& f64Min,
				OpenViBE::float64& f64Max);

			/**
			 * \brief Compute min/max values currently displayed, taking all channels into account
			 * \param [out] f64Min Minimum displayed value
			 * \param [out] f64Max Maximum displayed value
			 * \return True if values could be computed, false otherwise
			 */
			OpenViBE::boolean getGlobalMinMaxValues(
				OpenViBE::float64& f64Min,
				OpenViBE::float64& f64Max);

			/**
			 * \brief Compute min/max values in last buffer for a given channel
			 * \param [in] ui32Channel Index of channel
			 * \param [out] f64Min Minimum value for channel of interest
			 * \param [out] f64Max Maximum value for channel of interest
			 * \return True if values could be computed, false otherwise
			 */
			OpenViBE::boolean getLastBufferChannelMinMaxValues(
				OpenViBE::uint32 ui32Channel,
				OpenViBE::float64& f64Min,
				OpenViBE::float64& f64Max);

			/**
			 * \brief Compute min/max values in last buffer, taking all channels into account
			 * \param [out] f64Min Minimum value
			 * \param [out] f64Max Maximum value
			 * \return True if values could be computed, false otherwise
			 */
			OpenViBE::boolean getLastBufferGlobalMinMaxValues(
				OpenViBE::float64& f64Min,
				OpenViBE::float64& f64Max);

			OpenViBE::uint64 getMaxDisplayedBufferCount();

			OpenViBE::uint64 getBufferCount();

			const OpenViBE::float64* getBuffer(
				OpenViBE::uint32 ui32BufferIndex);

			OpenViBE::uint64 getStartTime(
				OpenViBE::uint32 ui32BufferIndex);

			OpenViBE::uint64 getEndTime(
				OpenViBE::uint32 ui32BufferIndex);

			OpenViBE::uint64 getBufferDuration();

			/**
			 * \brief Get number of samples per buffer
			 * \return Number of samples per buffer
			 */
			OpenViBE::uint32 getSampleCountPerBuffer();

			/**
			 * \brief Get number of channels
			 * \return Number of channels
			 */
			OpenViBE::uint32 getChannelCount();

			/**
			 * \brief Get channel label
			 * \param[in] ui32ChannelIndex index of channel
			 * \param[out] rChannelLabel channel label
			 * \return true if channel label could be retrieved, false otherwise
			 */
			OpenViBE::boolean getChannelLabel(
				const OpenViBE::uint32 ui32ChannelIndex,
				OpenViBE::CString& rElectrodeLabel);

			/**
			 * \brief Get number of frequency bands
			 * \return Number of frequency bands
			 */
			OpenViBE::uint32 getFrequencyBandCount();

			/**
			 * \brief Get width of a frequency band (in Hz)
			 * \return Frequency band width
			 */
			OpenViBE::float64 getFrequencyBandWidth();

			/**
			 * \brief Get frequency band start frequency
			 * \param ui32FrequencyBandIndex Index of frequency band
			 * \return Frequency band start if it could be retrieved, 0 otherwise
			 */
			OpenViBE::float64 getFrequencyBandStart(
				OpenViBE::uint32 ui32FrequencyBandIndex);

			/**
			 * \brief Get frequency band stop frequency
			 * \param ui32FrequencyBandIndex Index of frequency band
			 * \return Frequency band stop if it could be retrieved, 0 otherwise
			 */
			OpenViBE::float64 getFrequencyBandStop(
				OpenViBE::uint32 ui32FrequencyBandIndex);

		private:
			// parent plugin
			OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>& m_oParentPlugin;
			//spectrum decoder algorithm proxy
			OpenViBE::Kernel::IAlgorithmProxy* m_pProxy;
			//drawable object to update (if needed)
			IStreamDisplayDrawable* m_pDrawable;
			//flag stating whether to redraw the IStreamDisplayDrawable upon new data reception if true (default)
			OpenViBE::boolean m_bRedrawOnNewData;
			//flag stating whether first samples buffer has been received
			OpenViBE::boolean m_bFirstBufferReceived;

			//sampling frequency of incoming stream
			OpenViBE::uint32 m_ui32SamplingFrequency;
			//maximum number of buffers displayed at the same time per channel
			OpenViBE::uint64 m_ui64MaxDisplayedBufferCount;
			//maximum duration of displayed buffers (in seconds)
			OpenViBE::float64 m_f64TimeScale;
			//double-linked list of start times of stored buffers
			std::deque<OpenViBE::uint64> m_oStartTime;
			//double-linked list of end times of stored buffers
			std::deque<OpenViBE::uint64> m_oEndTime;

			//streamed matrix header : number of channels & frequency bands + labels
			OpenViBE::CMatrix m_oStreamedMatrixHeader;
			//streamed matrix	history (1st dimension : channels, 2nd dimension : frequency bands)
			std::deque<OpenViBE::CMatrix*> m_oStreamedMatrices;
			//min/max values for each channel
			std::vector<std::deque<std::pair<OpenViBE::float64, OpenViBE::float64> > > m_oChannelMinMaxValues;

			//frequency bands header
			//OpenViBE::CMatrix m_oFrequencyBandsHeader;
			std::vector< std::pair<OpenViBE::float64, OpenViBE::float64> > m_oFrequencyBands;
			//frequency bands (1st dimension : frequency bands, 2nd dimension : start/stop frequencies)
			//OpenViBE::CMatrix m_oFrequencyBands;

			//OpenViBE::boolean m_bError;
		};
	}
}

#endif
