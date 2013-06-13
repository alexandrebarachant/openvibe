#include "ovkCLogListenerFile.h"

#include <cstdio>
#include <sstream>
#include <iostream>

#include <fs/Files.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace std;

CLogListenerFile::CLogListenerFile(const IKernelContext& rKernelContext, const CString& sApplicationName, const CString& sLogFilename)
	:TKernelObject<ILogListener>(rKernelContext)
	,m_sApplicationName(sApplicationName)
	,m_sLogFilename(sLogFilename)
	,m_bTimeInSeconds(true)
	,m_bLogWithHexa(false)
	,m_ui64TimePrecision(3)
{

	// Create the path to the log file
	FS::Files::createParentPath(sLogFilename.toASCIIString());

	m_fsFileStream.open(sLogFilename.toASCIIString(), ios::out);

	if (!m_fsFileStream.is_open())
	{
		std::cout << "[  ERR  ] Error while creating FileLogListener into '" << sLogFilename << "'" << std::endl;
		return;
	}
	m_fsFileStream << flush;
	std::cout << "[  INF  ] Logging to '" << sLogFilename << "'" << std::endl;
}

CLogListenerFile::~CLogListenerFile()
{
	m_fsFileStream.close();
}

void CLogListenerFile::configure(const IConfigurationManager& rConfigurationManager)
{
	m_bTimeInSeconds = rConfigurationManager.expandAsBoolean("${Kernel_FileLogTimeInSecond}",false);
	m_bLogWithHexa = rConfigurationManager.expandAsBoolean("${Kernel_FileLogWithHexa}",true);
	m_ui64TimePrecision = rConfigurationManager.expandAsUInteger("${Kernel_FileLogTimePrecision}",3);
}

boolean CLogListenerFile::isActive(ELogLevel eLogLevel)
{
	map<ELogLevel, boolean>::iterator itLogLevel=m_vActiveLevel.find(eLogLevel);
	if(itLogLevel==m_vActiveLevel.end())
	{
		return true;
	}
	return itLogLevel->second;
}

boolean CLogListenerFile::activate(ELogLevel eLogLevel, boolean bActive)
{
	m_vActiveLevel[eLogLevel]=bActive;
	return true;
}

boolean CLogListenerFile::activate(ELogLevel eStartLogLevel, ELogLevel eEndLogLevel, boolean bActive)
{
	for(int i=eStartLogLevel; i<=eEndLogLevel; i++)
	{
		m_vActiveLevel[ELogLevel(i)]=bActive;
	}
	return true;
}

boolean CLogListenerFile::activate(boolean bActive)
{
	return activate(LogLevel_First, LogLevel_Last, bActive);
}

void CLogListenerFile::log(const time64 time64Value)
{
	if(m_bTimeInSeconds)
	{
		float64 l_f64Time=(time64Value.m_ui64TimeValue>>22)/1024.;
		std::stringstream ss;
		ss.precision(m_ui64TimePrecision);
		ss.setf(std::ios::fixed,std::ios::floatfield);
		ss << l_f64Time;
		ss << " sec";
		if(m_bLogWithHexa)
		{
			ss << " (0x" << hex << time64Value.m_ui64TimeValue << ")";
		}

		m_fsFileStream << ss;
	}
	else
	{
		logInteger(time64Value.m_ui64TimeValue);
	}
}

void CLogListenerFile::log(const uint64 ui64Value)
{
	logInteger(ui64Value);
}

void CLogListenerFile::log(const uint32 ui32Value)
{
	logInteger(ui32Value);
}

void CLogListenerFile::log(const uint16 ui16Value)
{
	logInteger(ui16Value);
}

void CLogListenerFile::log(const uint8 ui8Value)
{
	logInteger(ui8Value);
}

void CLogListenerFile::log(const int64 i64Value)
{
	logInteger(i64Value);
}

void CLogListenerFile::log(const int32 i32Value)
{
	logInteger(i32Value);
}

void CLogListenerFile::log(const int16 i16Value)
{
	logInteger(i16Value);
}

void CLogListenerFile::log(const int8 i8Value)
{
	logInteger(i8Value);
}

void CLogListenerFile::log(const float32 f32Value)
{
	m_fsFileStream << f32Value;
}

void CLogListenerFile::log(const float64 f64Value)
{
	m_fsFileStream << f64Value;
}

void CLogListenerFile::log(const boolean bValue)
{
	m_fsFileStream << (bValue ? "true" : "false");
}

void CLogListenerFile::log(const CIdentifier& rValue)
{
	m_fsFileStream << rValue.toString();
}

void CLogListenerFile::log(const CString& rValue)
{
	m_fsFileStream << rValue;
	m_fsFileStream << flush;
}

void CLogListenerFile::log(const char* pValue)
{
	m_fsFileStream << pValue;
	m_fsFileStream << flush;
}

void CLogListenerFile::log(const ELogLevel eLogLevel)
{
	switch(eLogLevel)
	{
		case LogLevel_Debug:
			m_fsFileStream << "[ DEBUG ] ";
			break;

		case LogLevel_Benchmark:
			m_fsFileStream << "[ BENCH ] ";
			break;

		case LogLevel_Trace:
			m_fsFileStream << "[ TRACE ] ";
			break;

		case LogLevel_Info:
			m_fsFileStream << "[  INF  ] ";
			break;

		case LogLevel_Warning:
			m_fsFileStream << "[WARNING] ";
			break;

		case LogLevel_ImportantWarning:
			m_fsFileStream << "[WARNING] ";
			break;

		case LogLevel_Error:
			m_fsFileStream << "[ ERROR ] ";
			break;

		case LogLevel_Fatal:
			m_fsFileStream << "[ FATAL ] ";
			break;

		default:
			m_fsFileStream << "[UNKNOWN] ";
			break;
	}
}

void CLogListenerFile::log(const ELogColor eLogColor)
{
}
