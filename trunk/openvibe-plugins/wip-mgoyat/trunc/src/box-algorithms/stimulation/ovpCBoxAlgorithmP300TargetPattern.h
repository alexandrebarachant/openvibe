#ifndef __OpenViBEPlugins_BoxAlgorithm_P300TargetPattern_H__
#define __OpenViBEPlugins_BoxAlgorithm_P300TargetPattern_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <vector>

#define OVP_ClassId_BoxAlgorithm_P300TargetPattern  		   		OpenViBE::CIdentifier(0x6C112D09, 0x3147450F)
#define OVP_ClassId_BoxAlgorithm_P300TargetPatternDesc 				OpenViBE::CIdentifier(0x066071DE, 0x70C500E1)

namespace OpenViBEPlugins
{
	namespace Stimulation
	{
		class CBoxAlgorithmP300TargetPattern : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::uint64 getClockFrequency(void);
			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32Index);
			virtual OpenViBE::boolean processClock(OpenViBE::CMessageClock& rMessageClock);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_P300TargetPattern);

		protected:

			OpenViBE::uint64 m_ui64FlashCountRow;
			OpenViBE::uint64 m_ui64FlashCountColumn;
			OpenViBE::uint64 m_ui64TrialCount;
			OpenViBE::uint64 m_ui64BlocCount;

		private:

			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationEncoder;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > ip_pStimulationToEncode;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pEncodedMemoryBuffer;
			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationDecoder;

			OpenViBE::boolean m_bHeaderSent;
			OpenViBE::boolean m_bStartReceived;

		private:
			std::vector<int> generateTarget();

		};

		class CBoxAlgorithmP300TargetPatternDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("P300 Target Pattern"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Matthieu Goyat"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("Gipsa-lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Stimulation"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-select-font"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_P300TargetPattern; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Stimulation::CBoxAlgorithmP300TargetPattern; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("Incoming stimulations",           OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addOutput ("Produced stimulations",           OV_TypeId_Stimulations);

				rBoxAlgorithmPrototype.addSetting("Number of row",           		OV_TypeId_Integer,     "6");
				rBoxAlgorithmPrototype.addSetting("Number of column",           	OV_TypeId_Integer,     "6");
				rBoxAlgorithmPrototype.addSetting("Number of trials",               OV_TypeId_Integer,     "10");
				rBoxAlgorithmPrototype.addSetting("Number of blocs",                OV_TypeId_Integer,     "1");
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_P300TargetPatternDesc);
		};
	};
};

#endif
