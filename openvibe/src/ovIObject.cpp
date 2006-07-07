#include "ovIObject.h"

namespace OpenViBE
{

IObject::~IObject(void)
{
}

OpenViBE::boolean IObject::initialize(void)
{
	return true;
}

OpenViBE::boolean IObject::uninitialize(void)
{
	return true;
}

OpenViBE::boolean IObject::processMessage(OpenViBE::IObjectContext& rObjectContext, OpenViBE::CMessage& rMessage)
{
	return false;
}

};
