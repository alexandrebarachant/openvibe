#include "ovpCEBMLStreamSpy.h"

#include <iostream>
#include <fstream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Tools;
using namespace OpenViBEToolkit;
using namespace std;

CEBMLStreamSpy::CEBMLStreamSpy(void)
	:m_pReader(NULL)
	,m_pReaderHelper(NULL)
{
}

void CEBMLStreamSpy::release(void)
{
	delete this;
}

boolean CEBMLStreamSpy::initialize(void)
{
	m_pReader=EBML::createReader(*this);
	m_pReaderHelper=EBML::createReaderHelper();

	CString l_sFileName;
	getBoxAlgorithmContext()->getStaticBoxContext()->getSettingValue(0, l_sFileName);

	CString l_sLogLevel;
	getBoxAlgorithmContext()->getStaticBoxContext()->getSettingValue(1, l_sLogLevel);
	m_eLogLevel=static_cast<ELogLevel>(getBoxAlgorithmContext()->getPlayerContext()->getTypeManager().getEnumerationEntryValueFromName(OV_TypeId_LogLevel, l_sLogLevel));

	ifstream l_oFile(l_sFileName);
	while(l_oFile.good() && !l_oFile.eof())
	{
		unsigned int l_ui32Identifier1;
		unsigned int l_ui32Identifier2;
		string l_sIdentifier1;
		string l_sIdentifier2;
		string l_sName;
		string l_sType;

		l_oFile>>l_sName;
		l_oFile>>l_sIdentifier1;
		l_oFile>>l_sIdentifier2;
		l_oFile>>l_sType;

		sscanf(l_sIdentifier1.c_str(), "EBML::CIdentifier(0x%08x", &l_ui32Identifier1);
		sscanf(l_sIdentifier2.c_str(), "0x%08x)", &l_ui32Identifier2);

		// cout << "[" << l_sIdentifier1 << "][" << l_sIdentifier2 << "]" << endl;
		// printf("[EBML::CIdentifier(0x%08X,][0x%08X]\n", l_ui32Identifier1, l_ui32Identifier2);
		// cout << EBML::CIdentifier(l_ui32Identifier1, l_ui32Identifier2) << endl;

		m_vName[EBML::CIdentifier(l_ui32Identifier1, l_ui32Identifier2)]=l_sName;
		m_vType[EBML::CIdentifier(l_ui32Identifier1, l_ui32Identifier2)]=l_sType;
	}

	return true;
}

boolean CEBMLStreamSpy::uninitialize(void)
{
	m_pReaderHelper->release();
	m_pReaderHelper=NULL;

	m_pReader->release();
	m_pReader=NULL;

	return true;
}

EBML::boolean CEBMLStreamSpy::isMasterChild(const EBML::CIdentifier& rIdentifier)
{
	map<EBML::CIdentifier, string>::iterator n;
	map<EBML::CIdentifier, string>::iterator t;
	n=m_vName.find(rIdentifier);
	t=m_vType.find(rIdentifier);
	if(n!=m_vName.end() && t!=m_vType.end())
	{
		return (t->second=="master");
	}
	return false;
}

void CEBMLStreamSpy::openChild(const EBML::CIdentifier& rIdentifier)
{
	map<EBML::CIdentifier, string>::iterator n;
	map<EBML::CIdentifier, string>::iterator t;
	n=m_vName.find(rIdentifier);
	t=m_vType.find(rIdentifier);

	getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << m_eLogLevel;

	for(size_t i=0; i<=m_vNodes.size(); i++)
	{
		getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << "  ";
	}

	getBoxAlgorithmContext()->getPlayerContext()->getLogManager()
		<< "Opened EBML node [id:"
		<< CIdentifier(rIdentifier)
		<< "]-[name:"
		<< CString(n!=m_vName.end()?n->second.c_str():"unknown")
		<< "]";

	if(isMasterChild(rIdentifier))
	{
		getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << "\n";
	}

	m_vNodes.push(rIdentifier);
}

void CEBMLStreamSpy::processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize)
{
	map<EBML::CIdentifier, string>::iterator n;
	map<EBML::CIdentifier, string>::iterator t;
	n=m_vName.find(m_vNodes.top());
	t=m_vType.find(m_vNodes.top());

	if(t!=m_vType.end())
	{
		if(t->second=="uinteger")
			getBoxAlgorithmContext()->getPlayerContext()->getLogManager()
				<< "-[type:" << CString(t->second.c_str()) << "]-[value:" << m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize) << "]";
		else if(t->second=="integer")
			getBoxAlgorithmContext()->getPlayerContext()->getLogManager()
				<< "-[type:" << CString(t->second.c_str()) << "]-[value:" << m_pReaderHelper->getSIntegerFromChildData(pBuffer, ui64BufferSize) << "]";
		else if(t->second=="float")
			getBoxAlgorithmContext()->getPlayerContext()->getLogManager()
				<< "-[type:" << CString(t->second.c_str()) << "]-[value:" << m_pReaderHelper->getFloatFromChildData(pBuffer, ui64BufferSize) << "]";
		else if(t->second=="string")
			getBoxAlgorithmContext()->getPlayerContext()->getLogManager()
				<< "-[type:" << CString(t->second.c_str()) << "]-[value:" << m_pReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize) << "]";
		else if(t->second=="binary")
			getBoxAlgorithmContext()->getPlayerContext()->getLogManager()
				<< "-[type:" << CString(t->second.c_str()) << "]-[bytes:" << ui64BufferSize << "]";
		else
			getBoxAlgorithmContext()->getPlayerContext()->getLogManager()
				<< "-[type:" << CString("unknown") << "]-[bytes:" << ui64BufferSize << "]";
	}
	getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << "\n";
}

void CEBMLStreamSpy::closeChild(void)
{
	m_vNodes.pop();
}

boolean CEBMLStreamSpy::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CEBMLStreamSpy::process(void)
{
	IBoxIO* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();
	IBox* l_pStaticBoxContext=getBoxAlgorithmContext()->getStaticBoxContext();

	uint64 l_ui64StartTime=0;
	uint64 l_ui64EndTime=0;
	uint64 l_ui64ChunkSize=0;
	const uint8* l_pChunkBuffer=NULL;

	for(uint32 i=0; i<l_pStaticBoxContext->getInputCount(); i++)
	{
		for(uint32 j=0; j<l_pDynamicBoxContext->getInputChunkCount(i); j++)
		{
			l_pDynamicBoxContext->getInputChunk(i, j, l_ui64StartTime, l_ui64EndTime, l_ui64ChunkSize, l_pChunkBuffer);
			l_pDynamicBoxContext->markInputAsDeprecated(i, j);

			getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << m_eLogLevel
				<< "For chunk [id:" << i << "," << j << "] at [time:" << CIdentifier(l_ui64StartTime) << "," << CIdentifier(l_ui64EndTime) << "]\n";

			m_pReader->processData(l_pChunkBuffer, l_ui64ChunkSize);
		}
	}

	return true;
}
