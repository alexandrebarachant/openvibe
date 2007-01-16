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
	PsDuplicatedContextCreator(const IKernelContext& rKernelContext, const IScenario& rScenario, IPluginManager& rPluginManager)
		:m_rKernelContext(rKernelContext)
		,m_rScenario(rScenario)
		,m_rPluginManager(rPluginManager)
	{
	}

	virtual ::PsSimulatedObject* createSimulatedObject(
		::PsController& rControler,
		const ::PsObjectDescriptor& rObjectDescriptor) const
	{
		return new ::PsDuplicatedContext(rControler, rObjectDescriptor, m_rKernelContext, m_rScenario, m_rPluginManager);
	}

protected:
	const IKernelContext& m_rKernelContext;
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

	::PsMultipleConfigurationParameter* l_pSimulationSchedulingDescription=new ::PsMultipleConfigurationParameter();
	::PsMultipleConfigurationParameter* l_pSimulationSchedulingMachinesDescription=new ::PsMultipleConfigurationParameter();
	l_pSimulationSchedulingMachinesDescription->appendSubDescriptorNamed("ProcessA", new ::PsUniqueConfigurationParameter("hiboux.irisa.fr"));
	l_pSimulationSchedulingDescription->appendSubDescriptorNamed("Machines", l_pSimulationSchedulingMachinesDescription);
	l_pSimulationSchedulingDescription->appendSubDescriptorNamed("Latency", new ::PsUniqueConfigurationParameter("10"));

	m_pSimulation=new ::PsObjectDescriptor("root", "Controller", l_pSimulationSchedulingDescription, NULL);

	::PsMultipleConfigurationParameter* l_pDuplicatedContextConfiguration=new ::PsMultipleConfigurationParameter();
	::PsObjectDescriptor* l_pDuplicatedContext=new ::PsObjectDescriptor("OpenViBEContext", "PsDuplicatedContext", 0, l_pDuplicatedContextConfiguration);
	m_pSimulation->addSon(l_pDuplicatedContext);

	rScenario.enumerateBoxes(*this);

	// ::omk::xml::save("/tmp/OpenViBE-log-[dumpedconfig.OpenMASK3].log", l_pSimulation);

#ifdef __Distributed_Controller__
	m_pController=new ::PsPvmController(*m_pSimulation, 0, g_argc, g_argv);
#else
	m_pController=new ::PsController(*m_pSimulation, 0);
#endif
	m_pController->addInstanceCreator("PsSimulatedBox", new ::PsSimpleSimulatedObjectCreator< ::PsSimulatedBox >());
	m_pController->addInstanceCreator("PsDuplicatedContext", new ::PsDuplicatedContextCreator(getKernelContext(), rScenario, rPluginManager));

	m_pController->init();
	m_pControllerHandle=dynamic_cast< ::PsnReferenceObjectHandle*>(m_pController->getObjectHandle());

	return true;
}

boolean CPlayer::loop(void)
{
	// Tools::CScopeTester("CPlayer::loop");

	m_pController->runControllersStep(m_pControllerHandle);

	return true;
}

boolean CPlayer::callback(const IScenario& rScenario, IBox& rBox)
{
	if(!m_pSimulation) return false;

	// TODO choose a valid object frequency
	log() << LogLevel_Debug << "CPlayer::callback - TODO choose a valid object frequency\n";

	::PsMultipleConfigurationParameter* l_pSimulatedBoxConfiguration=new ::PsMultipleConfigurationParameter();
	::PsObjectDescriptor* l_pSimulationBox=new ::PsObjectDescriptor(idToString(rBox.getIdentifier()), "PsSimulatedBox", "ProcessA", 1000, l_pSimulatedBoxConfiguration);
	m_pSimulation->addSon(l_pSimulationBox);

	return true;
}
