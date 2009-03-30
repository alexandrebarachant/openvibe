#ifndef __OpenViBEPlugins_BoxAlgorithm_ClassifierTrainer_H__
#define __OpenViBEPlugins_BoxAlgorithm_ClassifierTrainer_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include "ovpCBoxAlgorithmCommonClassifierListener.inl"

#include <map>

#define OVP_ClassId_BoxAlgorithm_ClassifierTrainer     OpenViBE::CIdentifier(0x128703B1, 0x0E2441F5)
#define OVP_ClassId_BoxAlgorithm_ClassifierTrainerDesc OpenViBE::CIdentifier(0x0A0A3F0A, 0x17C8569E)

namespace OpenViBEPlugins
{
	namespace Classification
	{
		class CBoxAlgorithmClassifierTrainer : virtual public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_ClassifierTrainer);

		protected:

			OpenViBE::Kernel::IAlgorithmProxy* m_pFeaturesDecoder;
			OpenViBE::Kernel::IAlgorithmProxy* m_pLabelsDecoder;
			OpenViBE::Kernel::IAlgorithmProxy* m_pClassifierTrainer;
			OpenViBE::uint64 m_ui64StimulationEndTime;
			OpenViBE::uint64 m_ui64TrainStimulation;
			std::map < OpenViBE::uint64, OpenViBE::float64 > m_vClass;

			typedef struct
			{
				OpenViBE::CMatrix* m_pFeatureVectorMatrix;
				OpenViBE::uint64 m_ui64StartTime;
				OpenViBE::uint64 m_ui64EndTime;
			} SFeatureVector;

			std::vector < CBoxAlgorithmClassifierTrainer::SFeatureVector > m_vFeatureVector;
			OpenViBE::CStimulationSet m_oStimulationSet;
		};

		class CBoxAlgorithmClassifierTrainerDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Classifier trainer"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Generic classification, relying on several box algorithms"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Classification"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-apply"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_ClassifierTrainer; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Classification::CBoxAlgorithmClassifierTrainer; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("Features",                          OV_TypeId_FeatureVector);
				rBoxAlgorithmPrototype.addInput  ("Labels",                            OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addSetting("Classifier trainer to use",         OVTK_TypeId_ClassificationTrainerAlgorithm, "");
				rBoxAlgorithmPrototype.addSetting("Filename to save configuration to", OV_TypeId_Filename,    "");
				rBoxAlgorithmPrototype.addSetting("Train trigger",                     OV_TypeId_Stimulation, "OVTK_StimulationId_Train");
				rBoxAlgorithmPrototype.addSetting("Class 1 label",                     OV_TypeId_Stimulation, "OVTK_StimulationId_Label_01");
				rBoxAlgorithmPrototype.addSetting("Class 2 label",                     OV_TypeId_Stimulation, "OVTK_StimulationId_Label_02");
				return true;
			}

			virtual OpenViBE::Plugins::IBoxListener* createBoxListener(void) const { return new CBoxAlgorithmCommonClassifierListener(5); }
			virtual void releaseBoxListener(OpenViBE::Plugins::IBoxListener* pBoxListener) { delete pBoxListener; }

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_ClassifierTrainerDesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_ClassifierTrainer_H__
