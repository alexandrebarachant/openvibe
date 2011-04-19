#ifndef __OpenViBEPlugins_BoxAlgorithm_P300SpellerSteadyStateStimulator_H__
#define __OpenViBEPlugins_BoxAlgorithm_P300SpellerSteadyStateStimulator_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <map>

#define OVP_ClassId_BoxAlgorithm_P300SpellerSteadyStateStimulator     OpenViBE::CIdentifier(0x070D43D5, 0x40123FCC)
#define OVP_ClassId_BoxAlgorithm_P300SpellerSteadyStateStimulatorDesc OpenViBE::CIdentifier(0x7EBE7538, 0x6EC74654)

namespace OpenViBEPlugins
{
	namespace Stimulation
	{
		class CBoxAlgorithmP300SpellerSteadyStateStimulator : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::uint64 getClockFrequency(void);
			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32Index);
			virtual OpenViBE::boolean processClock(OpenViBE::CMessageClock& rMessageClock);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_P300SpellerSteadyStateStimulator);

		protected:

			OpenViBE::uint64 m_ui64StartStimulation;
			OpenViBE::uint64 m_ui64RowStimulationBase;
			OpenViBE::uint64 m_ui64ColumnStimulationBase;

			OpenViBE::uint64 m_ui64RowCount;
			OpenViBE::uint64 m_ui64ColumnCount;

			OpenViBE::uint64 m_ui64RepetitionCountInTrial;
			OpenViBE::uint64 m_ui64TrialCount;
			OpenViBE::uint64 m_ui64FlashDuration;
			OpenViBE::uint64 m_ui64NoFlashDuration;
			OpenViBE::uint64 m_ui64InterRepetitionDuration;
			OpenViBE::uint64 m_ui64InterTrialDuration;

			OpenViBE::uint64 m_ui64SteadyStateStopComponent;			
			OpenViBE::uint64 m_ui64FlashSteadyStateDuration;
			OpenViBE::uint64 m_ui64NoFlashSteadyStateDuration; 
			OpenViBE::uint64 m_ui64FlashSteadyStateDuration2;
			OpenViBE::uint64 m_ui64NoFlashSteadyStateDuration2; 
			
			OpenViBE::uint64 m_ui64SequenceType;
			OpenViBE::boolean m_bAvoidNeighborFlashing;

		private:

			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationEncoder;
			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationDecoder;
			OpenViBE::uint64 m_ui64LastTime;
			OpenViBE::boolean m_bHeaderSent;
			OpenViBE::boolean m_bStartReceived;

			OpenViBE::uint32 m_ui32LastState;
			OpenViBE::uint64 m_ui64TrialStartTime;

			OpenViBE::uint64 m_ui64FlashCountInRepetition;
			OpenViBE::uint64 m_ui64RepetitionDuration;
			OpenViBE::uint64 m_ui64TrialDuration;
			OpenViBE::uint64 m_ui64TrialIndex;
			
			OpenViBE::uint64 m_ui64SteadyStateStartTime;
			OpenViBE::boolean m_ui64SteadyStateFlash;
			OpenViBE::boolean m_ui32LastFlashSteadyState;
			OpenViBE::uint64 m_ui64SteadyStateStartTime2;
			OpenViBE::boolean m_ui64SteadyStateFlash2;
			OpenViBE::boolean m_ui32LastFlashSteadyState2;
			
			std::map < OpenViBE::uint64, OpenViBE::uint64 > m_vRow;
			std::map < OpenViBE::uint64, OpenViBE::uint64 > m_vColumn;

			OpenViBE::uint64 m_ui64InterSegmentRestCount;
			
		private:

			void generate_sequence(void);
		};

		class CBoxAlgorithmP300SpellerSteadyStateStimulatorDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("P300 Speller + Steady State Stimulator"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Matthieu GOYAT"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("Gipsa-lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Stimulation"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-select-font"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_P300SpellerSteadyStateStimulator; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Stimulation::CBoxAlgorithmP300SpellerSteadyStateStimulator; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("Incoming stimulations",           OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addOutput ("Produced stimulations",           OV_TypeId_Stimulations);

				rBoxAlgorithmPrototype.addSetting("Start stimulation",               OV_TypeId_Stimulation, "OVTK_StimulationId_Label_00");
				rBoxAlgorithmPrototype.addSetting("Row stimulation base",            OV_TypeId_Stimulation, "OVTK_StimulationId_Label_01");
				rBoxAlgorithmPrototype.addSetting("Column stimulation base",         OV_TypeId_Stimulation, "OVTK_StimulationId_Label_07");

				rBoxAlgorithmPrototype.addSetting("Number of rows",                  OV_TypeId_Integer,     "6");
				rBoxAlgorithmPrototype.addSetting("Number of columns",               OV_TypeId_Integer,     "5");

				rBoxAlgorithmPrototype.addSetting("Number of repetitions",           OV_TypeId_Integer,     "5");
				rBoxAlgorithmPrototype.addSetting("Number of trials",                OV_TypeId_Integer,     "5");
				rBoxAlgorithmPrototype.addSetting("Component of Steady State stop",  OV_TypeId_SSComponent,     "No SS stop");
				rBoxAlgorithmPrototype.addSetting("Flash duration (in sec)",         OV_TypeId_Float,       "0.250");
				rBoxAlgorithmPrototype.addSetting("No flash duration (in sec)",      OV_TypeId_Float,       "0.150");
				rBoxAlgorithmPrototype.addSetting("Inter-repetition delay (in sec)", OV_TypeId_Float,       "2");
				rBoxAlgorithmPrototype.addSetting("Inter-trial delay (in sec)",      OV_TypeId_Float,       "5");

				rBoxAlgorithmPrototype.addSetting("Flash Steady State duration (in sec)",         OV_TypeId_Float,       "0.066");
				rBoxAlgorithmPrototype.addSetting("No flash Steady State duration (in sec)",      OV_TypeId_Float,       "0.067");
				rBoxAlgorithmPrototype.addSetting("Flash Steady State 2 duration (in sec)",         OV_TypeId_Float,       "0.075");
				rBoxAlgorithmPrototype.addSetting("No flash Steady State 2 duration (in sec)",      OV_TypeId_Float,       "0.075");

				rBoxAlgorithmPrototype.addSetting("Sequence generation type",        OVP_TypeId_SequenceGenerationType,     "Random Raw + Column");				

				rBoxAlgorithmPrototype.addSetting("Avoid neighbor flashing",         OV_TypeId_Boolean,     "false");
				
				rBoxAlgorithmPrototype.addSetting("no flash period during inter Row/Column", OV_TypeId_Integer, "3");

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_P300SpellerSteadyStateStimulatorDesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_P300SpellerSteadyStateStimulator_H__
