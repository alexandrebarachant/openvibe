#ifndef __OpenViBEPlugins_BoxAlgorithm_P300StimulateurPattern_H__
#define __OpenViBEPlugins_BoxAlgorithm_P300StimulateurPattern_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <vector>

#define OVP_ClassId_BoxAlgorithm_P300StimulateurPattern  		   		OpenViBE::CIdentifier(0x7C502E72, 0x460C6D6D)
#define OVP_ClassId_BoxAlgorithm_P300StimulateurPatternDesc 			OpenViBE::CIdentifier(0x042E52F3, 0x407A718D)

namespace OpenViBEPlugins
{
	namespace Stimulation
	{
		class CBoxAlgorithmP300StimulateurPattern : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::uint64 getClockFrequency(void);
			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32Index);
			virtual OpenViBE::boolean processClock(OpenViBE::CMessageClock& rMessageClock);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_P300StimulateurPattern);

		protected:

			OpenViBE::uint64 m_ui64FlashCountInRepetition;
			OpenViBE::uint64 m_ui64RepetitionCountInTrial;
			OpenViBE::uint64 m_ui64TrialCount;
			OpenViBE::uint64 m_ui64BlocCount;
			OpenViBE::boolean m_bSequenceRowThenColumn;

		private:

			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationEncoder;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > ip_pStimulationToEncode;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pEncodedMemoryBuffer;
			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationDecoder;
			
			
			OpenViBE::boolean m_bHeaderSent;
			OpenViBE::boolean m_bStartReceived;

			OpenViBE::uint64 m_ui64TrialStartTime;
			OpenViBE::uint64 m_ui64TrialIndex;

		private:
			std::vector<int> generateSequenceRepetition();

		};

		class CBoxAlgorithmP300StimulateurPatternDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("P300 Stimulateur Pattern"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Matthieu Goyat"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("Gipsa-lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Stimulation"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-select-font"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_P300StimulateurPattern; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Stimulation::CBoxAlgorithmP300StimulateurPattern; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("Incoming stimulations",           OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addOutput ("Produced Stimulations",           OV_TypeId_Stimulations);

				rBoxAlgorithmPrototype.addSetting("Number of flash",           		 OV_TypeId_Integer,     "12");
				rBoxAlgorithmPrototype.addSetting("Number of repetitions",           OV_TypeId_Integer,     "5");
				rBoxAlgorithmPrototype.addSetting("Number of trials",                OV_TypeId_Integer,     "10");
				rBoxAlgorithmPrototype.addSetting("Number of blocs",                 OV_TypeId_Integer,     "1");
				rBoxAlgorithmPrototype.addSetting("Sequence Row then column",        OV_TypeId_Boolean,     "true");
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_P300StimulateurPatternDesc);
		};
	};
};

#endif
