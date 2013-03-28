
#include <iostream>
#include <iomanip>

#include <openvibe/ovITimeArithmetics.h>

using namespace OpenViBE;
using namespace std;

int main(int argc, char *argv[]) 
{
	uint64 samplingRates[] = {100,128,200,400,512,1000};

	int retVal = 0;

	uint64 secsAndBackTolerance = {ITimeArithmetics::secondsToTime(0.005)};

	cout << "To seconds and back conversion tolerance has been set to " << ITimeArithmetics::timeToSeconds(secsAndBackTolerance) << " secs\n\n";

	{
		cout << "--\n Test 'time to sample and back' using predefined times \n--\n";

		uint64 timeNow[] = {ITimeArithmetics::secondsToTime(0.0),
			ITimeArithmetics::secondsToTime(1.0),
			ITimeArithmetics::secondsToTime(1.5),
			ITimeArithmetics::secondsToTime(2.0),
			ITimeArithmetics::secondsToTime(128.0)};

		cout << " " << setw(6) << "Rate"
			<< " " << setw(10) << "Sample"
			<< " " << setw(15) << "Time"
			<< " " << setw(15) << "TimeInv"
			<< " " << setw(5)  << "Secs"
			<< " " << setw(15) << "TimeInv2"
			<< "\n";
		for(int i=0;i<sizeof(samplingRates)/sizeof(samplingRates[0]);i++) {
			for(int j=0;j<sizeof(timeNow)/sizeof(timeNow[0]);j++) {
				uint64 sampleIdx = ITimeArithmetics::timeToSampleIndex(samplingRates[i], timeNow[j]);
				uint64 timeFromSample =  ITimeArithmetics::sampleIndexToTime(samplingRates[i], sampleIdx);
				float64 secondsFromTime = ITimeArithmetics::timeToSeconds(timeNow[j]);
				uint64 timeFromSeconds = ITimeArithmetics::secondsToTime(secondsFromTime);
				cout << " " << setw(6) << samplingRates[i] 
					<< " " << setw(10) << sampleIdx
					<< " " << setw(15) << timeNow[j] 
					<< " " << setw(15) << timeFromSample
					<< " " << setw(5)  << secondsFromTime
					<< " " << setw(15) << timeFromSeconds;
				boolean hadError = false;
				if(timeFromSample != timeNow[j]) {
					cout << "  Error: Converting time to sample and back don't match\n";
					retVal |= 1;
					hadError = true;
				}  
				if(std::abs((int64)timeFromSeconds - (int64)timeNow[j]) > (int64) secsAndBackTolerance) {
					cout << "  Error: Converting time to seconds and back don't match\n";
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
		cout << "--\n Test 'sample to time and back' using predefined samples\n--\n";

		uint64 sampleIdxs[] = {0,1,1000,1021,1024};

		cout << " " << setw(6) << "Rate"
			<< " " << setw(10) << "Sample"
			<< " " << setw(15) << "SampleInv"
			<< " " << setw(15) << "Time"
			<< " " << setw(15) << "Secs"
			<< " " << setw(15) << "TimeInv2"
			<< "\n";
		for(int i=0;i<sizeof(samplingRates)/sizeof(samplingRates[0]);i++) {
			for(int j=0;j<sizeof(sampleIdxs)/sizeof(sampleIdxs[0]);j++) {
				uint64 timeFromSample =  ITimeArithmetics::sampleIndexToTime(samplingRates[i], sampleIdxs[j]);
				float64 secondsFromTime = ITimeArithmetics::timeToSeconds(timeFromSample);
				uint64 sampleFromTime =  ITimeArithmetics::timeToSampleIndex(samplingRates[i], timeFromSample);
				uint64 timeFromSeconds = ITimeArithmetics::secondsToTime(secondsFromTime);

				cout << " " << setw(6) << samplingRates[i] 
					<< " " << setw(10) << sampleIdxs[j]
					<< " " << setw(15) << sampleFromTime
					<< " " << setw(15) << timeFromSample
					<< " " << setw(15) << secondsFromTime
					<< " " << setw(15) << timeFromSeconds;
				boolean hadError = false;
				if(sampleIdxs[j] != sampleFromTime) {
					cout << "  Error: Converting sample to time and back don't match\n";
					retVal |= 3;
					hadError = true;
				}
				if(std::abs((int64)timeFromSeconds - (int64)timeFromSample) > (int64)secsAndBackTolerance) {
					cout << "  Error: Converting time to seconds and back don't match\n";
					retVal |= 4;
					hadError = true;
				} 
				if(!hadError) { 
					cout << " Ok\n";
				}
			}
		}
	}

	return retVal;
} 