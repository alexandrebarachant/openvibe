#ifndef __OpenViBE_Plugins_IBoxAlgorithm_H__
#define __OpenViBE_Plugins_IBoxAlgorithm_H__

#include "ovIPluginObject.h"

namespace OpenViBE
{
	class CMessageClock;
	class CMessageEvent;
	class CMessageSignal;

	namespace Plugins
	{
		class IBoxAlgorithmContext;

		/**
		 * \class IBoxAlgorithm
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2006-06-19
		 * \brief Algorithm to create/process/transform §OpenViBE§ data
		 *
		 * This class should be derived by any plugin that is related
		 * to data processing. It can be data acquisition/production
		 * from an hardware device or from a file. It can be data
		 * processing/transforming, moving time information into
		 * frequency space for example. It can be data classification
		 * generating discrete classification events better than
		 * continuous data flow.
		 *
		 * This is the heart of the extension mechanism of the
		 * §OpenViBE§ platform.
		 *
		 * \sa OpenViBE::Plugins::IBoxAlgorithmDesc
		 *
		 * \todo details about building new plugins
		 */
		class OV_API IBoxAlgorithm : virtual public OpenViBE::Plugins::IPluginObject
		{
		public:

			/** \name Initialization / Uninitialization */
			//@{

			/**
			 * \brief Prepares plugin object
			 * \param rContext [in] : the plugin object context
			 * \return \e true when this object successfully initialized
			 *         or \e false if it didn't succeed to initialize.
			 * \note Default implementation simply returns \e true.
			 *
			 * After a successfull initialization, the caller knows
			 * the object can safely be used... On failure, this object
			 * should be ready to be uninitialized and then released.
			 *
			 * \sa uninitialize
			 */
			virtual OpenViBE::boolean initialize(
				const OpenViBE::Plugins::IBoxAlgorithmContext& rContext) { return true; }
			/**
			 * \brief Unprepares the object so it can be deleted
			 * \param rContext [in] : the plugin object context
			 * \return \e true when this object sucessfully uninitialized
			 *         or \e false if didn't succeed to uninitialize.
			 * \note Default implementation simply returns \e true.
			 *
			 * If this function returns \e false, it means it could not
			 * uninitialize the object members correctly. Thus, the
			 * object won't be released and none of its method will
			 * be called anymore for security. Note that this results
			 * in memory leaks. This is why this method should return
			 * \e true as often as possible.
			 *
			 * \sa initialize
			 */
			virtual OpenViBE::boolean uninitialize(
				const OpenViBE::Plugins::IBoxAlgorithmContext& rContext) { return true; }

			//@}
			/** \name Several event processing callbacks */
			//@{

			/**
			 * \brief Reaction to an event launched by another box
			 * \param rBoxAlgorithmContext [in] : the box algorithm context to use
			 * \param rMessageEvent [in] : the message the box just received
			 * \return \e true when the message is processed.
			 * \return \e false when the message is not processed.
			 * \note Default implementation returns \e false
			 *
			 * This function is called by the §OpenViBE§ kernel when
			 * another box tries to send an event message to this
			 * box. This event message is described in the
			 * rMessageEvent parameter and can be interpreted by this
			 * algorithm.
			 *
			 * \sa OpenViBE::IBoxAlgorithmContext
			 */
			virtual OpenViBE::boolean processEvent(
				OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext,
				OpenViBE::CMessageEvent& rMessageEvent) { return false; }
			/**
			 * \brief Reaction to a signal
			 * \param rBoxAlgorithmContext [in] : the box algorithm context to use
			 * \param rMessageSignal [in] : the signal the box just received
			 * \return \e true when the message is processed.
			 * \return \e false when the message is not processed.
			 * \note Default implementation returns \e false
			 *
			 * This function is called by the §OpenViBE§ kernel when
			 * it has sent a signal. Signal are special messages,
			 * mainly sent by the kernel to all of the §OpenViBE§
			 * boxes in order to tell them it is about to start,
			 * processing, stop processing, load a new scenario
			 * and so on...
			 *
			 * \sa OpenViBE::Plugins::IBoxAlgorithmContext
			 */
			virtual OpenViBE::boolean processSignal(
				OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext,
				OpenViBE::CMessageSignal& rMessageSignal) { return false; }
			/**
			 * \brief Reaction to a clock tick
			 * \param rBoxAlgorithmContext [in] : the box algorithm context to use
			 * \param rMessageClock [in] : the clock message the box received
			 * \return \e true when the message is processed.
			 * \return \e false when the message is not processed.
			 * \note Default implementation returns \e false
			 *
			 * This function is called by the §OpenViBE§ kernel when
			 * it has sent clock messages. Clock messages are used for
			 * processes that should be executed regularly and which
			 * can not be triggered thanks to their inputs (for example
			 * acquisition modules). They also can be used for example
			 * when viewing inputs on smaller range than what input
			 * sends periodically, thus needing a moving
			 * 'viewed-window' on lastly received data.
			 *
			 * \sa OpenViBE::Plugins::IBoxAlgorithmContext
			 */
			virtual OpenViBE::boolean processClock(
				OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext,
				OpenViBE::CMessageClock& rMessageClock) { return false; }
			/**
			 * \brief Reaction to an input update
			 * \param rBoxAlgorithmContext [in] : the box algorithm context to use
			 * \param ui32InputIndex [in] : the index of the input which has ben updated
			 * \return \e true when the message is processed.
			 * \return \e false when the message is not processed.
			 * \note Default implementation returns \e false
			 *
			 * This function is called by the §OpenViBE§ kernel each
			 * time an input of this box is updated. This allows the
			 * algorithm to decide to call the process function and
			 * eventually to the received data.
			 *
			 * \sa OpenViBE::Plugins::IBoxAlgorithmContext
			 */
			virtual OpenViBE::boolean processInput(
				OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext,
				OpenViBE::uint32 ui32InputIndex) { return false; }

			//@}
			/** \name Algorithm processing */
			//@{

			/**
			 * \brief Processing function
			 * \param rBoxAlgorithmContext [in] : the box algorithm context to use
			 * \return \e true on success, \e false when something went wrong.
			 *
			 * This function is used to process the arrived data and
			 * eventually generate results. See §OpenViBE§ global
			 * architecture to understand how the commponents interact
			 * and how an §OpenViBE§ box works internally.
			 *
			 * The processing function may use the provided context
			 * in order to read its inputs and write its outputs...
			 * Also it could use the provided context to send 
			 * messages/events to other boxes. Finally, it may use
			 * the provided context in order to perform rendering
			 * tasks !
			 *
			 * \sa OpenViBE::Kernel::IBoxAlgorithmContext
			 */
			virtual OpenViBE::boolean process(OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext)=0;

			//@}

			_IsDerivedFromClass_(OpenViBE::Plugins::IBoxAlgorithm, OV_ClassId_Plugins_BoxAlgorithm)
		};
	};
};

#endif // __OpenViBE_Plugins_IBoxAlgorithm_H__
