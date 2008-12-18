#include "ovpCAlgorithmSphericalSplineInterpolation.h"

//INSERM lib
#include "spline_sph.h"

#include <float.h> //DBL_MAX
#include <math.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Test;

boolean CAlgorithmSphericalSplineInterpolation::initialize(void)
{
	m_bFirstProcess = true;
	m_pDoubleCoords = NULL;
	m_pInsermCoords = NULL;
	m_pSplineCoefs = NULL;
	m_pLaplacianSplineCoefs = NULL;

	ip_i64SplineOrder.initialize(getInputParameter(OVP_Algorithm_SphericalSplineInterpolation_InputParameterId_SplineOrder));
	ip_i64ControlPointsCount.initialize(getInputParameter(OVP_Algorithm_SphericalSplineInterpolation_InputParameterId_ControlPointsCount));
	ip_pControlPointsCoords.initialize(getInputParameter(OVP_Algorithm_SphericalSplineInterpolation_InputParameterId_ControlPointsCoordinates));
	ip_pControlPointsValues.initialize(getInputParameter(OVP_Algorithm_SphericalSplineInterpolation_InputParameterId_ControlPointsValues));
	ip_pSamplePointsCoords.initialize(getInputParameter(OVP_Algorithm_SphericalSplineInterpolation_InputParameterId_SamplePointsCoordinates));

	op_pSamplePointsValues.initialize(getOutputParameter(OVP_Algorithm_SphericalSplineInterpolation_OutputParameterId_SamplePointsValues));
	op_pSamplePointsValues->setDimensionCount(1);
	op_f64MinSamplePointValue.initialize(getOutputParameter(OVP_Algorithm_SphericalSplineInterpolation_OutputParameterId_MinSamplePointValue));
	op_f64MaxSamplePointValue.initialize(getOutputParameter(OVP_Algorithm_SphericalSplineInterpolation_OutputParameterId_MaxSamplePointValue));

	return true;
}

boolean CAlgorithmSphericalSplineInterpolation::uninitialize(void)
{
	ip_i64SplineOrder.uninitialize();
	ip_i64ControlPointsCount.uninitialize();
	ip_pControlPointsCoords.uninitialize();
	ip_pControlPointsValues.uninitialize();
	ip_pSamplePointsCoords.uninitialize();

	op_pSamplePointsValues.uninitialize();
	op_f64MinSamplePointValue.uninitialize();
	op_f64MaxSamplePointValue.uninitialize();

	if(m_pDoubleCoords != NULL)
	{
		delete[] m_pDoubleCoords;
	}

	if(m_pInsermCoords != NULL)
	{
		delete[] m_pInsermCoords;
	}

	if(m_pSplineCoefs != NULL)
	{
		delete[] m_pSplineCoefs;
	}

	if(m_pLaplacianSplineCoefs != NULL)
	{
		delete[] m_pLaplacianSplineCoefs;
	}

	return true;
}

