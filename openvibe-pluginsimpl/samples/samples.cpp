#include "ovCBoxAlgorithm.h"
#include "ovCBoxBehavior.h"

static OpenViBESamples::CBoxAlgorithmDesc* lst_pBoxAlgorithmDesc=NULL;
static OpenViBESamples::CBoxBehaviorDesc* lst_pBoxBehaviorDesc=NULL;

extern "C"
{

OpenViBE::boolean onLoad(void)
{
	lst_pBoxAlgorithmDesc=new OpenViBESamples::CBoxAlgorithmDesc();
	lst_pBoxBehaviorDesc=new OpenViBESamples::CBoxBehaviorDesc();
	return true;
}

OpenViBE::boolean onUnload(void)
{
	delete lst_pBoxAlgorithmDesc;
	delete lst_pBoxBehaviorDesc;
	return true;
}

OpenViBE::boolean onGetPluginObjectDescription(OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
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
