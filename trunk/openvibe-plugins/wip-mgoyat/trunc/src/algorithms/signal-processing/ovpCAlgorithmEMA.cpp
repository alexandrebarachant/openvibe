#include "ovpCAlgorithmEMA.h"

#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessing;
using namespace OpenViBEToolkit;
using namespace std;


CAlgorithmEMA::~CAlgorithmEMA(void)
{
}

void CAlgorithmEMA::release(void)
{
	delete this;
}

boolean CAlgorithmEMA::initialize(void)
{
   op_pMatrix.initialize(this->getOutputParameter(OVP_Algorithm_EMA_OutputParameter));
   ip_pMatrix.initialize(this->getInputParameter(OVP_Algorithm_EMA_InputParameter));
   
   ip_pParameter.initialize(this->getInputParameter(OVP_Algorithm_EMA_Parameter));

   return true;
}

boolean CAlgorithmEMA::uninitialize(void)
{
   op_pMatrix.uninitialize();
   ip_pMatrix.uninitialize();
   
   ip_pParameter.uninitialize();

 return true;
}

boolean CAlgorithmEMA::processInput(uint32 ui32InputIndex)
{
	//getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CAlgorithmEMA::process(void)
{
	 IMatrix* l_pInputMatrix=ip_pMatrix;
     IMatrix* l_pOutputMatrix=op_pMatrix;
	 float64 l_pParameter=ip_pParameter;
	 if(l_pParameter<0) {l_pParameter=-l_pParameter;}
	 if(l_pParameter>1) {l_pParameter=1.0/l_pParameter;}

	 if(this->isInputTriggerActive(OVP_Algorithm_EMA_InputTriggerId_Initialize))
        {
			//propriétés conservées
            OpenViBEToolkit::Tools::Matrix::copyDescription(*l_pOutputMatrix, *l_pInputMatrix);
			uint32 size=l_pInputMatrix->getDimensionSize(0);
			lastBuffer=std::vector<OpenViBE::float64>(size,0);
        }

     if(this->isInputTriggerActive(OVP_Algorithm_EMA_InputTriggerId_Process))
        {
			for(uint32 i=0; i<l_pInputMatrix->getDimensionSize(0); i++)
				{
					for(uint32 j=0; j<l_pInputMatrix->getDimensionSize(1); j++)
					{
					
						lastBuffer[i]=l_pParameter*l_pInputMatrix->getBuffer()[i*l_pInputMatrix->getDimensionSize(1)+j]
										+(1-l_pParameter)*lastBuffer[i];
						
						l_pOutputMatrix->getBuffer()[i*l_pOutputMatrix->getDimensionSize(1)+j]=lastBuffer[i];
							
						
					}
										
				}
			this->activateOutputTrigger(OVP_Algorithm_EMA_OutputTriggerId_ProcessDone,true);
		}

	return true;
}

