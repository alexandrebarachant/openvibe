#include "ovkCObjectFactory.h"

#include "ovkCBox.h"
#include "ovkCLink.h"
#include "ovkCScenario.h"

#include "ovkCPluginModule.h"

#include "player/ovkCPlayer.h"

#include "ovkCLogListenerConsole.h"
#include "ovkCLogListenerNull.h"

#include <string>
#include <algorithm>

using namespace std;
using namespace OpenViBE;
#define boolean OpenViBE::boolean

#define create(rcid,cid,sptr,cl) \
	if(rcid==cid) \
	{ \
		sptr=new cl(getKernelContext()); \
		if(sptr) \
		{ \
			m_oCreatedObjects.push_back(sptr); \
		} \
	}

Kernel::CObjectFactory::CObjectFactory(const Kernel::IKernelContext& rKernelContext)
	:TKernelObject<IObjectFactory>(rKernelContext)
{
}

IObject* Kernel::CObjectFactory::createObject(
	const CIdentifier& rClassIdentifier)
{
	IObject* l_pResult=NULL;

	create(rClassIdentifier, OV_ClassId_Kernel_Box,                 l_pResult, Kernel::CBox);
	create(rClassIdentifier, OV_ClassId_Kernel_Link,                l_pResult, Kernel::CLink);
	create(rClassIdentifier, OV_ClassId_Kernel_Scenario,            l_pResult, Kernel::CScenario);

	create(rClassIdentifier, OV_ClassId_Kernel_PluginModule,        l_pResult, Kernel::CPluginModule);

	create(rClassIdentifier, OV_ClassId_Kernel_Player_Player,       l_pResult, Kernel::Player::CPlayer);

	create(rClassIdentifier, OVK_ClassId_Kernel_LogListenerConsole, l_pResult, Kernel::CLogListenerConsole);
	create(rClassIdentifier, OVK_ClassId_Kernel_LogListenerNull,    l_pResult, Kernel::CLogListenerNull);

	// create(rClassIdentifier, OV_ClassId_, l_pResult, Plugins::CBoxContext);

	if(l_pResult)
	{
		log() << LogLevel_Trace << "Created object with class id " << rClassIdentifier << " and final class id " << l_pResult->getClassIdentifier() << "\n";
	}
	else
	{
		log() << LogLevel_Warning << "Unable to allocate object with class id " << rClassIdentifier << "\n";
	}

	return l_pResult;
}

boolean Kernel::CObjectFactory::releaseObject(
	IObject* pObject)
{
	if(!pObject)
	{
		return true;
	}

	CIdentifier l_rClassIdentifier;
	l_rClassIdentifier=pObject->getClassIdentifier();

	vector<IObject*>::iterator i;
	i=find(m_oCreatedObjects.begin(), m_oCreatedObjects.end(), pObject);
	if(i==m_oCreatedObjects.end())
	{
		log() << LogLevel_Warning << "Can not release object with final class id " << l_rClassIdentifier << " - it is not owned by this fatory\n";
		return false;
	}
	m_oCreatedObjects.erase(i);
	delete pObject;

	log() << LogLevel_Trace << "Released object with final class id " << l_rClassIdentifier << "\n";

	return true;
}
