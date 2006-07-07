#include "ovIPluginObject.h"

namespace OpenViBE
{
namespace Plugins
{

OpenViBE::boolean IPluginObject::load(OpenViBE::Kernel::IPluginObjectLoadContext& rLoadContext)
{
	return true;
}

OpenViBE::boolean IPluginObject::save(OpenViBE::Kernel::IPluginObjectSaveContext& rSaveContext)
{
	return true;
}

};
};
