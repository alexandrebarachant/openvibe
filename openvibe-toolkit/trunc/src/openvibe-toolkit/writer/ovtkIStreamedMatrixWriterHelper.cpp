#include "ovtkIStreamedMatrixWriterHelper.h"

#include <ebml/IWriter.h>
#include <ebml/IWriterHelper.h>

#include <map>
#include <string>
#include <iostream>

using namespace OpenViBE;
using namespace OpenViBEToolkit;
using namespace std;

// ________________________________________________________________________________________________________________
//

namespace OpenViBEToolkit
{
	class CStreamedMatrixWriterHelper : virtual public OpenViBEToolkit::IStreamedMatrixWriterHelper
	{
	public:

		enum
		{
			Status_SendingNothing,
			Status_SendingHeader,
			Status_SendingBuffer,
		};

		CStreamedMatrixWriterHelper(void);
		virtual ~CStreamedMatrixWriterHelper(void);

		virtual boolean setDimmensionCount(const uint32 ui32DimmensionCount);
		virtual boolean setDimmensionSize(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionSize);
		virtual boolean setDimmensionLabel(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel);
		virtual boolean setBuffer(const float64* pBuffer);

		virtual boolean writeHeader(EBML::IWriter& rWriter);
		virtual boolean writeBuffer(EBML::IWriter& rWriter);

		_IsDerivedFromClass_Final_(OpenViBEToolkit::IStreamedMatrixWriterHelper, OVTK_ClassId_);

	protected:

		EBML::IWriter* m_pWriter;
		EBML::IWriterHelper* m_pWriterHelper;

		const float64* m_pMatrixBuffer;
		uint64 m_ui64MatrixBufferSize;

		uint32 m_ui32DimmensionCount;
		map<uint32, uint32> m_vDimmensionSize;
		map<uint32, map< uint32, string > > m_vDimmensionLabel;

		uint32 m_ui32Status;
	};
};

// ________________________________________________________________________________________________________________
//

CStreamedMatrixWriterHelper::CStreamedMatrixWriterHelper(void)
	:m_pWriter(NULL)
	,m_pWriterHelper(NULL)
	,m_ui32Status(Status_SendingNothing)
	,m_ui32DimmensionCount(0)
	,m_ui64MatrixBufferSize(0)
{
	m_pWriterHelper=EBML::createWriterHelper();
}

CStreamedMatrixWriterHelper::~CStreamedMatrixWriterHelper(void)
{
	m_pWriterHelper->release();
	m_pWriterHelper=NULL;
}

// ________________________________________________________________________________________________________________
//

boolean CStreamedMatrixWriterHelper::setDimmensionCount(const uint32 ui32DimmensionCount)
{
	if(m_ui32Status!=Status_SendingNothing)
	{
		return false;
	}

	m_ui32DimmensionCount=ui32DimmensionCount;
	m_vDimmensionSize.clear();
	m_vDimmensionLabel.clear();
	m_ui64MatrixBufferSize=0;
	return true;
}

boolean CStreamedMatrixWriterHelper::setDimmensionSize(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionSize)
{
	if(ui32DimmensionIndex>=m_ui32DimmensionCount)
	{
		return false;
	}

	m_vDimmensionSize[ui32DimmensionIndex]=ui32DimmensionSize;
	return true;
}

boolean CStreamedMatrixWriterHelper::setDimmensionLabel(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel)
{
	if(ui32DimmensionIndex>=m_ui32DimmensionCount)
	{
		return false;
	}

	if(ui32DimmensionEntryIndex>=m_vDimmensionSize[ui32DimmensionIndex])
	{
		return false;
	}

	m_vDimmensionLabel[ui32DimmensionIndex][ui32DimmensionEntryIndex]=sDimmensionLabel;
	return true;
}

// ________________________________________________________________________________________________________________
//

boolean CStreamedMatrixWriterHelper::setBuffer(const float64* pBuffer)
{
	m_pMatrixBuffer=pBuffer;
	return m_pMatrixBuffer!=NULL;
}

// ________________________________________________________________________________________________________________
//

boolean CStreamedMatrixWriterHelper::writeHeader(EBML::IWriter& rWriter)
{
	if(m_ui32Status!=Status_SendingNothing || !m_pWriterHelper)
	{
		return false;
	}

	m_ui64MatrixBufferSize=(m_ui32DimmensionCount==0?0:sizeof(float64));

	m_ui32Status=Status_SendingHeader;

	m_pWriterHelper->connect(&rWriter);
	 m_pWriterHelper->openChild(OVTK_NodeId_StreamedMatrix_Header);
	  m_pWriterHelper->openChild(OVTK_NodeId_StreamedMatrix_DimmensionCount);
	   m_pWriterHelper->setUIntegerAsChildData(m_ui32DimmensionCount);
	  m_pWriterHelper->closeChild();
		for(uint32 i=0; i<m_ui32DimmensionCount; i++)
		{
			m_ui64MatrixBufferSize*=m_vDimmensionSize[i];
			m_pWriterHelper->openChild(OVTK_NodeId_StreamedMatrix_Dimmension);
			 m_pWriterHelper->openChild(OVTK_NodeId_StreamedMatrix_Dimmension_Size);
			  m_pWriterHelper->setUIntegerAsChildData(m_vDimmensionSize[i]);
			 m_pWriterHelper->closeChild();
				for(uint32 j=0; j<m_vDimmensionSize[i]; j++)
				{
					m_pWriterHelper->openChild(OVTK_NodeId_StreamedMatrix_Dimmension_Label);
					 m_pWriterHelper->setASCIIStringAsChildData(m_vDimmensionLabel[i][j].c_str());
					m_pWriterHelper->closeChild();
				}
			m_pWriterHelper->closeChild();
		}
	 m_pWriterHelper->closeChild();
	m_pWriterHelper->disconnect();

	m_ui32Status=Status_SendingNothing;
	return true;
}

boolean CStreamedMatrixWriterHelper::writeBuffer(EBML::IWriter& rWriter)
{
	if(m_ui32Status!=Status_SendingNothing || !m_pWriterHelper)
	{
		return false;
	}

	m_ui32Status=Status_SendingBuffer;

	m_pWriterHelper->connect(&rWriter);
	 m_pWriterHelper->openChild(OVTK_NodeId_StreamedMatrix_Buffer);
	  m_pWriterHelper->openChild(OVTK_NodeId_StreamedMatrix_RawBuffer);
	   m_pWriterHelper->setBinaryAsChildData(m_pMatrixBuffer, m_ui64MatrixBufferSize);
	  m_pWriterHelper->closeChild();
	 m_pWriterHelper->closeChild();
	m_pWriterHelper->disconnect();

	m_ui32Status=Status_SendingNothing;
	return true;
}

// ________________________________________________________________________________________________________________
//

IStreamedMatrixWriterHelper* OpenViBEToolkit::createStreamedMatrixWriterHelper(void)
{
	return new CStreamedMatrixWriterHelper();
}

void OpenViBEToolkit::releaseStreamedMatrixWriterHelper(IStreamedMatrixWriterHelper* pOutputWriter)
{
	delete pOutputWriter;
}

// ________________________________________________________________________________________________________________
//
