#include "ovpCTopographicMapDatabase.h"

#include <algorithm>

#include <math.h>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;
using namespace OpenViBE::Kernel;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;

using namespace OpenViBEToolkit;

using namespace std;

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
#ifdef ELAN_VALIDATION
		static const uint32 s_nbElanChannels = 143;

		static float64 s_elanChannelValues[s_nbElanChannels] = {
		0.836,
		1.255,
		1.686,
		1.985,
		1.960,
		1.857,
		0.319,
		0.613,
		1.073,
		1.537,
		1.671,
		1.639,
		1.468,
		1.232,
		0.306,
		0.687,
		1.071,
		1.360,
		1.295,
		1.024,
		0.612,
		0.493,
		0.870,
		0.463,
		0.158,
		0.230,
		0.268,
		0.304,
		0.291,
		0.382,
		0.111,
		0.225,
		0.325,
		0.495,
		0.671,
		0.580,
		0.189,
		0.312,
		0.612,
		0.992,
		1.029,
		0.146,
		0.319,
		0.494,
		1.110,
		1.628,
		0.500,
		0.563,
		0.549,
		0.400,
		0.260,
		0.828,
		1.007,
		1.314,
		1.152,
		0.827,
		0.429,
		1.405,
		1.686,
		2.042,
		1.666,
		0.995,
		2.255,
		2.317,
		2.136,
		1.631,
		0.698,
		0.008,
		-0.104,
		-0.228,
		-0.278,
		-0.160,
		0.072,
		0.034,
		0.082,
		0.068,
		-0.929,
		-1.447,
		-1.992,
		-2.336,
		-2.295,
		-2.237,
		-0.307,
		-0.658,
		-1.246,
		-1.864,
		-2.012,
		-2.001,
		-1.882,
		-1.570,
		-0.359,
		-0.852,
		-1.398,
		-1.714,
		-1.658,
		-1.435,
		-1.103,
		-0.842,
		-1.274,
		-0.981,
		-0.035,
		-0.123,
		-0.154,
		-0.155,
		-0.136,
		-0.218,
		-0.024,
		-0.159,
		-0.280,
		-0.429,
		-0.543,
		-0.428,
		-0.146,
		-0.291,
		-0.638,
		-1.024,
		-0.958,
		-0.112,
		-0.292,
		-0.498,
		-1.228,
		-1.765,
		-0.307,
		-0.342,
		-0.316,
		-0.212,
		-0.097,
		-0.648,
		-0.842,
		-1.103,
		-0.945,
		-0.633,
		-0.214,
		-1.397,
		-1.801,
		-2.080,
		-1.665,
		-0.927,
		-2.535,
		-2.638,
		-2.417,
		-1.871,
		-0.791
		};
