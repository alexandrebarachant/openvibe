#ifndef __OpenViBE_Plugins_IBoxBehavior_H__
#define __OpenViBE_Plugins_IBoxBehavior_H__

#include "ovIPluginObject.h"

namespace OpenViBE
{
	class CMessageEvent;
	class CMessageSignal;
	class CMessageClock;

	namespace Plugins
	{
		class IBoxBehaviorContext;

		/**
		 * \class IBoxBehavior
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2006-06-19
		 * \brief Box behavior to define reaction against events, data reception etc...
		 *
		 * This class should be derived by any plugin that is related
		 * to box behavior. It should define the reaction to events,
		 * clocks, signals, data reception etc... and finaly chose to
		 * trigger the algorithm processing or not.
		 *
		 * \sa OpenViBE::Plugins::IBoxBehaviorDesc
		 */
		class OV_API IBoxBehavior : virtual public OpenViBE::Plugins::IPluginObject
		{
		public:

			/** \name Several events processing callbacks */
			//@{

			/**
			 * \brief Reaction to an event launched by another box
			 * \param rBoxBehaviorContext [in] : the box behavior context to use
			 * \param rMessageEvent [in] : the message the box just received
			 * \return \e true if the message is processed, \e false in other case.
			 *
			 * This function is called by the §OpenViBE§ kernel when
			 * another box tries to send an event message to this
			 * box. This event message is described in the
			 * rMessageEvent parameter and can be interpreted by this
			 * behavior.
			 *
			 * \sa OpenViBE::IBoxBehaviorContext
			 */
			virtual OpenViBE::boolean processEvent(
				OpenViBE::Plugins::IBoxBehaviorContext& rBoxBehaviorContext,
				OpenViBE::CMessageEvent& rMessageEvent)=0;
			/**
			 * \brief Reaction to a signal
			 * \param rBoxBehaviorContext [in] : the box behavior context to use
			 * \param rMessageSignal [in] : the signal the box just received
			 * \return \e true if the message is processed, \e false in other case.
			 *
			 * This function is called by the §OpenViBE§ kernel when
			 * it has sent a signal. Signal are special messages,
			 * mainly sent by the kernel to all of the §OpenViBE§
			 * boxes in order to tell them it is about to start,
			 * processing, stop processing, load a new scenario
			 * and so on...
			 *
			 * \sa OpenViBE::Plugins::IBoxBehaviorContext
			 */
			virtual OpenViBE::boolean processSignal(
				OpenViBE::Plugins::IBoxBehaviorContext& rBoxBehaviorContext,
				OpenViBE::CMessageSignal& rMessageSignal)=0;
			/**
			 * \brief Reaction to a clock tick
			 * \param rBoxBehaviorContext [in] : the box behavior context to use
			 * \param rMessageClock [in] : the clock message the box received
			 * \return \e true if the message is processed, \e false in other case.
			 *
			 * This function is called by the §OpenViBE§ kenerl when
			 * it has sent clock messages. Clock messages are used for
			 * processes that should be executed regularly and which
			 * can not be triggered thanks to their inputs (for example
			 * acquisition modules). They also can be used for example
			 * when viewing inputs on smaller range than what input
			 * sends periodically, thus needing a moving
			 * 'viewed-window' on lastly received data.
			 *
			 * \sa OpenViBE::Plugins::IBoxBehaviorContext
			 */
			virtual OpenViBE::boolean processClock(
				OpenViBE::Plugins::IBoxBehaviorContext& rBoxBehaviorContext,
				OpenViBE::CMessageClock& rMessageClock)=0;
			/**
			 * \brief Reaction to an input update
			 * \param rBoxBehaviorContext [in] : the box behavior context to use
			 * \param ui32InputIndex [in] : the index of the input which has ben updated
			 * \return \e true if the message is processed, \e false in other case.
			 *
			 * This function is called by the §OpenViBE§ kernel each
			 * time an input of this box is updated. This allows the
			 * behavior to decide to call the box algorithm and
			 * eventually to process the received data.
			 *
			 * \sa OpenViBE::Plugins::IBoxBehaviorContext
			 */
			virtual OpenViBE::boolean processInput(
				OpenViBE::Plugins::IBoxBehaviorContext& rBoxBehaviorContext,
				OpenViBE::uint32 ui32InputIndex)=0;

			//@}

			_IsDerivedFromClass_(OpenViBE::Plugins::IPluginObject, OV_ClassId_Plugins_BoxBehavior)
		};
	};
};

#endif // __OpenViBE_Plugins_IBoxBehavior_H__
