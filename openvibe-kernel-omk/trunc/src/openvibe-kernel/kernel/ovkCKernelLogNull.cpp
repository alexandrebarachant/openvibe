#include "ovkCKernelLogNull.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;

CKernelLogNull::CKernelLogNull(IKernel& rKernel)
	:TKernelObject<IKernelLog>(rKernel)
{
}

IKernelLog& CKernelLogNull::log(const uint64 ui64Value)
{
	return *this;
}

IKernelLog& CKernelLogNull::log(const uint32 ui32Value)
{
	return *this;
}

IKernelLog& CKernelLogNull::log(const uint16 ui16Value)
{
	return *this;
}

IKernelLog& CKernelLogNull::log(const uint8 ui8Value)
{
	return *this;
}

IKernelLog& CKernelLogNull::log(const int64 i64Value)
{
	return *this;
}

IKernelLog& CKernelLogNull::log(const int32 i32Value)
{
	return *this;
}

IKernelLog& CKernelLogNull::log(const int16 i16Value)
{
	return *this;
}

IKernelLog& CKernelLogNull::log(const int8 i8Value)
{
	return *this;
}

IKernelLog& CKernelLogNull::log(const float32 f32Value)
{
	return *this;
}

IKernelLog& CKernelLogNull::log(const float64 f64Value)
{
	return *this;
}

IKernelLog& CKernelLogNull::log(const CIdentifier& rValue)
{
	return *this;
}

IKernelLog& CKernelLogNull::log(const CString& rValue)
{
	return *this;
}

IKernelLog& CKernelLogNull::log(const char* rValue)
{
	return *this;
}
