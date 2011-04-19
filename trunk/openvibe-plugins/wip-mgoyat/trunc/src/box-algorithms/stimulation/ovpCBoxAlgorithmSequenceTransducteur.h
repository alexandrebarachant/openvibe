#ifndef __OpenViBEPlugins_BoxAlgorithm_SequenceTransducteur_H__
#define __OpenViBEPlugins_BoxAlgorithm_SequenceTransducteur_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>
#include <iostream>
#include <fstream>
#include <queue>

#define OVP_ClassId_BoxAlgorithm_SequenceTransducteurDesc 				OpenViBE::CIdentifier(0x2C2B6CF8, 0x1F8563F7)
#define OVP_ClassId_BoxAlgorithm_SequenceTransducteur     			OpenViBE::CIdentifier(0x11822AAB, 0x028136FC)

namespace OpenViBEPlugins
{
	namespace Stimulation
	{
		class CBoxAlgorithmSequenceTransducteur : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32Index);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_SequenceTransducteur);

		protected:

			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationEncoder;
			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationDecoder;
			
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > iop_pStimulationSet;

		private:

			std::fstream m_fileWords;
			std::fstream m_fileSequence;
			
			void getSequenceHeader();
			OpenViBE::uint32 m_uiMode;
			OpenViBE::uint32 m_uiRow;
			OpenViBE::uint32 m_uiColumn;
			OpenViBE::uint32 m_uiRep;
			void getNextLine();
			// std::vector<OpenViBE::uint32> m_vectValue;
			std::queue<OpenViBE::uint32> m_queValue;
			OpenViBE::uint64 getStimulationId(OpenViBE::uint32);
			void generate_random_sequence();
		};

		class CBoxAlgorithmSequenceTransducteurDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Sequence transducteur"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Matthieu GOYAT"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("Gipsa-lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Stimulation"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-select-font"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_SequenceTransducteur; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Stimulation::CBoxAlgorithmSequenceTransducteur; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("Incoming stimulations",           OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addOutput ("Produced stimulations",           OV_TypeId_Stimulations);
				//
				rBoxAlgorithmPrototype.addSetting("Words list",         OV_TypeId_Filename,       "../");
				rBoxAlgorithmPrototype.addSetting("Sequence list",      OV_TypeId_Filename,       "../");
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_SequenceTransducteurDesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_SequenceTransducteur_H__
