#include "ovtkIStreamedMatrixReaderCallbackHelper.h"

#include <ebml/IReader.h>
#include <ebml/IReaderHelper.h>

#include <vector>
#include <string>
#include <stack>

using namespace OpenViBEToolkit;
using namespace OpenViBE;
using namespace std;

// ________________________________________________________________________________________________________________
//

namespace OpenViBEToolkit
{
	class CStreamedMatrixReaderCallbackHelper : virtual public IStreamedMatrixReaderCallbackHelper
	{
	public:

		enum
		{
			Status_ParsingNothing,
			Status_ParsingHeader,
			Status_ParsingBuffer,
			Status_ParsingDimmension,
		};

		CStreamedMatrixReaderCallbackHelper(ICallback &rCallback);
		virtual ~CStreamedMatrixReaderCallbackHelper(void);

		virtual EBML::boolean isMasterChild(const EBML::CIdentifier& rIdentifier);
		virtual void openChild(const EBML::CIdentifier& rIdentifier);
		virtual void processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize);
		virtual void closeChild(void);

		_IsDerivedFromClass_Final_(IStreamedMatrixReaderCallbackHelper, OVTK_ClassId_);

	protected:

		virtual void sendHeaderToOwner(void);
		virtual void sendBufferToOwner(void);

	protected:

		ICallback& m_rCallback;
		uint32 m_ui32Status;

		stack<EBML::CIdentifier> m_vNodes;
		EBML::IReaderHelper* m_pReaderHelper;

		const float64* m_pMatrixBuffer;
		uint32 m_ui32DimmensionCount;
		vector<uint32> m_vDimmensionSize;
		vector<vector< string > > m_vDimmensionLabel;
		vector<string> m_vCurrentDimmensionLabel;
	};
};

// ________________________________________________________________________________________________________________
//

CStreamedMatrixReaderCallbackHelper::CStreamedMatrixReaderCallbackHelper(ICallback &rCallback)
	:m_rCallback(rCallback)
	,m_ui32Status(Status_ParsingNothing)
	,m_pReaderHelper(NULL)
	,m_pMatrixBuffer(NULL)
	,m_ui32DimmensionCount(0)
{
	m_pReaderHelper=EBML::createReaderHelper();
}

CStreamedMatrixReaderCallbackHelper::~CStreamedMatrixReaderCallbackHelper(void)
{
	m_pReaderHelper->release();
	m_pReaderHelper=NULL;
}

// ________________________________________________________________________________________________________________
//

EBML::boolean CStreamedMatrixReaderCallbackHelper::isMasterChild(const EBML::CIdentifier& rIdentifier)
{
	// ...

	     if(rIdentifier==OVTK_NodeId_StreamedMatrix_Header)     return true;
	else if(rIdentifier==OVTK_NodeId_StreamedMatrix_Dimmension) return true;
	else if(rIdentifier==OVTK_NodeId_StreamedMatrix_Buffer)     return true;
	return false;
}

void CStreamedMatrixReaderCallbackHelper::openChild(const EBML::CIdentifier& rIdentifier)
{
	m_vNodes.push(rIdentifier);

	EBML::CIdentifier& l_rTop=m_vNodes.top();

	// ...

	if(l_rTop==OVTK_NodeId_StreamedMatrix_Header && m_ui32Status==Status_ParsingNothing)
	{
		m_ui32Status=Status_ParsingHeader;
		m_ui32DimmensionCount=0;
		m_vDimmensionSize.clear();
		m_vDimmensionLabel.clear();
	}
	else if(l_rTop==OVTK_NodeId_StreamedMatrix_Dimmension && m_ui32Status==Status_ParsingHeader)
	{
		m_ui32Status=Status_ParsingDimmension;
		m_vCurrentDimmensionLabel.clear();
	}
	else if(l_rTop==OVTK_NodeId_StreamedMatrix_Buffer && m_ui32Status==Status_ParsingNothing)
	{
		m_ui32Status=Status_ParsingBuffer;
	}
}

