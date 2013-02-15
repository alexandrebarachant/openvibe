
#if defined(TARGET_HAS_ThirdPartyEIGEN)

#ifndef __OpenViBEPlugins_Algorithm_ARBurgMethod_H__
#define __OpenViBEPlugins_Algorithm_ARBurgMethod_H__
 
#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <Eigen/Dense> 

namespace OpenViBEPlugins
{
        namespace SignalProcessingGpl
        {
                class CAlgorithmARBurgMethod : public OpenViBEToolkit::TAlgorithm < OpenViBE::Plugins::IAlgorithm >
                {
                public:
 
                        virtual void release(void) { delete this; }
 
                        virtual OpenViBE::boolean initialize(void);
                        virtual OpenViBE::boolean uninitialize(void);
                        virtual OpenViBE::boolean process(void);
 
                        _IsDerivedFromClass_Final_(OpenViBEToolkit::TAlgorithm < OpenViBE::Plugins::IAlgorithm >, OVP_ClassId_Algorithm_ARBurgMethod);
 
                protected:
 
                        OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > ip_pMatrix; // input matrix
                        OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > op_pMatrix; // output matrix
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint32 > ip_ui32Order;

		private:

			Eigen::VectorXd m_vecXdErrForward; // Error Forward		
			Eigen::VectorXd m_vecXdErrBackward; //Error Backward
			Eigen::VectorXd m_vecXdARCoefs; // AutoRegressive Coefficents
			
			Eigen::VectorXd m_vecXdErrForwardPrediction; // Error Forward prediction
			Eigen::VectorXd m_vecXdErrBackwardPrediction; //Error Backward prediction

			Eigen::VectorXd m_vecXdError; // Total error vector

			OpenViBE::float64 m_f64K; 
			OpenViBE::uint32 m_ui32Order;

			
                };
 
                class CAlgorithmARBurgMethodDesc : public OpenViBE::Plugins::IAlgorithmDesc
                {
                public:
 
                        virtual void release(void) { }
 
                        virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("AR Burg's Method algorithm"); }
                        virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Alison Cellard"); }
                        virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA"); }
                        virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Extract AR coefficient using Burg's Method"); }
                        virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
                        virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Signal Processing"); }
                        virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
                        virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-execute"); }
 
                        virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_Algorithm_ARBurgMethod; }
                        virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SignalProcessingGpl::CAlgorithmARBurgMethod; }
 
                        virtual OpenViBE::boolean getAlgorithmPrototype(
                                OpenViBE::Kernel::IAlgorithmProto& rAlgorithmPrototype) const
                        {
                                rAlgorithmPrototype.addInputParameter (OVP_Algorithm_ARBurgMethod_InputParameterId_Matrix,     "Vector", OpenViBE::Kernel::ParameterType_Matrix);
                                rAlgorithmPrototype.addOutputParameter(OVP_Algorithm_ARBurgMethod_OutputParameterId_Matrix,    "Coefficents Vector", OpenViBE::Kernel::ParameterType_Matrix);
				
				rAlgorithmPrototype.addInputParameter (OVP_Algorithm_ARBurgMethod_InputParameterId_UInteger,     "Order", OpenViBE::Kernel::ParameterType_UInteger);
 
                                rAlgorithmPrototype.addInputTrigger   (OVP_Algorithm_ARBurgMethod_InputTriggerId_Initialize,   "Initialize");
                                rAlgorithmPrototype.addInputTrigger   (OVP_Algorithm_ARBurgMethod_InputTriggerId_Process,      "Process");
                                rAlgorithmPrototype.addOutputTrigger  (OVP_Algorithm_ARBurgMethod_OutputTriggerId_ProcessDone, "Process done");
 
                                return true;
                        }
 
                        _IsDerivedFromClass_Final_(OpenViBE::Plugins::IAlgorithmDesc, OVP_ClassId_Algorithm_ARBurgMethodDesc);
                };
        };
};

#endif // __OpenViBEPlugins_Algorithm_ARBurgMethod_H__

#endif // TARGET_HAS_ThirdPartyEIGEN
 


