#include "ovpCGenericStreamWriter.h"

#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;
using namespace OpenViBE::Kernel;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::FileIO;
using namespace OpenViBEToolkit;
using namespace std;

CGenericStreamWriter::CGenericStreamWriter(void)
{

}

void CGenericStreamWriter::release(void)
{
	delete this;
}

boolean CGenericStreamWriter::initialize(void)
{
	const IBox* l_pBox=getBoxAlgorithmContext()->getStaticBoxContext();

	// Parses box settings to find filename
	l_pBox->getSettingValue(0, m_sFileName);

	if(!m_oFile.is_open())
	{
		m_oFile.open(m_sFileName, ios::binary | ios::trunc);

		if(m_oFile.bad())
		{
			getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Warning <<"Couldn't open the output file : "<<m_sFileName<<".\n";
			return false;
		}
	}

	return true;
}

boolean CGenericStreamWriter::uninitialize(void)
{
	if(m_oFile.is_open())
	{
		m_oFile.close();
	}

	return true;
}

boolean CGenericStreamWriter::processInput(uint32 ui32InputIndex)
{
	//add this input to the list of inputs with pending data
	m_vInputWithPendingData.push_back(ui32InputIndex);

	//process only if the file has been opened
	if(m_oFile.is_open())
	{
		getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	}

	return true;
}

boolean CGenericStreamWriter::process(void)
{
	IBoxIO* l_pBoxIO=getBoxAlgorithmContext()->getDynamicBoxContext();

	//destination buffer when converting to little endian
	uint8 l_pTempBuffer[8];

	uint32 l_ui32CurrentInput=0;
	uint64 l_ui64StartTime=0;
	uint64 l_ui64EndTime=0;
	uint64 l_ui64ChunkSize=0;
	const uint8* l_pChunkBuffer=NULL;

	//for every input with pending data
	for(uint32 i=0 ; i<m_vInputWithPendingData.size() ; i++)
	{
		l_ui32CurrentInput = m_vInputWithPendingData[i];

		//for every pending data on that input
		for(uint32 j=0; j<l_pBoxIO->getInputChunkCount(l_ui32CurrentInput); j++)
		{
			l_pBoxIO->getInputChunk(l_ui32CurrentInput, j, l_ui64StartTime, l_ui64EndTime, l_ui64ChunkSize, l_pChunkBuffer);

			//write input number
			System::Memory::hostToLittleEndian(l_ui32CurrentInput, l_pTempBuffer);
			m_oFile.write(reinterpret_cast<char *>(l_pTempBuffer), sizeof(uint32));

			//write start time
			System::Memory::hostToLittleEndian(l_ui64StartTime, l_pTempBuffer);
			m_oFile.write(reinterpret_cast<char *>(l_pTempBuffer), sizeof(uint64));

			//write end time
			System::Memory::hostToLittleEndian(l_ui64EndTime, l_pTempBuffer);
			m_oFile.write(reinterpret_cast<char *>(l_pTempBuffer), sizeof(uint64));

			//write chunk size
			System::Memory::hostToLittleEndian(l_ui64ChunkSize, l_pTempBuffer);
			m_oFile.write(reinterpret_cast<char *>(l_pTempBuffer), sizeof(uint64));

			//write the chunk
			m_oFile.write(reinterpret_cast<const char *>(l_pChunkBuffer), (std::streamsize)l_ui64ChunkSize);

			l_pBoxIO->markInputAsDeprecated(l_ui32CurrentInput, j);

		}
	}

	//clear the vector of inputs with pending data
	m_vInputWithPendingData.clear();

	return true;
}
