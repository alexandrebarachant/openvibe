#include "ovkCKernelContext.h"

#include "ovkCObjectFactory.h"
#include "ovkCPluginManager.h"
#include "ovkCScenarioManager.h"
#include "ovkCLogManager.h"
#include "ovkCTypeManager.h"

#include <string>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;

CKernelContext::CKernelContext(IKernel& rKernel)
	:m_rKernel(rKernel)
	,m_pObjectFactory(NULL)
	,m_pPluginManager(NULL)
	,m_pScenarioManager(NULL)
	,m_pTypeManager(NULL)
	,m_pLogManager(NULL)
{
	m_pObjectFactory=new CObjectFactory(*this);
	m_pPluginManager=new CPluginManager(*this);
	m_pScenarioManager=new CScenarioManager(*this);
	m_pTypeManager=new CTypeManager(*this);
	m_pLogManager=new CLogManager(*this);

	// $$$
	m_pTypeManager->registerEnumerationType(CIdentifier(0,1), CString("enum 1 value 1;enum 1 value 2;enum 1 value 3;enum 1 value 4"));
	m_pTypeManager->registerEnumerationType(CIdentifier(0,2), CString("enum 2 v1;enum 2 v2"));
	m_pTypeManager->registerBitMaskType    (CIdentifier(1,1), CString("bitmask1 bit1;bitmask1 bit2;bitmask1 bit3;bitmask1 bit4;bitmask1 bit5"));
	m_pTypeManager->registerBitMaskType    (CIdentifier(1,2), CString("bitmask2 b1;bitmask2 b2"));

	// $$$
	m_pLogManager->addListener(dynamic_cast<ILogListener*>(m_pObjectFactory->createObject(OVK_ClassId_Kernel_LogListenerConsole)));

	(*m_pLogManager) << LogLevel_Debug << "Added Console Log Listener - should be removed\n";
	(*m_pLogManager) << LogLevel_Debug << "Added test enumeration" << CIdentifier(0, 1) << " - TODO remove this\n";
	(*m_pLogManager) << LogLevel_Debug << "Added test enumeration" << CIdentifier(0, 2) << " - TODO remove this\n";
	(*m_pLogManager) << LogLevel_Debug << "Added test bitmask" << CIdentifier(1, 1) << " - TODO remove this\n";
	(*m_pLogManager) << LogLevel_Debug << "Added test bitmask" << CIdentifier(1, 2) << " - TODO remove this\n";
}

CKernelContext::~CKernelContext(void)
{
	delete m_pLogManager;
	delete m_pTypeManager;
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

ITypeManager& CKernelContext::getTypeManager(void) const
{
	return *m_pTypeManager;
}

ILogManager& CKernelContext::getLogManager(void) const
{
	return *m_pLogManager;
}
