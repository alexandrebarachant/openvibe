#include "ovIPluginObjectDesc.h"

using namespace OpenViBE;
using namespace OpenViBE::Plugins;

CString IPluginObjectDesc::getName(void) const
{
	return CString("no name");
}

CString IPluginObjectDesc::getAuthorName(void) const
{
	return CString("unknown");
}

CString IPluginObjectDesc::getAuthorCompanyName(void) const
{
	return CString("unknown");
}

CString IPluginObjectDesc::getShortDescription(void) const
{
	return CString("");
}

CString IPluginObjectDesc::getDetailedDescription(void) const
{
	return CString("");
}

CString IPluginObjectDesc::getCategory(void) const
{
	return CString("unknown");
}

CString IPluginObjectDesc::getVersion(void) const
{
	return CString("unknown");
}
