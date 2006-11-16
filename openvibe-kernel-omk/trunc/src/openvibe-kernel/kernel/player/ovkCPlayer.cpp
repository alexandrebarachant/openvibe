#include "ovkCPlayer.h"

// #define __Distributed_Controller__

#include "ovkEventId.h"

#include "types/ovkPsTypeChunk.h"

#include "simulated-objects/ovkPsSimulatedBox.h"
#include "simulated-objects/ovkPsDuplicatedContext.h"

#include "ovkCStaticBoxContext.h"
#include "ovkCDynamicBoxContext.h"

#include <PsMultipleConfigurationParameter.h>
#include <PsUniqueConfigurationParameter.h>
#include <PsSimulatedObjectCreator.h>
#include <OmkXml.h>
#ifdef __Distributed_Controller__
 #include <PsPvmController.h>
#endif

#include <string>
#include <iostream>

//___________________________________________________________________//
//                                                                   //

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Kernel::Player;
using namespace OpenViBE::Plugins;
#define boolean OpenViBE::boolean

//___________________________________________________________________//
//                                                                   //

#define idToString(id) (::PsString((const char*)id.toString()))

//___________________________________________________________________//
//                                                                   //

class PsDuplicatedContextCreator : public ::PsSimulatedObjectCreator
{
public:
	PsDuplicatedContextCreator(const IScenario& rScenario, IPluginManager& rPluginManager)
		:m_rScenario(rScenario)
		,m_rPluginManager(rPluginManager)
	{
	}

	virtual ::PsSimulatedObject* createSimulatedObject(
		::PsController& rControler,
		const ::PsObjectDescriptor& rObjectDescriptor) const
	{
		return new ::PsDuplicatedContext(rControler, rObjectDescriptor, m_rScenario, m_rPluginManager);
	}

protected:
	const IScenario& m_rScenario;
	IPluginManager& m_rPluginManager;
};

//___________________________________________________________________//
//                                                                   //

class CPlayerBoxBuilder : virtual public IScenario::IBoxEnum
{
public:

	CPlayerBoxBuilder(::PsObjectDescriptor* pSimulationRoot, const IKernelContext& rKernelContext)
		:m_pSimulationRoot(pSimulationRoot)
		,m_bIsFirst(true)
		,m_rKernelContext(rKernelContext)
		,m_pBoxAlgorithmDesc(NULL)
	{
	}

	boolean callback(const IScenario& rScenario, const IBox& rBox)
	{
		IPluginManager& l_rPluginManager=m_rKernelContext.getPluginManager();

		if(m_bIsFirst)
		{
			::PsMultipleConfigurationParameter* l_pObjectConfiguration=new ::PsMultipleConfigurationParameter();
			::PsObjectDescriptor* l_pSimulationBox=new ::PsObjectDescriptor("OpenViBEContext", "PsDuplicatedContext", 0, l_pObjectConfiguration);
			m_pSimulationRoot->addSon(l_pSimulationBox);
		}

		m_pBoxAlgorithmDesc=dynamic_cast<const IBoxAlgorithmDesc*>(l_rPluginManager.getPluginObjectDescCreating(rBox.getAlgorithmClassIdentifier()));

		::PsMultipleConfigurationParameter* l_pObjectConfiguration=new ::PsMultipleConfigurationParameter();
		::PsObjectDescriptor* l_pSimulationBox=new ::PsObjectDescriptor(idToString(rBox.getIdentifier()), "PsSimulatedBox", "ProcessA", (int)m_pBoxAlgorithmDesc->getClockFrequency(CStaticBoxContext(&rBox)), l_pObjectConfiguration);
		m_pSimulationRoot->addSon(l_pSimulationBox);

		return true;
	}

public:

	::PsObjectDescriptor* m_pSimulationRoot;
	boolean m_bIsFirst;
	const IKernelContext& m_rKernelContext;
	const IBoxAlgorithmDesc* m_pBoxAlgorithmDesc;
};

//___________________________________________________________________//
//                                                                   //

CPlayer::CPlayer(const IKernelContext& rKernelContext)
	:TKernelObject<IPlayer>(rKernelContext)
	,m_pController(NULL)
	,m_pControllerHandle(NULL)
	,m_pSimulation(NULL)
	,m_pScenario(NULL)
{
}

CPlayer::~CPlayer(void)
{
}

//___________________________________________________________________//
//                                                                   //

extern char** g_argv;
extern int g_argc;

boolean CPlayer::reset(
	const IScenario& rScenario,
	IPluginManager& rPluginManager)
{
	::PsMultipleConfigurationParameter* l_pSimulationSchedulingDescription=new ::PsMultipleConfigurationParameter();
	::PsMultipleConfigurationParameter* l_pSimulationSchedulingMachinesDescription=new ::PsMultipleConfigurationParameter();
	l_pSimulationSchedulingMachinesDescription->appendSubDescriptorNamed("ProcessA", new ::PsUniqueConfigurationParameter("hiboux.irisa.fr"));
	l_pSimulationSchedulingDescription->appendSubDescriptorNamed("Machines", l_pSimulationSchedulingMachinesDescription);
	l_pSimulationSchedulingDescription->appendSubDescriptorNamed("Latency", new ::PsUniqueConfigurationParameter("10"));

	::PsObjectDescriptor* l_pSimulation=new ::PsObjectDescriptor("root", "Controller", l_pSimulationSchedulingDescription, NULL);
	::CPlayerBoxBuilder l_oPlayerBoxBuilder(l_pSimulation, getKernelContext());
	rScenario.enumerateBoxes(l_oPlayerBoxBuilder);

	// ::omk::xml::save("/tmp/OpenViBE-log-[dumpedconfig.OpenMASK3].log", l_pSimulation);

	::PsController* l_pController=NULL;
	::PsnReferenceObjectHandle* l_pControllerHandle=NULL;
#ifdef __Distributed_Controller__
	l_pController=new ::PsPvmController(*l_pSimulation, 0, g_argc, g_argv);
#else
	l_pController=new ::PsController(*l_pSimulation, 0);
#endif
	l_pController->addInstanceCreator("PsSimulatedBox", new ::PsSimpleSimulatedObjectCreator< ::PsSimulatedBox >());
	l_pController->addInstanceCreator("PsDuplicatedContext", new ::PsDuplicatedContextCreator(rScenario, rPluginManager));

	l_pController->init();
	l_pControllerHandle=dynamic_cast< ::PsnReferenceObjectHandle*>(l_pController->getObjectHandle());

	if(m_pController)
	{
		delete m_pController;
		m_pController=NULL;
		m_pControllerHandle=NULL;
	}

	if(m_pSimulation)
	{
		delete m_pSimulation;
		m_pSimulation=NULL;
	}

	m_pController=l_pController;
	m_pControllerHandle=l_pControllerHandle;
	m_pSimulation=l_pSimulation;

	return true;
}

boolean CPlayer::loop(void)
{
	// Tools::CScopeTester("CPlayer::loop");

	m_pController->runControllersStep(m_pControllerHandle);

	return true;
}
