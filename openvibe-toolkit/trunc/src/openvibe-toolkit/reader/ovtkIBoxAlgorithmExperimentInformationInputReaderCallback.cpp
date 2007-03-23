#include "ovtkIBoxAlgorithmExperimentInformationInputReaderCallback.h"

#include <ebml/IReader.h>
#include <ebml/IReaderHelper.h>

#include <stack>

using namespace OpenViBEToolkit;
using namespace OpenViBE;
using namespace std;

// ________________________________________________________________________________________________________________
//

namespace OpenViBEToolkit
{
	class CBoxAlgorithmExperimentInformationInputReaderCallback : virtual public IBoxAlgorithmExperimentInformationInputReaderCallback
	{
	public:

		CBoxAlgorithmExperimentInformationInputReaderCallback(ICallback& rCallback);
		virtual ~CBoxAlgorithmExperimentInformationInputReaderCallback(void);

		virtual EBML::boolean isMasterChild(const EBML::CIdentifier& rIdentifier);
		virtual void openChild(const EBML::CIdentifier& rIdentifier);
		virtual void processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize);
		virtual void closeChild(void);

		_IsDerivedFromClass_Final_(IBoxAlgorithmExperimentInformationInputReaderCallback, OVTK_ClassId_);

	protected:

		ICallback& m_rCallback;

		stack<EBML::CIdentifier> m_vNodes;
		EBML::IReaderHelper* m_pReaderHelper;
	};
};

// ________________________________________________________________________________________________________________
//

CBoxAlgorithmExperimentInformationInputReaderCallback::CBoxAlgorithmExperimentInformationInputReaderCallback(IBoxAlgorithmExperimentInformationInputReaderCallback::ICallback& rCallback)
	:m_rCallback(rCallback)
	,m_pReaderHelper(NULL)
{
	m_pReaderHelper=EBML::createReaderHelper();
}

CBoxAlgorithmExperimentInformationInputReaderCallback::~CBoxAlgorithmExperimentInformationInputReaderCallback(void)
{
	m_pReaderHelper->release();
	m_pReaderHelper=NULL;
}

// ________________________________________________________________________________________________________________
//

EBML::boolean CBoxAlgorithmExperimentInformationInputReaderCallback::isMasterChild(const EBML::CIdentifier& rIdentifier)
{
	if(rIdentifier==OVTK_NodeId_ExperimentInformation_Header)     { return true; }
	if(rIdentifier==OVTK_NodeId_ExperimentInformation_Experiment) { return true; }
	if(rIdentifier==OVTK_NodeId_ExperimentInformation_Subject)    { return true; }
	if(rIdentifier==OVTK_NodeId_ExperimentInformation_Context)    { return true; }
	return false;
}

void CBoxAlgorithmExperimentInformationInputReaderCallback::openChild(const EBML::CIdentifier& rIdentifier)
{
	m_vNodes.push(rIdentifier);

	// EBML::CIdentifier& l_rTop=m_vNodes.top();

	// ...
}

void CBoxAlgorithmExperimentInformationInputReaderCallback::processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize)
{
	EBML::CIdentifier& l_rTop=m_vNodes.top();

	// ...

	if(l_rTop==OVTK_NodeId_ExperimentInformation_Experiment_Identifier)        m_rCallback.setValue(Value_ExperimentIdentifier, m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize));
	if(l_rTop==OVTK_NodeId_ExperimentInformation_Experiment_Date)              m_rCallback.setValue(Value_ExperimentDate,       m_pReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize));

	if(l_rTop==OVTK_NodeId_ExperimentInformation_Subject_Identifier)           m_rCallback.setValue(Value_SubjectIdentifier,    m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize));
	if(l_rTop==OVTK_NodeId_ExperimentInformation_Subject_Name)                 m_rCallback.setValue(Value_SubjectName,          m_pReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize));
	if(l_rTop==OVTK_NodeId_ExperimentInformation_Subject_Age)                  m_rCallback.setValue(Value_SubjectAge,           m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize));
	if(l_rTop==OVTK_NodeId_ExperimentInformation_Subject_Sex)                  m_rCallback.setValue(Value_SubjectSex,           m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize));

	if(l_rTop==OVTK_NodeId_ExperimentInformation_Context_LaboratoryIdentifier) m_rCallback.setValue(Value_LaboratoryIdentifier, m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize));
	if(l_rTop==OVTK_NodeId_ExperimentInformation_Context_LaboratoryName)       m_rCallback.setValue(Value_LaboratoryName,       m_pReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize));
	if(l_rTop==OVTK_NodeId_ExperimentInformation_Context_TechnicianIdentifier) m_rCallback.setValue(Value_TechnicianIdentifier, m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize));
	if(l_rTop==OVTK_NodeId_ExperimentInformation_Context_TechnicianName)       m_rCallback.setValue(Value_TechnicianName,       m_pReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize));
}

void CBoxAlgorithmExperimentInformationInputReaderCallback::closeChild(void)
{
	// EBML::CIdentifier& l_rTop=m_vNodes.top();

	// ...

	m_vNodes.pop();
}

// ________________________________________________________________________________________________________________
//

IBoxAlgorithmExperimentInformationInputReaderCallback* OpenViBEToolkit::createBoxAlgorithmExperimentInformationInputReaderCallback(IBoxAlgorithmExperimentInformationInputReaderCallback::ICallback& rCallback)
{
	return new CBoxAlgorithmExperimentInformationInputReaderCallback(rCallback);
}

void OpenViBEToolkit::releaseBoxAlgorithmExperimentInformationInputReaderCallback(OpenViBEToolkit::IBoxAlgorithmExperimentInformationInputReaderCallback* pBoxAlgorithmExperimentInformationInputReaderCallback)
{
	delete pBoxAlgorithmExperimentInformationInputReaderCallback;
}

// ________________________________________________________________________________________________________________
//
