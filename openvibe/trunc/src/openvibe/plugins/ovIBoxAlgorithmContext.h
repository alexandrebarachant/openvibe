#ifndef __OpenViBE_Plugins_IBoxAlgorithmContext_H__
#define __OpenViBE_Plugins_IBoxAlgorithmContext_H__

#include "ovIPluginObjectContext.h"

namespace OpenViBE
{
	namespace Plugins
	{
		class IStaticBoxContext;
		class IDynamicBoxContext;
		class IPlayerContext;

		/**
		 * \class IBoxAlgorithmContext
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2006-07-07
		 * \brief Specific context to use for box algorithm plugins
		 *
		 * This 'context' is given by the player to an algorithm plugin
		 * so it has all the necessary tools to work. This algorithm
		 * context mainly consists in getting other contexts which
		 * could be defined and used more generically (such as the
		 * box interface or a global player interface for example).
		 *
		 * \sa OpenViBE::Plugins::IBoxAlgorithm
		 * \sa OpenViBE::Plugins::IPlayerContext
		 */
		class OV_API IBoxAlgorithmContext : virtual public OpenViBE::Plugins::IPluginObjectContext
		{
		public:

			/**
			 * \brief Gets the static context of the box corresponding to the algorithm
			 * \return A pointer on the corresponding context
			 *
			 * The returned context can be used by the algorithm to
			 * access the box' prototype.
			 *
			 * \warning The algorithm should not use this reference after it
			 *          has finished its work, it could be deprecated.
			 * \sa OpenViBE::Plugins::IBoxAlgorithmContext::getDynamicBoxContext
			 */
			virtual OpenViBE::Plugins::IStaticBoxContext* getStaticBoxContext(void)=0;
			virtual const OpenViBE::Plugins::IStaticBoxContext* getStaticBoxContext(void) const=0;
			/**
			 * \brief Gets the dynamic context of the box corresponding to the algorithm
			 * \return A pointer on the corresponding context
			 *
			 * The returned context can be used by the algorithm to
			 * access the box' inputs, outputs and settings.
			 *
			 * \warning The algorithm should not use this reference after it
			 *          has finished its work, it could be deprecated.
			 * \sa OpenViBE::Plugins::IBoxAlgorithmContext::getStaticBoxContext
			 */
			virtual OpenViBE::Plugins::IDynamicBoxContext* getDynamicBoxContext(void)=0;
			virtual const OpenViBE::Plugins::IDynamicBoxContext* getDynamicBoxContext(void) const=0;
			/**
			 * \brief Gets the player context
			 * \return A pointer on the player context
			 *
			 * The returned context can be used by the algorithm to
			 * access the player functionnalities.
			 *
			 * \warning The algorithm should not use this reference after it
			 *          has finished its work, it could be deprecated.
			 */
			virtual OpenViBE::Plugins::IPlayerContext* getPlayerContext(void)=0;
			virtual const OpenViBE::Plugins::IPlayerContext* getPlayerContext(void) const=0;
			/**
			 * \brief Marks the algorithm as 'ready to process' so its process function is called by the kernel
			 * \return \e true in case of success.
			 * \return \e false when an error occurs.
			 *
			 * This function should be called by the algorithm itself
			 * after receiving a message so the kernel knows it is
			 * ready to process some data... As soon as the kernel
			 * knows the algorithm is ready, the \c process function
			 * is called so the algorithm does its work and produces
			 * its outputs.
			 */
			virtual OpenViBE::boolean markAlgorithmAsReadyToProcess(void)=0;

			_IsDerivedFromClass_(OpenViBE::Plugins::IPluginObjectContext, OV_ClassId_Plugins_BoxAlgorithmContext)
		};
	};
};

#endif // __OpenViBE_Plugins_IBoxAlgorithmContext_H__
