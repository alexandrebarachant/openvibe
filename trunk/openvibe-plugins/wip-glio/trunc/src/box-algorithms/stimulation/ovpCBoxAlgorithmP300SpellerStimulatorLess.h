#ifndef __OpenViBEPlugins_BoxAlgorithm_P300SpellerStimulatorLess_H__
#define __OpenViBEPlugins_BoxAlgorithm_P300SpellerStimulatorLess_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <map>

#define OVP_ClassId_BoxAlgorithm_P300SpellerStimulatorLessDesc 			OpenViBE::CIdentifier(0x2BB21FB6, 0x5FF152DB)
#define OVP_ClassId_BoxAlgorithm_P300SpellerStimulatorLess     		OpenViBE::CIdentifier(0x05FD14E1, 0x4B8B28FF)

namespace OpenViBEPlugins
{
	namespace Stimulation
	{
		class CBoxAlgorithmP300SpellerStimulatorLess : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::uint64 getClockFrequency(void);
			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32Index);
			virtual OpenViBE::boolean processClock(OpenViBE::CMessageClock& rMessageClock);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_P300SpellerStimulatorLess);

		protected:

			OpenViBE::uint64 m_ui64StartStimulation;

			OpenViBE::uint64 m_ui64RepetitionCountInTrial;
			OpenViBE::uint64 m_ui64TrialCount;
			OpenViBE::uint64 m_ui64FlashDuration;
			OpenViBE::uint64 m_ui64NoFlashDuration;
			OpenViBE::uint64 m_ui64InterRepetitionDuration;
			OpenViBE::uint64 m_ui64InterTrialDuration;

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

		private:

		};

		class CBoxAlgorithmP300SpellerStimulatorLessDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("P300 Speller StimulatorLess"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Stimulation"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-select-font"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_P300SpellerStimulatorLess; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Stimulation::CBoxAlgorithmP300SpellerStimulatorLess; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("Incoming stimulations",           OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addOutput ("Produced stimulations",           OV_TypeId_Stimulations);

				rBoxAlgorithmPrototype.addSetting("Start stimulation",               OV_TypeId_Stimulation, "OVTK_StimulationId_Label_00");
				rBoxAlgorithmPrototype.addSetting("Number of flash",           		 OV_TypeId_Integer,     "12");
				rBoxAlgorithmPrototype.addSetting("Number of repetitions",           OV_TypeId_Integer,     "5");
				rBoxAlgorithmPrototype.addSetting("Number of trials",                OV_TypeId_Integer,     "5");
				rBoxAlgorithmPrototype.addSetting("Flash duration (in sec)",         OV_TypeId_Float,       "0.075");
				rBoxAlgorithmPrototype.addSetting("No flash duration (in sec)",      OV_TypeId_Float,       "0.125");
				rBoxAlgorithmPrototype.addSetting("Inter-repetition delay (in sec)", OV_TypeId_Float,       "2");
				rBoxAlgorithmPrototype.addSetting("Inter-trial delay (in sec)",      OV_TypeId_Float,       "5");
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_P300SpellerStimulatorLessDesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_P300SpellerStimulatorLess_H__
