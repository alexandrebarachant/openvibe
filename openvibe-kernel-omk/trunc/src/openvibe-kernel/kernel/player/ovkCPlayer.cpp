#include "ovkCPlayer.h"

// #define __Distributed_Controller__
// #define __Has_XML_Simulation_File__

#include "ovkEventId.h"

#include "types/ovkPsTypeChunk.h"

#include "simulated-objects/ovkPsSimulatedBox.h"

#include "ovkCStaticBoxContext.h"
#include "ovkCDynamicBoxContext.h"

#include <PsMultipleConfigurationParameter.h>
#include <PsUniqueConfigurationParameter.h>
#include <PsSimulatedObjectCreator.h>
#if defined __Has_XML_Simulation_File__
 #include <OmkXml.h>
#endif
#if defined __Distributed_Controller__
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

class PsSimulatedBoxCreator : public ::PsSimulatedObjectCreator
{
public:
	PsSimulatedBoxCreator(const IKernelContext& rKernelContext, const IScenario& rScenario)
		:m_rKernelContext(rKernelContext)
		,m_rScenario(rScenario)
	{
	}

	virtual ::PsSimulatedObject* createSimulatedObject(
		::PsController& rControler,
		const ::PsObjectDescriptor& rObjectDescriptor) const
	{
		return new ::PsSimulatedBox(rControler, rObjectDescriptor, m_rKernelContext, m_rScenario);
	}

protected:
	const IKernelContext& m_rKernelContext;
	const IScenario& m_rScenario;
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
	if(m_pController)
	{
		m_pController->finish();
		m_pController->runControllersStep(m_pControllerHandle);

		delete m_pController;
		m_pController=NULL;
		m_pControllerHandle=NULL;
	}

	if(m_pSimulation)
	{
		delete m_pSimulation;
		m_pSimulation=NULL;
	}
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
		m_pController->finish();
		m_pController->runControllersStep(m_pControllerHandle);

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
	CIdentifier l_oBoxIdentifier=rScenario.getNextBoxIdentifier(OV_UndefinedIdentifier);
	while(l_oBoxIdentifier!=OV_UndefinedIdentifier)
	{
		// TODO choose a valid object frequency
		log() << LogLevel_Debug << "CPlayer::callback - TODO choose a valid object frequency\n";

		::PsMultipleConfigurationParameter* l_pSimulatedBoxConfiguration=new ::PsMultipleConfigurationParameter();
		::PsObjectDescriptor* l_pSimulationBox=new ::PsObjectDescriptor(idToString(l_oBoxIdentifier), "PsSimulatedBox", "ProcessA", 1000, l_pSimulatedBoxConfiguration);
		m_pSimulation->addSon(l_pSimulationBox);

		l_oBoxIdentifier=rScenario.getNextBoxIdentifier(l_oBoxIdentifier);
	}

#if defined __Has_XML_Simulation_File__
	::omk::xml::save("/tmp/OpenViBE-log-[dumpedconfig.OpenMASK3].log", l_pSimulation);
#endif

#ifdef __Distributed_Controller__
	m_pController=new ::PsPvmController(*m_pSimulation, 0, g_argc, g_argv);
#else
	m_pController=new ::PsController(*m_pSimulation, 0);
#endif
	m_pController->addInstanceCreator("PsSimulatedBox", new ::PsSimulatedBoxCreator(getKernelContext(), rScenario));

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
