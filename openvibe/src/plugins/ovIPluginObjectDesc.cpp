#include "ovIPluginObjectDesc.h"

namespace OpenViBE
{
namespace Plugins
{

std::string IPluginObjectDesc::getName(void)
{
	return std::string("no name");
}

std::string IPluginObjectDesc::getAuthorName(void)
{
	return std::string("unknown");
}

std::string IPluginObjectDesc::getAuthorCompanyName(void)
{
	return std::string("unknown");
}

std::string IPluginObjectDesc::getShortDescription(void)
{
	return std::string("");
}

std::string IPluginObjectDesc::getDetailedDescription(void)
{
	return std::string("");
}

std::string IPluginObjectDesc::getCategory(void)
{
	return std::string("unknown");
}

std::string IPluginObjectDesc::getVersion(void)
{
	return std::string("unknown");
}

};
};
