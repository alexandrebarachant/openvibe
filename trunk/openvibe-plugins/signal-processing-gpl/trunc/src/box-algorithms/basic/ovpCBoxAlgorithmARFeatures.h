#ifndef __OpenViBEPlugins_BoxAlgorithm_ARFeatures_H__
#define __OpenViBEPlugins_BoxAlgorithm_ARFeatures_H__

#if defined(TARGET_HAS_ThirdPartyEIGEN)

//You may have to change this path to match your folder organisation
#include "../../ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

// The unique identifiers for the box and its descriptor.
// Identifier are randomly chosen by the skeleton-generator.
#define OVP_ClassId_BoxAlgorithm_ARFeatures OpenViBE::CIdentifier(0xBAADC2F3, 0xB556A07B)
#define OVP_ClassId_BoxAlgorithm_ARFeaturesDesc OpenViBE::CIdentifier(0xBAADC2F3, 0xB556A07B)

namespace OpenViBEPlugins
{
	namespace SignalProcessingGpl
	{
		/**
		 * \class CBoxAlgorithmARFeatures
		 * \author Alison Cellard (Inria)
		 * \date Wed Nov 28 10:40:52 2012
		 * \brief The class CBoxAlgorithmARFeatures describes the box AR Features.
		 *
		 */
		class CBoxAlgorithmARFeatures : virtual public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:
			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
				
			// Process callbacks on new input received
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);

			
			virtual OpenViBE::boolean process(void);

			// As we do with any class in openvibe, we use the macro below 
			// to associate this box to an unique identifier. 
			// The inheritance information is also made available, 
			// as we provide the superclass OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_ARFeatures);

		protected:
			
			// Signal stream decoder
			OpenViBEToolkit::TSignalDecoder < CBoxAlgorithmARFeatures > m_oAlgo0_SignalDecoder;

			OpenViBE::Kernel::IAlgorithmProxy* m_pARBurgMethodAlgorithm;

			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > ip_pARBurgMethodAlgorithm_Matrix;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > op_pARBurgMethodAlgorithm_Matrix;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > ip_ui64ARBurgMethodAlgorithm_Order;
			
			// Feature vector stream encoder
			OpenViBEToolkit::TFeatureVectorEncoder < CBoxAlgorithmARFeatures > m_oAlgo1_FeatureVectorEncoder;

		};


		/**
		 * \class CBoxAlgorithmARFeaturesDesc
		 * \author Alison Cellard (Inria)
		 * \date Wed Nov 28 10:40:52 2012
		 * \brief Descriptor of the box AR Features.
		 *
		 */
		class CBoxAlgorithmARFeaturesDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("AR Features"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Alison Cellard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("Inria"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Extract AutoRegressive (AR) features from a set of EEG signals"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("Calculate AutoRegressive coefficients using Burg's method"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Signal processing"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-add"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_ARFeatures; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SignalProcessingGpl::CBoxAlgorithmARFeatures; }
		
			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput("EEG Signal",OV_TypeId_Signal);
				rBoxAlgorithmPrototype.addOutput("AR Features",OV_TypeId_FeatureVector);
				rBoxAlgorithmPrototype.addSetting("Order",OV_TypeId_Integer,"1");
		
				rBoxAlgorithmPrototype.addFlag(OpenViBE::Kernel::BoxFlag_IsUnstable);
				
				return true;
			}
			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_ARFeaturesDesc);
		};
	};
};

#endif  // TARGET_HAS_ThirdPartyEIGEN

#endif // __OpenViBEPlugins_BoxAlgorithm_ARFeatures_H__


