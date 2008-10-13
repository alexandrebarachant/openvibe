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
			CTopographicMapDatabase(
				OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>& oPlugin,
				OpenViBE::Kernel::IAlgorithmProxy& rProxy);
			~CTopographicMapDatabase();

			void setMatrixDimmensionSize(
				const OpenViBE::uint32 ui32DimmensionIndex,
				const OpenViBE::uint32 ui32DimmensionSize);

			OpenViBE::boolean setDelay(OpenViBE::float64 f64Delay);

			/** 
			 * \brief Set interpolation type
			 * Spline values (potentials) can be interpolated directly, but the spline laplacian (currents) may
			 * be used as well
			 * \sa OVP_TypeId_SphericalLinearInterpolationType enumeration
			 * \return True if interpolation type could be set, false otherwise
			 */
			OpenViBE::boolean setInterpolationType(
				OpenViBE::uint64 ui64InterpolationType);

			OpenViBE::boolean processValues();

			OpenViBE::boolean interpolateValues();

			//! Returns min/max interpolated values using the last buffer arrived (all channels taken into account)
			void getLastBufferInterpolatedMinMaxValue(
				OpenViBE::float64& f64Min,
				OpenViBE::float64& f64Max);

		private:
			/**
			 * \brief Looks for buffer whose timeframe contains time passed as parameter
			 * \param ui64Time [in] Time of buffer to be retrieved
			 * \param rBufferIndex [out] Index of buffer closest to time passed as parameter
			 * \return True if time passed as parameter lies within a buffer's timeframe, false otherwise
			 */
			OpenViBE::boolean getBufferIndexFromTime(
				OpenViBE::uint64 ui64Time, 
				OpenViBE::uint32& rBufferIndex);

		private:
			//true until process() is called for the first time
			OpenViBE::boolean m_bFirstProcess;
			//spherical spline algorithm proxy
			OpenViBE::Kernel::IAlgorithmProxy& m_rProxy;
			//order of spherical spline used for interpolation - mapped to IN param (0,1)
			OpenViBE::int64 m_i64SplineOrder;
			/** 
			 * \brief Type of interpolation 
			 * \sa OVP_TypeId_SphericalLinearInterpolationType enumeration
			 */
			OpenViBE::uint64 m_ui64InterpolationType; 
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
			OpenViBE::Kernel::TParameterHandler < OpenViBE::float64 > m_oMinInterpolatedValue;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::float64 > m_oMaxInterpolatedValue;
			//delay to apply to interpolated values
			OpenViBE::uint64 m_ui64Delay;
		};
	}
}

#endif // __OpenViBEPlugins_SimpleVisualisation_CElectrodePotentialsDatabase_H__
