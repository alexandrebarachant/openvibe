#include "ovkCLogManagerConsole.h"

#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace std;

CLogManagerConsole::CLogManagerConsole(const IKernelContext& rKernelContext)
	:TKernelObject<ILogManager>(rKernelContext)
	,m_ui32MaximumLogLevel(0xffffffff)
	,m_ui32CurrentLogLevel(0)
{
}

boolean CLogManagerConsole::setMaximumLogLevel(const uint32 ui32LogLevel)
{
	m_ui32MaximumLogLevel=ui32LogLevel;
	return true;
}

boolean CLogManagerConsole::setCurrentLogLevel(const uint32 ui32LogLevel)
{
	m_ui32CurrentLogLevel=ui32LogLevel;
	return true;
}

ILogManager& CLogManagerConsole::log(const uint64 ui64Value)
{
	if(m_ui32CurrentLogLevel<m_ui32MaximumLogLevel) cout << ui64Value;
	return *this;
}

ILogManager& CLogManagerConsole::log(const uint32 ui32Value)
{
	if(m_ui32CurrentLogLevel<m_ui32MaximumLogLevel) cout << ui32Value;
	return *this;
}

ILogManager& CLogManagerConsole::log(const uint16 ui16Value)
{
	if(m_ui32CurrentLogLevel<m_ui32MaximumLogLevel) cout << ui16Value;
	return *this;
}

ILogManager& CLogManagerConsole::log(const uint8 ui8Value)
{
	if(m_ui32CurrentLogLevel<m_ui32MaximumLogLevel) cout << ui8Value;
	return *this;
}

ILogManager& CLogManagerConsole::log(const int64 i64Value)
{
	if(m_ui32CurrentLogLevel<m_ui32MaximumLogLevel) cout << i64Value;
	return *this;
}

ILogManager& CLogManagerConsole::log(const int32 i32Value)
{
	if(m_ui32CurrentLogLevel<m_ui32MaximumLogLevel) cout << i32Value;
	return *this;
}

ILogManager& CLogManagerConsole::log(const int16 i16Value)
{
	if(m_ui32CurrentLogLevel<m_ui32MaximumLogLevel) cout << i16Value;
	return *this;
}

ILogManager& CLogManagerConsole::log(const int8 i8Value)
{
	if(m_ui32CurrentLogLevel<m_ui32MaximumLogLevel) cout << i8Value;
	return *this;
}

ILogManager& CLogManagerConsole::log(const float64 f64Value)
{
	if(m_ui32CurrentLogLevel<m_ui32MaximumLogLevel) cout << f64Value;
	return *this;
}

ILogManager& CLogManagerConsole::log(const float32 f32Value)
{
	if(m_ui32CurrentLogLevel<m_ui32MaximumLogLevel) cout << f32Value;
	return *this;
}

ILogManager& CLogManagerConsole::log(const CIdentifier& rValue)
{
	if(m_ui32CurrentLogLevel<m_ui32MaximumLogLevel) cout << rValue.toString();
	return *this;
}

ILogManager& CLogManagerConsole::log(const CString& rValue)
{
	if(m_ui32CurrentLogLevel<m_ui32MaximumLogLevel) cout << rValue;
	return *this;
}

ILogManager& CLogManagerConsole::log(const char* rValue)
{
	if(m_ui32CurrentLogLevel<m_ui32MaximumLogLevel) cout << rValue;
	return *this;
}
