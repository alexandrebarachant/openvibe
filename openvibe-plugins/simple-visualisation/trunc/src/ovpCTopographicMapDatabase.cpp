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
		CTopographicMapDatabase::CTopographicMapDatabase(OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>& oPlugin, IAlgorithmProxy& rProxy)
			:CBufferDatabase(oPlugin)
			//,m_oParentPlugin(oPlugin)
			,m_bFirstProcess(true)
			,m_rProxy(rProxy)
			,m_i64SplineOrder(4)
			,m_oInterpolationMode(InterpolationMode_Laplacian)
			,m_pCoordsINSERMMatrix(NULL)
			,m_pPotentialsMatrix(NULL)
			,m_pSampleCoordinatesMatrix(NULL)
/*
			m_f64MinPotential(+DBL_MAX),
			m_f64MaxPotential(-DBL_MAX),
*/
			//m_pBuffer(NULL)
		{
			//map input parameters
			//--------------------

			//spline order
			m_rProxy.getInputParameter(CIdentifier(0,1))->setReferenceTarget(&m_i64SplineOrder);
			//number of channels (or electrodes)
			m_rProxy.getInputParameter(CIdentifier(0,2))->setReferenceTarget(&m_i64NbElectrodes);
			//matrix of pointers to electrode coordinates
			m_pCoordsINSERMMatrix = &m_oElectrodesCoordsINSERM;
			m_rProxy.getInputParameter(CIdentifier(0,3))->setReferenceTarget(&m_pCoordsINSERMMatrix);
			//matrix of potentials measured at each electrode
			m_pPotentialsMatrix = &m_oElectrodesPotentials;
			m_rProxy.getInputParameter(CIdentifier(0,4))->setReferenceTarget(&m_pPotentialsMatrix);
			//matrix holding sample coordinates mapped at runtime (its size is not known a priori and may vary)
			//

			//map output parameters
			//---------------------
			m_oMinInterpolatedValue.initialize(m_rProxy.getOutputParameter(OpenViBE::CIdentifier(1, 1)));
			m_oMaxInterpolatedValue.initialize(m_rProxy.getOutputParameter(OpenViBE::CIdentifier(1, 2)));
		}

		CTopographicMapDatabase::~CTopographicMapDatabase()
		{
		}

		void CTopographicMapDatabase::setMatrixDimmensionSize(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionSize)
		{
			CBufferDatabase::setMatrixDimmensionSize(ui32DimmensionIndex, ui32DimmensionSize);

			if(ui32DimmensionIndex == 0)
			{
				m_oElectrodesPotentials.setDimensionCount(1);
				m_oElectrodesPotentials.setDimensionSize(0, (uint32)m_i64NbElectrodes);

				//resize and setup matrix of pointers to electrode coords matrix
				//--------------------------------------------------------------
				m_oElectrodesCoordsINSERM.setDimensionCount(1);
				//we only need room for ceil(m_i64NbElectrodes/2) elements because pointers, not doubles, will be stored here
				m_oElectrodesCoordsINSERM.setDimensionSize(0, (uint32)ceil(m_i64NbElectrodes/2.));

				for(int i=0; i<m_i64NbElectrodes; i++)
				{
					//handle matrix as if it were a matrix of double*
					void* l_pointer = m_oElectrodesCoords.getBuffer() + 3*i;
					memcpy((unsigned long*)m_oElectrodesCoordsINSERM.getBuffer() + i, &l_pointer, sizeof(unsigned long*));
				}
			}
		}

		void CTopographicMapDatabase::getLastBufferInterpolatedMinMaxValue(OpenViBE::float64& f64Min, OpenViBE::float64& f64Max)
		{
			f64Min = m_oMinInterpolatedValue;
			f64Max = m_oMaxInterpolatedValue;
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
				m_rProxy.activateInputTrigger(OpenViBE::CIdentifier(0, 5), true);

				m_bFirstProcess = false;
			}
#if 1
			//retrieve electrode values (here, values of last sample of last buffer)
			//TODO : use average values to smooth things out
			for(int i=0; i<m_i64NbElectrodes; i++)
			{
				float64 l_f64Value = 0;
				uint32 l_ui32SampleBuffersAveraged = 0;
				uint32 l_ui32SampleBuffersToAverage = 1;
				int32 l_i32CurrentSampleBuffer;
				//average over last buffers received
				for(l_i32CurrentSampleBuffer=m_oSampleBuffers.size()-1;
					l_i32CurrentSampleBuffer >= 0 && l_ui32SampleBuffersAveraged < l_ui32SampleBuffersToAverage;
					l_i32CurrentSampleBuffer--, l_ui32SampleBuffersAveraged++)
				{
					float64* l_f64SampleBuffer = m_oSampleBuffers[l_i32CurrentSampleBuffer];

					//average current buffer
					for(uint32 j=0; j<m_pDimmensionSizes[1]; j++)
					{
						l_f64Value += *(l_f64SampleBuffer + i*m_pDimmensionSizes[1] + j);
					}
				}
				l_f64Value /= (l_ui32SampleBuffersAveraged * m_pDimmensionSizes[1]);
				*(m_oElectrodesPotentials.getBuffer()+i) = l_f64Value;
				//*(m_oElectrodesPotentials.getBuffer()+i) = m_oSampleBuffers[m_oSampleBuffers.size()-1][(i+1)*m_pDimmensionSizes[1]-1];
			}
