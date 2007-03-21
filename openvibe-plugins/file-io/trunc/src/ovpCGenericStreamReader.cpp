#include "ovpCGenericStreamReader.h"

#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::FileIO;
using namespace OpenViBEToolkit;
using namespace std;

CGenericStreamReader::CGenericStreamReader(void)
{

}

void CGenericStreamReader::release(void)
{
	delete this;
}

boolean CGenericStreamReader::initialize(void)
{
	const IStaticBoxContext* l_pBoxContext=getBoxAlgorithmContext()->getStaticBoxContext();

	// Parses box settings to find filename
	l_pBoxContext->getSettingValue(0, m_sFileName);

	if(!m_oFile.is_open())
	{
		m_oFile.open(m_sFileName, ios::binary | ios::in);

		if(m_oFile.bad())
		{
			cout<<"Couldn't open the input file : "<<m_sFileName<<endl;
			return false;
		}
	}

	return true;
}

boolean CGenericStreamReader::uninitialize(void)
{
	if(m_oFile.is_open())
	{
		m_oFile.close();
	}

	return true;
}

boolean CGenericStreamReader::processClock(CMessageClock &rMessageClock)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CGenericStreamReader::process(void)
{
	if(!m_oFile.eof())
	{
		IDynamicBoxContext* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();

		uint32 l_ui32CurrentInput=0;
		uint64 l_ui64StartTime=0;
		uint64 l_ui64EndTime=0;
		uint64 l_ui64ChunkSize=0;
		uint8 * l_pChunkBuffer=NULL;

		//buffer to read the next data header
		const uint64 l_ui64HeaderSize = sizeof(l_ui32CurrentInput)+sizeof(l_ui64StartTime)+sizeof(l_ui64EndTime)+sizeof(l_ui64ChunkSize);

		uint8 l_pTempBuffer[l_ui64HeaderSize];

		//reads all the information about the next chunk
		m_oFile.read(reinterpret_cast<char * >(l_pTempBuffer), l_ui64HeaderSize);

		if(m_oFile.bad() || m_oFile.fail())
		{
			//if the eof has been reached
			if(m_oFile.eof())
			{
				cout<<"Reached the end of the input file."<<endl;
				return true;
			}
			else
			{
				cout<<"Error while reading the input file : "<<m_sFileName<<endl;
				return false;
			}
		}

		System::Memory::littleEndianToHost(l_pTempBuffer, &l_ui32CurrentInput);
		System::Memory::littleEndianToHost(l_pTempBuffer+sizeof(l_ui32CurrentInput), &l_ui64StartTime);
		System::Memory::littleEndianToHost(l_pTempBuffer+sizeof(l_ui32CurrentInput)+sizeof(l_ui64StartTime), &l_ui64EndTime);
		System::Memory::littleEndianToHost(l_pTempBuffer+sizeof(l_ui32CurrentInput)+sizeof(l_ui64StartTime)+sizeof(l_ui64EndTime), &l_ui64ChunkSize);

		//reads the actual chunk datas
		l_pChunkBuffer = new uint8[l_ui64ChunkSize];
		m_oFile.read(reinterpret_cast<char * >(l_pChunkBuffer), l_ui64ChunkSize);

		if(m_oFile.bad() || m_oFile.fail())
		{
			cout<<"Error while reading the input file : "<<m_sFileName<<endl;
			return false;
		}

		//TODO check if still needed
		l_pDynamicBoxContext->setOutputChunkSize(l_ui32CurrentInput, 0, true);

		//sends the data
		l_pDynamicBoxContext->appendOutputChunkData(l_ui32CurrentInput, l_pChunkBuffer, l_ui64ChunkSize);
		l_pDynamicBoxContext->markOutputAsReadyToSend(l_ui32CurrentInput, l_ui64StartTime, l_ui64EndTime);

		delete[] l_pChunkBuffer;
	}

	return true;
}
