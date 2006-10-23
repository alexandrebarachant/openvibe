

#if !defined _SIMULATED_OBJECTS_OVKPSSIMULATEDBOX_H_
#define _SIMULATED_OBJECTS_OVKPSSIMULATEDBOX_H_

#include "ovkPsSimulatedBoxBase.h"
#include "ovkPsDuplicatedContext.h"

#include <vector>
#include <string>

class IScenarioHandle
{
public:

	IScenarioHandle(::PsSimulatedObject &rObject)
		:m_rObject(rObject)
	{
	}

	const OpenViBE::Kernel::IScenario& getConcreteScenario(void)
	{
		::PsDuplicatedContext* l_pOpenViBEContext=dynamic_cast< ::PsDuplicatedContext* >(m_rObject.getController().getPointerToDuplicatedObjectNamed("OpenViBEContext"));
		assert(l_pOpenViBEContext);

		return l_pOpenViBEContext->getScenario();
	}

protected:

	::PsSimulatedObject& m_rObject;
};

class IPluginManagerHandle
{
public:

	IPluginManagerHandle(::PsSimulatedObject& rObject)
		:m_rObject(rObject)
	{
	}

	OpenViBE::Kernel::IPluginManager& getConcretePluginManager(void)
	{
		::PsDuplicatedContext* l_pOpenViBEContext=dynamic_cast< ::PsDuplicatedContext* >(m_rObject.getController().getPointerToDuplicatedObjectNamed("OpenViBEContext"));
		assert(l_pOpenViBEContext);

		return l_pOpenViBEContext->getPluginManager();
	}

protected:

	::PsSimulatedObject& m_rObject;
};

/** \brief Implementation class of \ref PsSimulatedBoxBase.
 * \date 2006-09-12 at 18:00:28
 * \author Yann Renard (INRIA/IRISA)
 * @description
 * This class is the implementation class of \ref PsSimulatedBoxBase.\n
 * All the members of this class are protected except the constructor and
 * the destructor according to the OpenMASK precepts.\n\n
 * @configurationParameters
 * \n See the base class \ref PsSimulatedBoxBase to see used configuration parameters.\n
 * \todo \ref computeParameters must be written.
 */
class PsSimulatedBox : public PsSimulatedBoxBase
{
public:

	/// @name Constructors and destructor.
	//@{
	/// \brief Destructor of \ref PsSimulatedBox.
	virtual ~PsSimulatedBox() ;
	/// \brief Default constructor of \ref PsSimulatedBox.
	PsSimulatedBox( PsController& ctrl, const PsObjectDescriptor& objectDescriptor );
	//@}

protected:

	virtual void init(void);

	/// @name PsSimulatedObject interfaces. Compute method inherited from \ref PsSimulatedBoxBase.
	//@{
	/// \brief Compute methods inherited from \ref PsSimulatedBoxBase.
	///
	/// The implementation of the behaviour.
	/// This method updates nothing.
	virtual void computeParameters() ;
	//@}

	virtual void doProcess(void);

public:

	virtual OpenViBE::uint32 getInputChunkCount(
		const OpenViBE::uint32 ui32InputIndex);
	virtual OpenViBE::boolean getInputChunk(
		const OpenViBE::uint32 ui32InputIndex,
		const OpenViBE::uint32 ui32ChunkIndex,
		OpenViBE::uint64& rStartTime,
		OpenViBE::uint64& rEndTime,
		OpenViBE::uint64& rChunkSize,
		const OpenViBE::uint8*& rpChunkBuffer);
	virtual OpenViBE::boolean markInputAsDeprecated(
		const OpenViBE::uint32 ui32InputIndex,
		const OpenViBE::uint32 ui32ChunkIndex);

	virtual OpenViBE::uint64 getOutputChunkSize(
		const OpenViBE::uint32 ui32OutputIndex);
	virtual OpenViBE::boolean setOutputChunkSize(
		const OpenViBE::uint32 ui32OutputIndex,
		const OpenViBE::uint64 ui64Size,
		const OpenViBE::boolean bDiscard=true);
	virtual OpenViBE::uint8* getOutputChunkBuffer(
		const OpenViBE::uint32 ui32OutputIndex);
	virtual OpenViBE::boolean markOutputAsReadyToSend(
		const OpenViBE::uint32 ui32OutputIndex,
		const OpenViBE::uint64 ui64StartTime,
		const OpenViBE::uint64 ui64EndTime);

protected:

	/// @name Call back methods and listeners for events processing.
	//@{
	/// \brief Call back to process the "OpenViBEDataUpdate" event
	/// \return see \ref PsValuedEventListenerCallBack::CallBackFct
	virtual bool processOpenViBEDataUpdateEvent( ::PsValuedEvent< ::PsTypeChunk > *e ) ;
	//@}

	IScenarioHandle m_oScenarioHandle;
	IPluginManagerHandle m_oPluginManagerHandle;

	const OpenViBE::Kernel::IBox* m_pBox;

	const OpenViBE::Plugins::IBoxAlgorithmDesc* m_pBoxAlgorithmDesc;
	OpenViBE::Plugins::IBoxAlgorithm* m_pBoxAlgorithm;

public:
	using PsSimulatedBoxBase::sendOpenViBEDataUpdateEvent;

	std::vector<std::vector< ::PsTypeChunk > > m_vInput;
	std::vector< ::PsTypeChunk > m_vOutput;
	// std::vector<std::string> m_vSetting;
};

#endif // defined _SIMULATED_OBJECTS_OVKPSSIMULATEDBOX_H_
