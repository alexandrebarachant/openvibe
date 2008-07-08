#ifndef __SamplePlugin_CAlgorithmSphericalSplineInterpolation_H__
#define __SamplePlugin_CAlgorithmSphericalSplineInterpolation_H__

#include "../ovp_defines.h"

#include <openvibe/ov_all.h>

#include <openvibe-toolkit/ovtk_all.h>

namespace OpenViBEPlugins
{
	namespace Test
	{
		class CAlgorithmSphericalSplineInterpolation : public OpenViBEToolkit::TAlgorithm < OpenViBE::Plugins::IAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TAlgorithm < OpenViBE::Plugins::IAlgorithm >, OVP_ClassId_AlgorithmSphericalSplineInterpolation);

		protected:

			//input parameters
			//----------------
			//step 1 : sin/cos computation
			OpenViBE::Kernel::TParameterHandler < OpenViBE::int64 > m_oSplineOrder;
			//step 2 : spline/laplacian coefficients computation
			OpenViBE::Kernel::TParameterHandler < OpenViBE::int64 > m_oNbValues;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > m_oCoords; //1 dimensional
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > m_oValues; //1 dimensional
			//step 3 : interpolation
			//filled from box algorithm
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > m_oSampleCoords; //2 dimensional

			//output parameters
			//-----------------
			//step 3 : interpolated values
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > m_oSampleValues; //1 dimensional
			OpenViBE::Kernel::TParameterHandler < OpenViBE::float64 > m_oMinSampleValue;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::float64 > m_oMaxSampleValue;

			//internal data
			//-------------
			double m_ScdTable[2004];
			double m_PotTable[2004];
			double* m_pSplineCoefs;
			double* m_pLaplacianSplineCoefs;
		};

		class CAlgorithmSphericalSplineInterpolationDesc : public OpenViBE::Plugins::IAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Spherical spline interpolation"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Vincent Delannoy"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Interpolates potentials/laplacians using a spherical spline"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Algorithm/Signal processing"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_AlgorithmSphericalSplineInterpolation; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Test::CAlgorithmSphericalSplineInterpolation(); }

			virtual OpenViBE::boolean getAlgorithmPrototype(
				OpenViBE::Kernel::IAlgorithmProto& rAlgorithmPrototype) const
			{
				//input
				rAlgorithmPrototype.addInputParameter (OpenViBE::CIdentifier(0,1), "Spline order",  OpenViBE::Kernel::ParameterType_Integer);
				rAlgorithmPrototype.addInputParameter (OpenViBE::CIdentifier(0,2), "Number of values",  OpenViBE::Kernel::ParameterType_Integer);
				rAlgorithmPrototype.addInputParameter (OpenViBE::CIdentifier(0,3), "Values coordinates",  OpenViBE::Kernel::ParameterType_Matrix);
				rAlgorithmPrototype.addInputParameter (OpenViBE::CIdentifier(0,4), "Values",  OpenViBE::Kernel::ParameterType_Matrix);
				rAlgorithmPrototype.addInputParameter (OpenViBE::CIdentifier(0,10),"Coordinates where to interpolate values", OpenViBE::Kernel::ParameterType_Matrix);

				rAlgorithmPrototype.addInputTrigger   (OpenViBE::CIdentifier(0,5), OpenViBE::CString("Precomputation"));
				rAlgorithmPrototype.addInputTrigger   (OpenViBE::CIdentifier(0,6), OpenViBE::CString("Spline coefficients computation"));
				rAlgorithmPrototype.addInputTrigger   (OpenViBE::CIdentifier(0,7), OpenViBE::CString("Laplacian coefficients computation"));
				rAlgorithmPrototype.addInputTrigger   (OpenViBE::CIdentifier(0,8), OpenViBE::CString("Interpolation using spline coefficients"));
				rAlgorithmPrototype.addInputTrigger   (OpenViBE::CIdentifier(0,9), OpenViBE::CString("Interpolation using laplacian coefficients"));

				//output
				rAlgorithmPrototype.addOutputParameter(OpenViBE::CIdentifier(1,0), "Interpolated values", OpenViBE::Kernel::ParameterType_Matrix);
				rAlgorithmPrototype.addOutputParameter(OpenViBE::CIdentifier(1,1), "Min interpolated value", OpenViBE::Kernel::ParameterType_Float);
				rAlgorithmPrototype.addOutputParameter(OpenViBE::CIdentifier(1,2), "Max interpolated value", OpenViBE::Kernel::ParameterType_Float);

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IAlgorithmDesc, OVP_ClassId_AlgorithmSphericalSplineInterpolationDesc);
		};
	};
};

#endif // __SamplePlugin_CAlgorithmAddition_H__
