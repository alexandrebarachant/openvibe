#ifndef __OpenViBE_Plugins_IBoxBehaviorDesc_H__
#define __OpenViBE_Plugins_IBoxBehaviorDesc_H__

#include "ovIPluginObjectDesc.h"

namespace OpenViBE
{
	namespace Plugins
	{
		/**
		 * \class IBoxBehaviorDesc
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2006-06-20
		 * \brief Box behavior prototype
		 *
		 * This class should be derived by any plugin developer in
		 * order to describe a specific §OpenViBE§ box behavior.
		 *
		 * \sa OpenViBE::Plugins::IBoxBehavior
		 */
		class OV_API IBoxBehaviorDesc : virtual public OpenViBE::Plugins::IPluginObjectDesc
		{

		public:

			/** \name Configuration */
			//@{

			/**
			 * \brief Gets the clock frequency to call this behavior
			 * \return The clock frequency to call this behavior
			 *
			 * This function is used for behavior that are triggered on
			 * clock signals. The returned frequency should be the
			 * frequency itself multiplied by a factor 1 000. Thus
			 * returning 1 000 means the behavior would be clock
			 * activated each second. Returning 25 000 would clock
			 * activate the block each 40ms etc...
			 *
			 * \note Returning 0 means the block should not be
			 *       clock activated.
			 */
			virtual OpenViBE::uint32 getClockFrequency(void) const=0;

			//@}

			_IsDerivedFromClass_(OpenViBE::Plugins::IPluginObjectDesc, OV_ClassId_Plugins_BoxBehaviorDesc)
		};
	};
};

#endif // __OpenViBE_Plugins_IBoxBehaviorDesc_H__
