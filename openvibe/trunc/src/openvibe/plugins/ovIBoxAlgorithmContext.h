#ifndef __OpenViBE_Plugins_IBoxAlgorithmContext_H__
#define __OpenViBE_Plugins_IBoxAlgorithmContext_H__

#include "ovIPluginObjectContext.h"

namespace OpenViBE
{
	namespace Plugins
	{
		class IBoxContext;
		class IKernelContext;

		/**
		 * \class IBoxAlgorithmContext
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2006-07-07
		 * \brief Specific context to use for box algorithm plugins
		 *
		 * This 'context' is given by the kernel to an algorithm plugin
		 * so it has all the necessary tools to work. This algorithm
		 * context mainly consists in getting other contexts which
		 * could be defined and used more generically (such as the
		 * box interface or a global kernel interface for example).
		 *
		 * \sa OpenViBE::Plugins::IBoxAlgorithm
		 * \sa OpenViBE::Plugins::IBoxBehaviorContext
		 */
		class OV_API IBoxAlgorithmContext : virtual public OpenViBE::Plugins::IPluginObjectContext
		{
		public:

			/**
			 * \brief Gets the context of the box corresponding to the algorithm
			 * \return A reference on the corresponding context
			 *
			 * The returned context can be used by the algorithm to
			 * access the box' inputs, outputs and settings.
			 *
			 * \warning The algorithm should use this reference after it
			 *          has finished its work, it could be deprecated.
			 */
			virtual OpenViBE::Plugins::IBoxContext& getBoxContext(void)=0;
			/**
			 * \brief Gets the kernel context
			 * \return A reference on the kernel context
			 *
			 * The returned context can be used by the algorithm to
			 * access the kernel functionnalities.
			 *
			 * \warning The algorithm should use this reference after it
			 *          has finished its work, it could be deprecated.
			 */
			virtual OpenViBE::Plugins::IKernelContext& getKernelContext(void)=0;

			_IsDerivedFromClass_(OpenViBE::Plugins::IPluginObjectContext, OV_ClassId_Plugins_BoxAlgorithmContext)
		};
	};
};

#endif // __OpenViBE_Plugins_IBoxAlgorithmContext_H__
