#include "ovpCApplyTemporalFilter.h"

#include <cmath>

using namespace OpenViBE;
using namespace Kernel;
using namespace Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessingGpl;
using namespace itpp;
using namespace std;
// ________________________________________________________________________________________________________________
//

boolean CApplyTemporalFilter::initialize(void)
{
	ip_pSignalMatrix.initialize(getInputParameter(OVP_Algorithm_ApplyTemporalFilter_InputParameterId_SignalMatrix));
	ip_pFilterCoefficientsMatrix.initialize(getInputParameter(OVP_Algorithm_ApplyTemporalFilter_InputParameterId_FilterCoefficientsMatrix));
	op_pSignalMatrix.initialize(getOutputParameter(OVP_Algorithm_ApplyTemporalFilter_OutputParameterId_FilteredSignalMatrix));

	return true;
}

boolean CApplyTemporalFilter::uninitialize(void)
{
	op_pSignalMatrix.uninitialize();
	ip_pFilterCoefficientsMatrix.uninitialize();
	ip_pSignalMatrix.uninitialize();

	return true;
}

// ________________________________________________________________________________________________________________
//

boolean CApplyTemporalFilter::process(void)
{
	IMatrix* l_pSignalInputMatrix=ip_pSignalMatrix;
	IMatrix* l_pOutputMatrix=op_pSignalMatrix;

	if(isInputTriggerActive(OVP_Algorithm_ApplyTemporalFilter_InputTriggerId_Initialize))
	{
		m_bFlagInitialize  = true;

		OpenViBEToolkit::Tools::MatrixManipulation::copyDescription(*l_pOutputMatrix, *l_pSignalInputMatrix);
		OpenViBEToolkit::Tools::MatrixManipulation::clearContent(*l_pOutputMatrix);

		// dimension of input coef (numerator, denominator) filter
		uint32 l_ui32FilterCoefficientsNumeratorDimensionSize=ip_pFilterCoefficientsMatrix->getDimensionSize(1);
		uint32 l_ui32FilterCoefficientsDenominatorDimensionSize=ip_pFilterCoefficientsMatrix->getDimensionSize(0);

		//coef filters vars
		IMatrix* l_pFilterCoefficientsInputMatrix=ip_pFilterCoefficientsMatrix;
		float64* l_pFilterCoefficientsInput =l_pFilterCoefficientsInputMatrix->getBuffer();

		m_vecDenomCoefFilter = zeros(l_ui32FilterCoefficientsDenominatorDimensionSize);
		m_vecNumCoefFilter= zeros(l_ui32FilterCoefficientsNumeratorDimensionSize);
		for (uint32 i=0; i<l_ui32FilterCoefficientsDenominatorDimensionSize;i++)
		{
			m_vecDenomCoefFilter[i] = l_pFilterCoefficientsInput[i];
		}
		for (uint32 i=0; i<l_ui32FilterCoefficientsNumeratorDimensionSize;i++)
		{
			m_vecNumCoefFilter[i] = l_pFilterCoefficientsInput[l_ui32FilterCoefficientsDenominatorDimensionSize+i];
		}
	}
	if(isInputTriggerActive(OVP_Algorithm_ApplyTemporalFilter_InputTriggerId_ApplyFilter))
	{
		// signal input vars
		float64* l_pSignalInput = l_pSignalInputMatrix->getBuffer();

		// dimension of input signal buffer
		// uint32 l_ui32SignalInputMatrixDimensionCount=ip_pSignalMatrix->getDimensionCount();
		uint32 l_ui32SignalInputMatrixDimensionNbChannels=ip_pSignalMatrix->getDimensionSize(0);
		uint32 l_ui32SignalInputMatrixDimensionSizeEpoch=ip_pSignalMatrix->getDimensionSize(1);

/*
		// signal output vars
		l_pOutputMatrix->setDimensionCount(l_ui32SignalInputMatrixDimensionCount);
		for (uint32 i=0 ; i<l_ui32SignalInputMatrixDimensionCount; i++)
		{
			l_pOutputMatrix->setDimensionSize(i,ip_pSignalMatrix->getDimensionSize(i));
		}
*/
		float64* l_pFilteredSignalMatrix= l_pOutputMatrix->getBuffer();

		vec x(l_ui32SignalInputMatrixDimensionSizeEpoch);
		vec y(l_ui32SignalInputMatrixDimensionSizeEpoch);
		x = zeros(l_ui32SignalInputMatrixDimensionSizeEpoch);

		for (uint64 i=0;  i < l_ui32SignalInputMatrixDimensionNbChannels; i++)
		{
			for(uint64 j=0 ; j<l_ui32SignalInputMatrixDimensionSizeEpoch; j++)
			{
				x[j] =  (double)l_pSignalInput[i*l_ui32SignalInputMatrixDimensionSizeEpoch+j];
			}

			y = filter(m_vecDenomCoefFilter, m_vecNumCoefFilter, x);

			for(uint64 k=0 ; k<l_ui32SignalInputMatrixDimensionSizeEpoch ; k++)
			{
				l_pFilteredSignalMatrix[i*l_ui32SignalInputMatrixDimensionSizeEpoch+k] = y[k];
			}
		}
	}

	if(isInputTriggerActive(OVP_Algorithm_ApplyTemporalFilter_InputTriggerId_ApplyFilterWithHistoric))
	{
		// signal input vars
		float64* l_pSignalInput = l_pSignalInputMatrix->getBuffer();

		// dimension of input signal biuffer
		// uint32 l_ui32SignalInputMatrixDimensionCount=ip_pSignalMatrix->getDimensionCount();
		uint32 l_ui32SignalInputMatrixDimensionNbChannels=ip_pSignalMatrix->getDimensionSize(0);
		uint32 l_ui32SignalInputMatrixDimensionSizeEpoch=ip_pSignalMatrix->getDimensionSize(1);

		// historic buffers
		if (m_bFlagInitialize)
		{
			m_oCurrentStates.resize(l_ui32SignalInputMatrixDimensionNbChannels);
			for(size_t i=0 ; i<l_ui32SignalInputMatrixDimensionNbChannels ; i++)
			{
				m_oCurrentStates[i] = zeros(ip_pFilterCoefficientsMatrix->getDimensionSize(0)-1);
			}
			m_bFlagInitialize = false;
		}
/*
		// signal output vars
		l_pOutputMatrix->setDimensionCount(l_ui32SignalInputMatrixDimensionCount);
		for (uint32 i=0 ; i<l_ui32SignalInputMatrixDimensionCount; i++)
		{
			l_pOutputMatrix->setDimensionSize(i,ip_pSignalMatrix->getDimensionSize(i));
		}
*/
		float64* l_pFilteredSignalMatrix= l_pOutputMatrix->getBuffer();

		vec x(l_ui32SignalInputMatrixDimensionSizeEpoch);
		vec y(l_ui32SignalInputMatrixDimensionSizeEpoch);
		x = zeros(l_ui32SignalInputMatrixDimensionSizeEpoch);

		for (uint64 i=0;  i < l_ui32SignalInputMatrixDimensionNbChannels; i++)
		{
			for(uint64 j=0 ; j<l_ui32SignalInputMatrixDimensionSizeEpoch; j++)
			{
				x[j] =  (double)l_pSignalInput[i*l_ui32SignalInputMatrixDimensionSizeEpoch+j];
			}

			y = filter(m_vecDenomCoefFilter, m_vecNumCoefFilter, x, m_oCurrentStates[i], m_oCurrentStates[i]);

			for(uint64 k=0 ; k<l_ui32SignalInputMatrixDimensionSizeEpoch ; k++)
			{
				l_pFilteredSignalMatrix[i*l_ui32SignalInputMatrixDimensionSizeEpoch+k] = y[k];
			}
		}
	}

	return true;
}
