#include "ovtkIBoxAlgorithmExperimentInformationOutputWriter.h"
#include "ovtkTBoxAlgorithmOutputWriter.h"

#include <string>

#define OVTK_Version_ExperimentInformationOutputWriter 1

using namespace OpenViBE;
using namespace OpenViBEToolkit;
using namespace std;

#define _NoValueI_ 0xffffffff
#define _NoValueS_ "_unspecified_"

// ________________________________________________________________________________________________________________
//

namespace OpenViBEToolkit
{
	class CBoxAlgorithmExperimentInformationOutputWriter : virtual public TBoxAlgorithmOutputWriter<IBoxAlgorithmExperimentInformationOutputWriter>
	{
	public:

		CBoxAlgorithmExperimentInformationOutputWriter(void);
		virtual ~CBoxAlgorithmExperimentInformationOutputWriter(void);

		virtual boolean setValue(const uint32 ui32ValueIdentifier, const uint32 ui32Value);
		virtual boolean setValue(const uint32 ui32ValueIdentifier, const char* sValue);

		virtual boolean writeHeader(EBML::IWriter& rWriter);
		virtual boolean writeBuffer(EBML::IWriter& rWriter);

		_IsDerivedFromClass_Final_(TBoxAlgorithmOutputWriter<IBoxAlgorithmExperimentInformationOutputWriter>, OVTK_ClassId_);

	protected:

		uint32 m_ui32ExperimentIdentifier;
		string m_sExperimentDate;

		uint32 m_ui32SubjectIdentifier;
		string m_sSubjectName;
		uint32 m_ui32SubjectAge;
		uint32 m_ui32SubjectSex;

		uint32 m_ui32LaboratoryIdentifier;
		string m_sLaboratoryName;
		uint32 m_ui32TechnicianIdentifier;
		string m_sTechnicianName;
	};
};

// ________________________________________________________________________________________________________________
//

CBoxAlgorithmExperimentInformationOutputWriter::CBoxAlgorithmExperimentInformationOutputWriter(void)
	:m_ui32ExperimentIdentifier(_NoValueI_)
	,m_sExperimentDate(_NoValueS_)
	,m_ui32SubjectIdentifier(_NoValueI_)
	,m_sSubjectName(_NoValueS_)
	,m_ui32SubjectAge(_NoValueI_)
	,m_ui32SubjectSex(_NoValueI_)
	,m_ui32LaboratoryIdentifier(_NoValueI_)
	,m_sLaboratoryName(_NoValueS_)
	,m_ui32TechnicianIdentifier(_NoValueI_)
	,m_sTechnicianName(_NoValueS_)
{
}

CBoxAlgorithmExperimentInformationOutputWriter::~CBoxAlgorithmExperimentInformationOutputWriter(void)
{
}

// ________________________________________________________________________________________________________________
//

boolean CBoxAlgorithmExperimentInformationOutputWriter::setValue(const uint32 ui32ValueIdentifier, const uint32 ui32Value)
{
	switch(ui32ValueIdentifier)
	{
		case Value_ExperimentIdentifier: m_ui32ExperimentIdentifier=ui32Value; break;
		case Value_SubjectIdentifier:    m_ui32SubjectIdentifier   =ui32Value; break;
		case Value_SubjectAge:           m_ui32SubjectAge          =ui32Value; break;
		case Value_SubjectSex:           m_ui32SubjectSex          =ui32Value; break;
		case Value_LaboratoryIdentifier: m_ui32LaboratoryIdentifier=ui32Value; break;
		case Value_TechnicianIdentifier: m_ui32TechnicianIdentifier=ui32Value; break;
		default : return false;
	}
	return true;
}

boolean CBoxAlgorithmExperimentInformationOutputWriter::setValue(const uint32 ui32ValueIdentifier, const char* sValue)
{
	switch(ui32ValueIdentifier)
	{
		case Value_ExperimentDate: m_sExperimentDate=sValue; break;
		case Value_SubjectName:    m_sSubjectName   =sValue; break;
		case Value_LaboratoryName: m_sLaboratoryName=sValue; break;
		case Value_TechnicianName: m_sTechnicianName=sValue; break;
		default : return false;
	}
	return true;
}

// ________________________________________________________________________________________________________________
//

