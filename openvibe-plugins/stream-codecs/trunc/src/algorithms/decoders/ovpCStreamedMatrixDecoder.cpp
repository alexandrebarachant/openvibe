#include "ovpCStreamedMatrixDecoder.h"

#include <system/Memory.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::StreamCodecs;

CStreamedMatrixDecoder::CStreamedMatrixDecoder(void)
	:m_ui32Status(Status_ParsingNothing)
	,m_ui64MatrixBufferSize(0)
{
}

// ________________________________________________________________________________________________________________
//

boolean CStreamedMatrixDecoder::initialize(void)
{
	CEBMLBaseDecoder::initialize();

	m_oMatrixHandle.initialize(getOutputParameter(OVP_StreamedMatrixDecoder_Matrix_OutParameterId));

	return true;
}

boolean CStreamedMatrixDecoder::uninitialize(void)
{
	m_oMatrixHandle.uninitialize();

	CEBMLBaseDecoder::uninitialize();

	return true;
}

// ________________________________________________________________________________________________________________
//

EBML::boolean CStreamedMatrixDecoder::isMasterChild(const EBML::CIdentifier& rIdentifier)
{
	     if(rIdentifier==OVTK_NodeId_Header_StreamedMatrix)                  return true;
	else if(rIdentifier==OVTK_NodeId_Header_StreamedMatrix_Dimension)       return true;
	else if(rIdentifier==OVTK_NodeId_Header_StreamedMatrix_DimensionCount)  return false;
	else if(rIdentifier==OVTK_NodeId_Header_StreamedMatrix_Dimension_Size)  return false;
	else if(rIdentifier==OVTK_NodeId_Header_StreamedMatrix_Dimension_Label) return false;
	else if(rIdentifier==OVTK_NodeId_Buffer_StreamedMatrix)                  return true;
	else if(rIdentifier==OVTK_NodeId_Buffer_StreamedMatrix_RawBuffer)        return false;
	return CEBMLBaseDecoder::isMasterChild(rIdentifier);
}

void CStreamedMatrixDecoder::openChild(const EBML::CIdentifier& rIdentifier)
{
	m_vNodes.push(rIdentifier);

	EBML::CIdentifier& l_rTop=m_vNodes.top();

	if((l_rTop==OVTK_NodeId_Header_StreamedMatrix)
	 ||(l_rTop==OVTK_NodeId_Header_StreamedMatrix_Dimension)
	 ||(l_rTop==OVTK_NodeId_Header_StreamedMatrix_DimensionCount)
	 ||(l_rTop==OVTK_NodeId_Header_StreamedMatrix_Dimension_Size)
	 ||(l_rTop==OVTK_NodeId_Header_StreamedMatrix_Dimension_Label)
	 ||(l_rTop==OVTK_NodeId_Buffer_StreamedMatrix)
	 ||(l_rTop==OVTK_NodeId_Buffer_StreamedMatrix_RawBuffer))
	{
		if(l_rTop==OVTK_NodeId_Header_StreamedMatrix && m_ui32Status==Status_ParsingNothing)
		{
			m_ui32Status=Status_ParsingHeader;
			m_ui32DimensionIndex=0;
		}
		else if(l_rTop==OVTK_NodeId_Header_StreamedMatrix_Dimension && m_ui32Status==Status_ParsingHeader)
		{
			m_ui32Status=Status_ParsingDimension;
			m_ui32DimensionEntryIndex=0;
		}
		else if(l_rTop==OVTK_NodeId_Buffer_StreamedMatrix && m_ui32Status==Status_ParsingNothing)
		{
			m_ui32Status=Status_ParsingBuffer;
		}
	}
	else
	{
		CEBMLBaseDecoder::openChild(rIdentifier);
	}
}

