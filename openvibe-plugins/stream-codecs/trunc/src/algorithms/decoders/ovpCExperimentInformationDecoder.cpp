#include "ovpCExperimentInformationDecoder.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::StreamCodecs;

CExperimentInformationDecoder::CExperimentInformationDecoder(void)
{
}

// ________________________________________________________________________________________________________________
//

boolean CExperimentInformationDecoder::initialize(void)
{
	CEBMLBaseDecoder::initialize();

	m_oExperimentIdentifierHandle.initialize(getOutputParameter(OVP_ExperimentInformationDecoder_ExperimentIdentifier_OutParameterId));
	m_oExperimentDateHandle      .initialize(getOutputParameter(OVP_ExperimentInformationDecoder_ExperimentDate_OutParameterId));
	m_oSubjectIdentifierHandle   .initialize(getOutputParameter(OVP_ExperimentInformationDecoder_SubjectIdentifier_OutParameterId));
	m_oSubjectNameHandle         .initialize(getOutputParameter(OVP_ExperimentInformationDecoder_SubjectName_OutParameterId));
	m_oSubjectAgeHandle          .initialize(getOutputParameter(OVP_ExperimentInformationDecoder_SubjectAge_OutParameterId));
	m_oSubjectGenderHandle       .initialize(getOutputParameter(OVP_ExperimentInformationDecoder_SubjectGender_OutParameterId));
	m_oLaboratoryIdentifierHandle.initialize(getOutputParameter(OVP_ExperimentInformationDecoder_LaboratoryIdentifier_OutParameterId));
	m_oLaboratoryNameHandle      .initialize(getOutputParameter(OVP_ExperimentInformationDecoder_LaboratoryName_OutParameterId));
	m_oTechnicianIdentifierHandle.initialize(getOutputParameter(OVP_ExperimentInformationDecoder_TechnicianIdentifier_OutParameterId));
	m_oTechnicianNameHandle      .initialize(getOutputParameter(OVP_ExperimentInformationDecoder_TechnicianName_OutParameterId));

	return true;
}

boolean CExperimentInformationDecoder::uninitialize(void)
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

	CEBMLBaseDecoder::uninitialize();

	return true;
}

// ________________________________________________________________________________________________________________
//

EBML::boolean CExperimentInformationDecoder::isMasterChild(const EBML::CIdentifier& rIdentifier)
{
	     if(rIdentifier==OVTK_NodeId_Header_ExperimentInformation)                              { return true; }
	else if(rIdentifier==OVTK_NodeId_Header_ExperimentInformation_Experiment)                   { return true; }
	else if(rIdentifier==OVTK_NodeId_Header_ExperimentInformation_Experiment_Identifier)        { return false; }
	else if(rIdentifier==OVTK_NodeId_Header_ExperimentInformation_Experiment_Date)              { return false; }
	else if(rIdentifier==OVTK_NodeId_Header_ExperimentInformation_Subject)                      { return true; }
	else if(rIdentifier==OVTK_NodeId_Header_ExperimentInformation_Subject_Identifier)           { return false; }
	else if(rIdentifier==OVTK_NodeId_Header_ExperimentInformation_Subject_Name)                 { return false; }
	else if(rIdentifier==OVTK_NodeId_Header_ExperimentInformation_Subject_Age)                  { return false; }
	else if(rIdentifier==OVTK_NodeId_Header_ExperimentInformation_Subject_Gender)               { return false; }
	else if(rIdentifier==OVTK_NodeId_Header_ExperimentInformation_Context)                      { return true; }
	else if(rIdentifier==OVTK_NodeId_Header_ExperimentInformation_Context_LaboratoryIdentifier) { return false; }
	else if(rIdentifier==OVTK_NodeId_Header_ExperimentInformation_Context_LaboratoryName)       { return false; }
	else if(rIdentifier==OVTK_NodeId_Header_ExperimentInformation_Context_TechnicianIdentifier) { return false; }
	else if(rIdentifier==OVTK_NodeId_Header_ExperimentInformation_Context_TechnicianName)       { return false; }
	return CEBMLBaseDecoder::isMasterChild(rIdentifier);
}

void CExperimentInformationDecoder::openChild(const EBML::CIdentifier& rIdentifier)
{
	m_vNodes.push(rIdentifier);

	EBML::CIdentifier& l_rTop=m_vNodes.top();

	if((l_rTop==OVTK_NodeId_Header_ExperimentInformation)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Experiment)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Experiment_Identifier)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Experiment_Date)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject_Identifier)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject_Name)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject_Age)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject_Gender)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context_LaboratoryIdentifier)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context_LaboratoryName)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context_TechnicianIdentifier)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context_TechnicianName))
	{
	}
	else
	{
		CEBMLBaseDecoder::openChild(rIdentifier);
	}
}

void CExperimentInformationDecoder::processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize)
{
	EBML::CIdentifier& l_rTop=m_vNodes.top();

	if((l_rTop==OVTK_NodeId_Header_ExperimentInformation)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Experiment)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Experiment_Identifier)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Experiment_Date)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject_Identifier)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject_Name)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject_Age)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject_Gender)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context_LaboratoryIdentifier)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context_LaboratoryName)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context_TechnicianIdentifier)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context_TechnicianName))
	{
		if(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Experiment_Identifier)        { m_oExperimentIdentifierHandle=m_pEBMLReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize); }
		if(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Experiment_Date)              { m_oExperimentDateHandle->set(m_pEBMLReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize)); }
		if(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject_Identifier)           { m_oSubjectIdentifierHandle=m_pEBMLReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize); }
		if(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject_Name)                 { m_oSubjectNameHandle->set(m_pEBMLReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize)); }
		if(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject_Age)                  { m_oSubjectAgeHandle=m_pEBMLReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize); }
		if(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject_Gender)               { m_oSubjectGenderHandle=m_pEBMLReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize); }
		if(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context_LaboratoryIdentifier) { m_oLaboratoryIdentifierHandle=m_pEBMLReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize); }
		if(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context_LaboratoryName)       { m_oLaboratoryNameHandle->set(m_pEBMLReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize)); }
		if(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context_TechnicianIdentifier) { m_oTechnicianIdentifierHandle=m_pEBMLReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize); }
		if(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context_TechnicianName)       { m_oTechnicianNameHandle->set(m_pEBMLReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize)); }
	}
	else
	{
		CEBMLBaseDecoder::processChildData(pBuffer, ui64BufferSize);
	}
}

void CExperimentInformationDecoder::closeChild(void)
{
	EBML::CIdentifier& l_rTop=m_vNodes.top();

	if((l_rTop==OVTK_NodeId_Header_ExperimentInformation)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Experiment)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Experiment_Identifier)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Experiment_Date)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject_Identifier)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject_Name)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject_Age)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Subject_Gender)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context_LaboratoryIdentifier)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context_LaboratoryName)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context_TechnicianIdentifier)
	 ||(l_rTop==OVTK_NodeId_Header_ExperimentInformation_Context_TechnicianName))
	{
	}
	else
	{
		CEBMLBaseDecoder::closeChild();
	}

	m_vNodes.pop();
}
