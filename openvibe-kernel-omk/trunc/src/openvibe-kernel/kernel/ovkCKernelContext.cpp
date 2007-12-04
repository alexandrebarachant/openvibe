#include "ovkCKernelContext.h"
#include "ovkCKernelObjectFactory.h"
#include "ovkCTypeManager.h"

#include "player/ovkCPlayerManager.h"
#include "plugins/ovkCPluginManager.h"
#include "scenario/ovkCScenarioManager.h"
#include "log/ovkCLogManager.h"
#include "visualisation/ovkCVisualisationManager.h"

#include <string>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;

CKernelContext::CKernelContext(IKernel& rKernel)
	:m_rKernel(rKernel)
	,m_pKernelObjectFactory(NULL)
	,m_pPlayerManager(NULL)
	,m_pPluginManager(NULL)
	,m_pScenarioManager(NULL)
	,m_pTypeManager(NULL)
	,m_pLogManager(NULL)
	,m_pVisualisationManager(NULL)
{
	m_pKernelObjectFactory=new CKernelObjectFactory(*this);

	m_pLogManager=new CLogManager(*this);
	m_pLogManager->addListener(dynamic_cast<ILogListener*>(m_pKernelObjectFactory->createObject(OVK_ClassId_Kernel_Log_LogListenerConsole)));
	m_pLogManager->addListener(dynamic_cast<ILogListener*>(m_pKernelObjectFactory->createObject(OVK_ClassId_Kernel_Log_LogListenerFile)));
	(*m_pLogManager) << LogLevel_Debug << "Added Console Log Listener - should be removed\n";
	(*m_pLogManager) << LogLevel_Debug << "Added File Log Listener - should be removed\n";

	m_pPlayerManager=new CPlayerManager(*this);

	m_pTypeManager=new CTypeManager(*this);

	m_pScenarioManager=new CScenarioManager(*this);

	m_pVisualisationManager=new CVisualisationManager(*this);

	m_pPluginManager=new CPluginManager(*this);
}

CKernelContext::~CKernelContext(void)
{
	delete m_pPluginManager;
	delete m_pVisualisationManager;
	delete m_pScenarioManager;
	delete m_pTypeManager;
	delete m_pPlayerManager;
	delete m_pLogManager;
	delete m_pKernelObjectFactory;
}

IKernelObjectFactory& CKernelContext::getKernelObjectFactory(void) const
{
	return *m_pKernelObjectFactory;
}

IPlayerManager& CKernelContext::getPlayerManager(void) const
{
	return *m_pPlayerManager;
}

IPluginManager& CKernelContext::getPluginManager(void) const
{
	return *m_pPluginManager;
}

IScenarioManager& CKernelContext::getScenarioManager(void) const
{
	return *m_pScenarioManager;
}

ITypeManager& CKernelContext::getTypeManager(void) const
{
	return *m_pTypeManager;
}

ILogManager& CKernelContext::getLogManager(void) const
{
	return *m_pLogManager;
}

IVisualisationManager& CKernelContext::getVisualisationManager(void) const
{
	return *m_pVisualisationManager;
}
