/*
 * \author Jussi T. Lindgren / Inria
 * \date 29.03.2013
 * \brief Some tests for the Time Arithmetics class
 */
#include <iostream>
#include <iomanip>
#include <cstdlib> // abs() on Linux

#include <openvibe/ovITimeArithmetics.h>

using namespace OpenViBE;
using namespace std;

int main(int argc, char *argv[]) 
{
	// @note A rate of 101 can cause a glitch. Are we ever using uneven sampling rates?
	const uint64 samplingRatesToTest[] = {100,128,200,400,512,1000,16000,44100};

	int retVal = 0;

	const uint64 secsAndBackTolerance = ITimeArithmetics::secondsToTime(0.001);

	cout << "Conversion tolerance from fixed point to float and back has been set to " << ITimeArithmetics::timeToSeconds(secsAndBackTolerance) << " secs\n\n";

	{
		cout << "------\nTest: 'time to sample and back' using predefined times (given orig. as secs)\n------\n";

		const uint64 timesToTest[] = {ITimeArithmetics::secondsToTime(0.0),
			ITimeArithmetics::secondsToTime(1.0),
			ITimeArithmetics::secondsToTime(1.5),
			ITimeArithmetics::secondsToTime(2.0),
			ITimeArithmetics::secondsToTime(100.0),
			ITimeArithmetics::secondsToTime(128.0),
			ITimeArithmetics::secondsToTime(10*60.0)};

		cout << " " << setw(6) << "Rate"
			<< " " << setw(10) << "Sample"
			<< " " << setw(15) << "Time"
			<< " " << setw(15) << "TimeInv"
			<< " " << setw(5)  << "Secs"
			<< " " << setw(15) << "TimeInv2"
			<< " Tests\n";
		for(size_t i=0;i<sizeof(samplingRatesToTest)/sizeof(samplingRatesToTest[0]);i++) {
			for(size_t j=0;j<sizeof(timesToTest)/sizeof(timesToTest[0]);j++) {
				uint64 sampleIdx = ITimeArithmetics::timeToSampleIndex(samplingRatesToTest[i], timesToTest[j]);
				uint64 timeFromSample =  ITimeArithmetics::sampleIndexToTime(samplingRatesToTest[i], sampleIdx);
				float64 secondsFromTime = ITimeArithmetics::timeToSeconds(timesToTest[j]);
				uint64 timeFromSeconds = ITimeArithmetics::secondsToTime(secondsFromTime);
				cout << " " << setw(6) << samplingRatesToTest[i] 
					<< " " << setw(10) << sampleIdx
					<< " " << setw(15) << timesToTest[j] 
					<< " " << setw(15) << timeFromSample
					<< " " << setw(5)  << secondsFromTime
					<< " " << setw(15) << timeFromSeconds;
				boolean hadError = false;
				if(timeFromSample != timesToTest[j]) {
					cout << " Error: Converting time to sample and back don't match\n";
					retVal |= 1;
					hadError = true;
				}  
				if(std::abs((int64)timeFromSeconds - (int64)timesToTest[j]) > (int64) secsAndBackTolerance) {
					cout << " Error: Converting time to seconds and back exceed tolerance\n";
					retVal |= 2;
					hadError = true;
				} 
				if(!hadError) { 
					cout << " Ok\n";
				}
			}
		}
	}

	{
		cout << "------\nTest: 'sample to time and back' using predefined samples\n------\n";

		const uint64 samplesToTest[] = {0,1,100,128,512,1000,1021,1024,1000001};

		cout << " " << setw(6) << "Rate"
			<< " " << setw(10) << "Sample"
			<< " " << setw(15) << "SampleInv"
			<< " " << setw(15) << "Time"
			<< " " << setw(15) << "Secs"
			<< " " << setw(15) << "TimeInv2"
			<< " Tests\n";
		for(size_t i=0;i<sizeof(samplingRatesToTest)/sizeof(samplingRatesToTest[0]);i++) {
			for(size_t j=0;j<sizeof(samplesToTest)/sizeof(samplesToTest[0]);j++) {
				uint64 timeFromSample =  ITimeArithmetics::sampleIndexToTime(samplingRatesToTest[i], samplesToTest[j]);
				float64 secondsFromTime = ITimeArithmetics::timeToSeconds(timeFromSample);
				uint64 sampleFromTime =  ITimeArithmetics::timeToSampleIndex(samplingRatesToTest[i], timeFromSample);
				uint64 timeFromSeconds = ITimeArithmetics::secondsToTime(secondsFromTime);

				cout << " " << setw(6) << samplingRatesToTest[i] 
					<< " " << setw(10) << samplesToTest[j]
					<< " " << setw(15) << sampleFromTime
					<< " " << setw(15) << timeFromSample
					<< " " << setw(15) << secondsFromTime
					<< " " << setw(15) << timeFromSeconds;
				boolean hadError = false;
				if(samplesToTest[j] != sampleFromTime) {
					cout << " Error: Converting sample to time and back don't match\n";
					retVal |= 3;
					hadError = true;
				}
				if(std::abs((int64)timeFromSeconds - (int64)timeFromSample) > (int64)secsAndBackTolerance) {
					cout << " Error: Converting time to seconds and back exceeded tolerance\n";
					retVal |= 4;
					hadError = true;
				} 
				if(!hadError) { 
					cout << " Ok\n";
				}
			}
		}
	}

	{
		cout << "------\nTest: One second of signal at 'rate' should equal 'rate' samples\n------\n";

		cout << setw(6) << "Rate" 
		     << setw(8) << "Samples"
			 << " Test\n";
		for(size_t i=0;i<sizeof(samplingRatesToTest)/sizeof(samplingRatesToTest[0]);i++) {
			uint64 nSamples = ITimeArithmetics::timeToSampleIndex(samplingRatesToTest[i], ITimeArithmetics::secondsToTime(1.0));
			cout << setw(6) << samplingRatesToTest[i] 
			     << setw(8) << nSamples;
			if(nSamples == samplingRatesToTest[i]) {
				cout << " Ok\n";
			} else {
				cout << " Error\n";
				retVal |= 5;
			}
		}
	}

	return retVal;
} 