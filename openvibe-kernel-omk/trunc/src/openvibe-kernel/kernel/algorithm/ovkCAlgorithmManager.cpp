#include "ovkCAlgorithmManager.h"
#include "ovkCAlgorithmProxy.h"
#include "ovkCAlgorithm.h"

#include <system/Math.h>

using namespace OpenViBE;
using namespace Kernel;
using namespace Plugins;
using namespace std;

CAlgorithmManager::CAlgorithmManager(const IKernelContext& rKernelContext)
	:TKernelObject<IAlgorithmManager>(rKernelContext)
{
}

CAlgorithmManager::~CAlgorithmManager(void)
{
	map < CIdentifier, pair < CAlgorithm*, CAlgorithmProxy* > >::iterator itAlgorithm;
	for(itAlgorithm=m_vAlgorithm.begin(); itAlgorithm!=m_vAlgorithm.end(); itAlgorithm++)
	{
		delete itAlgorithm->second.second;
		delete itAlgorithm->second.first;
	}
}

CIdentifier CAlgorithmManager::createAlgorithm(
	const CIdentifier& rAlgorithmClassIdentifier)
{
	const IAlgorithmDesc* l_pAlgorithmDesc=NULL;
	IAlgorithm* l_pAlgorithm=getKernelContext().getPluginManager().createAlgorithm(rAlgorithmClassIdentifier, &l_pAlgorithmDesc);
	if(!l_pAlgorithm || !l_pAlgorithmDesc)
	{
		return OV_UndefinedIdentifier;
	}

	CIdentifier l_oAlgorithmIdentifier=getUnusedIdentifier();
	CAlgorithm* l_pTrueAlgorithm=new CAlgorithm(getKernelContext(), *l_pAlgorithm, *l_pAlgorithmDesc);
	CAlgorithmProxy* l_pAlgorithmProxy=new CAlgorithmProxy(getKernelContext(), *l_pTrueAlgorithm);
	m_vAlgorithm[l_oAlgorithmIdentifier]=pair < CAlgorithm*, CAlgorithmProxy* >(l_pTrueAlgorithm, l_pAlgorithmProxy);
	return l_oAlgorithmIdentifier;
}

boolean CAlgorithmManager::releaseAlgorithm(
	const CIdentifier& rAlgorithmIdentifier)
{
	map < CIdentifier, pair < CAlgorithm*, CAlgorithmProxy* > >::iterator itAlgorithm;
	itAlgorithm=m_vAlgorithm.find(rAlgorithmIdentifier);
	if(itAlgorithm==m_vAlgorithm.end())
	{
		return false;
	}
	delete itAlgorithm->second.second;
	delete itAlgorithm->second.first;
	m_vAlgorithm.erase(itAlgorithm);
	return true;
}

boolean CAlgorithmManager::releaseAlgorithm(
	IAlgorithmProxy& rAlgorithm)
{
	map < CIdentifier, pair < CAlgorithm*, CAlgorithmProxy* > >::iterator itAlgorithm;
	for(itAlgorithm=m_vAlgorithm.begin(); itAlgorithm!=m_vAlgorithm.end(); itAlgorithm++)
	{
		if((IAlgorithmProxy*)itAlgorithm->second.second==&rAlgorithm)
		{
			delete itAlgorithm->second.second;
			delete itAlgorithm->second.first;
			m_vAlgorithm.erase(itAlgorithm);
			return true;
		}
	}
	return false;
}

IAlgorithmProxy& CAlgorithmManager::getAlgorithm(
	const CIdentifier& rAlgorithmIdentifier)
{
	map < CIdentifier, pair < CAlgorithm*, CAlgorithmProxy* > >::const_iterator itAlgorithm;
	itAlgorithm=m_vAlgorithm.find(rAlgorithmIdentifier);
	if(itAlgorithm==m_vAlgorithm.end())
	{
		log() << LogLevel_Fatal << "Algorithm " << rAlgorithmIdentifier << " does not exist !\n";
	}
	return *itAlgorithm->second.second;
}

CIdentifier CAlgorithmManager::getNextAlgorithmIdentifier(
	const CIdentifier& rPreviousIdentifier) const
{
	map < CIdentifier, pair < CAlgorithm*, CAlgorithmProxy* > >::const_iterator itAlgorithm=m_vAlgorithm.begin();

	if(rPreviousIdentifier==OV_UndefinedIdentifier)
	{
		itAlgorithm=m_vAlgorithm.begin();
	}
	else
	{
		itAlgorithm=m_vAlgorithm.find(rPreviousIdentifier);
		if(itAlgorithm==m_vAlgorithm.end())
		{
			return OV_UndefinedIdentifier;
		}
		itAlgorithm++;
	}

	return itAlgorithm!=m_vAlgorithm.end()?itAlgorithm->first:OV_UndefinedIdentifier;
}

CIdentifier CAlgorithmManager::getUnusedIdentifier(void) const
{
	uint64 l_ui64Identifier=System::Math::randomUInteger64();
	CIdentifier l_oResult;
	map < CIdentifier, pair < CAlgorithm*, CAlgorithmProxy* > >::const_iterator i;
	do
	{
		l_ui64Identifier++;
		l_oResult=CIdentifier(l_ui64Identifier);
		i=m_vAlgorithm.find(l_oResult);
	}
	while(i!=m_vAlgorithm.end() || l_oResult==OV_UndefinedIdentifier);
	return l_oResult;
}
