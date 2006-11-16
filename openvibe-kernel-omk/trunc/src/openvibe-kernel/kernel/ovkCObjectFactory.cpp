#include "ovkCObjectFactory.h"

#include "ovkCBox.h"
#include "ovkCLink.h"
#include "ovkCScenario.h"

#include "ovkCPluginManager.h"
#include "ovkCPluginModule.h"

#include "ovkCLogManager.h"
#include "ovkCLogManagerConsole.h"
#include "ovkCLogManagerNull.h"

#include "player/ovkCPlayer.h"

#include <string>
#include <algorithm>

using namespace std;
using namespace OpenViBE;
#define boolean OpenViBE::boolean

#define create(rcid,cid,sptr,cl) \
	if(rcid==cid) \
	{ \
		sptr=new cl(getKernelContext()); \
	}

#define create2(rcid,cid,sptr,ud,cl) \
	if(rcid==cid) \
	{ \
		sptr=new cl(getKernelContext(),ud); \
	}

Kernel::CObjectFactory::CObjectFactory(const Kernel::IKernelContext& rKernelContext)
	:TKernelObject<IObjectFactory>(rKernelContext)
{
}

IObject* Kernel::CObjectFactory::createObject(
	const CIdentifier& rClassIdentifier)
{
	IObject* l_pResult=NULL;
	create(rClassIdentifier, OV_ClassId_Kernel_Box,                l_pResult, Kernel::CBox);
	create(rClassIdentifier, OV_ClassId_Kernel_Link,               l_pResult, Kernel::CLink);
	create(rClassIdentifier, OV_ClassId_Kernel_Scenario,           l_pResult, Kernel::CScenario);

//	create(rClassIdentifier, OV_ClassId_Kernel_PluginManager,      l_pResult, Kernel::CPluginManager);
	create(rClassIdentifier, OV_ClassId_Kernel_PluginModule,       l_pResult, Kernel::CPluginModule);

	create(rClassIdentifier, OV_ClassId_Kernel_Player_Player,      l_pResult, Kernel::Player::CPlayer);

	// create(rClassIdentifier, OV_ClassId_, l_pResult, Plugins::CBoxContext);

	create(rClassIdentifier, OVK_ClassId_Kernel_LogManagerConsole, l_pResult, Kernel::CLogManagerConsole);
	create(rClassIdentifier, OVK_ClassId_Kernel_LogManagerNull,    l_pResult, Kernel::CLogManagerNull);

	return l_pResult;
}

IObject* Kernel::CObjectFactory::createObject(
	const CIdentifier& rClassIdentifier,
	IObject& rUserData)
{
	IObject* l_pResult=NULL;
	// create2(rClassIdentifier, OV_ClassId_, l_pResult, rUserData, );

	return l_pResult;
}

IObject* Kernel::CObjectFactory::createObject(
	const CIdentifier& rClassIdentifier,
	const IObject& rUserData)
{
	IObject* l_pResult=NULL;
	// create2(rClassIdentifier, OV_ClassId_, l_pResult, dynamic_cast<>(rUserData), );

	return l_pResult;
}

boolean Kernel::CObjectFactory::releaseObject(
	IObject* pObject)
{
	if(!pObject)
	{
		return true;
	}

	vector<IObject*>::iterator i;
	i=find(m_oCreatedObjects.begin(), m_oCreatedObjects.end(), pObject);
	if(i==m_oCreatedObjects.end())
	{
		return false;
	}
	m_oCreatedObjects.erase(i);
	// delete pObject;

	return true;
}
