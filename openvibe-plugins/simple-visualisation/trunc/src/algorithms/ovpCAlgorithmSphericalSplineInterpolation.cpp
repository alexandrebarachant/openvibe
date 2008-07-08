#include "ovpCAlgorithmSphericalSplineInterpolation.h"

//INSERM lib
#include "spline_sph.h"

#include <float.h> //DBL_MAX

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Test;

boolean CAlgorithmSphericalSplineInterpolation::initialize(void)
{
	m_oSplineOrder.initialize(getInputParameter(CIdentifier(0,1)));
	m_oNbValues.initialize(getInputParameter(CIdentifier(0,2)));
	m_oCoords.initialize(getInputParameter(CIdentifier(0,3)));
	m_oValues.initialize(getInputParameter(CIdentifier(0,4)));
	m_oSampleCoords.initialize(getInputParameter(CIdentifier(0, 10)));

	m_pSplineCoefs = NULL;
	m_pLaplacianSplineCoefs = NULL;

	m_oSampleValues.initialize(getOutputParameter(CIdentifier(1, 0)));
	m_oSampleValues->setDimensionCount(1);
	m_oMinSampleValue.initialize(getOutputParameter(CIdentifier(1, 1)));
	m_oMaxSampleValue.initialize(getOutputParameter(CIdentifier(1, 2)));

	return true;
}

boolean CAlgorithmSphericalSplineInterpolation::uninitialize(void)
{
	m_oSplineOrder.uninitialize();

	if(m_pSplineCoefs != NULL)
		delete m_pSplineCoefs;

	if(m_pLaplacianSplineCoefs != NULL)
		delete m_pLaplacianSplineCoefs;

	return true;
}

boolean CAlgorithmSphericalSplineInterpolation::process(void)
{
	//do we want to precompute tables?
	if(isInputTriggerActive(OpenViBE::CIdentifier(0,5)))
	{
		//compute cos/sin values used in spline polynomias
		spline_tables((int)m_oSplineOrder, m_PotTable, m_ScdTable);
	}

	if(isInputTriggerActive(OpenViBE::CIdentifier(0,6)))
	{
		if(m_pSplineCoefs == NULL && (int)m_oNbValues != 0)
		{
			m_pSplineCoefs = new double[(int)m_oNbValues+1];
		}

		//compute spline ponderation coefficients using spline values
		spline_coef((int)m_oNbValues, (double**)m_oCoords->getBuffer(), (double*)m_oValues->getBuffer(), m_PotTable, m_pSplineCoefs);
	}

	if(isInputTriggerActive(OpenViBE::CIdentifier(0,7)))
	{
		if(m_pLaplacianSplineCoefs == NULL && (int)m_oNbValues != 0)
		{
			m_pLaplacianSplineCoefs = new double[(int)m_oNbValues+1];
		}

		//compute spline ponderation coefficients using spline values
		spline_coef((int)m_oNbValues, (double**)m_oCoords->getBuffer(), (double*)m_oValues->getBuffer(), m_PotTable, m_pLaplacianSplineCoefs);
		m_pLaplacianSplineCoefs[(int)m_oNbValues] = 0;
	}

	if(isInputTriggerActive(OpenViBE::CIdentifier(0,8)))
	{
		//ensure we got enough storage space for interpolated values
		if(m_oSampleValues->getDimensionSize(0) != m_oSampleCoords->getDimensionSize(0))
		{
			m_oSampleValues->setDimensionSize(0, m_oSampleCoords->getDimensionSize(0));
		}

		//compute interpolated values using spline
		double* l_pSampleValue = (double*)m_oSampleValues->getBuffer();

		m_oMinSampleValue = +DBL_MAX;
		m_oMaxSampleValue = -DBL_MAX;

		for(uint32 i=0; i<m_oSampleCoords->getDimensionSize(0); i++, l_pSampleValue++)
		{
			*l_pSampleValue = spline_interp(
				(int)m_oNbValues, //number of fixed values
				(double**)m_oCoords->getBuffer(), //coordinates of fixed values
				m_PotTable, //sin/cos table for spline
				m_pSplineCoefs, //spline coefficients
				*(m_oSampleCoords->getBuffer()+3*i),
				*(m_oSampleCoords->getBuffer()+3*i+1),
				*(m_oSampleCoords->getBuffer()+3*i+2) //coordinate where to interpolate
				);

			if(*l_pSampleValue < m_oMinSampleValue)
			{
				m_oMinSampleValue = *l_pSampleValue;
			}
			if(*l_pSampleValue > m_oMaxSampleValue)
			{
				m_oMaxSampleValue = *l_pSampleValue;
			}
		}
	}
	else if(isInputTriggerActive(OpenViBE::CIdentifier(0,9)))
	{
		//ensure we got enough storage space for interpolated values
		if(m_oSampleValues->getDimensionSize(0) != m_oSampleCoords->getDimensionSize(0))
		{
			m_oSampleValues->setDimensionSize(0, m_oSampleCoords->getDimensionSize(0));
		}

		//compute interpolated values using spline
		double* l_pSampleValue = (double*)m_oSampleValues->getBuffer();

		m_oMinSampleValue = +DBL_MAX;
		m_oMaxSampleValue = -DBL_MAX;

		for(uint32 i=0; i<m_oSampleCoords->getDimensionSize(0); i++, l_pSampleValue++)
		{
			*l_pSampleValue = spline_interp(
				(int)m_oNbValues, //number of fixed values
				(double**)m_oCoords->getBuffer(), //coordinates of fixed values
				m_ScdTable, //sin/cos table for laplacian
				m_pLaplacianSplineCoefs, //laplacian coefficients
				*(m_oSampleCoords->getBuffer()+3*i),
				*(m_oSampleCoords->getBuffer()+3*i+1),
				*(m_oSampleCoords->getBuffer()+3*i+2) //coordinate where to interpolate
				);

			/***************************************************************************/
			/*** Units : potential values being very often expressed as micro-Volts  ***/
			/***         SCD values should be multiplied by a scaling factor         ***/
			/***         to get nano-Amperes/m3                                      ***/
			/***         Since the output of spline_interp corresponds to the        ***/
			/***         Laplacian operator only, SCD are obtained with a scaling    ***/
			/***         factor equal to 10-3 * sigma / (R*R)                        ***/
			/***         with sigma = conductivity of the scalp    = 0.45 Siemens/m  ***/
			/***         and  R     = radius of the spherical head = 0.09 m          ***/
			/***************************************************************************/
			*l_pSampleValue = *l_pSampleValue * (0.001 * 0.45 / 0.09 / 0.09);

			if(*l_pSampleValue < m_oMinSampleValue)
			{
				m_oMinSampleValue = *l_pSampleValue;
			}
			if(*l_pSampleValue > m_oMaxSampleValue)
			{
				m_oMaxSampleValue = *l_pSampleValue;
			}
		}
	}

	return true;
}
