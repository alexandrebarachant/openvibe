#ifndef __OpenViBEPlugins_Acquisition_CGenericNetworkAcquisition_H__
#define __OpenViBEPlugins_Acquisition_CGenericNetworkAcquisition_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>

#include <socket/IConnectionClient.h>

#include <ebml/IReader.h>
#include <ebml/IReaderHelper.h>
#include <ebml/IWriter.h>
#include <ebml/IWriterHelper.h>

#define EEG_NodeId_Header                      EBML::CIdentifier(0x4239)
 #define EEG_NodeId_AcquisitionInformation     EBML::CIdentifier(0x4240)
  #define EEG_NodeId_ExperimentId              EBML::CIdentifier(0x4241)
  #define EEG_NodeId_SubjectAge                EBML::CIdentifier(0x4242)
  #define EEG_NodeId_SubjectSex                EBML::CIdentifier(0x4243)
  #define EEG_NodeId_ChannelCount              EBML::CIdentifier(0x4244)
  #define EEG_NodeId_SamplingFrequency         EBML::CIdentifier(0x4245)
  #define EEG_NodeId_ChannelNames              EBML::CIdentifier(0x4246)
   #define EEG_NodeId_ChannelName              EBML::CIdentifier(0x4247)
  #define EEG_NodeId_GainFactors               EBML::CIdentifier(0x4248)
   #define EEG_NodeId_GainFactor               EBML::CIdentifier(0x4249)
  #define EEG_NodeId_ChannelLocations          EBML::CIdentifier(0x4250)
   #define EEG_NodeId_ChannelLocation          EBML::CIdentifier(0x4251)
#define EEG_NodeId_Buffer                      EBML::CIdentifier(0x5A)
 #define EEG_NodeId_Samples                    EBML::CIdentifier(0x5B)
  #define EEG_NodeId_SamplesPerChannelCount    EBML::CIdentifier(0x5C)
  #define EEG_NodeId_SampleBlock               EBML::CIdentifier(0x5D)
 #define EEG_NodeId_Stimulations               EBML::CIdentifier(0x60)
  #define EEG_NodeId_StimulationsCount         EBML::CIdentifier(0x61)
  #define EEG_NodeId_Stimulation               EBML::CIdentifier(0x62)
   #define EEG_NodeId_StimulationSampleIndex   EBML::CIdentifier(0x63)
   #define EEG_NodeId_StimulationIdentifier    EBML::CIdentifier(0x64)

namespace OpenViBEPlugins
{
	namespace Acquisition
	{
		class CGenericNetworkAcquisition : virtual public OpenViBE::Plugins::IBoxAlgorithm
		{
		public:

			CGenericNetworkAcquisition(void);

			virtual void release(void) { delete this; }
			virtual OpenViBE::boolean initialize(
				const OpenViBE::Plugins::IPluginObjectContext& rContext);
			virtual OpenViBE::boolean uninitialize(
				const OpenViBE::Plugins::IPluginObjectContext& rContext);
			virtual OpenViBE::boolean processClock(
				OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext,
				OpenViBE::CMessageClock& rMessageClock);
			virtual OpenViBE::boolean process(
				OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext);

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_GenericNetworkAcquisition)

		public:

			OpenViBE::CString m_sServerHostName;
			OpenViBE::uint32 m_ui32ServerHostPort;
			Socket::IConnectionClient* m_pConnectionClient;
			EBML::IReaderCallBack* m_pReaderCallBack;
			EBML::IReader* m_pReader;
			EBML::IWriterCallBack* m_pWriterCallBack;
			EBML::IWriter* m_pWriter;
			OpenViBE::Plugins::IBoxContext::IOutput* m_pOutput;
			OpenViBE::uint64 m_ui64CurrentBufferSize;
		};

		class CGenericNetworkAcquisitionDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Generic network acquisition"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("A generic network based acquisition algorithm"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("This algorithm waits for EEG data from the network and distributes it into the scenario"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Acquisition"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.1"); }
			virtual void release(void)                                   { }
			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_GenericNetworkAcquisition; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Acquisition::CGenericNetworkAcquisition(); }
			virtual OpenViBE::boolean getBoxPrototype(OpenViBE::Plugins::IBoxProto& rPrototype) const;
			virtual OpenViBE::uint32 getClockFrequency(void) const;

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_GenericNetworkAcquisitionDesc)
		};
	};
};

#endif // __OpenViBEPlugins_Acquisition_CGenericNetworkAcquisition_H__
