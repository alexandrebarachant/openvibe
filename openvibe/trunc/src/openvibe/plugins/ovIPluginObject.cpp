#include "ovIPluginObject.h"

using namespace OpenViBE;
using namespace OpenViBE::Plugins;

boolean IPluginObject::initialize(void)
{
	return true;
}

boolean IPluginObject::uninitialize(void)
{
	return true;
}

boolean IPluginObject::load(IPluginObjectLoadContext& rLoadContext)
{
	return true;
}

boolean IPluginObject::save(IPluginObjectSaveContext& rSaveContext)
{
	return true;
}
