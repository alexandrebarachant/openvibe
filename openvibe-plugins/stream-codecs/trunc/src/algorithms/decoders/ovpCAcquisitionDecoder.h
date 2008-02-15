#ifndef __SamplePlugin_Algorithms_CAcquisitionDecoder_H__
#define __SamplePlugin_Algorithms_CAcquisitionDecoder_H__

#include "ovpCEBMLBaseDecoder.h"

namespace OpenViBEPlugins
{
	namespace StreamCodecs
	{
		class CAcquisitionDecoder : virtual public OpenViBEPlugins::StreamCodecs::CEBMLBaseDecoder
		{
		public:

			CAcquisitionDecoder(void);

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);

			_IsDerivedFromClass_Final_(OpenViBEPlugins::StreamCodecs::CEBMLBaseDecoder, OVP_ClassId_Algorithm_AcquisitionDecoder);

			// ebml callbacks
			virtual EBML::boolean isMasterChild(const EBML::CIdentifier& rIdentifier);
			virtual void openChild(const EBML::CIdentifier& rIdentifier);
			virtual void processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize);
			virtual void closeChild(void);

		protected:

			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > m_oExperimentIdHandle;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > m_oSubjectAgeHandle;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > m_oSubjectGenderHandle;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > m_oSamplingRateHandle;

			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > m_oChannelLocationHandle;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > m_oSignalHandle;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > m_oStimulationSetHandle;

		private:

			std::stack<EBML::CIdentifier> m_vNodes;

			OpenViBE::uint32 m_ui32ChannelIndex;
			OpenViBE::uint32 m_ui32SampleCountPerBuffer;
			std::vector < OpenViBE::float64 > m_vChannelGainFactor;
			std::vector < OpenViBE::float64 > m_vChannelLocation;
			std::vector < OpenViBE::uint64 > m_vStimulationIdentifier;
			std::vector < OpenViBE::uint64 > m_vStimulationSampleIndex;
		};

		class CAcquisitionDecoderDesc : virtual public OpenViBEPlugins::StreamCodecs::CEBMLBaseDecoderDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Acquisition stream decoder"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Algorithm/Stream codecs/Decoders"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_Algorithm_AcquisitionDecoder; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::StreamCodecs::CAcquisitionDecoder(); }

			virtual OpenViBE::boolean getAlgorithmPrototype(
				OpenViBE::Kernel::IAlgorithmProto& rAlgorithmPrototype) const
			{
				OpenViBEPlugins::StreamCodecs::CEBMLBaseDecoderDesc::getAlgorithmPrototype(rAlgorithmPrototype);

				rAlgorithmPrototype.addOutputParameter(OVP_AcquisitionDecoder_ExperimentId_OutParameterId,    "Experiment identifier", OpenViBE::Kernel::ParameterType_UInteger);
				rAlgorithmPrototype.addOutputParameter(OVP_AcquisitionDecoder_SubjectAge_OutParameterId,      "Subject age",           OpenViBE::Kernel::ParameterType_UInteger);
				rAlgorithmPrototype.addOutputParameter(OVP_AcquisitionDecoder_SubjectGender_OutParameterId,   "Subject gender",        OpenViBE::Kernel::ParameterType_UInteger);
				rAlgorithmPrototype.addOutputParameter(OVP_AcquisitionDecoder_SamplingRate_OutParameterId,    "Sampling rate",         OpenViBE::Kernel::ParameterType_UInteger);
				rAlgorithmPrototype.addOutputParameter(OVP_AcquisitionDecoder_ChannelLocation_OutParameterId, "Channel location",      OpenViBE::Kernel::ParameterType_Matrix);
				rAlgorithmPrototype.addOutputParameter(OVP_AcquisitionDecoder_Signal_OutParameterId,          "Signal",                OpenViBE::Kernel::ParameterType_Matrix);
				rAlgorithmPrototype.addOutputParameter(OVP_AcquisitionDecoder_StimulationSet_OutParameterId,  "Stimulation set",       OpenViBE::Kernel::ParameterType_StimulationSet);

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBEPlugins::StreamCodecs::CEBMLBaseDecoderDesc, OVP_ClassId_Algorithm_AcquisitionDecoderDesc);
		};
	};
};

#endif // __SamplePlugin_Algorithms_CAcquisitionDecoder_H__
