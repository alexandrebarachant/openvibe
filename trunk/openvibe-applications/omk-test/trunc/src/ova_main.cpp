#include <OMKController.h>
#include <OMKMultipleConfigurationParameter.h>
#include <OMKUniqueConfigurationParameter.h>
#include <OMKSimpleOpenMASK3Loader.h>
#include <OMKPluginsLoader.h>

#include <OMKValuedEventListenerCallBack.h>
#include <OMKEventListenerCallBack.h>
#include <OMKSystemEventIdentifier.h>

#include <OMKExtensibleSimulatedObject.h>

#include <OMKSimpleOpenMASK3Loader.h>
#include <OMKObjectDescriptor.h>
#include <OMKController.h>
#include <OMKPluginsLoader.h>

#include <unistd.h>
#include <ctime>
#include <sys/ctime>

#include <string>

//___________________________________________________________________________
//
// 64 bits time estimation (fixed point 32:32 in seconds)
//___________________________________________________________________________
//

unsigned long long get_time(void)
{
	unsigned long long l_ui64Result=0;
	struct timeval l_oTimeValue;
	gettimeofday(&l_oTimeValue, NULL);
	l_ui64Result+=(((unsigned long long)l_oTimeValue.tv_sec)<<32);
	l_ui64Result+=(((unsigned long long)l_oTimeValue.tv_usec)<<32)/1000000;
	return l_ui64Result;
}

//___________________________________________________________________________
//
// Declare and register a simulated object that does nothing
//___________________________________________________________________________
//

class CSimulatedNothing : public OMK::SimulatedObject
{
public:

	DECLARE_OBJECT_FACTORY(CSimulatedNothing);
};

REGISTER_OBJECT_FACTORY(CSimulatedNothing, "SimulatedNothing");

CSimulatedNothing::CSimulatedNothing(OMK::Controller& rController, const OMK::ObjectDescriptor& rObjectDescriptor)
	:OMK::SimulatedObject(rController, rObjectDescriptor)
{
}

CSimulatedNothing::~CSimulatedNothing(void)
{
}

//___________________________________________________________________________
//
// Main function
//___________________________________________________________________________
//

int main(int argc, char** argv)
{
	if(argc!=2)
	{
		std::cout << "syntax: " << argv[0] << " omk_configuration.OpenMASK3\n";
		return 1;
	}

	OMK::SimpleOpenMASK3Loader* m_pLoader=NULL;
	OMK::ObjectDescriptor* m_pSimulation=NULL;
	OMK::Controller* m_pController=NULL;
	OMK::ReferenceObjectHandle* m_pControllerHandle=NULL;

	std::cout << "Loading configuration file [" << argv[1] << "]...\n";

	m_pLoader=new OMK::SimpleOpenMASK3Loader(argv[1]);
	m_pSimulation=m_pLoader->getRootObjectDescriptor();
	m_pController=new OMK::Controller(*m_pSimulation, 0);

	std::cout << "Initializing plugins loader...\n";

	OMK::PluginsLoader::getInstance().init(*m_pController);

	std::cout << "Initializing controller...\n";

	m_pController->init();
	m_pControllerHandle=dynamic_cast< OMK::ReferenceObjectHandle*>(m_pController->getObjectHandle());

	const unsigned long long l_ui64SimulatedSeconds=32;
	const unsigned long long l_ui64SimulatedFrequency=200;

	std::cout << "Computing " << l_ui64SimulatedSeconds << " seconds of simulated time\n";
	std::cout << "If all simulated objects have the same frequency and this frequency is assumed to be " << l_ui64SimulatedFrequency << "Hz, it should be done in " << l_ui64SimulatedFrequency*l_ui64SimulatedSeconds << " controller steps\n";

	unsigned long long l_ui64Time1=::get_time();
	for(unsigned long long i=0; i<l_ui64SimulatedFrequency*l_ui64SimulatedSeconds; i++)
	{
		m_pController->runControllersStep(m_pControllerHandle);
	}
	unsigned long long l_ui64Time2=::get_time();

	std::cout << "Simulation done in " << ((((l_ui64Time2-l_ui64Time1)>>16)*1000)>>16)/1000.0 << " seconds !\n";
	std::cout << "CPU use would be : " << (((((l_ui64Time2-l_ui64Time1)>>16)*1000)/l_ui64SimulatedSeconds)>>16)/10.0 << "%\n";

	m_pController->finish();

	delete m_pController;
	delete m_pSimulation;
	delete m_pLoader;

	return 0;
}
