#include "ovIPluginObject.h"

using namespace OpenViBE;
using namespace OpenViBE::Plugins;

boolean IPluginObject::initialize(
	const IPluginObjectContext& rContext)
{
	return true;
}

boolean IPluginObject::uninitialize(
	const IPluginObjectContext& rContext)
{
	return true;
}

#if 0
boolean IPluginObject::load(IPluginObjectLoadContext& rLoadContext)
{
	return true;
}

boolean IPluginObject::save(IPluginObjectSaveContext& rSaveContext)
{
	return true;
}
#endif
