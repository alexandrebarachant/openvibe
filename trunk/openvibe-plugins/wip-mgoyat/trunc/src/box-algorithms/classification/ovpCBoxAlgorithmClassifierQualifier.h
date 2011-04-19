#ifndef __OpenViBEPlugins_BoxAlgorithm_ClassifierQualifier_H__
#define __OpenViBEPlugins_BoxAlgorithm_ClassifierQualifier_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <string>

#define OVP_ClassId_BoxAlgorithm_ClassifierQualifier     OpenViBE::CIdentifier(0x777E5A51, 0x271165D6)
#define OVP_ClassId_BoxAlgorithm_ClassifierQualifierDesc OpenViBE::CIdentifier(0x6C6C3A2C, 0x6DD777C4)

namespace OpenViBEPlugins
{
	namespace Classification
	{
		class CBoxAlgorithmClassifierQualifier : virtual public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_ClassifierQualifier);

		protected:

			OpenViBE::Kernel::IAlgorithmProxy* m_pRowDecoder;
			OpenViBE::Kernel::IAlgorithmProxy* m_pColumnDecoder;
			OpenViBE::Kernel::IAlgorithmProxy* m_pTargetDecoder;

			OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pRowDecoderMemoryBuffer;
			OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pColumnDecoderMemoryBuffer;
			OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pTargetDecoderMemoryBuffer;
			//OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > op_pStimulationDecoded;
			
			OpenViBE::uint64 m_ui64LastTargetRow;
			OpenViBE::uint64 m_ui64LastTargetColumn;
			
			OpenViBE::uint64 m_ui64VraiPositif;
			OpenViBE::uint64 m_ui64FauxPositif;
			OpenViBE::uint64 m_ui64VraiNegatif;
			OpenViBE::uint64 m_ui64FauxNegatif;
			
			OpenViBE::boolean m_bStimulationNotEmpty;
			
			void saveClassifierQualification();
			std::string float2string(float f);
		};

		class CBoxAlgorithmClassifierQualifierDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Classifier qualifier"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Matthieu Goyat"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("Gipsa-lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("give caracteristics of a specific classifier"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Classification"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-apply"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_ClassifierQualifier; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Classification::CBoxAlgorithmClassifierQualifier; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput ("Classification Label Row1",      OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addInput ("Classification Label Row2",      OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addInput ("Classification Label Row3",      OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addInput ("Classification Label Row4",      OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addInput ("Classification Label Row5",      OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addInput ("Classification Label Row6",      OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addInput ("Classification Label Column1",   OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addInput ("Classification Label Column2",   OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addInput ("Classification Label Column3",   OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addInput ("Classification Label Column4",   OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addInput ("Classification Label Column5",   OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addInput ("Classification Label Column6",   OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addInput ("Targets",   OV_TypeId_Stimulations);
				//rBoxAlgorithmPrototype.addSetting("Filename to load configuration from", OV_TypeId_Filename,    "");
				//rBoxAlgorithmPrototype.addSetting("Reject class label",                  OV_TypeId_Stimulation, "OVTK_StimulationId_Label_00");
				//rBoxAlgorithmPrototype.addSetting("Class 1 label",                       OV_TypeId_Stimulation, "OVTK_StimulationId_Label_01");
				//rBoxAlgorithmPrototype.addSetting("Class 2 label",                       OV_TypeId_Stimulation, "OVTK_StimulationId_Label_02");
				return true;
			}

			// virtual OpenViBE::Plugins::IBoxListener* createBoxListener(void) const { return new CBoxAlgorithmCommonClassifierListener(5); }
			virtual void releaseBoxListener(OpenViBE::Plugins::IBoxListener* pBoxListener) { delete pBoxListener; }

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_ClassifierQualifierDesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_ClassifierQualifier_H__
