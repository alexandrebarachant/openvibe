/*
 * Prints user-specified greeting to the log with given frequency 
 */ 
#include "ovpCHelloWorld.h"

#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Samples;
using namespace OpenViBEToolkit;
using namespace std;


OpenViBE::uint64 CHelloWorld::getClockFrequency(void)
{
	CString l_sFrequency;
	getBoxAlgorithmContext()->getStaticBoxContext()->getSettingValue(0, l_sFrequency);

	// Once a second
	return 	(uint64)( double( (uint64)1<<32 ) * atof(l_sFrequency));
}

void CHelloWorld::release(void)
{
	delete this;
}

boolean CHelloWorld::processClock(OpenViBE::Kernel::IMessageClock& /* rMessageClock */)
{

	CString l_sMyGreeting;
	getBoxAlgorithmContext()->getStaticBoxContext()->getSettingValue(1, l_sMyGreeting);

	getLogManager() << OpenViBE::Kernel::LogLevel_Info << ": " << l_sMyGreeting << "\n";

	return true;
}

boolean CHelloWorld::process(void)
{
	return true;
}

