#include "ovkCKernelLogConsole.h"

#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace std;

CKernelLogConsole::CKernelLogConsole(IKernel& rKernel)
	:TKernelObject<IKernelLog>(rKernel)
{
}

IKernelLog& CKernelLogConsole::log(const uint64 ui64Value)
{
	cout << ui64Value;
	return *this;
}

IKernelLog& CKernelLogConsole::log(const uint32 ui32Value)
{
	cout << ui32Value;
	return *this;
}

IKernelLog& CKernelLogConsole::log(const uint16 ui16Value)
{
	cout << ui16Value;
	return *this;
}

IKernelLog& CKernelLogConsole::log(const uint8 ui8Value)
{
	cout << ui8Value;
	return *this;
}

IKernelLog& CKernelLogConsole::log(const int64 i64Value)
{
	cout << i64Value;
	return *this;
}

IKernelLog& CKernelLogConsole::log(const int32 i32Value)
{
	cout << i32Value;
	return *this;
}

IKernelLog& CKernelLogConsole::log(const int16 i16Value)
{
	cout << i16Value;
	return *this;
}

IKernelLog& CKernelLogConsole::log(const int8 i8Value)
{
	cout << i8Value;
	return *this;
}

IKernelLog& CKernelLogConsole::log(const float64 f64Value)
{
	cout << f64Value;
	return *this;
}

IKernelLog& CKernelLogConsole::log(const float32 f32Value)
{
	cout << f32Value;
	return *this;
}

IKernelLog& CKernelLogConsole::log(const CIdentifier& rValue)
{
	cout << rValue.toString();
	return *this;
}

IKernelLog& CKernelLogConsole::log(const CString& rValue)
{
	cout << rValue;
	return *this;
}

IKernelLog& CKernelLogConsole::log(const char* rValue)
{
	cout << rValue;
	return *this;
}
