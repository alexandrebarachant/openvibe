#include "ovp_defines.h"
#include <openvibe/ov_all.h>

extern "C"
{

OVP_API OpenViBE::boolean onInitialize(void)
{
	return true;
}

OVP_API OpenViBE::boolean onUninitialize(void)
{
	return true;
}

OVP_API OpenViBE::boolean onGetPluginObjectDescription(OpenViBE::uint32 ui32Index, OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)
{
#if 0
	switch(ui32Index)
	{
		case 0: rpPluginObjectDescription=/* TODO */; break;

		default: return false;
	}
	return true;
#endif
	return false;
}

}
