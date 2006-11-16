#include "ovkCKernelContext.h"

#include "ovkCObjectFactory.h"
#include "ovkCPluginManager.h"
#include "ovkCScenarioManager.h"
#include "ovkCLogManager.h"

#include <string>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;

CKernelContext::CKernelContext(OpenViBE::Kernel::IKernel& rKernel)
	:m_rKernel(rKernel)
	,m_pObjectFactory(NULL)
	,m_pPluginManager(NULL)
	,m_pScenarioManager(NULL)
	,m_pLogManager(NULL)
{
	m_pObjectFactory=new CObjectFactory(*this);
	m_pPluginManager=new CPluginManager(*this);
	m_pScenarioManager=new CScenarioManager(*this);
	m_pLogManager=new CLogManager(*this);
}

CKernelContext::~CKernelContext(void)
{
	delete m_pLogManager;
	delete m_pScenarioManager;
	delete m_pPluginManager;
	delete m_pObjectFactory;
}

IObjectFactory& CKernelContext::getObjectFactory(void) const
{
	return *m_pObjectFactory;
}
IPluginManager& CKernelContext::getPluginManager(void) const
{
	return *m_pPluginManager;
}

IScenarioManager& CKernelContext::getScenarioManager(void) const
{
	return *m_pScenarioManager;
}

ILogManager& CKernelContext::getLogManager(void) const
{
	return *m_pLogManager;
}