#else
			//debug test : null potentials on all but F4 electrode
			for(int i=0; i<m_i64NbElectrodes; i++)
			{
				if(m_pDimmesionLabels[0][i].find("F4") != string::npos)
				{
					*(m_oElectrodesPotentials.getBuffer()+i) = 1;//m_oSampleBuffers[m_oSampleBuffers.size()-1][(i+1)*m_pDimmensionSizes[1]-1];
				}
				else
				{
					*(m_oElectrodesPotentials.getBuffer()+i) = 0;
				}
			}
#endif

			/*
			//compute min/max potentials
			m_f64MinPotential = DBL_MAX;
			m_f64MaxPotential = -DBL_MAX;

			for(uint32 i=0; i<m_oElectrodesPotentials.getDimensionSize(0); i++)
			{
				//update min/max potentials
				if(*(m_oElectrodesPotentials.getBuffer() + i) < m_f64MinPotential)
					m_f64MinPotential = *(m_oElectrodesPotentials.getBuffer() + i);
				if(*(m_oElectrodesPotentials.getBuffer() + i) > m_f64MaxPotential)
					m_f64MaxPotential = *(m_oElectrodesPotentials.getBuffer() + i);
			}
			*/

			//compute spline or laplacian coefficients depending on interpolation mode
			if(m_oInterpolationMode == InterpolationMode_Spline)
			{
				m_rProxy.activateInputTrigger(OpenViBE::CIdentifier(0, 6), true);
			}
			else
			{
				m_rProxy.activateInputTrigger(OpenViBE::CIdentifier(0, 7), true);
			}

			//retrieve up-to-date pointer to sample matrix
			m_pSampleCoordinatesMatrix = dynamic_cast<CTopographicMapDrawable*>(m_pDrawable)->getSampleCoordinatesMatrix();

			if(m_pSampleCoordinatesMatrix != NULL)
			{
				//map pointer to input parameter
				m_rProxy.getInputParameter(CIdentifier(0,10))->setReferenceTarget(&m_pSampleCoordinatesMatrix);

				//interpolate using spline or laplacian coefficients depending on interpolation mode
				if(m_oInterpolationMode == InterpolationMode_Spline)
				{
					m_rProxy.activateInputTrigger(OpenViBE::CIdentifier(0, 8), true);
				}
				else
				{
					m_rProxy.activateInputTrigger(OpenViBE::CIdentifier(0, 9), true);
				}
			}

			m_rProxy.process();

			if(m_pSampleCoordinatesMatrix != NULL)
			{
				//retrieve interpolation results
				OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > l_oSampleValuesMatrix;
				l_oSampleValuesMatrix.initialize(m_rProxy.getOutputParameter(OpenViBE::CIdentifier(1, 0)));
				dynamic_cast<CTopographicMapDrawable*>(m_pDrawable)->setSampleValuesMatrix(l_oSampleValuesMatrix);
			}

			//tells the drawable to redraw himself since the signal information has been updated
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

		boolean CTopographicMapDatabase::setInterpolationMode(EInterpolationMode eMode)
		{
			m_oInterpolationMode = eMode;
			return true;
		}

		boolean CTopographicMapDatabase::interpolateValues()
		{
			//REMOVE ME
			return false;

			//can't interpolate before first buffer has been received
			if(m_bFirstProcess == true)
				return false;

			//retrieve up-to-date pointer to sample matrix
			m_pSampleCoordinatesMatrix = dynamic_cast<CTopographicMapDrawable*>(m_pDrawable)->getSampleCoordinatesMatrix();

			if(m_pSampleCoordinatesMatrix != NULL)
			{
				//map pointer to input parameter
				m_rProxy.getInputParameter(CIdentifier(0,10))->setReferenceTarget(&m_pSampleCoordinatesMatrix);

				//interpolate using spline or laplacian coefficients depending on interpolation mode
				if(m_oInterpolationMode == InterpolationMode_Spline)
				{
					m_rProxy.activateInputTrigger(OpenViBE::CIdentifier(0, 8), true);
				}
				else
				{
					m_rProxy.activateInputTrigger(OpenViBE::CIdentifier(0, 9), true);
				}
			}

			m_rProxy.process();

			if(m_pSampleCoordinatesMatrix != NULL)
			{
				//retrieve interpolation results
				OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > l_oSampleValuesMatrix;
				l_oSampleValuesMatrix.initialize(m_rProxy.getOutputParameter(OpenViBE::CIdentifier(1, 0)));
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

		void CTopographicMapDatabase::computeInterpolatedMinMaxValues(IMatrix* pValueMatrix)
		{
			m_f64MinInterpolatedPotential = +DBL_MAX;
			m_f64MaxInterpolatedPotential = -DBL_MAX;

			for(uint32 i=0; i<m_pSampleMatrix->getDimensionSize(0); i++)
			{
				//update min/max potentials
				if(*(pValueMatrix->getBuffer() + i) < m_f64MinInterpolatedPotential)
					m_f64MinInterpolatedPotential = *(pValueMatrix->getBuffer() + i);
				if(*(pValueMatrix->getBuffer() + i) > m_f64MaxInterpolatedPotential)
					m_f64MaxInterpolatedPotential = *(pValueMatrix->getBuffer() + i);
			}
		}*/
	};
};
