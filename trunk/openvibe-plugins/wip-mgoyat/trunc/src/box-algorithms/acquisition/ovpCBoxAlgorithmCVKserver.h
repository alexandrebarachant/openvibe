#ifndef __OpenViBEPlugins_BoxAlgorithm_CVKServer_H__
#define __OpenViBEPlugins_BoxAlgorithm_CVKServer_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <socket/IConnectionServer.h>
#include <map>
#include <string>

#define OVP_ClassId_BoxAlgorithm_CVKServer          OpenViBE::CIdentifier(0x6FB04E32, 0x1B296AFA)
#define OVP_ClassId_BoxAlgorithm_CVKServerDesc      OpenViBE::CIdentifier(0x46D742FF, 0x0A3C0094)

namespace OpenViBEPlugins
{
	namespace Acquisition
	{
		class CBoxAlgorithmCVKServer : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::uint64 getClockFrequency(void);
			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processClock(OpenViBE::CMessageClock& rMessageClock);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_CVKServer);

		protected:
			
			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationDecoder;
			OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pStimulationMemoryBuffer;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > op_pStimulationSet;
			
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pAcquisitionMemoryBuffer;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > ip_ui64BufferDuration;
			OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pExperimentInformationMemoryBuffer;
			OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pSignalMemoryBuffer;
			OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pChannelLocalisationMemoryBuffer;

			Socket::IConnectionServer* m_pConnectionServer;
			Socket::IConnection* m_pConnectionClient;

			OpenViBE::uint64 m_ui64LastChunkStartTime;
			OpenViBE::uint64 m_ui64LastChunkEndTime;
		
			OpenViBE::float64 timeConvertion(OpenViBE::uint64 t);
		private : 
			std::map<OpenViBE::uint64, std::string> m_mapId;
			std::vector<OpenViBE::uint64> m_vect64;
			void initStimulationMap();
			void UseStimulationForMap(OpenViBE::uint64);
			void applyOnFile();
			void WriteTime(OpenViBE::uint64);
			
			OpenViBE::boolean parseCVKincome(OpenViBE::uint32);
			OpenViBE::uint64 m_timeBack;
			OpenViBE::uint64 m_actionBack;
			OpenViBE::uint64 m_DataBack;
			std::string extractBalise(std::string& base, std::string& balise, std::string& ender);
			OpenViBE::boolean Action2value(std::string&);
			OpenViBE::boolean Time2value(std::string&);
			OpenViBE::boolean Data2value(std::string&);
		};

		class CBoxAlgorithmCVKServerDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("CVK Server"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Matthieu Goyat"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("Gipsa-lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Acquisition"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_CVKServer; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Acquisition::CBoxAlgorithmCVKServer; }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-connect"); }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput ("Experiment information",      OV_TypeId_ExperimentationInformation);
				rBoxAlgorithmPrototype.addInput ("Signal strea",                OV_TypeId_Signal);
				rBoxAlgorithmPrototype.addInput ("Stimulations",                OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addInput ("Channel localisation",        OV_TypeId_ChannelLocalisation);
				rBoxAlgorithmPrototype.addSetting("Acquisition server port",     OV_TypeId_Integer, "700");
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_CVKServerDesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_CVKServer_H__
