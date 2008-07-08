#ifndef __OpenViBEPlugins_SimpleVisualisation_CElectrodePotentialsDatabase_H__
#define __OpenViBEPlugins_SimpleVisualisation_CElectrodePotentialsDatabase_H__

#include "ovp_defines.h"

#include "ovpCBufferDatabase.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

//#include <ebml/IReader.h>

#include <vector>
#include <deque>
#include <queue>
#include <string>
#include <cfloat>
#include <iostream>

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		class CTopographicMapDrawable : public CSignalDisplayDrawable
		{
		public:
			virtual ~CTopographicMapDrawable(){}
			virtual OpenViBE::CMatrix* getSampleCoordinatesMatrix() = 0;
			virtual OpenViBE::boolean setSampleValuesMatrix(OpenViBE::IMatrix* pSampleValuesMatrix) = 0;
		};

		/**
		* This class is used to store information about the incoming signal stream. It can request a CSignalDisplayDrawable
		* object to redraw himself in case of some changes in its data.
		*/
		class CTopographicMapDatabase : public CBufferDatabase
		{
		public:
			enum EInterpolationMode
			{
				InterpolationMode_Spline,
				InterpolationMode_Laplacian
			};

			CTopographicMapDatabase(
				OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>& oPlugin,
				OpenViBE::Kernel::IAlgorithmProxy& rProxy);
			~CTopographicMapDatabase();

			void setMatrixDimmensionSize(
				const OpenViBE::uint32 ui32DimmensionIndex,
				const OpenViBE::uint32 ui32DimmensionSize);

			OpenViBE::boolean setInterpolationMode(
				EInterpolationMode eMode);

			OpenViBE::boolean processValues();

			OpenViBE::boolean interpolateValues();

			//! Returns min/max interpolated values using the last buffer arrived (all channels taken into account)
			void getLastBufferInterpolatedMinMaxValue(
				OpenViBE::float64& f64Min,
				OpenViBE::float64& f64Max);

		private:
			//void computeInterpolatedMinMaxValues(OpenViBE::IMatrix* pValueMatrix);

			OpenViBE::boolean m_bFirstProcess;

			//spherical spline algorithm proxy
			OpenViBE::Kernel::IAlgorithmProxy& m_rProxy;
			//order of spherical spline used for interpolation - mapped to IN param (0,1)
			OpenViBE::int64 m_i64SplineOrder;
			//type of interpolation
			EInterpolationMode m_oInterpolationMode;
			//number of electrodes (see CBufferDatabase) - mapped to IN param (0,2)
			//OpenViBE::int64 m_i64NbElectrodes;
			//electrode cartesian coordinates stored in an INSERM algorithm compliant way
			OpenViBE::CMatrix m_oElectrodesCoordsINSERM;
			//pointer to electrode coordinates matrix - mapped to IN param (0,3)
			OpenViBE::IMatrix* m_pCoordsINSERMMatrix;
			//electrode potentials
			OpenViBE::CMatrix m_oElectrodesPotentials;
			//pointer to electrode potentials matrix - mapped to IN param (0,4)
			OpenViBE::IMatrix* m_pPotentialsMatrix;
			//pointer to samples matrix - mapped to IN param (0,10)
			OpenViBE::IMatrix* m_pSampleCoordinatesMatrix;
			//interpolated values' min/max
			//OpenViBE::float64 m_f64MinInterpolatedPotential, m_f64MaxInterpolatedPotential;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::float64 > m_oMinInterpolatedValue;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::float64 > m_oMaxInterpolatedValue;
		};
	}
}

#endif // __OpenViBEPlugins_SimpleVisualisation_CElectrodePotentialsDatabase_H__
