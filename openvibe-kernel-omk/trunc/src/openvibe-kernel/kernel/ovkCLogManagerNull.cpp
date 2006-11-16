#include "ovkCLogManagerNull.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;

CLogManagerNull::CLogManagerNull(const IKernelContext& rKernelContext)
	:TKernelObject<ILogManager>(rKernelContext)
{
}

boolean CLogManagerNull::setMaximumLogLevel(const OpenViBE::uint32 ui32LogLevel)
{
	return true;
}

boolean CLogManagerNull::setCurrentLogLevel(const OpenViBE::uint32 ui32LogLevel)
{
	return true;
}

ILogManager& CLogManagerNull::log(const uint64 ui64Value)
{
	return *this;
}

ILogManager& CLogManagerNull::log(const uint32 ui32Value)
{
	return *this;
}

ILogManager& CLogManagerNull::log(const uint16 ui16Value)
{
	return *this;
}

ILogManager& CLogManagerNull::log(const uint8 ui8Value)
{
	return *this;
}

ILogManager& CLogManagerNull::log(const int64 i64Value)
{
	return *this;
}

ILogManager& CLogManagerNull::log(const int32 i32Value)
{
	return *this;
}

ILogManager& CLogManagerNull::log(const int16 i16Value)
{
	return *this;
}

ILogManager& CLogManagerNull::log(const int8 i8Value)
{
	return *this;
}

ILogManager& CLogManagerNull::log(const float32 f32Value)
{
	return *this;
}

ILogManager& CLogManagerNull::log(const float64 f64Value)
{
	return *this;
}

ILogManager& CLogManagerNull::log(const CIdentifier& rValue)
{
	return *this;
}

ILogManager& CLogManagerNull::log(const CString& rValue)
{
	return *this;
}

ILogManager& CLogManagerNull::log(const char* rValue)
{
	return *this;
}
