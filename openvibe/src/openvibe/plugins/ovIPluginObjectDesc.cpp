#include "ovIPluginObjectDesc.h"

using namespace OpenViBE;
using namespace OpenViBE::Plugins;

CString IPluginObjectDesc::getName(void)
{
	return CString("no name");
}

CString IPluginObjectDesc::getAuthorName(void)
{
	return CString("unknown");
}

CString IPluginObjectDesc::getAuthorCompanyName(void)
{
	return CString("unknown");
}

CString IPluginObjectDesc::getShortDescription(void)
{
	return CString("");
}

CString IPluginObjectDesc::getDetailedDescription(void)
{
	return CString("");
}

CString IPluginObjectDesc::getCategory(void)
{
	return CString("unknown");
}

CString IPluginObjectDesc::getVersion(void)
{
	return CString("unknown");
}
