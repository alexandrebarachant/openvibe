#include "ovpCSimpleDSP.h"

#include <system/Memory.h>

#include <iostream>
#include <sstream>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessing;
using namespace OpenViBEPlugins::SignalProcessing::SimpleDSP;
using namespace OpenViBEToolkit;
using namespace std;


void CSimpleDSP::setChannelCount(const uint32 ui32ChannelCount)
{
	//gets the channel count
	m_pSignalDescription->m_ui32ChannelCount = ui32ChannelCount;
	m_pSignalDescription->m_pChannelName.resize(ui32ChannelCount);
}

void CSimpleDSP::setChannelName(const uint32 ui32ChannelIndex, const char* sChannelName)
{
	m_pSignalDescription->m_pChannelName[ui32ChannelIndex]=sChannelName;
}

void CSimpleDSP::setSampleCountPerBuffer(const uint32 ui32SampleCountPerBuffer)
{
	//gets the sample count
	m_pSignalDescription->m_ui32SampleCount = ui32SampleCountPerBuffer;

	//the matrix buffer hasn't been allocated yet, allocate it
	if(!m_pMatrixBuffer)
	{
		m_ui64MatrixBufferSize = m_pSignalDescription -> m_ui32SampleCount * m_pSignalDescription -> m_ui32ChannelCount;

		m_pMatrixBuffer = new EBML::float64[(size_t)m_ui64MatrixBufferSize];
	}

	//we have everything needed to send the stream header
	m_pSignalOutputWriterHelper->setSamplingRate(m_pSignalDescription->m_ui32SamplingRate);
	m_pSignalOutputWriterHelper->setChannelCount(m_pSignalDescription->m_ui32ChannelCount);

	for(uint32 i=0 ; i<m_pSignalDescription->m_ui32ChannelCount ; i++)
	{
		m_pSignalOutputWriterHelper->setChannelName(i, m_pSignalDescription->m_pChannelName[i].c_str());
	}

	m_pSignalOutputWriterHelper->setSampleCountPerBuffer(m_pSignalDescription->m_ui32SampleCount);
	m_pSignalOutputWriterHelper->setSampleBuffer(m_pMatrixBuffer);

	m_pSignalOutputWriterHelper->writeHeader(*m_pWriter);

	getBoxAlgorithmContext()->getDynamicBoxContext()->markOutputAsReadyToSend(0, m_ui64LastChunkStartTime, m_ui64LastChunkEndTime);


}

void CSimpleDSP::setSamplingRate(const uint32 ui32SamplingFrequency)
{
	m_pSignalDescription->m_ui32SamplingRate = ui32SamplingFrequency;
}

void CSimpleDSP::setSampleBuffer(const float64* pBuffer)
{

	switch(m_ui64EquationType)
	{
		//The equation is not a special one, we have to execute the whole stack of function calls
		case OP_USERDEF:
			//for every samples
			for(uint64 i=0 ; i<m_ui64MatrixBufferSize ; i++)
			{
				m_f64Variable = pBuffer[i];
				m_pMatrixBuffer[i] = m_pEquationParser->executeEquation();
			}
			break;

		case OP_X2:
			for(uint64 i=0 ; i<m_ui64MatrixBufferSize ; i++)
			{
				m_pMatrixBuffer[i] = pBuffer[i]*pBuffer[i];
			}
			break;

		case OP_NONE:
			System::Memory::copy(m_pMatrixBuffer, pBuffer, m_ui64MatrixBufferSize*sizeof(float64));
			break;

		case OP_ABS:
			for(uint64 i=0 ; i<m_ui64MatrixBufferSize ; i++)
			{
				m_pMatrixBuffer[i] = abs(pBuffer[i]);
			}
			break;
		case OP_ACOS:
			for(uint64 i=0 ; i<m_ui64MatrixBufferSize ; i++)
			{
				m_pMatrixBuffer[i] = acos(pBuffer[i]);
			}
			break;

		case OP_ASIN:
			for(uint64 i=0 ; i<m_ui64MatrixBufferSize ; i++)
			{
				m_pMatrixBuffer[i] = asin(pBuffer[i]);
			}
			break;

		case OP_ATAN:
			for(uint64 i=0 ; i<m_ui64MatrixBufferSize ; i++)
			{
				m_pMatrixBuffer[i] = atan(pBuffer[i]);
			}
			break;

		case OP_CEIL:
			for(uint64 i=0 ; i<m_ui64MatrixBufferSize ; i++)
			{
				m_pMatrixBuffer[i] = ceil(pBuffer[i]);
			}
			break;

		case OP_COS:
			for(uint64 i=0 ; i<m_ui64MatrixBufferSize ; i++)
			{
				m_pMatrixBuffer[i] = cos(pBuffer[i]);
			}
			break;

		case OP_EXP:
			for(uint64 i=0 ; i<m_ui64MatrixBufferSize ; i++)
			{
				m_pMatrixBuffer[i] = exp(pBuffer[i]);
			}
			break;

		case OP_FLOOR:
			for(uint64 i=0 ; i<m_ui64MatrixBufferSize ; i++)
			{
				m_pMatrixBuffer[i] = floor(pBuffer[i]);
			}
			break;

		case OP_LOG:
			for(uint64 i=0 ; i<m_ui64MatrixBufferSize ; i++)
			{
				m_pMatrixBuffer[i] = log(pBuffer[i]);
			}
			break;

		case OP_LOG10:
			for(uint64 i=0 ; i<m_ui64MatrixBufferSize ; i++)
			{
				m_pMatrixBuffer[i] = log10(pBuffer[i]);
			}
			break;

		case OP_SIN:
			for(uint64 i=0 ; i<m_ui64MatrixBufferSize ; i++)
			{
				m_pMatrixBuffer[i] = sin(pBuffer[i]);
			}
			break;

		case OP_SQRT:
			for(uint64 i=0 ; i<m_ui64MatrixBufferSize ; i++)
			{
				m_pMatrixBuffer[i] = sqrt(pBuffer[i]);
			}
			break;

		case OP_TAN:
			for(uint64 i=0 ; i<m_ui64MatrixBufferSize ; i++)
			{
				m_pMatrixBuffer[i] = tan(pBuffer[i]);
			}
			break;

		case OP_ADD:
			for(uint64 i=0 ; i<m_ui64MatrixBufferSize ; i++)
			{
				m_pMatrixBuffer[i] = pBuffer[i] + m_f64SpecialEquationParameter;
			}
			break;

		case OP_MUL:
			for(uint64 i=0 ; i<m_ui64MatrixBufferSize ; i++)
			{
				m_pMatrixBuffer[i] = pBuffer[i] * m_f64SpecialEquationParameter;
			}
			break;

		case OP_DIV:
			for(uint64 i=0 ; i<m_ui64MatrixBufferSize ; i++)
			{
				m_pMatrixBuffer[i] = pBuffer[i] / m_f64SpecialEquationParameter;
			}
			break;
	}


	//the matrix is ready to be sent
	m_pSignalOutputWriterHelper->writeBuffer(*m_pWriter);
	getBoxAlgorithmContext()->getDynamicBoxContext()->markOutputAsReadyToSend(0, m_ui64LastChunkStartTime, m_ui64LastChunkEndTime);

}

