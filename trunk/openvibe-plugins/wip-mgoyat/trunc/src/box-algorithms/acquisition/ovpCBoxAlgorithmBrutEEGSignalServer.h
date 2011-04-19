#ifndef __OpenViBEPlugins_BoxAlgorithm_BrutEEGSignalServer_H__
#define __OpenViBEPlugins_BoxAlgorithm_BrutEEGSignalServer_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <socket/IConnectionServer.h>
#include <map>

#define OVP_ClassId_BoxAlgorithm_BrutEEGSignalServer        OpenViBE::CIdentifier(0x167F79EC, 0x5CA8691B)
#define OVP_ClassId_BoxAlgorithm_BrutEEGSignalServerDesc    OpenViBE::CIdentifier(0x52BD2212, 0x23D351FD)

namespace OpenViBEPlugins
{
	namespace Acquisition
	{
		class CBoxAlgorithmBrutEEGSignalServer : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::uint64 getClockFrequency(void);
			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processClock(OpenViBE::CMessageClock& rMessageClock);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_BrutEEGSignalServer);

		protected:
			
			OpenViBE::Kernel::IAlgorithmProxy* m_pSignalDecoder;
			OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pSignalMemoryBuffer;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > op_pDecodedMatrix;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > op_ui64SamplingRate;
			
			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationDecoder;
			OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pStimulationMemoryBuffer;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > op_pStimulationSet;
			
			Socket::IConnectionServer* m_pConnectionServer;
			Socket::IConnection* m_pConnectionClient;
			OpenViBE::boolean m_bStarted;

			OpenViBE::uint64 m_ui64LastChunkStartTime;
			OpenViBE::uint64 m_ui64LastChunkEndTime;
		
			OpenViBE::float64 timeConvertion(OpenViBE::uint64 t);
		private : 

		};

		class CBoxAlgorithmBrutEEGSignalServerDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Brut EEG Signal Server"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Matthieu Goyat"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("Gipsa-lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Acquisition"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_BrutEEGSignalServer; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Acquisition::CBoxAlgorithmBrutEEGSignalServer; }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-connect"); }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput ("Experiment information",      OV_TypeId_ExperimentationInformation);
				rBoxAlgorithmPrototype.addInput ("Signal stream",               OV_TypeId_Signal);
				rBoxAlgorithmPrototype.addInput ("Stimulations",                OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addInput ("Channel localisation",        OV_TypeId_ChannelLocalisation);
				rBoxAlgorithmPrototype.addSetting("server port",    			OV_TypeId_Integer, "700");
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_BrutEEGSignalServerDesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_BrutEEGSignalServer_H__