void CStreamedMatrixDecoder::processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize)
{
	EBML::CIdentifier& l_rTop=m_vNodes.top();

	if((l_rTop==OVTK_NodeId_Header_StreamedMatrix)
	 ||(l_rTop==OVTK_NodeId_Header_StreamedMatrix_Dimension)
	 ||(l_rTop==OVTK_NodeId_Header_StreamedMatrix_DimensionCount)
	 ||(l_rTop==OVTK_NodeId_Header_StreamedMatrix_Dimension_Size)
	 ||(l_rTop==OVTK_NodeId_Header_StreamedMatrix_Dimension_Label)
	 ||(l_rTop==OVTK_NodeId_Buffer_StreamedMatrix)
	 ||(l_rTop==OVTK_NodeId_Buffer_StreamedMatrix_RawBuffer))
	{
		switch(m_ui32Status)
		{
			case Status_ParsingHeader:
				if(l_rTop==OVTK_NodeId_Header_StreamedMatrix_DimensionCount)  { m_oMatrixHandle->setDimensionCount(m_pEBMLReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize)); }
				break;

			case Status_ParsingDimension:
				if(l_rTop==OVTK_NodeId_Header_StreamedMatrix_Dimension_Size)  { m_oMatrixHandle->setDimensionSize(m_ui32DimensionIndex, m_pEBMLReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize)); }
				if(l_rTop==OVTK_NodeId_Header_StreamedMatrix_Dimension_Label) { m_oMatrixHandle->setDimensionLabel(m_ui32DimensionIndex, m_ui32DimensionEntryIndex++, m_pEBMLReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize)); }
				break;

			case Status_ParsingBuffer:
				if(l_rTop==OVTK_NodeId_Buffer_StreamedMatrix_RawBuffer)        { System::Memory::copy(m_oMatrixHandle->getBuffer(), pBuffer, m_ui64MatrixBufferSize*sizeof(float64)); }
				break;
		}
	}
	else
	{
		CEBMLBaseDecoder::processChildData(pBuffer, ui64BufferSize);
	}
}

void CStreamedMatrixDecoder::closeChild(void)
{
	EBML::CIdentifier& l_rTop=m_vNodes.top();

	if((l_rTop==OVTK_NodeId_Header_StreamedMatrix)
	 ||(l_rTop==OVTK_NodeId_Header_StreamedMatrix_Dimension)
	 ||(l_rTop==OVTK_NodeId_Header_StreamedMatrix_DimensionCount)
	 ||(l_rTop==OVTK_NodeId_Header_StreamedMatrix_Dimension_Size)
	 ||(l_rTop==OVTK_NodeId_Header_StreamedMatrix_Dimension_Label)
	 ||(l_rTop==OVTK_NodeId_Buffer_StreamedMatrix)
	 ||(l_rTop==OVTK_NodeId_Buffer_StreamedMatrix_RawBuffer))
	{
		if(l_rTop==OVTK_NodeId_Buffer_StreamedMatrix && m_ui32Status==Status_ParsingBuffer)
		{
			m_ui32Status=Status_ParsingNothing;
		}
		else if(l_rTop==OVTK_NodeId_Header_StreamedMatrix_Dimension && m_ui32Status==Status_ParsingDimension)
		{
			m_ui32Status=Status_ParsingHeader;
			m_ui32DimensionIndex++;
		}
		else if(l_rTop==OVTK_NodeId_Header_StreamedMatrix && m_ui32Status==Status_ParsingHeader)
		{
			m_ui32Status=Status_ParsingNothing;

			if(m_oMatrixHandle->getDimensionCount()==0)
			{
				m_ui64MatrixBufferSize=0;
			}
			else
			{
				m_ui64MatrixBufferSize=1;
				for(uint64 i=0; i<m_oMatrixHandle->getDimensionCount(); i++)
				{
					m_ui64MatrixBufferSize*=m_oMatrixHandle->getDimensionSize(i);
				}
			}
		}
	}
	else
	{
		CEBMLBaseDecoder::closeChild();
	}

	m_vNodes.pop();
}
