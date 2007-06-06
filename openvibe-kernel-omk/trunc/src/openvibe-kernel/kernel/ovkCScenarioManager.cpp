#include "ovkCScenarioManager.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace std;

CScenarioManager::CScenarioManager(const IKernelContext& rKernelContext)
	:TKernelObject<IScenarioManager>(rKernelContext)
{
}

boolean CScenarioManager::createScenario(
	CIdentifier& rScenarioIdentifier)
{
	IScenario* l_pScenario=OpenViBE::Tools::CObjectFactoryHelper(getKernelContext().getObjectFactory()).createObject<IScenario*>(OV_ClassId_Kernel_Scenario);
	rScenarioIdentifier=getUnusedIdentifier();
	m_vScenario[rScenarioIdentifier]=l_pScenario;
	return true;
}

boolean CScenarioManager::releaseScenario(
	const CIdentifier& rScenarioIdentifier)
{
	map<CIdentifier, IScenario*>::iterator itScenario;
	itScenario=m_vScenario.find(rScenarioIdentifier);
	if(itScenario==m_vScenario.end())
	{
		return false;
	}
	OpenViBE::Tools::CObjectFactoryHelper(getKernelContext().getObjectFactory()).releaseObject(itScenario->second);
	m_vScenario.erase(itScenario);
	return true;
}

IScenario& CScenarioManager::getScenario(
	const CIdentifier& rScenarioIdentifier)
{
	map<CIdentifier, IScenario*>::const_iterator itScenario;
	itScenario=m_vScenario.find(rScenarioIdentifier);
	if(itScenario==m_vScenario.end())
	{
		log() << LogLevel_Fatal << "Added test bitmask" << CIdentifier(1, 1) << " - TODO remove this\n";
	}
	return *itScenario->second;
}

boolean CScenarioManager::enumerateScenarios(
	IScenarioManager::IScenarioEnum& rCallback) const
{
	map<CIdentifier, IScenario*>::const_iterator itScenario;
	for(itScenario=m_vScenario.begin(); itScenario!=m_vScenario.end(); itScenario++)
	{
		if(!rCallback.callback(itScenario->first, *itScenario->second))
		{
			return true;
		}
	}
	return true;
}

CIdentifier CScenarioManager::getUnusedIdentifier(void) const
{
	uint64 l_ui64Identifier=(((uint64)rand())<<32)+((uint64)rand());
	CIdentifier l_oResult;
	map<CIdentifier, IScenario*>::const_iterator i;
	do
	{
		l_ui64Identifier++;
		l_oResult=CIdentifier(l_ui64Identifier);
		i=m_vScenario.find(l_oResult);
	}
	while(i!=m_vScenario.end() || l_oResult==OV_UndefinedIdentifier);
	return l_oResult;
}
