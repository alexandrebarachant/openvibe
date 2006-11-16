#include "ovkCLogManager.h"

#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace std;

CLogManager::CLogManager(const IKernelContext& rKernelContext)
	:TKernelObject<ILogManager>(rKernelContext)
{
	m_vImplementation.push_back(OpenViBE::Tools::CObjectFactoryHelper(getKernelContext().getObjectFactory()).createObject<ILogManager*>(OVK_ClassId_Kernel_LogManagerConsole));
}

CLogManager::~CLogManager(void)
{
	vector<ILogManager*>::iterator i;
	for(i=m_vImplementation.begin(); i!=m_vImplementation.end(); i++)
	{
		OpenViBE::Tools::CObjectFactoryHelper(getKernelContext().getObjectFactory()).releaseObject(*i);
	}
}

boolean CLogManager::setMaximumLogLevel(const uint32 ui32LogLevel)
{
	boolean l_bResult=true;
	vector<ILogManager*>::iterator i;
	for(i=m_vImplementation.begin(); i!=m_vImplementation.end(); i++)
	{
		l_bResult&=(*i)->setMaximumLogLevel(ui32LogLevel);
	}
	return l_bResult;
}

boolean CLogManager::setCurrentLogLevel(const uint32 ui32LogLevel)
{
	boolean l_bResult=true;
	vector<ILogManager*>::iterator i;
	for(i=m_vImplementation.begin(); i!=m_vImplementation.end(); i++)
	{
		l_bResult&=(*i)->setCurrentLogLevel(ui32LogLevel);
	}
	return l_bResult;
}

ILogManager& CLogManager::log(const uint64 ui64Value)
{
	return logForEach<const uint64>(ui64Value);
}

ILogManager& CLogManager::log(const uint32 ui32Value)
{
	return logForEach<const uint32>(ui32Value);
}

ILogManager& CLogManager::log(const uint16 ui16Value)
{
	return logForEach<const uint16>(ui16Value);
}

ILogManager& CLogManager::log(const uint8 ui8Value)
{
	return logForEach<const uint8>(ui8Value);
}

ILogManager& CLogManager::log(const int64 i64Value)
{
	return logForEach<const int64>(i64Value);
}

ILogManager& CLogManager::log(const int32 i32Value)
{
	return logForEach<const int32>(i32Value);
}

ILogManager& CLogManager::log(const int16 i16Value)
{
	return logForEach<const int16>(i16Value);
}

ILogManager& CLogManager::log(const int8 i8Value)
{
	return logForEach<const int8>(i8Value);
}

ILogManager& CLogManager::log(const float64 f64Value)
{
	return logForEach<const float64>(f64Value);
}

ILogManager& CLogManager::log(const float32 f32Value)
{
	return logForEach<const float32>(f32Value);
}

ILogManager& CLogManager::log(const CIdentifier& rValue)
{
	return logForEach<const CIdentifier&>(rValue);
}

ILogManager& CLogManager::log(const CString& rValue)
{
	return logForEach<const CString&>(rValue);
}

ILogManager& CLogManager::log(const char* rValue)
{
	return logForEach<const char*>(rValue);
}