boolean CBoxAlgorithmExperimentInformationOutputWriter::writeHeader(EBML::IWriter& rWriter)
{
	m_pWriterHelper->connect(&rWriter);
	 m_pWriterHelper->openChild(OVTK_NodeId_ExperimentInformation_Header);

	  m_pWriterHelper->openChild(OVTK_NodeId_StreamVersion);
	   m_pWriterHelper->setUIntegerAsChildData(OVTK_Version_ExperimentInformationOutputWriter);
	  m_pWriterHelper->closeChild();

	  m_pWriterHelper->openChild(OVTK_NodeId_ExperimentInformation_Experiment);
		if(m_ui32ExperimentIdentifier!=_NoValueI_)
		{
		   m_pWriterHelper->openChild(OVTK_NodeId_ExperimentInformation_Experiment_Identifier);
		    m_pWriterHelper->setUIntegerAsChildData(m_ui32ExperimentIdentifier);
		   m_pWriterHelper->closeChild();
		}
		if(m_sExperimentDate!=_NoValueS_)
		{
		   m_pWriterHelper->openChild(OVTK_NodeId_ExperimentInformation_Experiment_Date);
		    m_pWriterHelper->setASCIIStringAsChildData(m_sExperimentDate.c_str());
		   m_pWriterHelper->closeChild();
		}
	  m_pWriterHelper->closeChild();

	  m_pWriterHelper->openChild(OVTK_NodeId_ExperimentInformation_Subject);
		if(m_ui32SubjectIdentifier!=_NoValueI_)
		{
		   m_pWriterHelper->openChild(OVTK_NodeId_ExperimentInformation_Subject_Identifier);
		    m_pWriterHelper->setUIntegerAsChildData(m_ui32SubjectIdentifier);
		   m_pWriterHelper->closeChild();
		}
		if(m_sSubjectName!=_NoValueS_)
		{
		   m_pWriterHelper->openChild(OVTK_NodeId_ExperimentInformation_Subject_Name);
		    m_pWriterHelper->setASCIIStringAsChildData(m_sSubjectName.c_str());
		   m_pWriterHelper->closeChild();
		}
		if(m_ui32SubjectAge!=_NoValueI_)
		{
		   m_pWriterHelper->openChild(OVTK_NodeId_ExperimentInformation_Subject_Age);
		    m_pWriterHelper->setUIntegerAsChildData(m_ui32SubjectAge);
		   m_pWriterHelper->closeChild();
		}
		if(m_ui32SubjectSex!=_NoValueI_)
		{
		   m_pWriterHelper->openChild(OVTK_NodeId_ExperimentInformation_Subject_Sex);
		    m_pWriterHelper->setUIntegerAsChildData(m_ui32SubjectSex);
		   m_pWriterHelper->closeChild();
		}
	  m_pWriterHelper->closeChild();

	  m_pWriterHelper->openChild(OVTK_NodeId_ExperimentInformation_Context);
		if(m_ui32LaboratoryIdentifier!=_NoValueI_)
		{
		   m_pWriterHelper->openChild(OVTK_NodeId_ExperimentInformation_Context_LaboratoryIdentifier);
		    m_pWriterHelper->setUIntegerAsChildData(m_ui32LaboratoryIdentifier);
		   m_pWriterHelper->closeChild();
		}
		if(m_sLaboratoryName!=_NoValueS_)
		{
		   m_pWriterHelper->openChild(OVTK_NodeId_ExperimentInformation_Context_LaboratoryName);
		    m_pWriterHelper->setASCIIStringAsChildData(m_sLaboratoryName.c_str());
		   m_pWriterHelper->closeChild();
		}
		if(m_ui32TechnicianIdentifier!=_NoValueI_)
		{
		   m_pWriterHelper->openChild(OVTK_NodeId_ExperimentInformation_Context_TechnicianIdentifier);
		    m_pWriterHelper->setUIntegerAsChildData(m_ui32TechnicianIdentifier);
		   m_pWriterHelper->closeChild();
		}
		if(m_sTechnicianName!=_NoValueS_)
		{
		   m_pWriterHelper->openChild(OVTK_NodeId_ExperimentInformation_Context_TechnicianName);
		    m_pWriterHelper->setASCIIStringAsChildData(m_sTechnicianName.c_str());
		   m_pWriterHelper->closeChild();
		}
	  m_pWriterHelper->closeChild();

	 m_pWriterHelper->closeChild();
	m_pWriterHelper->disconnect();
	return true;
}

boolean CBoxAlgorithmExperimentInformationOutputWriter::writeBuffer(EBML::IWriter& rWriter)
{
	m_pWriterHelper->connect(&rWriter);
/*
	 m_pWriterHelper->openChild(OVTK_NodeId_ ...);
	  m_pWriterHelper->setChildData(... ...);
	 m_pWriterHelper->closeChild();
*/
	m_pWriterHelper->disconnect();

	return true;
}

// ________________________________________________________________________________________________________________
//

IBoxAlgorithmExperimentInformationOutputWriter* OpenViBEToolkit::createBoxAlgorithmExperimentInformationOutputWriter(void)
{
	return new CBoxAlgorithmExperimentInformationOutputWriter();
}

void OpenViBEToolkit::releaseBoxAlgorithmExperimentInformationOutputWriter(IBoxAlgorithmExperimentInformationOutputWriter* pOutputWriter)
{
	delete pOutputWriter;
}

// ________________________________________________________________________________________________________________
//
