#if defined(TARGET_HAS_ThirdPartyEIGEN)

#include "ovpCAlgorithmARBurgMethod.h"
#include <iostream>

#include <Eigen/Dense> 


using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
 
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessingGpl;
using namespace Eigen;
 
boolean CAlgorithmARBurgMethod::initialize(void)
{
    ip_pMatrix.initialize(this->getInputParameter(OVP_Algorithm_ARBurgMethod_InputParameterId_Matrix));
    op_pMatrix.initialize(this->getOutputParameter(OVP_Algorithm_ARBurgMethod_OutputParameterId_Matrix));
    ip_ui32Order.initialize(this->getInputParameter(OVP_Algorithm_ARBurgMethod_InputParameterId_UInteger));
    
    return true;
}
 
boolean CAlgorithmARBurgMethod::uninitialize(void)
{
    op_pMatrix.uninitialize();
    ip_pMatrix.uninitialize();
    ip_ui32Order.uninitialize();

    return true;
}
 
boolean CAlgorithmARBurgMethod::process(void)
{
    m_ui32Order = ip_ui32Order;

    int l_ui32ChannelCount = ip_pMatrix->getDimensionSize(0);
    int l_ui32SamplesPerChannel = ip_pMatrix->getDimensionSize(1);

    IMatrix* l_pInputMatrix=ip_pMatrix;
    IMatrix* l_pOutputMatrix=op_pMatrix;


    if(this->isInputTriggerActive(OVP_Algorithm_ARBurgMethod_InputTriggerId_Initialize))
    {

	if( l_pInputMatrix->getDimensionCount() != 2)
        {
	    this->getLogManager() << LogLevel_Error << "The input matrix must have 2 dimensions";
            return false;
        }

	if(l_pInputMatrix->getDimensionSize(1) < 2*m_ui32Order)
        {
            this->getLogManager() << LogLevel_Error << "The input vector must be greater than twice the order";
            return false;
        }

	// Setting size of output

	l_pOutputMatrix->setDimensionCount(2); // the output matrix will have 2 dimensions 
    	l_pOutputMatrix->setDimensionSize(0,1); // only one row vector giving the coefficients
    	l_pOutputMatrix->setDimensionSize(1,(m_ui32Order+1)*l_ui32ChannelCount);// The number of coefficients per channel is equal to the order

    }



    if(this->isInputTriggerActive(OVP_Algorithm_ARBurgMethod_InputTriggerId_Process))
    {

	// Compute the coefficients for each channel
	for(uint32 j=0; j<l_ui32ChannelCount; j++)
	{	
		// Initialization of all needed vectors	

    	m_vecXdErrForwardPrediction = RowVectorXd::Zero(l_ui32SamplesPerChannel); // Error Forward prediction
    	m_vecXdErrBackwardPrediction = RowVectorXd::Zero(l_ui32SamplesPerChannel); //Error Backward prediction

    	m_vecXdErrForward= RowVectorXd::Zero(l_ui32SamplesPerChannel); // Error Forward 
    	m_vecXdErrBackward= RowVectorXd::Zero(l_ui32SamplesPerChannel); // Error Backward

    	m_vecXdARCoefs= RowVectorXd::Zero(m_ui32Order+1); // Vector containing the AR coefficients for each channel, it will be our output vector
    	m_vecXdError= RowVectorXd::Zero(m_ui32Order+1); // Total error 


		float64 l_f64Knum = 0.0;
		float64 l_f64Kden = 0.0;
		m_f64K = 0.0;
		m_vecXdARCoefs(0) = 1.0;
		uint32 length;

		VectorXd l_vecXdARreversed;
		l_vecXdARreversed = VectorXd::Zero(m_ui32Order+1);

		// Retrieving input datas
		for(uint32 i=0; i<l_ui32SamplesPerChannel; i++)
		{
			m_vecXdErrForward(i) = l_pInputMatrix->getBuffer()[i+j*(l_ui32SamplesPerChannel)]; // Error Forward is the input matrix at first
			m_vecXdErrBackward(i) = l_pInputMatrix->getBuffer()[i+j*(l_ui32SamplesPerChannel)]; //Error Backward is the input matrix at first

			m_vecXdError(0) += (l_pInputMatrix->getBuffer()[i+j*(l_ui32SamplesPerChannel)] * l_pInputMatrix->getBuffer()[i+j*(l_ui32SamplesPerChannel)]) / l_ui32SamplesPerChannel;
		}

		// we iterate over the order
        	for(uint32 n=1; n<=m_ui32Order; n++)
        	{
            
			length = l_ui32SamplesPerChannel - n;

			m_vecXdErrForwardPrediction.resize(length);
			m_vecXdErrBackwardPrediction.resize(length);
	
			m_vecXdErrForwardPrediction = m_vecXdErrForward.tail(length);
			m_vecXdErrBackwardPrediction = m_vecXdErrBackward.head(length);

			l_f64Knum = -2.0 * m_vecXdErrBackwardPrediction.dot(m_vecXdErrForwardPrediction);
			l_f64Kden = (m_vecXdErrForwardPrediction.dot(m_vecXdErrForwardPrediction)) + (m_vecXdErrBackwardPrediction.dot(m_vecXdErrBackwardPrediction));

			m_f64K =  l_f64Knum / l_f64Kden;

			// Update errors forward and backward vectors

			m_vecXdErrForward = m_vecXdErrForwardPrediction + m_f64K * m_vecXdErrBackwardPrediction;
			m_vecXdErrBackward = m_vecXdErrBackwardPrediction + m_f64K * m_vecXdErrForwardPrediction;

			// Compute the AR coefficients

			for(uint32 i=1; i<=n; i++)
			{
				l_vecXdARreversed(i) = m_vecXdARCoefs(n - i);
			}

			m_vecXdARCoefs = m_vecXdARCoefs+m_f64K*l_vecXdARreversed;

			// Update Total Error
			m_vecXdError (n) = (1 - m_f64K*m_f64K) * m_vecXdError(n-1);


			}
		
		for(uint32 i=0; i<=m_ui32Order; i++)
		{
			l_pOutputMatrix->getBuffer()[i+j*m_ui32Order]= m_vecXdARCoefs(i);
		}

	}
 	

        this->activateOutputTrigger(OVP_Algorithm_ARBurgMethod_OutputTriggerId_ProcessDone, true);
    }

    return true;
}
#endif // TARGET_HAS_ThirdPartyEIGEN
