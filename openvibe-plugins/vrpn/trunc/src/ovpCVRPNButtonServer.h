#ifndef __OpenViBEPlugins_VRPN_CVRPNButtonServer_H__
#define __OpenViBEPlugins_VRPN_CVRPNButtonServer_H__

#if defined TARGET_HAS_ThirdPartyVRPN

#include "ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <map>

namespace OpenViBEPlugins
{
	namespace VRPN
	{
		class CVRPNButtonServer
			:public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>
			,public OpenViBEToolkit::IBoxAlgorithmStimulationInputReaderCallback::ICallback
		{
		public:

			CVRPNButtonServer(void);
			virtual void release(void) { delete this; }
			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_VRPNButtonServer)

			virtual void setStimulationCount(const OpenViBE::uint32 ui32StimulationCount);
			virtual void setStimulation(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint64 ui64StimulationIdentifier, const OpenViBE::uint64 ui64StimulationDate);

		protected:

			//ebml
			EBML::IReader* m_pReader;
			OpenViBEToolkit::IBoxAlgorithmStimulationInputReaderCallback* m_pStimulationReaderCallBack;

			//Start and end time of the last buffer
			OpenViBE::uint64 m_ui64StartTime;
			OpenViBE::uint64 m_ui64EndTime;

			OpenViBE::uint32 m_ui32CurrentInput;

			OpenViBE::CIdentifier m_oServerIdentifier;

			//Pairs of start/stop stimulations id
			std::map<OpenViBE::uint32, std::pair<OpenViBE::uint64, OpenViBE::uint64> > m_vStimulationPair;
		};

		class CVRPNButtonServerDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Button VRPN server"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Creates VRPN button servers (one per input)."); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("Creates VRPN button servers to make data from the plugin's inputs available to VRPN client applications."); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("VRPN"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.1"); }
			virtual void release(void)                                   { }
			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_VRPNButtonServer; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::VRPN::CVRPNButtonServer(); }

			virtual OpenViBE::boolean getBoxPrototype(OpenViBE::Kernel::IBoxProto& rPrototype) const
			{
				rPrototype.addInput("Input 1", OVTK_TypeId_Stimulations);
				// rPrototype.addInput("Input 2", OVTK_TypeId_Stimulations);

				rPrototype.addSetting("Peripheral name", OV_TypeId_String, "openvibe-vrpn");
				rPrototype.addSetting("Button 1 : ON stimulation", OV_TypeId_Stimulation, "OVTK_GDF_Feedback_Continuous");
				rPrototype.addSetting("Button 1 : OFF stimulation", OV_TypeId_Stimulation, "OVTK_GDF_End_Of_Trial");
				// rPrototype.addSetting("Button 2 : ON stimulation", OV_TypeId_Stimulation, "OVTK_GDF_Feedback_Continuous");
				// rPrototype.addSetting("Button 2 : OFF stimulation", OV_TypeId_Stimulation, "OVTK_GDF_End_Of_Trial");

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_VRPNButtonServerDesc)
		};
	};
};

#endif // OVP_HAS_Vrpn

#endif // __OpenViBEPlugins_VRPN_CVRPNButtonServer_H__
