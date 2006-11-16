#ifndef __OpenViBEPlugins_Acquisition_CEEGStreamWriterGDF_H__
#define __OpenViBEPlugins_Acquisition_CEEGStreamWriterGDF_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>

#include <ebml/IReader.h>
#include <ebml/IReaderHelper.h>

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
	namespace Utility
	{
		class CEEGStreamWriterGDF : virtual public OpenViBE::Plugins::IBoxAlgorithm
		{
		public:

			CEEGStreamWriterGDF(void);

			virtual void release(void) { delete this; }
			virtual OpenViBE::boolean initialize(
				const OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext);
			virtual OpenViBE::boolean uninitialize(
				const OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext);
			virtual OpenViBE::boolean processInput(
				OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext,
				OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(
				OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext);

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_EEGStreamWriterGDF)

		public:

			OpenViBE::CString m_sFileName;
			EBML::IReaderCallBack* m_pReaderCallBack;
			EBML::IReader* m_pReader;
		};

		class CEEGStreamWriterGDFDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("EEG stream writer - GDF"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("This algorithm records on disk what comes from a specific output"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("This algorithm dumps on disk a stream from a specific output in the standard GDF file format"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Utility"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.1"); }
			virtual void release(void)                                   { }
			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_EEGStreamWriterGDF; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Utility::CEEGStreamWriterGDF(); }
			virtual OpenViBE::boolean getBoxPrototype(OpenViBE::Plugins::IBoxProto& rPrototype) const;
			virtual OpenViBE::uint32 getClockFrequency(OpenViBE::Plugins::IStaticBoxContext& rStaticBoxContext) const;

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_EEGStreamWriterGDFDesc)
		};
	};
};

#endif // __OpenViBEPlugins_Acquisition_CEEGStreamWriterGDF_H__
