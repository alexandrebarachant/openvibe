/*
 * \author Jussi T. Lindgren / Inria
 * \date 29.03.2013
 * \brief Some tests for the Time Arithmetics class
 */
#include <iostream>
#include <iomanip>
#include <cstdlib> // abs() on Linux
#include <bitset>

#include <openvibe/ovITimeArithmetics.h>

using namespace OpenViBE;
using namespace std;

// Legacy code to convert time to seconds
float64 oldTimeToSeconds(const uint64 ui64Time)
{
	return (ui64Time>>22)/1024.0;
}

// Legacy code to convert seconds to time
uint64 oldSecondsToTime(const float64 f64Time)
{
	return ((uint64)(f64Time*1024.0))<<22;
}

// legacy code from ovIStimulationSet.h for comparison
OpenViBE::uint64 sampleIndexToTime(OpenViBE::uint32 ui32SamplingRate, OpenViBE::uint64 ui64SampleIndex)
{	
	return ((ui64SampleIndex<<32)+(ui32SamplingRate-1))/ui32SamplingRate;
}

// legacy code from ovIStimulationSet.h for comparison
OpenViBE::uint64 timeToSampleIndex(OpenViBE::uint32 ui32SamplingRate, OpenViBE::uint64 ui64Time)
{
	return (ui64Time*ui32SamplingRate)>>32;
}

// Alternate timeToSeconds and secondsToTime that very precise with decimals but not so great with huge numbers
//	f64seconds = float64(ui64Time)/float64(1LL<<32);
//	ui64time = uint64(f64Time*float64(1LL<<32)); 

