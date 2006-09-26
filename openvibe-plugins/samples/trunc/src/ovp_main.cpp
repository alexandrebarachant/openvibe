#include "ovp_defines.h"
#include "ovCBoxAlgorithm.h"
#include "ovCBoxBehavior.h"
#include <iostream>

static OpenViBESamples::CBoxAlgorithmDesc* lst_pBoxAlgorithmDesc=NULL;
static OpenViBESamples::CBoxBehaviorDesc* lst_pBoxBehaviorDesc=NULL;

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(void)
{
	lst_pBoxAlgorithmDesc=new OpenViBESamples::CBoxAlgorithmDesc();
	lst_pBoxBehaviorDesc=new OpenViBESamples::CBoxBehaviorDesc();
	return true;
}

OVP_API OpenViBE::boolean onUninitialize(void)
{
	delete lst_pBoxAlgorithmDesc;
	delete lst_pBoxBehaviorDesc;
	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=lst_pBoxAlgorithmDesc; break;
		case 1: rpPluginObjectDescription=lst_pBoxBehaviorDesc; break;
		case 2: rpPluginObjectDescription=NULL; break;
		default: return false;
	}
	return true;
}

}