boolean CAlgorithmSphericalSplineInterpolation::process(void)
{
	if(m_bFirstProcess == true)
	{
		//store coords as doubles
		m_pDoubleCoords = new double[static_cast<size_t>(3*ip_i64ControlPointsCount)];
		//set up matrix of pointers to double coords matrix
		m_pInsermCoords = new double* [static_cast<size_t>(ip_i64ControlPointsCount)];
		//fill both matrices
		for(int i=0; i<ip_i64ControlPointsCount; i++)
		{
			m_pDoubleCoords[i*3] = (double)(*ip_pControlPointsCoords)[i*3];
			m_pDoubleCoords[i*3+1] = (double)(*ip_pControlPointsCoords)[i*3+1];
			m_pDoubleCoords[i*3+2] = (double)(*ip_pControlPointsCoords)[i*3+2];
			m_pInsermCoords[i] = m_pDoubleCoords + 3*i;
		}
		m_bFirstProcess = false;
	}

	//do we want to precompute tables?
	if(isInputTriggerActive(OVP_Algorithm_SphericalSplineInterpolation_InputTriggerId_PrecomputeTables))
	{
		//compute cos/sin values used in spline polynomias
		spline_tables((int)ip_i64SplineOrder, m_PotTable, m_ScdTable);
	}

	if(isInputTriggerActive(OVP_Algorithm_SphericalSplineInterpolation_InputTriggerId_ComputeSplineCoefs))
	{
		if(m_pSplineCoefs == NULL && (int)ip_i64ControlPointsCount != 0)
		{
			m_pSplineCoefs = new double[(int)ip_i64ControlPointsCount+1];
		}

		//compute spline ponderation coefficients using spline values
		//FIXME : have a working copy of control points values stored as doubles?
		spline_coef((int)ip_i64ControlPointsCount, m_pInsermCoords, (double*)ip_pControlPointsValues->getBuffer(), m_PotTable, m_pSplineCoefs);
	}

	if(isInputTriggerActive(OVP_Algorithm_SphericalSplineInterpolation_InputTriggerId_ComputeLaplacianCoefs))
	{
		if(m_pLaplacianSplineCoefs == NULL && (int)ip_i64ControlPointsCount != 0)
		{
			m_pLaplacianSplineCoefs = new double[(int)ip_i64ControlPointsCount+1];
		}

		//compute spline ponderation coefficients using spline values
		//FIXME : have a working copy of control points values stored as doubles?
		spline_coef((int)ip_i64ControlPointsCount, m_pInsermCoords, (double*)ip_pControlPointsValues->getBuffer(), m_PotTable, m_pLaplacianSplineCoefs);
		m_pLaplacianSplineCoefs[(int)ip_i64ControlPointsCount] = 0;
	}

	if(isInputTriggerActive(OVP_Algorithm_SphericalSplineInterpolation_InputTriggerId_InterpolateSpline))
	{
		//ensure we got enough storage space for interpolated values
		if(op_pSamplePointsValues->getDimensionSize(0) != ip_pSamplePointsCoords->getDimensionSize(0))
		{
			op_pSamplePointsValues->setDimensionSize(0, ip_pSamplePointsCoords->getDimensionSize(0));
		}

		//compute interpolated values using spline
		double* l_pSampleValue = (double*)op_pSamplePointsValues->getBuffer();

		op_f64MinSamplePointValue = +DBL_MAX;
		op_f64MaxSamplePointValue = -DBL_MAX;

		for(uint32 i=0; i<ip_pSamplePointsCoords->getDimensionSize(0); i++, l_pSampleValue++)
		{
#if defined OVP_OS_Windows
#ifndef NDEBUG
			if(_finite(*(ip_pSamplePointsCoords->getBuffer()+3*i)) == 0 ||
				_finite(*(ip_pSamplePointsCoords->getBuffer()+3*i+1)) == 0 ||
				_finite(*(ip_pSamplePointsCoords->getBuffer()+3*i+2)) == 0) //tests whether a double is infinite or a NaN
			{
				getLogManager() << LogLevel_ImportantWarning << "Bad interpolation point coordinates !\n";
				getLogManager() << LogLevel_ImportantWarning << *(ip_pSamplePointsCoords->getBuffer()+3*i) << "\n";
				getLogManager() << LogLevel_ImportantWarning << *(ip_pSamplePointsCoords->getBuffer()+3*i+1) << "\n";
				getLogManager() << LogLevel_ImportantWarning << *(ip_pSamplePointsCoords->getBuffer()+3*i+2) << "\n";
			}
#endif
#endif

			*l_pSampleValue = spline_interp(
				(int)ip_i64ControlPointsCount, //number of fixed values
				m_pInsermCoords, //coordinates of fixed values
				m_PotTable, //sin/cos table for spline
				m_pSplineCoefs, //spline coefficients
				*(ip_pSamplePointsCoords->getBuffer()+3*i),
				*(ip_pSamplePointsCoords->getBuffer()+3*i+1),
				*(ip_pSamplePointsCoords->getBuffer()+3*i+2) //coordinate where to interpolate
				);

#if defined OVP_OS_Windows
#ifndef NDEBUG
			if(_finite(*l_pSampleValue) == 0) //tests whether a double is infinite or a NaN
			{
				getLogManager() << LogLevel_ImportantWarning << "Interpolation fails !\n";
				getLogManager() << LogLevel_ImportantWarning << *(ip_pSamplePointsCoords->getBuffer()+3*i) << "\n";
				getLogManager() << LogLevel_ImportantWarning << *(ip_pSamplePointsCoords->getBuffer()+3*i+1) << "\n";
				getLogManager() << LogLevel_ImportantWarning << *(ip_pSamplePointsCoords->getBuffer()+3*i+2) << "\n";
			}
#endif
#endif

			if(*l_pSampleValue < op_f64MinSamplePointValue)
			{
				op_f64MinSamplePointValue = *l_pSampleValue;
			}
			if(*l_pSampleValue > op_f64MaxSamplePointValue)
			{
				op_f64MaxSamplePointValue = *l_pSampleValue;
			}
		}
	}
	else if(isInputTriggerActive(OVP_Algorithm_SphericalSplineInterpolation_InputTriggerId_InterpolateLaplacian))
	{
		//ensure we got enough storage space for interpolated values
		if(op_pSamplePointsValues->getDimensionSize(0) != ip_pSamplePointsCoords->getDimensionSize(0))
		{
			op_pSamplePointsValues->setDimensionSize(0, ip_pSamplePointsCoords->getDimensionSize(0));
		}

		//compute interpolated values using spline
		double* l_pSampleValue = (double*)op_pSamplePointsValues->getBuffer();

		op_f64MinSamplePointValue = +DBL_MAX;
		op_f64MaxSamplePointValue = -DBL_MAX;

		for(uint32 i=0; i<ip_pSamplePointsCoords->getDimensionSize(0); i++, l_pSampleValue++)
		{
			*l_pSampleValue = spline_interp(
				(int)ip_i64ControlPointsCount, //number of fixed values
				m_pInsermCoords, //coordinates of fixed values
				m_ScdTable, //sin/cos table for laplacian
				m_pLaplacianSplineCoefs, //laplacian coefficients
				*(ip_pSamplePointsCoords->getBuffer()+3*i),
				*(ip_pSamplePointsCoords->getBuffer()+3*i+1),
				*(ip_pSamplePointsCoords->getBuffer()+3*i+2) //coordinate where to interpolate
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

			if(*l_pSampleValue < op_f64MinSamplePointValue)
			{
				op_f64MinSamplePointValue = *l_pSampleValue;
			}
			if(*l_pSampleValue > op_f64MaxSamplePointValue)
			{
				op_f64MaxSamplePointValue = *l_pSampleValue;
			}
		}
	}

	return true;
}