int main(int argc, char *argv[]) 
{
	const float64 l_f64secsAndBackTolerance = 0.00001;

	// @note A rate of 101 can cause a glitch. Are we ever using uneven sampling rates?
	const uint32 samplingRatesToTest[] = {100,128,512,1000,1024,16000,44100};
	const float64 l_f64timesToTest[] = {
		0, 0.001, 0.01, 0.1, 0.2, 0.25, 0.5, 1.0, 1.1, 1.5, 2, 5, 10, 50, 100, 123.456789, 128.0, 500, 1000, 2500, 5000};
	const uint64 l_ui64samplesToTest[] = {0,1,100,128,512,1000,1021,1024,5005,12345, 59876, 100000, 717893, 1000001};
	const uint64 l_ui64timesToTest[] = { 1LL<<8, 1LL<<16, 1L<<19, 1LL<<22, 1LL<<27, 1L<<30, 1LL<<32, 10LL<<32, 100LL<<32, 123LL<<32, 500LL<<32, 512LL<<32, 1000LL<<32, 1024LL<<32, 2001LL<<32, 5000LL<<32};

	cout << "Conversion tolerance from fixed point to float and back has been set to " << l_f64secsAndBackTolerance << " secs\n";
	cout << "ITimeArithmetics claims to support precision up to " << 1.0/(1L<<ITimeArithmetics::m_ui32DecimalPrecision) << " secs\n\n";

	int retVal = 0;

	{
		cout << "------\nTest: map time float->fixed->float\n---------\n";
		cout 
			<< " " << setw(15) << "TimeFloat"
			<< " " << setw(15) << "Time"
			<< " " << setw(15) << "TimeFloatInv"
			<< " Tests\n";

		for(uint32 i=0;i<sizeof(l_f64timesToTest)/sizeof(l_f64timesToTest[0]);i++) 
		{
			uint64 l_ui64mapped=ITimeArithmetics::secondsToTime(l_f64timesToTest[i]);
			float64 l_f64inverse=ITimeArithmetics::timeToSeconds(l_ui64mapped);

			cout << " " << setw(15) << l_f64timesToTest[i] << " " << setw(15) << l_ui64mapped << " " << setw(15) << l_f64inverse;
		
			float64 l_f64AbsDiff = (l_f64timesToTest[i] - l_f64inverse);
			l_f64AbsDiff = (l_f64AbsDiff >= 0 ? l_f64AbsDiff : -l_f64AbsDiff); // no std::abs() for float64 on Ubuntu LTS gcc at the moment

			if(l_f64AbsDiff < l_f64secsAndBackTolerance)
			{
				cout << " Ok\n";
			}
			else if(l_f64AbsDiff < 1.0/(1LL << ITimeArithmetics::m_ui32DecimalPrecision)) 
			{
				cout << " Ok(*)\n";
			}
			else {
				cout << " Err " << l_f64AbsDiff << "\n";
				retVal |= 1;
			}
		}
		cout << "*) Error is smaller than the minimum precision supported by ITimeArithmetics.h\n";
	}

	{
		cout << "------\nTest: map time fixed->float->fixed\n---------\n";
		cout 
			<< " " << setw(15) << "Time"
			<< " " << setw(15) << "TimeFloat"
			<< " " << setw(15) << "TimeInv"
			<< " Tests\n";

		for(uint32 i=0;i<sizeof(l_ui64timesToTest)/sizeof(l_ui64timesToTest[0]);i++) 
		{
			uint64 l_ui64Time = l_ui64timesToTest[i];
			float64 l_f64mapped=ITimeArithmetics::timeToSeconds(l_ui64Time);
			uint64 l_ui64inverse=ITimeArithmetics::secondsToTime(l_f64mapped);

			cout << " " << setw(15) << l_ui64Time << " " << setw(15) << l_f64mapped << " " << setw(15) << l_ui64inverse;
		
			if(l_ui64inverse == l_ui64Time) 
			{
				cout << " Ok\n";
			} else {
				uint64 l_ui64SignificantBitsA = (l_ui64inverse >> (32-ITimeArithmetics::m_ui32DecimalPrecision));
				uint64 l_ui64SignificantBitsB = (l_ui64Time >> (32-ITimeArithmetics::m_ui32DecimalPrecision));
				if(l_ui64SignificantBitsA == l_ui64SignificantBitsB) {
					cout << " Ok(*)\n";
				}
				else
				{
					cout << " Err\n";
					retVal |= 2;
				}
			}
		}
		cout << "*) Error is smaller than the minimum precision supported by ITimeArithmetics.h\n";
	}

	{
		cout << "------\nTest: time -> sample -> time using predefined times (orig. given as secs)\n------\n";

		cout << " " << setw(6) << "Rate"
			<< " " << setw(10) << "TimeSecs"
			<< " " << setw(15) << "Time"
			<< " " << setw(10) << "Sample"
			<< " " << setw(15) << "TimeInv"
			<< " Tests\n";
		for(size_t i=0;i<sizeof(samplingRatesToTest)/sizeof(samplingRatesToTest[0]);i++) {
			for(size_t j=0;j<sizeof(l_f64timesToTest)/sizeof(l_f64timesToTest[0]);j++) {

				float64 l_f64Time = l_f64timesToTest[j];

				uint64 l_ui64Time = ITimeArithmetics::secondsToTime(l_f64Time);
				uint64 sampleIdx = ITimeArithmetics::timeToSampleCount(samplingRatesToTest[i], l_ui64Time);
				uint64 timeFromSample =  ITimeArithmetics::sampleCountToTime(samplingRatesToTest[i], sampleIdx);

				cout << " " << setw(6) << samplingRatesToTest[i] 
					<< " " << setw(10) << l_f64Time
					<< " " << setw(15) << l_ui64Time
					<< " " << setw(10) << sampleIdx
					<< " " << setw(15) << timeFromSample
					;
				if(timeFromSample != l_ui64Time) {
					float64 l_f64Period = 1.0/(float64)samplingRatesToTest[i];
					uint64 l_ui64AbsDiff = std::abs((int64)timeFromSample - (int64)l_ui64Time);
					float64 l_f64AbsDiff = ITimeArithmetics::timeToSeconds(l_ui64AbsDiff);
					if(l_f64AbsDiff > l_f64Period)
					{
						cout << " Error: Mismatch of " << l_f64AbsDiff << "s, period " << l_f64Period << "\n";
						retVal |= 3;
					} else {
						cout << " Ok(*)\n";
					}
				}  
				else
				{
					cout << " Ok\n";
				}
			}
		}
		cout << "*) Error is smaller than precision allowed by the sampling rate\n";
	}

	{
		cout << "------\nTest: sample->time->sample using predefined samples\n------\n";

		cout << " " << setw(6) << "Rate"
			<< " " << setw(10) << "Sample"
			<< " " << setw(15) << "Time"
			<< " " << setw(10) << "SampleInv"
			<< " Tests\n";
		for(size_t i=0;i<sizeof(samplingRatesToTest)/sizeof(samplingRatesToTest[0]);i++) {
			for(size_t j=0;j<sizeof(l_ui64samplesToTest)/sizeof(l_ui64samplesToTest[0]);j++) {

				uint64 l_ui64testSample = l_ui64samplesToTest[j];
				uint64 timeFromSample =  ITimeArithmetics::sampleCountToTime(samplingRatesToTest[i], l_ui64testSample);
				uint64 sampleInv = ITimeArithmetics::timeToSampleCount(samplingRatesToTest[i], timeFromSample);

				cout << " " << setw(6) << samplingRatesToTest[i] 
					<< " " << setw(10) << l_ui64testSample
					<< " " << setw(15) << timeFromSample
					<< " " << setw(10) << sampleInv
					;
				if(l_ui64testSample == sampleInv) 
				{
					cout << " Ok\n";
				}
				else
				{
					cout << " Error: Mismatch\n";
					retVal |= 4;
				} 
			}
		}
	}


	{
		cout << "------\nTest: One second of signal at 'rate' should equal 'rate' samples\n------\n";
		cout << " " << setw(6) << "Rate" 
		     << " " << setw(8) << "Samples"
			 << " Test\n";
		for(size_t i=0;i<sizeof(samplingRatesToTest)/sizeof(samplingRatesToTest[0]);i++) {
			uint64 nSamples = ITimeArithmetics::timeToSampleCount(samplingRatesToTest[i], ITimeArithmetics::secondsToTime(1.0));
			cout << " " << setw(6) << samplingRatesToTest[i] 
			     << " " << setw(8) << nSamples;
			if(nSamples == samplingRatesToTest[i]) {
				cout << " Ok\n";
			} else {
				cout << " Error\n";
				retVal |= 5;
			}
		}
	}

	cout << "------\nTest: Comparing epoch durations between ITimeArithmetics and old code from stimulationBasedEpoching\n---------\n";
	cout << " " << setw(10) << "Duration" 
	     << " " << setw(15) << "ResultA"
	     << " " << setw(15) << "ResultB"
	     << " " << setw(10) << "AbsDiff"
		 << " Test\n";

	{
		const float64 l_f64EpochDurations[] = {0.01, 0.1, 0.2, 0.25, 0.5, 1.0, 1.1, 1.5, 2, 5, 10,50,100};
		for(uint32 i=0;i<sizeof(l_f64EpochDurations)/sizeof(const float64);i++) 
		{
			uint64 ui64resultA=(int64)(l_f64EpochDurations[i]*(1LL<<32)); // Code from stimulationBasedEpoching
			uint64 ui64resultB=ITimeArithmetics::secondsToTime(l_f64EpochDurations[i]);
			cout << " " << setw(10) << l_f64EpochDurations[i] << " " << setw(15) << ui64resultA << " " << setw(15) << ui64resultB;
			uint64 l_ui64AbsDiff = std::abs((int64)ui64resultA-int64(ui64resultB));
			float64 l_f64Diff = ITimeArithmetics::timeToSeconds(l_ui64AbsDiff);
			cout << " " << setw(10) << l_f64Diff;

			if(l_ui64AbsDiff==0) 
			{
				cout << " Ok\n";
			}
			else
			{
				uint64 l_ui64SignificantBitsA = (ui64resultA >> (32-ITimeArithmetics::m_ui32DecimalPrecision));
				uint64 l_ui64SignificantBitsB = (ui64resultB >> (32-ITimeArithmetics::m_ui32DecimalPrecision));
				if(l_ui64SignificantBitsA == l_ui64SignificantBitsB) {
					cout << " Ok(*)\n";
				} else {
					std::bitset<64> tmpA(ui64resultA);
					std::bitset<64> tmpB(ui64resultB);
					cout << " Err " << tmpA << " " << ITimeArithmetics::timeToSeconds(ui64resultA);
					cout << " " << tmpB << " " << ITimeArithmetics::timeToSeconds(ui64resultB) << "\n";
					retVal |= 6;
				} 
			}
		}
		cout << "*) Error is smaller than the minimum precision supported by ITimeArithmetics.h\n";
	}

	cout << "------\nMisc tests (no errors will be reported)\n------------\n";

	uint64 ui64resultA, ui64resultB;

	// Test with milliseconds, the divisor 1000 can be in different positions in OV code. Small differences are to be expected here.
	const int ui32MilliSeconds = 123;
	ui64resultA = ((((uint64)ui32MilliSeconds)<<22)/1000)<<10;;		// Conversion that was sometimes used in the code
	ui64resultB = ITimeArithmetics::secondsToTime(ui32MilliSeconds/1000.0);
	cout << "m1: A " << ui64resultA << " B " << ui64resultB << "\n";

	// Code from stimulationBasedEpoching. Expect 0 diff
	const float64 l_f64EpochDuration = 1.1f;
	ui64resultA=(int64)(l_f64EpochDuration*(1LL<<32));
	ui64resultB=ITimeArithmetics::secondsToTime(l_f64EpochDuration);
	cout << "m2: A " << ui64resultA << " B " << ui64resultB << "\n";

	// Code from ovIStimulationSet.h. Time is typically off by one between the old code and code from ovITimeArithmetics.h using the following parameters.
	const uint64 l_ui64SampleIndex = 99; const uint32 l_ui32SamplingRate = 310;
	const uint64 l_ui64TestTime = 123LL<<32;
	ui64resultA = sampleIndexToTime(l_ui32SamplingRate, l_ui64SampleIndex); 
	ui64resultB = ITimeArithmetics::sampleCountToTime(l_ui32SamplingRate, l_ui64SampleIndex);
	cout << "m3: A " << ui64resultA << " B " << ui64resultB << "\n";
	ui64resultA = timeToSampleIndex(l_ui32SamplingRate, l_ui64TestTime);
	ui64resultB = ITimeArithmetics::timeToSampleCount(l_ui32SamplingRate, l_ui64TestTime);
	cout << "m4: A " << ui64resultA << " B " << ui64resultB << "\n";

	// Test a sequence of small indexes
	// Code from ovIStimulationSet.h. Time is typically off by one between the old code and code from ovITimeArithmetics.h using the following parameters.
	const uint32 l_ui32TestFrequency = 100;
	for(uint32 i=0;i<32;i++) {
		ui64resultA = sampleIndexToTime(l_ui32TestFrequency, i);
		ui64resultB = ITimeArithmetics::sampleCountToTime(l_ui32TestFrequency, i);
		cout << "m5: " << setw(3) << i << ": A " << ui64resultA << " B " << ui64resultB << "\n";
	}
	// Code from ovIStimulationSet.h. Sample counts should be equivalent regardless of the time skew of 1 earlier.
	for(uint32 i=0;i<32;i++) {
		ui64resultA = timeToSampleIndex(l_ui32TestFrequency, sampleIndexToTime(l_ui32TestFrequency, i));
		ui64resultB = ITimeArithmetics::timeToSampleCount(l_ui32TestFrequency, ITimeArithmetics::sampleCountToTime(100, i));
		cout << "m6: " << setw(3) << i << ": A " << ui64resultA << " B " << ui64resultB << "\n";
	}

	const float64 l_f64TestNumber = 1.23456789;
	cout << "m7: A " << setprecision(10) << oldTimeToSeconds(oldSecondsToTime(l_f64TestNumber))
		<<  " B " << setprecision(10) << ITimeArithmetics::timeToSeconds(ITimeArithmetics::secondsToTime(l_f64TestNumber))
		<< "\n";

	return retVal;
} 
