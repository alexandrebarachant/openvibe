#ifndef __OpenViBE_Kernel_IPlayerContext_H__
#define __OpenViBE_Kernel_IPlayerContext_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IMessageClock;
		class IMessageEvent;
		class IMessageSignal;
	};

	namespace Kernel
	{
		/**
		 * \class IPlayerContext
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2006-07-07
		 * \brief Player interface for plugin objects
		 *
		 * Instances of this class are given to plugin object
		 * so they can communicate with the platform kernel,
		 * providing services such as message sending etc...
		 */
		class OV_API IPlayerContext : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			/** \name Messages sending */
			//@{

			/**
			 * \brief Sends signal to the kernel
			 * \param rMessageSignal [in] : the signal message to send
			 * \return \e true in case of success, \e false in other cases.
			 */
			virtual OpenViBE::boolean sendSignal(
				const OpenViBE::Kernel::IMessageSignal& rMessageSignal)=0;
			/**
			 * \brief Sends a message to another box
			 * \param rMessageEvent [in] : the message to send to the other box
			 * \param rTargetIdentifier [in] : the identifier of the other box
			 * \return \e true in case of success, \e false in other cases.
			 */
			virtual OpenViBE::boolean sendMessage(
				const OpenViBE::Kernel::IMessageEvent& rMessageEvent,
				const OpenViBE::CIdentifier& rTargetIdentifier)=0;
			/**
			 * \brief Broadcasts a message to several other boxes
			 * \param rMessageEvent [in] : the message to send
			 * \param pTargetIdentifier [in] : a pointer containing all
			 *        the targets which should receive the sent message.
			 * \param ui32TargetIdentifierCount [in] : the number of
			 *        targets contained in the \c pTargetIdentifier array.
			 * \return \e true in case of success, \e false in other cases.
			 */
			virtual OpenViBE::boolean sendMessage(
				const OpenViBE::Kernel::IMessageEvent& rMessageEvent,
				const OpenViBE::CIdentifier* pTargetIdentifier,
				const OpenViBE::uint32 ui32TargetIdentifierCount)=0;

			//@}

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_PlayerContext)
		};
	};
};

#endif // __OpenViBE_Kernel_IPlayerContext_H__
