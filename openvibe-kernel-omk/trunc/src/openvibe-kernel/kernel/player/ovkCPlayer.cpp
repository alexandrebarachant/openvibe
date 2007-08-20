#include "ovkCPlayer.h"

// #define __Distributed_Controller__
// #define __Has_XML_Simulation_File__
// #define __Show_Timings__

#include "ovkEventId.h"

#include "ovkPsTypeChunk.h"

#include "ovkPsSimulatedBox.h"

#include <system/Time.h>

#include <PsController.h>
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

#define __ControllerFrequency__  200

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
	,m_ui32ControllerSteps(0)
	,m_ui32StartTime(0)
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
		::PsObjectDescriptor* l_pSimulationBox=new ::PsObjectDescriptor(idToString(l_oBoxIdentifier), "PsSimulatedBox", "ProcessA", __ControllerFrequency__, l_pSimulatedBoxConfiguration);
		m_pSimulation->addSon(l_pSimulationBox);

		l_oBoxIdentifier=rScenario.getNextBoxIdentifier(l_oBoxIdentifier);
	}

#if defined __Has_XML_Simulation_File__
	::omk::xml::save("/tmp/OpenViBE-log-[dumpedconfig.OpenMASK3].log", m_pSimulation);
#endif

	std::stringstream l_sStringStream;
	l_sStringStream << *m_pSimulation;
	log() << LogLevel_Debug << "OpenMASK 3 Scenario :\n" << CString(l_sStringStream.str().c_str()) << "\n";

#ifdef __Distributed_Controller__
	m_pController=new ::PsPvmController(*m_pSimulation, 0, g_argc, g_argv);
#else
	m_pController=new ::PsController(*m_pSimulation, 0);
#endif
	m_pController->addInstanceCreator("PsSimulatedBox", new ::PsSimulatedBoxCreator(getKernelContext(), rScenario));

	cerr << "[  INF  ] Initializing player : "; // this is to introduce cerr'ed string from OpenMASK

	m_pController->init();
	m_pControllerHandle=dynamic_cast< ::PsnReferenceObjectHandle*>(m_pController->getObjectHandle());

	m_ui32ControllerSteps=0;
	m_ui32StartTime=System::Time::getTime();

	m_oBenchmarkChrono.reset(__ControllerFrequency__);
	return true;
}

boolean CPlayer::loop(void)
{
	// Tools::CScopeTester("CPlayer::loop");

	uint32 l_ui32CurrentTime=System::Time::getTime();
	if(l_ui32CurrentTime-m_ui32StartTime>m_ui32ControllerSteps*(1000.0/__ControllerFrequency__))
	{
		m_oBenchmarkChrono.stepIn();
		m_pController->runControllersStep(m_pControllerHandle);
		m_oBenchmarkChrono.stepOut();

		m_ui32ControllerSteps++;
		if((m_ui32ControllerSteps%__ControllerFrequency__)==0)
		{
			log() << LogLevel_Debug << "Player : time elapsed is " << m_ui32ControllerSteps/__ControllerFrequency__ << "s\n";
		}

		if(m_oBenchmarkChrono.hasNewEstimation())
		{
			log() << LogLevel_Benchmark << "Player : processor use is " << m_oBenchmarkChrono.getStepInPercentage() << "%\n";
		}
	}

	return true;
}

