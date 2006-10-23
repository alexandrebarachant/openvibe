#include "ovkCKernel.h"

#include "ovkCObjectFactory.h"
#include "ovkCPluginManager.h"
#include "ovkCScenarioManager.h"
#include "ovkCKernelLog.h"

#include <string>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;

CKernel::CKernel(void)
	:m_pObjectFactory(NULL)
	,m_pPluginManager(NULL)
	,m_pScenarioManager(NULL)
	,m_pKernelLog(NULL)
{
	m_pObjectFactory=new CObjectFactory(*this);
	m_pPluginManager=new CPluginManager(*this);
	m_pScenarioManager=new CScenarioManager(*this);
	m_pKernelLog=new CKernelLog(*this);
}

void CKernel::release(void)
{
	delete m_pKernelLog;
	delete m_pScenarioManager;
	delete m_pPluginManager;
	delete m_pObjectFactory;

	delete this;
}

IObjectFactory& CKernel::getObjectFactory(void)
{
	return *m_pObjectFactory;
}
const IObjectFactory& CKernel::getObjectFactory(void) const
{
	return *m_pObjectFactory;
}

IPluginManager& CKernel::getPluginManager(void)
{
	return *m_pPluginManager;
}

const IPluginManager& CKernel::getPluginManager(void) const
{
	return *m_pPluginManager;
}

IScenarioManager& CKernel::getScenarioManager(void)
{
	return *m_pScenarioManager;
}

const IScenarioManager& CKernel::getScenarioManager(void) const
{
	return *m_pScenarioManager;
}

IKernelLog& CKernel::getKernelLog(
	OpenViBE::uint32 ui32LogLevel) const
{
	return *m_pKernelLog;
}
