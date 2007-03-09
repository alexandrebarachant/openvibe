#include "ovpCEBMLStreamSpy.h"

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Samples;
using namespace OpenViBEToolkit;
using namespace std;

#if defined OVP_OS_Windows
 #define ConsoleColor_FG_Highlight ""
 #define ConsoleColor_FG_Downlight ""

 #define ConsoleColor_FG_Black     ""
 #define ConsoleColor_FG_Red       ""
 #define ConsoleColor_FG_Green     ""
 #define ConsoleColor_FG_Yellow    ""
 #define ConsoleColor_FG_Blue      ""
 #define ConsoleColor_FG_Magenta   ""
 #define ConsoleColor_FG_Cyan      ""
 #define ConsoleColor_FG_White     ""

 #define ConsoleColor_BG_Black     ""
 #define ConsoleColor_BG_Red       ""
 #define ConsoleColor_BG_Green     ""
 #define ConsoleColor_BG_Yellow    ""
 #define ConsoleColor_BG_Blue      ""
 #define ConsoleColor_BG_Magenta   ""
 #define ConsoleColor_BG_Cyan      ""
 #define ConsoleColor_BG_White     ""
#else
 #define ConsoleColor_FG_Highlight "\033[01m"
 #define ConsoleColor_FG_Downlight "\033[00m"

 #define ConsoleColor_FG_Black     "\033[30m"
 #define ConsoleColor_FG_Red       "\033[31m"
 #define ConsoleColor_FG_Green     "\033[32m"
 #define ConsoleColor_FG_Yellow    "\033[33m"
 #define ConsoleColor_FG_Blue      "\033[34m"
 #define ConsoleColor_FG_Magenta   "\033[35m"
 #define ConsoleColor_FG_Cyan      "\033[36m"
 #define ConsoleColor_FG_White     "\033[37m"

 #define ConsoleColor_BG_Black     "\033[40m"
 #define ConsoleColor_BG_Red       "\033[41m"
 #define ConsoleColor_BG_Green     "\033[42m"
 #define ConsoleColor_BG_Yellow    "\033[43m"
 #define ConsoleColor_BG_Blue      "\033[44m"
 #define ConsoleColor_BG_Magenta   "\033[45m"
 #define ConsoleColor_BG_Cyan      "\033[46m"
 #define ConsoleColor_BG_White     "\033[47m"
#endif

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

	ifstream l_oFile(l_sFileName);
	while(!l_oFile.eof())
	{
		uint32 l_ui32Identifier1;
		uint32 l_ui32Identifier2;
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

	for(size_t i=0; i<=m_vNodes.size(); i++)
	{
		cout << "  ";
	}
	uint32 l_ui32Identifier1=(((EBML::uint64)rIdentifier)>>32);
	uint32 l_ui32Identifier2=(((EBML::uint64)rIdentifier)&0xffffffff);
	cout
		<< "Opened EBML node [id:"
		<< ConsoleColor_FG_Blue
		<< "0x"
		<< hex
		<< setw(8)
		<< setfill('0')
		<< l_ui32Identifier1
		<< ",0x"
		<< hex
		<< setw(8)
		<< setfill('0')
		<< l_ui32Identifier2 << ConsoleColor_FG_Black
		<< "]-[name:"
		<< ConsoleColor_FG_Blue << (n!=m_vName.end()?n->second:"unknown") << ConsoleColor_FG_Black
		<< "]"
		<< dec;

	if(isMasterChild(rIdentifier))
	{
		cout << endl;
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
			cout
				<< "-[type:"
				<< ConsoleColor_FG_Blue << t->second << ConsoleColor_FG_Black
				<< "]-[value:"
				<< ConsoleColor_FG_Blue << m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize) << ConsoleColor_FG_Black
				<< "]";
		else if(t->second=="integer")
			cout
				<< "-[type:"
				<< ConsoleColor_FG_Blue << t->second << ConsoleColor_FG_Black
				<< "]-[value:"
				<< ConsoleColor_FG_Blue << m_pReaderHelper->getSIntegerFromChildData(pBuffer, ui64BufferSize) << ConsoleColor_FG_Black
				<< "]";
		else if(t->second=="string")
			cout
				<< "-[type:"
				<< ConsoleColor_FG_Blue << t->second << ConsoleColor_FG_Black
				<< "]-[value:"
				<< ConsoleColor_FG_Blue << m_pReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize) << ConsoleColor_FG_Black
				<< "]";
		else if(t->second=="binary")
			cout
				<< "-[type:"
				<< ConsoleColor_FG_Blue << t->second << ConsoleColor_FG_Black
				<< "]-[bytes:"
				<< ConsoleColor_FG_Blue << ui64BufferSize << ConsoleColor_FG_Black
				<< "]";
		else
			cout
				<< "-[type:"
				<< ConsoleColor_FG_Red << "unknown" << ConsoleColor_FG_Black
				<< "]-[bytes:"
				<< ConsoleColor_FG_Blue << ui64BufferSize << ConsoleColor_FG_Black
				<< "]";
		cout << endl;
	}
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
	IStaticBoxContext* l_pStaticBoxContext=getBoxAlgorithmContext()->getStaticBoxContext();
	IDynamicBoxContext* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();

	uint64 l_ui64StartTime=0;
	uint64 l_ui64EndTime=0;
	uint64 l_ui64ChunkSize=0;
	const uint8* l_pChunkBuffer=NULL;

	for(uint32 j=0; j<l_pDynamicBoxContext->getInputChunkCount(0); j++)
	{
		l_pDynamicBoxContext->getInputChunk(0, j, l_ui64StartTime, l_ui64EndTime, l_ui64ChunkSize, l_pChunkBuffer);
		l_pDynamicBoxContext->markInputAsDeprecated(0, j);

		cout
			<< "For chunk [id:"
			<< ConsoleColor_FG_Green << 0 << ConsoleColor_FG_Black
			<< ","
			<< ConsoleColor_FG_Green << j << ConsoleColor_FG_Black
			<< "] at [time:"
			<< ConsoleColor_FG_Green
			<< "0x"
			<< hex
			<< setw(16)
			<< setfill('0')
			<< l_ui64StartTime
			<< ConsoleColor_FG_Black
			<< ","
			<< ConsoleColor_FG_Green
			<< "0x"
			<< hex
			<< setw(16)
			<< setfill('0')
			<< l_ui64EndTime
			<< ConsoleColor_FG_Black
			<< "]"
			<< endl;

		m_pReader->processData(l_pChunkBuffer, l_ui64ChunkSize);
	}

	return true;
}
