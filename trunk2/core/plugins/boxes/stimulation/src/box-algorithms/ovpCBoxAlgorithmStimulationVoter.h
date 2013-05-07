#ifndef __OpenViBEPlugins_BoxAlgorithm_StimulationVoter_H__
#define __OpenViBEPlugins_BoxAlgorithm_StimulationVoter_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <vector>
#include <map>
#include <deque>
#include <cstdio>

namespace OpenViBEPlugins
{
	namespace Stimulation
	{
		class CBoxAlgorithmStimulationVoter : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32Index);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_StimulationVoter);

		protected:

			OpenViBE::uint64 m_ui64MinimumVotes;
			OpenViBE::float64 m_f64TimeWindow;
			OpenViBE::uint64 m_ui64RejectClassLabel;
			OpenViBE::CIdentifier m_oClearVotes;
			OpenViBE::CIdentifier m_oOutputDateMode;
			OpenViBE::CIdentifier m_oRejectClass_CanWin;

		private:

			OpenViBE::Kernel::IAlgorithmProxy* m_pEncoder;
			OpenViBE::Kernel::IAlgorithmProxy* m_pDecoder;
			OpenViBE::Kernel::TParameterHandler<const OpenViBE::IMemoryBuffer*> ip_pMemoryBuffer;
			OpenViBE::Kernel::TParameterHandler<OpenViBE::IStimulationSet*> op_pStimulationSet;

			std::deque< std::pair< OpenViBE::uint64, OpenViBE::uint64 > > m_oStimulusDeque; // <label,time>

			OpenViBE::uint64 m_ui64LatestStimulusDate;
			OpenViBE::uint64 m_ui64LastTime;
		};


		class CBoxAlgorithmStimulationVoterListener : public OpenViBEToolkit::TBoxListener < OpenViBE::Plugins::IBoxListener >
		{
		public:

			CBoxAlgorithmStimulationVoterListener(void)
				:m_oInputTypeIdentifier(OV_TypeId_Stimulations)
			{
			}

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxListener < OpenViBE::Plugins::IBoxListener >, OV_UndefinedIdentifier);

		protected:

			OpenViBE::CIdentifier m_oInputTypeIdentifier;
		};

		class CBoxAlgorithmStimulationVoterDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Stimulation Voter"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Jussi T. Lindgren"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("Inria"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Performs majority vote on the input stimuli"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("Votes the most frequent stimulus ID in a given time window. Outputs the winning stimulus type. Several options are possible. To process multiple inputs, use Stimulation Multiplexer first."); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Streaming"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-apply"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_StimulationVoter; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Stimulation::CBoxAlgorithmStimulationVoter; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("Stimulus input",               OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addOutput ("Selected stimulus",            OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addSetting("Number of stimuli required for vote",    OV_TypeId_Integer,     "4");
				rBoxAlgorithmPrototype.addSetting("Time window (secs)",           OV_TypeId_Float,       "2");
				rBoxAlgorithmPrototype.addSetting("Clear votes",                  OVP_TypeId_Voting_ClearVotes, "After output");
				rBoxAlgorithmPrototype.addSetting("Output timestamp",             OVP_TypeId_Voting_OutputTime, "Time of last voting stimulus");
				rBoxAlgorithmPrototype.addSetting("Reject class label",           OV_TypeId_Stimulation, "OVTK_StimulationId_Label_00");
				rBoxAlgorithmPrototype.addSetting("Reject class can win",         OVP_TypeId_Voting_RejectClass_CanWin, "No");

				return true;
			}

			virtual OpenViBE::Plugins::IBoxListener* createBoxListener(void) const { return new CBoxAlgorithmStimulationVoterListener; }
			virtual void releaseBoxListener(OpenViBE::Plugins::IBoxListener* pBoxListener) { delete pBoxListener; }

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_StimulationVoterDesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_StimulationVoter_H__
