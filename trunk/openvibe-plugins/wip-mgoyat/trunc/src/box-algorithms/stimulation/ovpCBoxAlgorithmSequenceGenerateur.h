#ifndef __OpenViBEPlugins_BoxAlgorithm_SequenceGenerateur_H__
#define __OpenViBEPlugins_BoxAlgorithm_SequenceGenerateur_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <map>

#define OVP_ClassId_BoxAlgorithm_SequenceGenerateur     	OpenViBE::CIdentifier(0x42AA13B9, 0x61F3583F)
#define OVP_ClassId_BoxAlgorithm_SequenceGenerateurDesc 	OpenViBE::CIdentifier(0x22DF0923, 0x57DD2B54)

namespace OpenViBEPlugins
{
	namespace Stimulation
	{
		class CBoxAlgorithmSequenceGenerateur : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32Index);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_SequenceGenerateur);

		protected:

			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationEncoder;
			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationDecoder;
			
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > iop_pStimulationSet;
			OpenViBE::CStimulationSet m_cStimulationToWrite;

		private:

			std::fstream m_fileWords;
			std::fstream m_fileSequence;
			
			//
			void getIndexCharTab();
			std::map<OpenViBE::uint32, std::pair<OpenViBE::uint32,OpenViBE::uint32> > m_mapIndexTab;
			std::map<std::string, OpenViBE::uint32> m_mapCharTab;
			//
			void getCharList();
			std::vector<std::string> m_vectChar;
			//
			OpenViBE::boolean getNextTarget();
			OpenViBE::int32 m_ui32IndexCurrentChar;
			OpenViBE::uint32 m_ui32Row;
			OpenViBE::uint32 m_ui32Column;
		};

		class CBoxAlgorithmSequenceGenerateurDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Sequence generateur"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Matthieu GOYAT"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("Gipsa-lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Stimulation"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-select-font"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_SequenceGenerateur; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Stimulation::CBoxAlgorithmSequenceGenerateur; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("Incoming stimulations",           OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addOutput ("Produced stimulations",           OV_TypeId_Stimulations);
				//
				rBoxAlgorithmPrototype.addSetting("Words list",         			OV_TypeId_Filename,       "../");
				rBoxAlgorithmPrototype.addSetting("Index Correspondance list",      OV_TypeId_Filename,       "../");
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_SequenceGenerateurDesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_SequenceGenerateur_H__
