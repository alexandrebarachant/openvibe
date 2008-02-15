#ifndef __SamplePlugin_Algorithms_CExperimentInformationDecoder_H__
#define __SamplePlugin_Algorithms_CExperimentInformationDecoder_H__

#include "ovpCEBMLBaseDecoder.h"

namespace OpenViBEPlugins
{
	namespace StreamCodecs
	{
		class CExperimentInformationDecoder : virtual public OpenViBEPlugins::StreamCodecs::CEBMLBaseDecoder
		{
		public:

			CExperimentInformationDecoder(void);

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);

			_IsDerivedFromClass_Final_(OpenViBEPlugins::StreamCodecs::CEBMLBaseDecoder, OVP_ClassId_Algorithm_ExperimentInformationDecoder);

			// ebml callbacks
			virtual EBML::boolean isMasterChild(const EBML::CIdentifier& rIdentifier);
			virtual void openChild(const EBML::CIdentifier& rIdentifier);
			virtual void processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize);
			virtual void closeChild(void);

		protected:

			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > m_oExperimentIdentifierHandle;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::CString* > m_oExperimentDateHandle;

			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > m_oSubjectIdentifierHandle;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::CString* > m_oSubjectNameHandle;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > m_oSubjectAgeHandle;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > m_oSubjectGenderHandle;

			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > m_oLaboratoryIdentifierHandle;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::CString* > m_oLaboratoryNameHandle;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > m_oTechnicianIdentifierHandle;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::CString* > m_oTechnicianNameHandle;

		private:

			std::stack<EBML::CIdentifier> m_vNodes;
		};

		class CExperimentInformationDecoderDesc : virtual public OpenViBEPlugins::StreamCodecs::CEBMLBaseDecoderDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Experiment information stream decoder"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Algorithm/Stream codecs/Decoders"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_Algorithm_ExperimentInformationDecoder; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::StreamCodecs::CExperimentInformationDecoder(); }

			virtual OpenViBE::boolean getAlgorithmPrototype(
				OpenViBE::Kernel::IAlgorithmProto& rAlgorithmPrototype) const
			{
				OpenViBEPlugins::StreamCodecs::CEBMLBaseDecoderDesc::getAlgorithmPrototype(rAlgorithmPrototype);

				rAlgorithmPrototype.addOutputParameter(OVP_ExperimentInformationDecoder_ExperimentIdentifier_OutParameterId, "Experiment identifier", OpenViBE::Kernel::ParameterType_UInteger);
				rAlgorithmPrototype.addOutputParameter(OVP_ExperimentInformationDecoder_ExperimentDate_OutParameterId,       "Experiment date",       OpenViBE::Kernel::ParameterType_String);
				rAlgorithmPrototype.addOutputParameter(OVP_ExperimentInformationDecoder_SubjectIdentifier_OutParameterId,    "Subject identifier",    OpenViBE::Kernel::ParameterType_UInteger);
				rAlgorithmPrototype.addOutputParameter(OVP_ExperimentInformationDecoder_SubjectName_OutParameterId,          "Subject name",          OpenViBE::Kernel::ParameterType_String);
				rAlgorithmPrototype.addOutputParameter(OVP_ExperimentInformationDecoder_SubjectAge_OutParameterId,           "Subject age",           OpenViBE::Kernel::ParameterType_UInteger);
				rAlgorithmPrototype.addOutputParameter(OVP_ExperimentInformationDecoder_SubjectGender_OutParameterId,        "Subject gender",        OpenViBE::Kernel::ParameterType_UInteger);
				rAlgorithmPrototype.addOutputParameter(OVP_ExperimentInformationDecoder_LaboratoryIdentifier_OutParameterId, "Laboratory identifier", OpenViBE::Kernel::ParameterType_UInteger);
				rAlgorithmPrototype.addOutputParameter(OVP_ExperimentInformationDecoder_LaboratoryName_OutParameterId,       "Laboratory name",       OpenViBE::Kernel::ParameterType_String);
				rAlgorithmPrototype.addOutputParameter(OVP_ExperimentInformationDecoder_TechnicianIdentifier_OutParameterId, "Technician identifier", OpenViBE::Kernel::ParameterType_UInteger);
				rAlgorithmPrototype.addOutputParameter(OVP_ExperimentInformationDecoder_TechnicianName_OutParameterId,       "Technician name",       OpenViBE::Kernel::ParameterType_String);

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBEPlugins::StreamCodecs::CEBMLBaseDecoderDesc, OVP_ClassId_Algorithm_ExperimentInformationDecoderDesc);
		};
	};
};

#endif // __SamplePlugin_Algorithms_CExperimentInformationDecoder_H__
