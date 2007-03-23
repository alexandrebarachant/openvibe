#include "ovtkIBoxAlgorithmSignalInputReaderCallback.h"
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
	class CBoxAlgorithmSignalInputReaderCallback : virtual public IBoxAlgorithmSignalInputReaderCallback
	{
	public:

		CBoxAlgorithmSignalInputReaderCallback(ICallback& rCallback);
		virtual ~CBoxAlgorithmSignalInputReaderCallback(void);

		virtual EBML::boolean isMasterChild(const EBML::CIdentifier& rIdentifier);
		virtual void openChild(const EBML::CIdentifier& rIdentifier);
		virtual void processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize);
		virtual void closeChild(void);

		virtual void setMatrixDimmensionCount(const OpenViBE::uint32 ui32DimmensionCount);
		virtual void setMatrixDimmensionSize(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionSize);
		virtual void setMatrixDimmensionLabel(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel);
		virtual void setMatrixBuffer(const OpenViBE::float64* pBuffer);

		_IsDerivedFromClass_Final_(IBoxAlgorithmSignalInputReaderCallback, OVTK_ClassId_);

	protected:

		ICallback& m_rCallback;
		IStreamedMatrixReaderCallbackHelper* m_pStreamedMatrixReaderCallbackHelper;
		IStreamedMatrixReaderCallbackHelper::TCallbackProxy1<CBoxAlgorithmSignalInputReaderCallback> m_oCallbackProxy;

		stack<EBML::CIdentifier> m_vNodes;
		EBML::IReaderHelper* m_pReaderHelper;
	};
};

// ________________________________________________________________________________________________________________
//

CBoxAlgorithmSignalInputReaderCallback::CBoxAlgorithmSignalInputReaderCallback(IBoxAlgorithmSignalInputReaderCallback::ICallback& rCallback)
	:m_rCallback(rCallback)
	,m_pStreamedMatrixReaderCallbackHelper(NULL)
	,m_oCallbackProxy(*this, &CBoxAlgorithmSignalInputReaderCallback::setMatrixDimmensionCount, &CBoxAlgorithmSignalInputReaderCallback::setMatrixDimmensionSize, &CBoxAlgorithmSignalInputReaderCallback::setMatrixDimmensionLabel, &CBoxAlgorithmSignalInputReaderCallback::setMatrixBuffer)
	,m_pReaderHelper(NULL)
{
	m_pStreamedMatrixReaderCallbackHelper=createStreamedMatrixReaderCallbackHelper(m_oCallbackProxy);
	m_pReaderHelper=EBML::createReaderHelper();
}

CBoxAlgorithmSignalInputReaderCallback::~CBoxAlgorithmSignalInputReaderCallback(void)
{
	releaseStreamedMatrixReaderCallbackHelper(m_pStreamedMatrixReaderCallbackHelper);
	m_pStreamedMatrixReaderCallbackHelper=NULL;
	m_pReaderHelper->release();
	m_pReaderHelper=NULL;
}

// ________________________________________________________________________________________________________________
//

EBML::boolean CBoxAlgorithmSignalInputReaderCallback::isMasterChild(const EBML::CIdentifier& rIdentifier)
{
	if((rIdentifier!=OVTK_NodeId_Signal_Header)
	&& (rIdentifier!=OVTK_NodeId_Signal_Buffer)
	&& (rIdentifier!=OVTK_NodeId_Signal_SamplingRate))
	{
		return m_pStreamedMatrixReaderCallbackHelper->isMasterChild(rIdentifier);
	}

	     if(rIdentifier==OVTK_NodeId_Signal_Header) { return true; }
	else if(rIdentifier==OVTK_NodeId_Signal_Buffer) { return true; }
	return false;
}

void CBoxAlgorithmSignalInputReaderCallback::openChild(const EBML::CIdentifier& rIdentifier)
{
	m_vNodes.push(rIdentifier);

	EBML::CIdentifier& l_rTop=m_vNodes.top();

	if((l_rTop!=OVTK_NodeId_Signal_Header)
	&& (l_rTop!=OVTK_NodeId_Signal_Buffer)
	&& (l_rTop!=OVTK_NodeId_Signal_SamplingRate))
	{
		m_pStreamedMatrixReaderCallbackHelper->openChild(rIdentifier);
	}
	else
	{
		// ...
	}
}

void CBoxAlgorithmSignalInputReaderCallback::processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize)
{
	EBML::CIdentifier& l_rTop=m_vNodes.top();

	if((l_rTop!=OVTK_NodeId_Signal_Header)
	&& (l_rTop!=OVTK_NodeId_Signal_Buffer)
	&& (l_rTop!=OVTK_NodeId_Signal_SamplingRate))
	{
		m_pStreamedMatrixReaderCallbackHelper->processChildData(pBuffer, ui64BufferSize);
	}
	else
	{
		// ...
		if(l_rTop==OVTK_NodeId_Signal_SamplingRate) m_rCallback.setSamplingRate(static_cast<uint32>(m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize)));
	}
}

void CBoxAlgorithmSignalInputReaderCallback::closeChild(void)
{
	EBML::CIdentifier& l_rTop=m_vNodes.top();

	if((l_rTop!=OVTK_NodeId_Signal_Header)
	&& (l_rTop!=OVTK_NodeId_Signal_Buffer)
	&& (l_rTop!=OVTK_NodeId_Signal_SamplingRate))
	{
		m_pStreamedMatrixReaderCallbackHelper->closeChild();
	}
	else
	{
		// ...
	}

	m_vNodes.pop();
}

// ________________________________________________________________________________________________________________
//

void CBoxAlgorithmSignalInputReaderCallback::setMatrixDimmensionCount(const uint32 ui32DimmensionCount)
{
	// assert(ui32DimmensionCount==2);
}

void CBoxAlgorithmSignalInputReaderCallback::setMatrixDimmensionSize(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionSize)
{
	switch(ui32DimmensionIndex)
	{
		case 0: m_rCallback.setChannelCount(ui32DimmensionSize); break;
		case 1: m_rCallback.setSampleCountPerBuffer(ui32DimmensionSize); break;
	}
}

void CBoxAlgorithmSignalInputReaderCallback::setMatrixDimmensionLabel(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel)
{
	switch(ui32DimmensionIndex)
	{
		case 0: m_rCallback.setChannelName(ui32DimmensionEntryIndex, sDimmensionLabel);
	}
}

void CBoxAlgorithmSignalInputReaderCallback::setMatrixBuffer(const float64* pBuffer)
{
	m_rCallback.setSampleBuffer(pBuffer);
}

// ________________________________________________________________________________________________________________
//

IBoxAlgorithmSignalInputReaderCallback* OpenViBEToolkit::createBoxAlgorithmSignalInputReaderCallback(IBoxAlgorithmSignalInputReaderCallback::ICallback& rCallback)
{
	return new CBoxAlgorithmSignalInputReaderCallback(rCallback);
}

void OpenViBEToolkit::releaseBoxAlgorithmSignalInputReaderCallback(OpenViBEToolkit::IBoxAlgorithmSignalInputReaderCallback* pBoxAlgorithmSignalInputReaderCallback)
{
	delete pBoxAlgorithmSignalInputReaderCallback;
}

// ________________________________________________________________________________________________________________
//