void CSimpleDSP::writeSignalOutput(const void* pBuffer, const EBML::uint64 ui64BufferSize)
{
	appendOutputChunkData<0>(pBuffer, ui64BufferSize);
}

CSimpleDSP::CSimpleDSP(void)
	: m_pReader(NULL),
	m_pSignalReaderCallBack(NULL),
	m_ui64LastChunkStartTime(0),
	m_ui64LastChunkEndTime(0),
	m_pWriter(NULL),
	m_oSignalOutputWriterCallbackProxy(
		*this,
		&CSimpleDSP::writeSignalOutput),
	m_pSignalOutputWriterHelper(NULL),
	m_pSignalDescription(NULL),
	m_ui64MatrixBufferSize(0),
	m_pMatrixBuffer(NULL),
	m_bValidEquation(false),
	m_pEquationParser(NULL),
	m_ui64EquationType(OP_USERDEF),
	m_f64SpecialEquationParameter(0)
{
}

void CSimpleDSP::release(void)
{
}

boolean CSimpleDSP::initialize()
{

	//reads the plugin settings
	CString l_oEquation;
	getBoxAlgorithmContext()->getStaticBoxContext()->getSettingValue(0, l_oEquation);


	//initialises the signal description structure
	m_pSignalDescription = new CSignalDescription();

	// Prepares EBML reader
	m_pSignalReaderCallBack = createBoxAlgorithmSignalInputReaderCallback(*this);
	m_pReader=EBML::createReader(*m_pSignalReaderCallBack);

	// Prepares EBML writer
	m_pSignalOutputWriterHelper=createBoxAlgorithmSignalOutputWriter();
	m_pWriter=EBML::createWriter(m_oSignalOutputWriterCallbackProxy);

	m_pEquationParser = new CEquationParser(*this, &m_f64Variable);

	if(m_pEquationParser->compileEquation((const char*)l_oEquation))
	{
		m_bValidEquation = true;
		m_ui64EquationType = m_pEquationParser->getTreeCategory();
		m_f64SpecialEquationParameter = m_pEquationParser->getTreeParameter();
		return true;
	}

	return false;
}

boolean CSimpleDSP::uninitialize()
{

	if(m_pMatrixBuffer)
	{
		delete[] m_pMatrixBuffer;
	}

	// Cleans up EBML writer
	m_pWriter->release();
	m_pWriter=NULL;

	releaseBoxAlgorithmSignalOutputWriter(m_pSignalOutputWriterHelper);
	m_pSignalOutputWriterHelper=NULL;

	// Cleans up EBML reader
	releaseBoxAlgorithmSignalInputReaderCallback(m_pSignalReaderCallBack);

	m_pReader->release();
	m_pReader=NULL;

	delete m_pSignalDescription;
	m_pSignalDescription = NULL;

	delete m_pEquationParser;

	return true;
}

boolean CSimpleDSP::processInput( uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CSimpleDSP::process()
{
	IDynamicBoxContext* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();

	//reset the OutPut chunk
	l_pDynamicBoxContext->setOutputChunkSize(0, 0);

	//if the equation is valid, we can process the data
	if(m_bValidEquation)
	{
		// Process input data
		for(uint32 i=0; i<l_pDynamicBoxContext->getInputChunkCount(0); i++)
		{

			uint64 l_ui64ChunkSize;
			const uint8* l_pBuffer;
			l_pDynamicBoxContext->getInputChunk(0, i, m_ui64LastChunkStartTime, m_ui64LastChunkEndTime, l_ui64ChunkSize, l_pBuffer);
			l_pDynamicBoxContext->markInputAsDeprecated(0, i);
			m_pReader->processData(l_pBuffer, l_ui64ChunkSize);
		}
	}

	return true;
}
