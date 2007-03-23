#include "ovtkIBoxAlgorithmStimulationInputReaderCallback.h"
#include "ovtkIStreamedMatrixReaderCallbackHelper.h"

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
	class CBoxAlgorithmStimulationInputReaderCallback : virtual public IBoxAlgorithmStimulationInputReaderCallback
	{
	public:

		CBoxAlgorithmStimulationInputReaderCallback(ICallback& rCallback);
		virtual ~CBoxAlgorithmStimulationInputReaderCallback(void);

		virtual EBML::boolean isMasterChild(const EBML::CIdentifier& rIdentifier);
		virtual void openChild(const EBML::CIdentifier& rIdentifier);
		virtual void processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize);
		virtual void closeChild(void);

		_IsDerivedFromClass_Final_(IBoxAlgorithmStimulationInputReaderCallback, OVTK_ClassId_);

	protected:

		ICallback& m_rCallback;

		stack<EBML::CIdentifier> m_vNodes;
		EBML::IReaderHelper* m_pReaderHelper;

		uint32 m_ui32StimulationIndex;
		uint64 m_ui64StimulationIdentifier;
		uint64 m_ui64StimulationDate;
	};
};

// ________________________________________________________________________________________________________________
//

CBoxAlgorithmStimulationInputReaderCallback::CBoxAlgorithmStimulationInputReaderCallback(IBoxAlgorithmStimulationInputReaderCallback::ICallback& rCallback)
	:m_rCallback(rCallback)
	,m_pReaderHelper(NULL)
{
	m_pReaderHelper=EBML::createReaderHelper();
}

CBoxAlgorithmStimulationInputReaderCallback::~CBoxAlgorithmStimulationInputReaderCallback(void)
{
	m_pReaderHelper->release();
	m_pReaderHelper=NULL;
}

// ________________________________________________________________________________________________________________
//

EBML::boolean CBoxAlgorithmStimulationInputReaderCallback::isMasterChild(const EBML::CIdentifier& rIdentifier)
{
	     if(rIdentifier==OVTK_NodeId_Stimulation_Header)      { return true; }
	else if(rIdentifier==OVTK_NodeId_Stimulation_Buffer)      { return true; }
	else if(rIdentifier==OVTK_NodeId_Stimulation_Stimulation) { return true; }
	return false;
}

void CBoxAlgorithmStimulationInputReaderCallback::openChild(const EBML::CIdentifier& rIdentifier)
{
	m_vNodes.push(rIdentifier);

	// EBML::CIdentifier& l_rTop=m_vNodes.top();

	// ...

}

void CBoxAlgorithmStimulationInputReaderCallback::processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize)
{
	EBML::CIdentifier& l_rTop=m_vNodes.top();

	// ...

	if(l_rTop==OVTK_NodeId_Stimulation_NumberOfStimulations)   { m_rCallback.setStimulationCount(static_cast<uint32>(m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize))); m_ui32StimulationIndex=0; }
	if(l_rTop==OVTK_NodeId_Stimulation_Stimulation_Identifier) { m_ui64StimulationIdentifier=m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize); }
	if(l_rTop==OVTK_NodeId_Stimulation_Stimulation_Date)       { m_ui64StimulationDate=m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize); }
}

void CBoxAlgorithmStimulationInputReaderCallback::closeChild(void)
{
	EBML::CIdentifier& l_rTop=m_vNodes.top();

	// ...

	if(l_rTop==OVTK_NodeId_Stimulation_Stimulation) { m_rCallback.setStimulation(m_ui32StimulationIndex, m_ui64StimulationIdentifier, m_ui64StimulationDate); m_ui32StimulationIndex++; }

	m_vNodes.pop();
}

// ________________________________________________________________________________________________________________
//

IBoxAlgorithmStimulationInputReaderCallback* OpenViBEToolkit::createBoxAlgorithmStimulationInputReaderCallback(IBoxAlgorithmStimulationInputReaderCallback::ICallback& rCallback)
{
	return new CBoxAlgorithmStimulationInputReaderCallback(rCallback);
}

void OpenViBEToolkit::releaseBoxAlgorithmStimulationInputReaderCallback(OpenViBEToolkit::IBoxAlgorithmStimulationInputReaderCallback* pBoxAlgorithmStimulationInputReaderCallback)
{
	delete pBoxAlgorithmStimulationInputReaderCallback;
}

// ________________________________________________________________________________________________________________
//
