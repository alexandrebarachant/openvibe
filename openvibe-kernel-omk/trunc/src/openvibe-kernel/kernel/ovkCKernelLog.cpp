#include "ovkCKernelLog.h"

#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace std;

CKernelLog::CKernelLog(OpenViBE::Kernel::IKernel& rKernel)
	:TKernelObject<IKernelLog>(rKernel)
{
	m_vImplementation.push_back(OpenViBE::Tools::CObjectFactoryHelper(getKernel().getObjectFactory()).createObject<IKernelLog*>(OVK_ClassId_Kernel_KernelLogConsole));
}

CKernelLog::~CKernelLog(void)
{
	vector<IKernelLog*>::iterator i;
	for(i=m_vImplementation.begin(); i!=m_vImplementation.end(); i++)
	{
		OpenViBE::Tools::CObjectFactoryHelper(getKernel().getObjectFactory()).releaseObject(*i);
	}
}


IKernelLog& CKernelLog::log(const uint64 ui64Value)
{
	return logForEach<const uint64>(ui64Value);
}

IKernelLog& CKernelLog::log(const uint32 ui32Value)
{
	return logForEach<const uint32>(ui32Value);
}

IKernelLog& CKernelLog::log(const uint16 ui16Value)
{
	return logForEach<const uint16>(ui16Value);
}

IKernelLog& CKernelLog::log(const uint8 ui8Value)
{
	return logForEach<const uint8>(ui8Value);
}

IKernelLog& CKernelLog::log(const int64 i64Value)
{
	return logForEach<const int64>(i64Value);
}

IKernelLog& CKernelLog::log(const int32 i32Value)
{
	return logForEach<const int32>(i32Value);
}

IKernelLog& CKernelLog::log(const int16 i16Value)
{
	return logForEach<const int16>(i16Value);
}

IKernelLog& CKernelLog::log(const int8 i8Value)
{
	return logForEach<const int8>(i8Value);
}

IKernelLog& CKernelLog::log(const float64 f64Value)
{
	return logForEach<const float64>(f64Value);
}

IKernelLog& CKernelLog::log(const float32 f32Value)
{
	return logForEach<const float32>(f32Value);
}

IKernelLog& CKernelLog::log(const CIdentifier& rValue)
{
	return logForEach<const CIdentifier&>(rValue);
}

IKernelLog& CKernelLog::log(const CString& rValue)
{
	return logForEach<const CString&>(rValue);
}

IKernelLog& CKernelLog::log(const char* rValue)
{
	return logForEach<const char*>(rValue);
}
