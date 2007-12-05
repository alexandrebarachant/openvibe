#include "ovtkMatrixManipulation.h"

#include <system/Memory.h>

using namespace OpenViBE;

boolean OpenViBEToolkit::Tools::MatrixManipulation::copy(IMatrix& rDestinationMatrix, const IMatrix& rSourceMatrix)
{
	if(!copyDescription(rDestinationMatrix, rSourceMatrix))
	{
		return false;
	}
	if(!copyContent(rDestinationMatrix, rSourceMatrix))
	{
		return false;
	}
	return true;
}

boolean OpenViBEToolkit::Tools::MatrixManipulation::copyDescription(IMatrix& rDestinationMatrix, const IMatrix& rSourceMatrix)
{
	uint32 l_ui32DimensionCount=rSourceMatrix.getDimensionCount();
	uint32 l_ui32DimensionSize=0;
	if(!rDestinationMatrix.setDimensionCount(l_ui32DimensionCount))
	{
		return false;
	}
	for(uint32 i=0; i<l_ui32DimensionCount; i++)
	{
		l_ui32DimensionSize=rSourceMatrix.getDimensionSize(i);
		if(!rDestinationMatrix.setDimensionSize(i, l_ui32DimensionSize))
		{
			return false;
		}
		for(uint32 j=0; j<l_ui32DimensionSize; j++)
		{
			if(!rDestinationMatrix.setDimensionLabel(i, j, rSourceMatrix.getDimensionLabel(i, j)))
			{
				return false;
			}
		}
	}
	return true;
}

boolean OpenViBEToolkit::Tools::MatrixManipulation::copyContent(IMatrix& rDestinationMatrix, const IMatrix& rSourceMatrix)
{
	uint32 l_ui32SourceElementCount=rSourceMatrix.getBufferElementCount();
	uint32 l_ui32DestinationElementCount=rDestinationMatrix.getBufferElementCount();
	if(l_ui32DestinationElementCount != l_ui32SourceElementCount)
	{
		return false;
	}
	const float64* l_pSourceBuffer=rSourceMatrix.getBuffer();
	float64* l_pDestinationBuffer=rDestinationMatrix.getBuffer();
	System::Memory::copy(l_pDestinationBuffer, l_pSourceBuffer, l_ui32SourceElementCount*sizeof(float64));
	return true;
}

boolean OpenViBEToolkit::Tools::MatrixManipulation::clearContent(IMatrix& rMatrix)
{
	System::Memory::set(rMatrix.getBuffer(), rMatrix.getBufferElementCount()*sizeof(float64), 0);
	return true;
}
