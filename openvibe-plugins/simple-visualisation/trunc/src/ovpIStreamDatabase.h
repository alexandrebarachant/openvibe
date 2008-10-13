#ifndef __OpenViBEPlugins_SimpleVisualisation_IStreamDatabase_H__
#define __OpenViBEPlugins_SimpleVisualisation_IStreamDatabase_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		class IStreamDisplayDrawable;

		/**
		 * \brief Abstract class of objects than can be updated by a CStreamDatabase
		 */
		class IStreamDisplayDrawable
		{
		public:
			virtual OpenViBE::boolean init() = 0;
			virtual OpenViBE::boolean redraw() = 0;
			virtual ~IStreamDisplayDrawable(){}
		};

		class IStreamDatabase
		{
		public:
			virtual ~IStreamDatabase() {}

			/**
			 * \brief Set drawable object to update.
			 * \param pDrawable drawable object to update.
			 */
			virtual void setDrawable(
				IStreamDisplayDrawable* pDrawable) = 0;

			/**
			 * \brief Set flag stating whether or not to redraw upon new data reception
			 * \param bRedrawOnNewData Redraw flag
			 */
			virtual void setRedrawOnNewData(
				OpenViBE::boolean bRedrawOnNewData) = 0;

			/**
			 * \brief Determine whether first buffer has been received yet
			 * \return True if first buffer has been received already, false otherwise
			 */
			virtual OpenViBE::boolean isFirstBufferReceived() = 0;

			/**
			 * \brief Set time scale
			 * Computes the maximum number of buffers that can be displayed simultaneously
			 * \param f64TimeScale Time window's width in seconds.
			 * \return True if buffer count changed, false otherwise
			 */
			virtual OpenViBE::boolean setTimeScale(
				OpenViBE::float64 f64TimeScale) = 0;

			/**
			 * \brief Decode a memory buffer using proxy
			 * \param pMemoryBuffer Memory buffer to decode
			 * \param ui64StartTime Start time of memory buffer
			 * \param ui64EndTime End time of memory buffer
			 * \return True if memory buffer could be properly decoded, false otherwise
			 */
			virtual OpenViBE::boolean decodeMemoryBuffer(
				const OpenViBE::IMemoryBuffer* pMemoryBuffer,
				OpenViBE::uint64 ui64StartTime,
				OpenViBE::uint64 ui64EndTime) = 0;

			/**
			 * \brief Compute min/max values currently displayed for a given channel
			 * \param [in] ui32Channel Index of channel
			 * \param [out] f64Min Minimum displayed value for channel of interest
			 * \param [out] f64Max Maximum displayed value for channel of interest
			 * \return True if values could be computed, false otherwise
			 */
			/*virtual OpenViBE::boolean getChannelMinMaxValues(
				OpenViBE::uint32 ui32Channel,
				OpenViBE::float64& f64Min,
				OpenViBE::float64& f64Max) = 0;*/

			/**
			 * \brief Compute min/max values currently displayed, taking all channels into account
			 * \param [out] f64Min Minimum displayed value
			 * \param [out] f64Max Maximum displayed value
			 * \return True if values could be computed, false otherwise
			 */
			/*virtual OpenViBE::boolean getGlobalMinMaxValues(
				OpenViBE::float64& f64Min,
				OpenViBE::float64& f64Max) = 0;*/

			/**
			 * \brief Compute min/max values in last buffer for a given channel
			 * \param [in] ui32Channel Index of channel
			 * \param [out] f64Min Minimum value for channel of interest
			 * \param [out] f64Max Maximum value for channel of interest
			 * \return True if values could be computed, false otherwise
			 */
			/*virtual OpenViBE::boolean getLastBufferChannelMinMaxValues(
				OpenViBE::uint32 ui32Channel,
				OpenViBE::float64& f64Min,
				OpenViBE::float64& f64Max) = 0;*/

			/**
			 * \brief Compute min/max values in last buffer, taking all channels into account
			 * \param [out] f64Min Minimum value
			 * \param [out] f64Max Maximum value
			 * \return True if values could be computed, false otherwise
			 */
			/*virtual OpenViBE::boolean getLastBufferGlobalMinMaxValues(
				OpenViBE::float64& f64Min,
				OpenViBE::float64& f64Max) = 0;*/

			virtual OpenViBE::uint64 getMaxDisplayedBufferCount() = 0;

			virtual OpenViBE::uint64 getBufferCount() = 0;

			virtual const OpenViBE::float64* getBuffer(OpenViBE::uint32 ui32Index) = 0;

			virtual OpenViBE::uint64 getStartTime(
				OpenViBE::uint32 ui32BufferIndex) = 0;

			virtual OpenViBE::uint64 getEndTime(
				OpenViBE::uint32 ui32BufferIndex) = 0;

			virtual OpenViBE::uint64 getBufferDuration() = 0;

			/**
			 * \brief Get number of samples per buffer
			 * \return Number of samples per buffer
			 */
			virtual OpenViBE::uint32 getSampleCountPerBuffer() = 0;

			/**
			 * \brief Get number of channels
			 * \return Number of channels
			 */
			virtual OpenViBE::uint32 getChannelCount() = 0;

			/**
			 * \brief Get channel label
			 * \param[in] ui32ChannelIndex index of channel
			 * \param[out] rChannelLabel channel label
			 * \return true if channel label could be retrieved, false otherwise
			 */
			virtual OpenViBE::boolean getChannelLabel(
				const OpenViBE::uint32 ui32ChannelIndex,
				OpenViBE::CString& rElectrodeLabel) = 0;
		};
	}
}

#endif
