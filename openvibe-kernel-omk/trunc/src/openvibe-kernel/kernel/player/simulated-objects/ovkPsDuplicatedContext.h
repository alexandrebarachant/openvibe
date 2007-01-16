#if !defined _SIMULATED_OBJECTS_OVKPSDUPLICATEDCONTEXT_H_
#define _SIMULATED_OBJECTS_OVKPSDUPLICATEDCONTEXT_H_

#include "ovkPsDuplicatedContextBase.h"

#include <openvibe/ov_all.h>

/** \brief Implementation class of \ref PsDuplicatedContextBase.
 * \date 2006-09-21 at 15:37:26
 * \author Yann Renard (INRIA/IRISA)
 * @description
 * This class is the implementation class of  \ref PsDuplicatedContext.\n
 * All the members of this class are protected except the constructor and
 * the destructor according to the OpenMASK precepts.\n\n
 * @configurationParameters
 * \n See the base class \ref PsDuplicatedContextBase to see used configuration parameters.\n
 * \todo \ref computeParameters must be written.
 */
class PsDuplicatedContext : public PsDuplicatedContextBase
{
	/// @name Constructors and destructor.
	//@{

public:

	/// \brief Destructor of \ref PsDuplicatedContext.
	virtual ~PsDuplicatedContext() ;
	/// \brief Protected default constructor of \ref PsDuplicatedContext call by creator.
	PsDuplicatedContext( PsController& ctrl, const PsObjectDescriptor& objectDescriptor, const OpenViBE::Kernel::IKernelContext& m_rKernelContext, const OpenViBE::Kernel::IScenario& rScenario, OpenViBE::Kernel::IPluginManager& rPluginManager );
	//@}
	const OpenViBE::Kernel::IKernelContext& getKernelContext(void) const;
	const OpenViBE::Kernel::IScenario& getScenario(void) const;
	OpenViBE::Kernel::IPluginManager& getPluginManager(void) const;

protected:

	/// @name PsSimulatedObject interfaces. Compute method inherited from \ref PsDuplicatedContextBase.
	//@{
	/// \brief Compute methods inherited from \ref PsDuplicatedContextBase.
	///
	/// The implementation of the behaviour.
	/// This method  updates nothing.
	virtual void computeParameters() ;
	//@}

	const OpenViBE::Kernel::IKernelContext& m_rKernelContext;
	const OpenViBE::Kernel::IScenario& m_rScenario;
	mutable OpenViBE::Kernel::IPluginManager& m_rPluginManager;
};

#endif // defined _SIMULATED_OBJECTS_OVKPSDUPLICATEDCONTEXT_H_