void CStreamedMatrixReaderCallbackHelper::processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize)
{
	EBML::CIdentifier& l_rTop=m_vNodes.top();

	// ...

	switch(m_ui32Status)
	{
		case Status_ParsingHeader:
			if(l_rTop==OVTK_NodeId_StreamedMatrix_DimmensionCount)  { m_ui32DimmensionCount=static_cast<uint32>(m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize)); }
			break;

		case Status_ParsingDimmension:
			if(l_rTop==OVTK_NodeId_StreamedMatrix_Dimmension_Size)  { m_vDimmensionSize.push_back(static_cast<uint32>(m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize))); }
			if(l_rTop==OVTK_NodeId_StreamedMatrix_Dimmension_Label) { m_vCurrentDimmensionLabel.push_back(m_pReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize)); }
			break;

		case Status_ParsingBuffer:
			if(l_rTop==OVTK_NodeId_StreamedMatrix_RawBuffer)        { m_pMatrixBuffer=static_cast<const float64*>(pBuffer); sendBufferToOwner(); }
			break;
	}
}

void CStreamedMatrixReaderCallbackHelper::closeChild(void)
{
	EBML::CIdentifier& l_rTop=m_vNodes.top();

	// ...

	if(l_rTop==OVTK_NodeId_StreamedMatrix_Buffer && m_ui32Status==Status_ParsingBuffer)
	{
		m_ui32Status=Status_ParsingNothing;
	}
	else if(l_rTop==OVTK_NodeId_StreamedMatrix_Dimmension && m_ui32Status==Status_ParsingDimmension)
	{
		m_ui32Status=Status_ParsingHeader;
		m_vDimmensionLabel.push_back(m_vCurrentDimmensionLabel);
	}
	else if(l_rTop==OVTK_NodeId_StreamedMatrix_Header && m_ui32Status==Status_ParsingHeader)
	{
		m_ui32Status=Status_ParsingNothing;
		sendHeaderToOwner();
	}

	m_vNodes.pop();
}

// ________________________________________________________________________________________________________________
//

void CStreamedMatrixReaderCallbackHelper::sendHeaderToOwner(void)
{
	// should send header to owner
	m_rCallback.setMatrixDimmensionCount(m_ui32DimmensionCount);
	for(uint32 i=0; i<m_ui32DimmensionCount; i++)
	{
		if(i<m_vDimmensionSize.size())
		{
			m_rCallback.setMatrixDimmensionSize(i, m_vDimmensionSize[i]);
			for(uint32 j=0; j<m_vDimmensionSize[i]; j++)
			{
				if(i<m_vDimmensionLabel.size())
				{
					if(j<m_vDimmensionLabel[i].size())
					{
						m_rCallback.setMatrixDimmensionLabel(i, j, m_vDimmensionLabel[i][j].c_str());
					}
					else
					{
						m_rCallback.setMatrixDimmensionLabel(i, j, "");
					}
				}
				else
				{
					// this reflects a problem !
					m_rCallback.setMatrixDimmensionLabel(i, j, "");
				}
			}
		}
		else
		{
			// this reflects a problem !
			m_rCallback.setMatrixDimmensionSize(i, 0);
		}
	}
}

void CStreamedMatrixReaderCallbackHelper::sendBufferToOwner(void)
{
	m_rCallback.setMatrixBuffer(m_pMatrixBuffer);
}

// ________________________________________________________________________________________________________________
//

IStreamedMatrixReaderCallbackHelper* OpenViBEToolkit::createStreamedMatrixReaderCallbackHelper(IStreamedMatrixReaderCallbackHelper::ICallback& rCallback)
{
	return new CStreamedMatrixReaderCallbackHelper(rCallback);
}

void OpenViBEToolkit::releaseStreamedMatrixReaderCallbackHelper(OpenViBEToolkit::IStreamedMatrixReaderCallbackHelper* pStreamedMatrixReaderCallbackHelper)
{
	delete pStreamedMatrixReaderCallbackHelper;
}

// ________________________________________________________________________________________________________________
//
