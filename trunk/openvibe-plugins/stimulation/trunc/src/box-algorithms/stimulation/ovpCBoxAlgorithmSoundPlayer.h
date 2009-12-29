#ifndef __OpenViBEPlugins_BoxAlgorithm_SoundPlayer_H__
#define __OpenViBEPlugins_BoxAlgorithm_SoundPlayer_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

namespace OpenViBEPlugins
{
	namespace Stimulation
	{
		class CBoxAlgorithmSoundPlayer : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			// virtual OpenViBE::uint64 getClockFrequency(void);
			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			// virtual OpenViBE::boolean processEvent(OpenViBE::CMessageEvent& rMessageEvent);
			// virtual OpenViBE::boolean processSignal(OpenViBE::CMessageSignal& rMessageSignal);
			// virtual OpenViBE::boolean processClock(OpenViBE::CMessageClock& rMessageClock);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_SoundPlayer);

		protected:

			OpenViBE::Kernel::IAlgorithmProxy* m_pStreamDecoder;
			OpenViBE::uint64 m_ui64StimulationId;
			OpenViBE::CString m_sSoundFilename;
		};

		class CBoxAlgorithmSoundPlayerDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Sound player"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Stimulation"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_SoundPlayer; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Stimulation::CBoxAlgorithmSoundPlayer; }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-media-play"); }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("Stimulation stream", OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addSetting("Stimulation", OV_TypeId_Stimulation,"OVTK_StimulationId_Beep");
				rBoxAlgorithmPrototype.addSetting("Sound to play", OV_TypeId_Filename, "../share/openvibe-plugins/stimulation/ov_beep.wav");
				rBoxAlgorithmPrototype.addFlag   (OpenViBE::Kernel::BoxFlag_IsUnstable);

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_SoundPlayerDesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_SoundPlayer_H__
