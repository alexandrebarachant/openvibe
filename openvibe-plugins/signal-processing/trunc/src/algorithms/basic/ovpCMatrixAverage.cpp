#include "ovpCMatrixAverage.h"

#include <system/Memory.h>
#include <cmath>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessing;

// ________________________________________________________________________________________________________________
//

boolean CMatrixAverage::initialize(void)
{
	ip_pMatrix.initialize(getInputParameter(OVP_Algorithm_MatrixAverage_InputParameterId_Matrix));
	ip_i64MatrixCount.initialize(getInputParameter(OVP_Algorithm_MatrixAverage_InputParameterId_MatrixCount));
	ip_bMovingAverage.initialize(getInputParameter(OVP_Algorithm_MatrixAverage_InputParameterId_MovingAverage));
	op_pAveragedMatrix.initialize(getOutputParameter(OVP_Algorithm_MatrixAverage_OutputParameterId_AveragedMatrix));

	m_ui32MatrixElementCount=0;
	m_ui64MatrixIndex=0;
	m_ui64MatrixCacheCount=0;
	m_ui64MatrixCount=0;
	m_bInfiniteAverage=false;
	m_bMovingAverage=false;
	m_bShouldPerformAverage=false;

	m_pMatrixCache=NULL;

	return true;
}

boolean CMatrixAverage::uninitialize(void)
{
	delete [] m_pMatrixCache;
	m_pMatrixCache=NULL;

	op_pAveragedMatrix.uninitialize();
	ip_bMovingAverage.uninitialize();
	ip_i64MatrixCount.uninitialize();
	ip_pMatrix.uninitialize();

	return true;
}

// ________________________________________________________________________________________________________________
//

boolean CMatrixAverage::process(void)
{
	IMatrix* l_pInputMatrix=ip_pMatrix;
	IMatrix* l_pOutputMatrix=op_pAveragedMatrix;

	uint32 l_ui32InputMatrixElementCount=l_pInputMatrix->getBufferElementCount();

	if(this->isInputTriggerActive(OVP_Algorithm_MatrixAverage_InputTriggerId_Reset))
	{
		if(ip_i64MatrixCount>0)
		{
			delete [] m_pMatrixCache;
			m_pMatrixCache=new CMatrix[(size_t)ip_i64MatrixCount];
			m_ui64MatrixCacheCount=ip_i64MatrixCount;
			m_bInfiniteAverage=false;
			m_bMovingAverage=ip_bMovingAverage;
			m_ui64MatrixCount=0;
			m_ui64MatrixIndex=0;
		}
		else
		{
			delete [] m_pMatrixCache;
			m_pMatrixCache=NULL;
			m_ui64MatrixCacheCount=0;
			m_bInfiniteAverage=true;
			m_bMovingAverage=false;
			m_ui64MatrixCount=0;
			m_ui64MatrixIndex=0;
		}

		OpenViBEToolkit::Tools::Matrix::copyDescription(*l_pOutputMatrix, *l_pInputMatrix);
		OpenViBEToolkit::Tools::Matrix::copyDescription(m_oSumMatrix, *l_pInputMatrix);
		for(size_t i=0; i<m_ui64MatrixCacheCount; i++)
			OpenViBEToolkit::Tools::Matrix::copyDescription(m_pMatrixCache[i], *l_pInputMatrix);

		OpenViBEToolkit::Tools::Matrix::clearContent(*l_pOutputMatrix);
		OpenViBEToolkit::Tools::Matrix::clearContent(m_oSumMatrix);
		for(size_t i=0; i<m_ui64MatrixCacheCount; i++)
			OpenViBEToolkit::Tools::Matrix::clearContent(m_pMatrixCache[i]);
	}

	if(this->isInputTriggerActive(OVP_Algorithm_MatrixAverage_InputTriggerId_FeedMatrix))
	{
		if(m_bInfiniteAverage)
		{
			float64* l_pSumMatrixBuffer=m_oSumMatrix.getBuffer();
			float64* l_pInputMatrixBuffer=l_pInputMatrix->getBuffer();

			uint32 j=l_ui32InputMatrixElementCount;
			while(j--)
			{
				*l_pSumMatrixBuffer+++=*l_pInputMatrixBuffer++;
			}

			m_ui64MatrixCount++;
		}
		else
		{
			m_ui64MatrixCount++;
			m_ui64MatrixCount=(m_ui64MatrixCount>m_ui64MatrixCacheCount?m_ui64MatrixCacheCount:m_ui64MatrixCount);
			m_ui64MatrixIndex++;
			m_ui64MatrixIndex%=m_ui64MatrixCacheCount;
			m_bShouldPerformAverage|=(m_bMovingAverage || m_ui64MatrixIndex==0);

			float64* l_pSumMatrixBuffer=m_oSumMatrix.getBuffer();
			float64* l_pMatrixCacheBuffer=m_pMatrixCache[m_ui64MatrixIndex].getBuffer();
			float64* l_pInputMatrixBuffer=l_pInputMatrix->getBuffer();

			uint32 j=l_ui32InputMatrixElementCount;
			while(j--)
			{
				*l_pSumMatrixBuffer-=*l_pMatrixCacheBuffer;
				*l_pSumMatrixBuffer+=*l_pInputMatrixBuffer;
				*l_pMatrixCacheBuffer=*l_pInputMatrixBuffer;

				l_pSumMatrixBuffer++;
				l_pMatrixCacheBuffer++;
				l_pInputMatrixBuffer++;
			}
		}
	}

	if(this->isInputTriggerActive(OVP_Algorithm_MatrixAverage_InputTriggerId_ForceAverage))
	{
		m_bShouldPerformAverage|=m_bInfiniteAverage;
	}

	if(m_bShouldPerformAverage)
	{
		if(m_ui64MatrixCount)
		{
			float64* l_pSumMatrixBuffer=m_oSumMatrix.getBuffer();
			float64* l_pOutputMatrixBuffer=l_pOutputMatrix->getBuffer();

			uint32 j=l_ui32InputMatrixElementCount;
			while(j--)
			{
				*l_pOutputMatrixBuffer++=*l_pSumMatrixBuffer++/m_ui64MatrixCount;
			}
		}
		else
		{
			OpenViBEToolkit::Tools::Matrix::clearContent(*l_pOutputMatrix);
		}

		m_bShouldPerformAverage=false;

		this->activateOutputTrigger(OVP_Algorithm_MatrixAverage_OutputTriggerId_AveragePerformed, true);
	}

	return true;
}
