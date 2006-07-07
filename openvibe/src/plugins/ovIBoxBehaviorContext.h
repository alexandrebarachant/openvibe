#ifndef __OpenViBE_Plugins_IBoxBehaviorContext_H__
#define __OpenViBE_Plugins_IBoxBehaviorContext_H__

#include "ovIPluginObjectContext.h"

namespace OpenViBE
{
	namespace Plugins
	{
		class IBoxContext;
		class IKernelContext;

		/**
		 * \class IBoxBehaviorContext
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2006-07-07
		 * \brief Specific context to use for box behavior plugins
		 *
		 * This 'context' is given by the kernel to a behavior plugin
		 * so it has all the necessary tools to work. This behavior
		 * context mainly consists in getting other contexts which
		 * could be defined and used more generically (such as the
		 * box interface or a global kernel interface for example).
		 *
		 * \sa OpenViBE::Plugins::IBoxBehavior
		 * \sa OpenViBE::Plugins::IBoxAlgorithmContext
		 */
		class OV_API IBoxBehaviorContext : virtual public OpenViBE::Plugins::IPluginObjectContext
		{
		public:

			/**
			 * \brief Gets the context of the box corresponding to the algorithm
			 * \return A reference on the corresponding context
			 *
			 * The returned context can be used by the behavior to
			 * access the box' inputs, outputs and settings.
			 *
			 * \warning The behavior should use this reference after it
			 *          has finished its work, it could be deprecated.
			 */
			virtual OpenViBE::Plugins::IBoxContext& getBoxContext(void)=0;
			/**
			 * \brief Gets the kernel context
			 * \return A reference on the kernel context
			 *
			 * The returned context can be used by the behavior to
			 * access the kernel functionnalities.
			 *
			 * \warning The behavior should use this reference after it
			 *          has finished its work, it could be deprecated.
			 */
			virtual OpenViBE::Plugins::IKernelContext& getKernelContext(void)=0;

			_IsDerivedFromClass_(OpenViBE::Plugins::IPluginObjectContext, OV_ClassIdentifier_BoxBehaviorContext)
		};
	};
};

#endif // __OpenViBE_Plugins_IBoxBehaviorContext_H__
