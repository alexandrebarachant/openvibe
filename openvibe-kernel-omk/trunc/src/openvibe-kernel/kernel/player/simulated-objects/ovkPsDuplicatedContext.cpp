

#include "ovkPsDuplicatedContext.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;

//========================================================================
// Default constructor
PsDuplicatedContext::PsDuplicatedContext( PsController& ctrl, const PsObjectDescriptor& objectDescriptor, const IScenario& rScenario, IPluginManager& rPluginManager )
: PsDuplicatedContextBase( ctrl, objectDescriptor )
, m_rScenario(rScenario)
, m_rPluginManager(rPluginManager)
{
}

//========================================================================
// Destructor
PsDuplicatedContext::~PsDuplicatedContext()
{
}

//========================================================================
// Accessor
const IScenario& PsDuplicatedContext::getScenario(void) const
{
	return m_rScenario;
}

IPluginManager& PsDuplicatedContext::getPluginManager(void) const
{
	return m_rPluginManager;
}

//========================================================================
void PsDuplicatedContext::computeParameters()
{
  // TODO : add your behaviour code here
}

//========================================================================
