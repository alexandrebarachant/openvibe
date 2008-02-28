#include "ovpCExperimentInformationEncoder.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::StreamCodecs;

boolean CExperimentInformationEncoder::initialize(void)
{
	CEBMLBaseEncoder::initialize();

	m_oExperimentIdentifierHandle.initialize(getInputParameter(OVP_ExperimentInformationEncoder_ExperimentIdentifier_InParameterId));
	m_oExperimentDateHandle.initialize(getInputParameter(OVP_ExperimentInformationEncoder_ExperimentDate_InParameterId));
	m_oSubjectIdentifierHandle.initialize(getInputParameter(OVP_ExperimentInformationEncoder_SubjectIdentifier_InParameterId));
	m_oSubjectNameHandle.initialize(getInputParameter(OVP_ExperimentInformationEncoder_SubjectName_InParameterId));
	m_oSubjectAgeHandle.initialize(getInputParameter(OVP_ExperimentInformationEncoder_SubjectAge_InParameterId));
	m_oSubjectGenderHandle.initialize(getInputParameter(OVP_ExperimentInformationEncoder_SubjectGender_InParameterId));
	m_oLaboratoryIdentifierHandle.initialize(getInputParameter(OVP_ExperimentInformationEncoder_LaboratoryIdentifier_InParameterId));
	m_oLaboratoryNameHandle.initialize(getInputParameter(OVP_ExperimentInformationEncoder_LaboratoryName_InParameterId));
	m_oTechnicianIdentifierHandle.initialize(getInputParameter(OVP_ExperimentInformationEncoder_TechnicianIdentifier_InParameterId));
	m_oTechnicianNameHandle.initialize(getInputParameter(OVP_ExperimentInformationEncoder_TechnicianName_InParameterId));

	return true;
}

boolean CExperimentInformationEncoder::uninitialize(void)
{
	m_oTechnicianNameHandle.uninitialize();
	m_oTechnicianIdentifierHandle.uninitialize();
	m_oLaboratoryNameHandle.uninitialize();
	m_oLaboratoryIdentifierHandle.uninitialize();
	m_oSubjectGenderHandle.uninitialize();
	m_oSubjectAgeHandle.uninitialize();
	m_oSubjectNameHandle.uninitialize();
	m_oSubjectIdentifierHandle.uninitialize();
	m_oExperimentDateHandle.uninitialize();
	m_oExperimentIdentifierHandle.uninitialize();

	CEBMLBaseEncoder::uninitialize();

	return true;
}

// ________________________________________________________________________________________________________________
//

boolean CExperimentInformationEncoder::processHeader(void)
{
	m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_ExperimentInformation);
	 m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_ExperimentInformation_Experiment);
	   m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_ExperimentInformation_Experiment_Identifier);
	    m_pEBMLWriterHelper->setUIntegerAsChildData(m_oExperimentIdentifierHandle);
	   m_pEBMLWriterHelper->closeChild();
	   m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_ExperimentInformation_Experiment_Date);
	    m_pEBMLWriterHelper->setASCIIStringAsChildData(m_oExperimentDateHandle->toASCIIString());
	   m_pEBMLWriterHelper->closeChild();
	  m_pEBMLWriterHelper->closeChild();
	  m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_ExperimentInformation_Subject);
	   m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_ExperimentInformation_Subject_Identifier);
	   m_pEBMLWriterHelper->setUIntegerAsChildData(m_oSubjectIdentifierHandle);
	   m_pEBMLWriterHelper->closeChild();
	   m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_ExperimentInformation_Subject_Name);
	    m_pEBMLWriterHelper->setASCIIStringAsChildData(m_oSubjectNameHandle->toASCIIString());
	   m_pEBMLWriterHelper->closeChild();
	   m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_ExperimentInformation_Subject_Age);
	    m_pEBMLWriterHelper->setUIntegerAsChildData(m_oSubjectAgeHandle);
	   m_pEBMLWriterHelper->closeChild();
	   m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_ExperimentInformation_Subject_Gender);
	    m_pEBMLWriterHelper->setUIntegerAsChildData(m_oSubjectGenderHandle);
	   m_pEBMLWriterHelper->closeChild();
	  m_pEBMLWriterHelper->closeChild();
	  m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_ExperimentInformation_Context);
	   m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_ExperimentInformation_Context_LaboratoryIdentifier);
	    m_pEBMLWriterHelper->setUIntegerAsChildData(m_oLaboratoryIdentifierHandle);
	   m_pEBMLWriterHelper->closeChild();
	   m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_ExperimentInformation_Context_LaboratoryName);
	    m_pEBMLWriterHelper->setASCIIStringAsChildData(m_oLaboratoryNameHandle->toASCIIString());
	   m_pEBMLWriterHelper->closeChild();
	   m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_ExperimentInformation_Context_TechnicianIdentifier);
	    m_pEBMLWriterHelper->setUIntegerAsChildData(m_oTechnicianIdentifierHandle);
	   m_pEBMLWriterHelper->closeChild();
	   m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_ExperimentInformation_Context_TechnicianName);
	    m_pEBMLWriterHelper->setASCIIStringAsChildData(m_oTechnicianNameHandle->toASCIIString());
	   m_pEBMLWriterHelper->closeChild();
	 m_pEBMLWriterHelper->closeChild();
	m_pEBMLWriterHelper->closeChild();

	return true;
}
