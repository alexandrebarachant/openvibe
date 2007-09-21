#include "ovtkCBoxAlgorithmStreamedMatrixOutputWriterPart.inl"

#include <iostream>
#include <math.h>
#include <stdlib.h>

using namespace OpenViBEToolkit;
using namespace OpenViBE;

// ________________________________________________________________________________________________________________
//

CBoxAlgorithmStreamedMatrixOutputWriterPart::CBoxAlgorithmStreamedMatrixOutputWriterPart(void)
	:m_pMatrixBuffer(NULL)
	,m_ui64MatrixBufferSize(0)
	,m_ui32Status(Status_SendingNothing)
	,m_ui32DimmensionCount(0)
{
}

// ________________________________________________________________________________________________________________
//

boolean CBoxAlgorithmStreamedMatrixOutputWriterPart::setDimmensionCount(const uint32 ui32DimmensionCount)
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

boolean CBoxAlgorithmStreamedMatrixOutputWriterPart::setDimmensionSize(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionSize)
{
	if(ui32DimmensionIndex>=m_ui32DimmensionCount)
	{
		return false;
	}

	m_vDimmensionSize[ui32DimmensionIndex]=ui32DimmensionSize;
	return true;
}

boolean CBoxAlgorithmStreamedMatrixOutputWriterPart::setDimmensionLabel(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel)
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

boolean CBoxAlgorithmStreamedMatrixOutputWriterPart::setBuffer(const float64* pBuffer)
{
	m_pMatrixBuffer=pBuffer;
	return m_pMatrixBuffer!=NULL;
}

// ________________________________________________________________________________________________________________
//

boolean CBoxAlgorithmStreamedMatrixOutputWriterPart::writeHeaderStart(EBML::IWriter& rWriter)
{
	if(m_ui32Status!=Status_SendingNothing)
	{
		return false;
	}

	m_ui64MatrixBufferSize=(m_ui32DimmensionCount==0?0:sizeof(float64));

	m_ui32Status=Status_SendingHeader;

	m_oWriterHelper.connect(&rWriter);

	 m_oWriterHelper.openChild(OVTK_NodeId_Header_StreamedMatrix);
	  m_oWriterHelper.openChild(OVTK_NodeId_Header_StreamedMatrix_DimmensionCount);
	   m_oWriterHelper.setUIntegerAsChildData(m_ui32DimmensionCount);
	  m_oWriterHelper.closeChild();
	  for(uint32 i=0; i<m_ui32DimmensionCount; i++)
	  {
	   m_ui64MatrixBufferSize*=m_vDimmensionSize[i];
	   m_oWriterHelper.openChild(OVTK_NodeId_Header_StreamedMatrix_Dimmension);
	    m_oWriterHelper.openChild(OVTK_NodeId_Header_StreamedMatrix_Dimmension_Size);
	     m_oWriterHelper.setUIntegerAsChildData(m_vDimmensionSize[i]);
	    m_oWriterHelper.closeChild();
	    for(uint32 j=0; j<m_vDimmensionSize[i]; j++)
	    {
	     m_oWriterHelper.openChild(OVTK_NodeId_Header_StreamedMatrix_Dimmension_Label);
	      m_oWriterHelper.setASCIIStringAsChildData(m_vDimmensionLabel[i][j].c_str());
	     m_oWriterHelper.closeChild();
	    }
	   m_oWriterHelper.closeChild();
	  }

	m_oWriterHelper.disconnect();
	return true;
}

boolean CBoxAlgorithmStreamedMatrixOutputWriterPart::writeHeaderStop(EBML::IWriter& rWriter)
{
	m_oWriterHelper.connect(&rWriter);

	 m_oWriterHelper.closeChild();

	m_oWriterHelper.disconnect();

	m_ui32Status=Status_SendingNothing;
	return true;
}

boolean CBoxAlgorithmStreamedMatrixOutputWriterPart::writeBufferStart(EBML::IWriter& rWriter)
{
	if(m_ui32Status!=Status_SendingNothing)
	{
		return false;
	}

	for(uint32 i=0; i<m_vDimmensionSize[0]; i++)
		for(uint32 j=0; j<m_vDimmensionSize[1]; j++)
		{
			if(isnan(m_pMatrixBuffer[i*m_vDimmensionSize[1]+j]))
			{
				std::cout << "YR-NAAAAAAAAAAAAAAAAAAAAAAAN-(c) !!! (replaced by 0)" << std::endl;
				const_cast<float64*>(m_pMatrixBuffer)[i*m_vDimmensionSize[1]+j]=0;
			}
		}

	m_ui32Status=Status_SendingBuffer;

	m_oWriterHelper.connect(&rWriter);

	 m_oWriterHelper.openChild(OVTK_NodeId_Buffer_StreamedMatrix);
	  m_oWriterHelper.openChild(OVTK_NodeId_Buffer_StreamedMatrix_RawBuffer);
	   m_oWriterHelper.setBinaryAsChildData(m_pMatrixBuffer, m_ui64MatrixBufferSize);
	  m_oWriterHelper.closeChild();

	m_oWriterHelper.disconnect();
	return true;
}

boolean CBoxAlgorithmStreamedMatrixOutputWriterPart::writeBufferStop(EBML::IWriter& rWriter)
{
	m_oWriterHelper.connect(&rWriter);

	 m_oWriterHelper.closeChild();

	m_oWriterHelper.disconnect();

	m_ui32Status=Status_SendingNothing;
	return true;
}
