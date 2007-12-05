#if !defined _SIMULATED_OBJECTS_OVKPSSIMULATEDBOX_H_
#define _SIMULATED_OBJECTS_OVKPSSIMULATEDBOX_H_

#include "ovkPsSimulatedBoxBase.h"

#include "../ovkTKernelObject.h"

#include <system/CChrono.h>

#include <vector>
#include <string>
#include <list>

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
class PsSimulatedBox : public PsSimulatedBoxBase, virtual public OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IBoxIO>
{
public:

	/// @name Constructors and destructor.
	//@{
	/// \brief Destructor of \ref PsSimulatedBox.
	virtual ~PsSimulatedBox() ;
	/// \brief Default constructor of \ref PsSimulatedBox.
	PsSimulatedBox(
		PsController& ctrl,
		const PsObjectDescriptor& objectDescriptor,
		const OpenViBE::Kernel::IKernelContext& rKernelContext,
		const OpenViBE::Kernel::IScenario& rScenario);
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

	virtual void handleCrash(const char* sHintName="");
	virtual void doProcess(void);

public:

	virtual OpenViBE::uint64 getCurrentTime() const;
	virtual OpenViBE::CString getOVName(void) const;
	virtual const OpenViBE::Kernel::IScenario& getScenario(void) const;

	virtual OpenViBE::uint32 getInputChunkCount(
		const OpenViBE::uint32 ui32InputIndex) const;
	virtual OpenViBE::boolean getInputChunk(
		const OpenViBE::uint32 ui32InputIndex,
		const OpenViBE::uint32 ui32ChunkIndex,
		OpenViBE::uint64& rStartTime,
		OpenViBE::uint64& rEndTime,
		OpenViBE::uint64& rChunkSize,
		const OpenViBE::uint8*& rpChunkBuffer) const;
	virtual const OpenViBE::IMemoryBuffer* getInputChunk(
		const OpenViBE::uint32 ui32InputIndex,
		const OpenViBE::uint32 ui32ChunkIndex) const;
	virtual OpenViBE::uint64 getInputChunkStartTime(
		const OpenViBE::uint32 ui32InputIndex,
		const OpenViBE::uint32 ui32ChunkIndex) const;
	virtual OpenViBE::uint64 getInputChunkEndTime(
		const OpenViBE::uint32 ui32InputIndex,
		const OpenViBE::uint32 ui32ChunkIndex) const;
	virtual OpenViBE::boolean markInputAsDeprecated(
		const OpenViBE::uint32 ui32InputIndex,
		const OpenViBE::uint32 ui32ChunkIndex);

	virtual OpenViBE::uint64 getOutputChunkSize(
		const OpenViBE::uint32 ui32OutputIndex) const;
	virtual OpenViBE::boolean setOutputChunkSize(
		const OpenViBE::uint32 ui32OutputIndex,
		const OpenViBE::uint64 ui64Size,
		const OpenViBE::boolean bDiscard=true);
	virtual OpenViBE::uint8* getOutputChunkBuffer(
		const OpenViBE::uint32 ui32OutputIndex);
	virtual OpenViBE::boolean appendOutputChunkData(
		const OpenViBE::uint32 ui32OutputIndex,
		const OpenViBE::uint8* pBuffer,
		const OpenViBE::uint64 ui64BufferSize);
	virtual OpenViBE::IMemoryBuffer* getOutputChunk(
		const OpenViBE::uint32 ui32OutputIndex);
	virtual OpenViBE::boolean markOutputAsReadyToSend(
		const OpenViBE::uint32 ui32OutputIndex,
		const OpenViBE::uint64 ui64StartTime,
		const OpenViBE::uint64 ui64EndTime);

	_IsDerivedFromClass_Final_(OpenViBE::Kernel::IBoxIO, OVK_ClassId_Kernel_Player_SimulatedBox);

protected:

	/// @name Call back methods and listeners for events processing.
	//@{
	/// \brief Call back to process the "OpenViBEDataUpdate" event
	/// \return see \ref PsValuedEventListenerCallBack::CallBackFct
	virtual bool processOpenViBEDataUpdateEvent( ::PsValuedEvent< ::PsTypeChunk > *e ) ;
	/// \brief Call back to process the "MaskStart" event
	/// \return see \ref PsEventListenerCallBack::CallBackFct
	virtual bool processMaskStartEvent( ::PsEvent *e ) ;
	/// \brief Call back to process the "MaskStop" event
	/// \return see \ref PsEventListenerCallBack::CallBackFct
	virtual bool processMaskStopEvent( ::PsEvent *e ) ;
	//@}

	OpenViBE::uint32 m_ui32CrashCount;
	OpenViBE::boolean m_bReadyToProcess;
	OpenViBE::boolean m_bActive;

	OpenViBE::Plugins::IBoxAlgorithm* m_pBoxAlgorithm;
	const OpenViBE::Kernel::IScenario* m_pScenario;
	const OpenViBE::Kernel::IBox* m_pBox;

	OpenViBE::uint64 m_ui64LastClockActivationDate;
	OpenViBE::uint64 m_ui64ClockFrequency;
	OpenViBE::uint64 m_ui64ClockActivationStep;
	OpenViBE::boolean m_bAlreadyClockActivatedOnce;

private:

	System::CChrono m_oBenchmarkChronoProcessClock;
	System::CChrono m_oBenchmarkChronoProcessInput;
	System::CChrono m_oBenchmarkChronoProcess;

public:

	using PsSimulatedBoxBase::sendOpenViBEDataUpdateEvent;

	std::vector<std::list< ::PsTypeChunk > > m_vInput;
	std::vector<std::list< ::PsTypeChunk > > m_vOutput;
	std::list< ::PsTypeChunk > m_vCurrentOutput;
	// std::vector<std::string> m_vSetting;
};

#endif // defined _SIMULATED_OBJECTS_OVKPSSIMULATEDBOX_H_
