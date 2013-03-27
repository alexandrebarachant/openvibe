#ifndef __OpenViBE_Kernel_TimeArithmetics_H__
#define __OpenViBE_Kernel_TimeArithmetics_H__

#include "ov_types.h"

namespace OpenViBE
{
	namespace ITimeArithmetics
	{

		/**
		 * \class ITimeArithmetics
		 * \author Loic Mahe. Minor tweaks by Jussi T. Lindgren.
		 * \date 2013-03-27
		 * \brief Static functions to perform various conversions to and from the OpenViBE 32:32 fixed point time format.
		 *
		 * @FIXME Interface is WIP, pending thorough review & test. Functions here may change from day to another while this line is still here.
		 *
		 * All functions of the class are implemented by this header.
		 *
		 */

		/**
		 * \brief Get the fixed point time of a sample
		 * \param ui64SamplingRate : the sampling rate of the signal
		 * \param ui64SampleIndex : the index of the sample
		 * \return Time in fixed point corresponding to the input parameters
		 */
		static uint64 sampleIndexToTime(const uint64 ui64SamplingRate, const uint64 ui64SampleIndex)
		{
			return (ui64SampleIndex<<32)/ui64SamplingRate;
		}

		/**
		 * \brief Get the sample index corresponding to a fixed point time
		 * \param ui64SamplingRate : sampling rate of the signal
		 * \param ui64Time : current time in fixed point format
		 * \return Index of the sample
		 */
		static uint64 timeToSampleIndex(const uint64 ui64SamplingRate, const uint64 ui64Time)
		{
			return (ui64Time*ui64SamplingRate)>>32;
		}
		
		/**
		 * \brief Get the time in seconds given fixed point time
		 * \param ui64FixedPointTime : current time in fixed point format
		 * \return Regular floating point time in seconds
		 */
		static float64 timeToSeconds(const uint64 ui64Time)
		{
			return (ui64Time>>22)/1024.0;
		}

	};

}

#endif
