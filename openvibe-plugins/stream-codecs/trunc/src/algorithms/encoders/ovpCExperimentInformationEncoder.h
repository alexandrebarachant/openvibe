#ifndef __SamplePlugin_Algorithms_CExperimentInformationEncoder_H__
#define __SamplePlugin_Algorithms_CExperimentInformationEncoder_H__

#include "ovpCEBMLBaseEncoder.h"

namespace OpenViBEPlugins
{
	namespace StreamCodecs
	{
		class CExperimentInformationEncoder : virtual public OpenViBEPlugins::StreamCodecs::CEBMLBaseEncoder
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processHeader(void);

			_IsDerivedFromClass_Final_(OpenViBEPlugins::StreamCodecs::CEBMLBaseEncoder, OVP_ClassId_Algorithm_ExperimentInformationEncoder);

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
		};

		class CExperimentInformationEncoderDesc : virtual public OpenViBEPlugins::StreamCodecs::CEBMLBaseEncoderDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Experiment information stream encoder"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Stream codecs/Encoders"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_Algorithm_ExperimentInformationEncoder; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::StreamCodecs::CExperimentInformationEncoder(); }

			virtual OpenViBE::boolean getAlgorithmPrototype(
				OpenViBE::Kernel::IAlgorithmProto& rAlgorithmPrototype) const
			{
				OpenViBEPlugins::StreamCodecs::CEBMLBaseEncoderDesc::getAlgorithmPrototype(rAlgorithmPrototype);

				rAlgorithmPrototype.addInputParameter(OVP_ExperimentInformationEncoder_ExperimentIdentifier_InParameterId, "Experiment identifier", OpenViBE::Kernel::ParameterType_UInteger);
				rAlgorithmPrototype.addInputParameter(OVP_ExperimentInformationEncoder_ExperimentDate_InParameterId,       "Experiment date",       OpenViBE::Kernel::ParameterType_String);
				rAlgorithmPrototype.addInputParameter(OVP_ExperimentInformationEncoder_SubjectIdentifier_InParameterId,    "Subject identifier",    OpenViBE::Kernel::ParameterType_UInteger);
				rAlgorithmPrototype.addInputParameter(OVP_ExperimentInformationEncoder_SubjectName_InParameterId,          "Subject name",          OpenViBE::Kernel::ParameterType_String);
				rAlgorithmPrototype.addInputParameter(OVP_ExperimentInformationEncoder_SubjectAge_InParameterId,           "Subject age",           OpenViBE::Kernel::ParameterType_UInteger);
				rAlgorithmPrototype.addInputParameter(OVP_ExperimentInformationEncoder_SubjectGender_InParameterId,        "Subject gender",        OpenViBE::Kernel::ParameterType_UInteger);
				rAlgorithmPrototype.addInputParameter(OVP_ExperimentInformationEncoder_LaboratoryIdentifier_InParameterId, "Laboratory identifier", OpenViBE::Kernel::ParameterType_UInteger);
				rAlgorithmPrototype.addInputParameter(OVP_ExperimentInformationEncoder_LaboratoryName_InParameterId,       "Laboratory name",       OpenViBE::Kernel::ParameterType_String);
				rAlgorithmPrototype.addInputParameter(OVP_ExperimentInformationEncoder_TechnicianIdentifier_InParameterId, "Technician identifier", OpenViBE::Kernel::ParameterType_UInteger);
				rAlgorithmPrototype.addInputParameter(OVP_ExperimentInformationEncoder_TechnicianName_InParameterId,       "Technician name",       OpenViBE::Kernel::ParameterType_String);

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBEPlugins::StreamCodecs::CEBMLBaseEncoderDesc, OVP_ClassId_Algorithm_ExperimentInformationEncoderDesc);
		};
	};
};

#endif // __SamplePlugin_Algorithms_CExperimentInformationEncoder_H__
