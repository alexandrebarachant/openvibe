#ifndef __OpenViBE_Plugins_IKernelContext_H__
#define __OpenViBE_Plugins_IKernelContext_H__

#include "ovIPluginObjectContext.h"

namespace OpenViBE
{
	namespace Plugins
	{
		/**
		 * \class IKernelContext
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2006-07-07
		 * \brief Kernel interface for plugin objects
		 *
		 * Instances of this class are given to plugin object
		 * so they can communicate with the platform kernel,
		 * providing services such as message sending etc...
		 */
		class IKernelContext : virtual public OpenViBE::Plugins::IPluginObjectContext
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
				const OpenViBE::CMessageSignal& rMessageSignal)=0;
			/**
			 * \brief Sends a message to another box
			 * \param rMessageEvent [in] : the message to send to the other box
			 * \param rTargetIdentifier [in] : the identifier of the other box
			 * \return \e true in case of success, \e false in other cases.
			 */
			virtual OpenViBE::boolean sendMessage(
				const OpenViBE::CMessageEvent& rMessageEvent,
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
				const OpenViBE::CMessageEvent& rMessageEvent,
				const OpenViBE::CIdentifier* pTargetIdentifier,
				const OpenViBE::uint32 ui32TargetIdentifierCount)=0;

			//@}

			_IsDerivedFromClass_(OpenViBE::Plugins::IPluginObjectContext, OV_ClassIdentifier_KernelContext)
		};
	};
};

#endif // __OpenViBE_Plugins_IKernelContext_H__