#endif

		CTopographicMapDatabase::CTopographicMapDatabase(OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>& oPlugin, IAlgorithmProxy& rProxy)
			:CBufferDatabase(oPlugin)
			,m_bFirstProcess(true)
			,m_rProxy(rProxy)
			,m_i64SplineOrder(4)
			,m_ui64InterpolationType(OVP_TypeId_SphericalLinearInterpolationType_Spline)
			,m_pElectrodeCoords(NULL)
			,m_pElectrodePotentials(NULL)
			,m_pSamplePointCoords(NULL)
			,m_ui64Delay(0)
		{
			//map input parameters
			//--------------------

			//spline order
			m_rProxy.getInputParameter(OVP_Algorithm_SphericalSplineInterpolation_InputParameterId_SplineOrder)->setReferenceTarget(&m_i64SplineOrder);
			//number of channels (or electrodes)
			m_rProxy.getInputParameter(OVP_Algorithm_SphericalSplineInterpolation_InputParameterId_ControlPointsCount)->setReferenceTarget(&m_i64NbElectrodes);
			//matrix of pointers to electrode coordinates
			m_pElectrodeCoords = &m_oElectrodesCoords;
			m_rProxy.getInputParameter(OVP_Algorithm_SphericalSplineInterpolation_InputParameterId_ControlPointsCoordinates)->setReferenceTarget(&m_pElectrodeCoords);
			//matrix of potentials measured at each electrode
			m_pElectrodePotentials = &m_oElectrodePotentials;
			m_rProxy.getInputParameter(OVP_Algorithm_SphericalSplineInterpolation_InputParameterId_ControlPointsValues)->setReferenceTarget(&m_pElectrodePotentials);
			//matrix holding sample coordinates mapped at runtime (its size is not known a priori and may vary)
			//

			//map output parameters
			//---------------------
			m_oMinSamplePointValue.initialize(m_rProxy.getOutputParameter(OVP_Algorithm_SphericalSplineInterpolation_OutputParameterId_MinSamplePointValue));
			m_oMaxSamplePointValue.initialize(m_rProxy.getOutputParameter(OVP_Algorithm_SphericalSplineInterpolation_OutputParameterId_MaxSamplePointValue));
		}

		CTopographicMapDatabase::~CTopographicMapDatabase()
		{
		}

		void CTopographicMapDatabase::setMatrixDimmensionSize(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionSize)
		{
			CBufferDatabase::setMatrixDimmensionSize(ui32DimmensionIndex, ui32DimmensionSize);

			if(ui32DimmensionIndex == 0)
			{
				m_oElectrodePotentials.setDimensionCount(1);
				m_oElectrodePotentials.setDimensionSize(0, (uint32)m_i64NbElectrodes);				
			}
		}

		void CTopographicMapDatabase::getLastBufferInterpolatedMinMaxValue(OpenViBE::float64& f64Min, OpenViBE::float64& f64Max)
		{
			f64Min = m_oMinSamplePointValue;
			f64Max = m_oMaxSamplePointValue;
		}

		boolean CTopographicMapDatabase::processValues()
		{
			if(m_bFirstBufferReceived == false)
			{
				return false;
			}

			if(m_bFirstProcess == true)
			{
				//done in CBufferDatabase::setMatrixBuffer
				//initialize the drawable object
				//m_pDrawable->init();

				//precompute sin/cos tables
				m_rProxy.activateInputTrigger(OVP_Algorithm_SphericalSplineInterpolation_InputTriggerId_PrecomputeTables, true);

				m_bFirstProcess = false;
			}

			//retrieve electrode values
#ifdef ELAN_VALIDATION
			for(int i=0; i<m_i64NbElectrodes; i++)
			{
				*(m_oElectrodePotentials.getBuffer()+i) = s_elanChannelValues[i];
			}
#elif 1
			//determine what buffer to use from delay
			uint32 l_ui32BufferIndex;
			uint64 l_ui64CurrentTime = m_oParentPlugin.getPlayerContext().getCurrentTime();
			uint64 l_ui64DisplayTime = l_ui64CurrentTime - m_ui64Delay;
			getBufferIndexFromTime(l_ui64DisplayTime, l_ui32BufferIndex);

			//determine what sample to use
			uint64 l_ui64SampleIndex;
			if(l_ui64DisplayTime <= m_oStartTime[l_ui32BufferIndex])
			{
				l_ui64SampleIndex = 0;
			}
			else if(l_ui64DisplayTime >= m_oEndTime[l_ui32BufferIndex])
			{
				l_ui64SampleIndex = m_pDimmensionSizes[1] - 1;
			}
			else
			{
				l_ui64SampleIndex = (uint64)((float64)(l_ui64DisplayTime - m_oStartTime[l_ui32BufferIndex])/(float64)m_ui64BufferDuration * m_pDimmensionSizes[1]);
			}

			for(int i=0; i<m_i64NbElectrodes; i++)
			{
				*(m_oElectrodePotentials.getBuffer()+i) = m_oSampleBuffers[l_ui32BufferIndex][i*m_pDimmensionSizes[1] + l_ui64SampleIndex];
			}
#else
			//debug test : null potentials on all but F4 electrode
			for(int i=0; i<m_i64NbElectrodes; i++)
			{
				if(m_pDimmesionLabels[0][i].find("F4") != string::npos)
				{
					*(m_oElectrodePotentials.getBuffer()+i) = 1;
				}
				else
				{
					*(m_oElectrodePotentials.getBuffer()+i) = 0;
				}
			}
#endif

			//interpolate spline values (potentials)
			if(m_ui64InterpolationType == OVP_TypeId_SphericalLinearInterpolationType_Spline)
			{
				m_rProxy.activateInputTrigger(OVP_Algorithm_SphericalSplineInterpolation_InputTriggerId_ComputeSplineCoefs, true);
			}
			else //interpolate spline laplacian (currents)
			{
				m_rProxy.activateInputTrigger(OVP_Algorithm_SphericalSplineInterpolation_InputTriggerId_ComputeLaplacianCoefs, true);
			}

			//retrieve up-to-date pointer to sample matrix
			m_pSamplePointCoords = dynamic_cast<CTopographicMapDrawable*>(m_pDrawable)->getSampleCoordinatesMatrix();

			if(m_pSamplePointCoords != NULL)
			{
				//map pointer to input parameter
				m_rProxy.getInputParameter(OVP_Algorithm_SphericalSplineInterpolation_InputParameterId_SamplePointsCoordinates)->setReferenceTarget(&m_pSamplePointCoords);

				if(m_ui64InterpolationType == OVP_TypeId_SphericalLinearInterpolationType_Spline)
				{
					m_rProxy.activateInputTrigger(OVP_Algorithm_SphericalSplineInterpolation_InputTriggerId_InterpolateSpline, true);
				}
				else
				{
					m_rProxy.activateInputTrigger(OVP_Algorithm_SphericalSplineInterpolation_InputTriggerId_InterpolateLaplacian, true);
				}
			}

			m_rProxy.process();

			if(m_pSamplePointCoords != NULL)
			{
				//retrieve interpolation results
				OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > l_oSampleValuesMatrix;
				l_oSampleValuesMatrix.initialize(m_rProxy.getOutputParameter(OVP_Algorithm_SphericalSplineInterpolation_OutputParameterId_SamplePointsValues));
				dynamic_cast<CTopographicMapDrawable*>(m_pDrawable)->setSampleValuesMatrix(l_oSampleValuesMatrix);
			}

			//tells the drawable to redraw itself since the signal information has been updated
			m_pDrawable->redraw();

			return true;
		}

		/*
		boolean CTopographicMapDatabase::setElectrodePosition(const OpenViBE::uint32 ui32ChannelIndex, float64* pElectrodePosition)
		{
			if(ui32ChannelIndex >= m_pElectrodeLabels.size())
			{
				setChannelCount(ui32ChannelIndex);
			}

			memcpy(m_oElectrodesCoords.getBuffer() + 3*ui32ChannelIndex, pElectrodePosition, 3*sizeof(float64));

			return true;
		}*/

		boolean CTopographicMapDatabase::setDelay(OpenViBE::float64 f64Delay)
		{
			if(f64Delay > m_f64TotalDuration)
			{
				return false;
			}

			//convert delay to 32:32 format
			m_ui64Delay = (int64)(f64Delay*(1LL<<32)); // $$$ Casted in (int64) because of Ubuntu 7.10 crash !
			return true;
		}

		boolean CTopographicMapDatabase::setInterpolationType(OpenViBE::uint64 ui64InterpolationType)
		{
			m_ui64InterpolationType = ui64InterpolationType;
			return true;
		}

		boolean CTopographicMapDatabase::interpolateValues()
		{			
			//can't interpolate before first buffer has been received
			if(m_bFirstProcess == true)
			{
				return false;
			}

			//retrieve up-to-date pointer to sample matrix
			m_pSamplePointCoords = dynamic_cast<CTopographicMapDrawable*>(m_pDrawable)->getSampleCoordinatesMatrix();

			if(m_pSamplePointCoords != NULL)
			{
				//map pointer to input parameter
				m_rProxy.getInputParameter(OVP_Algorithm_SphericalSplineInterpolation_InputParameterId_SamplePointsCoordinates)->setReferenceTarget(&m_pSamplePointCoords);

				//interpolate using spline or laplacian coefficients depending on interpolation mode
				if(m_ui64InterpolationType == OVP_TypeId_SphericalLinearInterpolationType_Spline)
				{
					m_rProxy.activateInputTrigger(OVP_Algorithm_SphericalSplineInterpolation_InputTriggerId_InterpolateSpline, true);
				}
				else
				{
					m_rProxy.activateInputTrigger(OVP_Algorithm_SphericalSplineInterpolation_InputTriggerId_InterpolateLaplacian, true);
				}
			}

			m_rProxy.process();

			if(m_pSamplePointCoords != NULL)
			{
				//retrieve interpolation results
				OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > l_oSampleValuesMatrix;
				l_oSampleValuesMatrix.initialize(m_rProxy.getOutputParameter(OVP_Algorithm_SphericalSplineInterpolation_OutputParameterId_SamplePointsValues));
				dynamic_cast<CTopographicMapDrawable*>(m_pDrawable)->setSampleValuesMatrix(l_oSampleValuesMatrix);
			}

			return true;
		}
/*
		OpenViBE::uint32 CTopographicMapDatabase::getChannelCount()
		{
			return m_pElectrodeLabels.size();
		}

		OpenViBE::boolean CTopographicMapDatabase::getChannelName(const OpenViBE::uint32 ui32ChannelIndex, std::string& sChannelName)
		{
			if(ui32ChannelIndex >= m_pElectrodeLabels.size())
			{
				return false;
			}

			sChannelName = m_pElectrodeLabels[ui32ChannelIndex];
			return true;
		}
*/
		boolean CTopographicMapDatabase::getBufferIndexFromTime(uint64 ui64Time, uint32& rBufferIndex)
		{
			if(m_oSampleBuffers.size() == 0)
			{
				return false;
			}

			if(ui64Time < m_oStartTime[0])
			{
				rBufferIndex = 0;
				return false;
			}
			else if(ui64Time > m_oEndTime.back())
			{
				rBufferIndex = m_oSampleBuffers.size() - 1;
				return false;
			}
			else
			{
				for(uint32 i=0; i<m_oSampleBuffers.size(); i++)
				{
					if(ui64Time <= m_oEndTime[i])
					{
						rBufferIndex = i;
						break;
					}
				}

				return true;
			}
		}
	};
};
