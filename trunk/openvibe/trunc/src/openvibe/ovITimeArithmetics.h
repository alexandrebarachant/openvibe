#ifndef __OpenViBE_Kernel_TimeArithmetics_H__
#define __OpenViBE_Kernel_TimeArithmetics_H__

#include "ov_types.h"

namespace OpenViBE
{
	class ITimeArithmetics
	{

		/**
		 * \class ITimeArithmetics
		 * \author Loic Mahe. Minor tweaks by Jussi T. Lindgren.
		 * \date 2013-03-27
		 * \brief Static functions to perform various conversions to and from the OpenViBE 32:32 fixed point time format.
		 *
		 * All functions of the class are implemented by this header.
		 *
		 */
	public:

		/**
		 * \brief Given a sample count and the sampling rate, returns the duration of the samples in fixed point time
		 * \param ui64SamplingRate : the sampling rate of the signal
		 * \param ui64SampleCount : the size of the sample
		 * \return Time in fixed point format corresponding to the input parameters
		 */
		static uint64 sampleCountToTime(const uint64 ui64SamplingRate, const uint64 ui64SampleCount)
		{
			return (ui64SampleCount<<32)/ui64SamplingRate;
		}

		/**
		 * \brief Given a fixed point time and the sampling rate, returns the number of samples obtained
		 * \param ui64SamplingRate : the sampling rate of the signal
		 * \param ui64Time : elapsed time in fixed point format
		 * \return Sample count corresponding to the input parameters
		 */
		static uint64 timeToSampleCount(const uint64 ui64SamplingRate, const uint64 ui64Time)
		{
			return ((ui64Time+1)*ui64SamplingRate-1)>>32; 
		}
		
		/**
		 * \brief Get the time in seconds given a fixed point time
		 * \param ui64FixedPointTime : time in fixed point format
		 * \return Regular floating point time in seconds
		 *
		 * @FIXME Computation still subject to change.
		 *
		 */
		static float64 timeToSeconds(const uint64 ui64Time)
		{
			// return (ui64Time>>22)/1024.0;
			return float64(ui64Time)/float64(1LL<<32);
		}

		/**
		 * \brief Get the time in fixed point format given float time in seconds
		 * \param f64Time : Regular floating point time in seconds 
		 * \return Time in fixed point format
		 *
		 * @FIXME Computation still subject to change.
		 *
		 */
		static uint64 secondsToTime(const float64 f64Time)
		{
			// return ((uint64)(f64Time*1024.0))<<22;
			return uint64(f64Time*float64(1LL<<32)); 
		}

	private:
		// Static class, don't allow instances
		ITimeArithmetics();

		// These calls are probably mistakes and prohibited. Use explicit casts if you're certain of your intention.
		uint64 timeToSampleCount(const uint64 ui64SamplingRate, const uint32 ui32Time);
		uint64 timeToSampleCount(const uint64 ui64SamplingRate, const float64 f64Time);
		float64 timeToSeconds(const uint32 ui32Time);
		float64 timeToSeconds(const float64 f64Time);
		uint64 secondsToTime(const uint32 ui32Time);
		uint64 secondsToTime(const uint64 ui64Time);
	};
}

#endif
