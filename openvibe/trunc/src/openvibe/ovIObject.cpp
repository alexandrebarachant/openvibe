#include "ovIObject.h"

using namespace OpenViBE;

IObject::~IObject(void)
{
}

boolean IObject::processMessage(IObjectContext& rObjectContext, CMessage& rMessage)
{
	return false;
}
