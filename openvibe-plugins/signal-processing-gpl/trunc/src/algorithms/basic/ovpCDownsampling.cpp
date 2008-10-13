#include "ovpCDownsampling.h"

#include <math.h> //floor, ceil

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessingGpl;
using namespace std;
// ________________________________________________________________________________________________________________
//

boolean CDownsampling::initialize(void)
{
	ip_ui64SamplingFrequency.initialize(getInputParameter(OVP_Algorithm_Downsampling_InputParameterId_SamplingFrequency));
	ip_ui64NewSamplingFrequency.initialize(getInputParameter(OVP_Algorithm_Downsampling_InputParameterId_NewSamplingFrequency));
	ip_pSignalMatrix.initialize(getInputParameter(OVP_Algorithm_Downsampling_InputParameterId_SignalMatrix));
	op_pSignalMatrix.initialize(getOutputParameter(OVP_Algorithm_Downsampling_OutputParameterId_SignalMatrix));

	return true;
}

boolean CDownsampling::uninitialize(void)
{
	op_pSignalMatrix.uninitialize();
	ip_pSignalMatrix.uninitialize();
	ip_ui64NewSamplingFrequency.uninitialize();
	ip_ui64SamplingFrequency.uninitialize();

	return true;
}

// ________________________________________________________________________________________________________________
//

boolean CDownsampling::process(void)
{
	// signal input vars
	IMatrix* l_pSignalInputMatrix=ip_pSignalMatrix;
	float64* l_pSignalInput = l_pSignalInputMatrix->getBuffer();
	uint32 l_ui32SignalInputMatrixDimensionNbChannels=ip_pSignalMatrix->getDimensionSize(0);
	uint32 l_ui32SignalInputMatrixDimensionSizeEpoch=ip_pSignalMatrix->getDimensionSize(1);

	// signal output vars
	IMatrix* l_pSignalOutputMatrix=op_pSignalMatrix;
	l_pSignalOutputMatrix->setDimensionCount(ip_pSignalMatrix->getDimensionCount());
	uint32 l_ui32SignalOutputMatrixDimensionNbChannels=l_ui32SignalInputMatrixDimensionNbChannels;
	uint32 l_ui32SignalOutputMatrixDimensionSizeEpoch= (uint64)floor(ip_pSignalMatrix->getDimensionSize(1) * ((float64)ip_ui64NewSamplingFrequency/(float64)ip_ui64SamplingFrequency));
	l_pSignalOutputMatrix->setDimensionSize(0, l_ui32SignalOutputMatrixDimensionNbChannels);
	l_pSignalOutputMatrix->setDimensionSize(1, l_ui32SignalOutputMatrixDimensionSizeEpoch);
	float64* l_pSignalOutput = l_pSignalOutputMatrix->getBuffer();

	// RatioSamplingFrequency
	float64 l_f64RatioSamplingFrequency = (float64)ip_ui64SamplingFrequency/(float64)ip_ui64NewSamplingFrequency;

	if(isInputTriggerActive(OVP_Algorithm_Downsampling_InputTriggerId_Initialize))
	{
	}

	if(isInputTriggerActive(OVP_Algorithm_Downsampling_InputTriggerId_Resample))
	{
		for (uint64 i=0;  i < l_ui32SignalOutputMatrixDimensionNbChannels; i++)
		{
			for(uint64 j=0 ; j<l_ui32SignalOutputMatrixDimensionSizeEpoch; j++)
			{
				uint32 l_ui32IndexInput = (uint32)floor(j*l_f64RatioSamplingFrequency);
				l_pSignalOutput[i*l_ui32SignalOutputMatrixDimensionSizeEpoch+j] =  l_pSignalInput[i*l_ui32SignalInputMatrixDimensionSizeEpoch+l_ui32IndexInput];
			}
		}
	}

	return true;
}
