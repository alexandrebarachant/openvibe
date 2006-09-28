#ifndef __OpenViBE_Plugins_IPluginObjectContext_H__
#define __OpenViBE_Plugins_IPluginObjectContext_H__

#include "../ovIObject.h"

namespace OpenViBE
{
	namespace Plugins
	{
		/**
		 * \class IPluginObjectContext
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2006-07-07
		 * \brief Base class for plugin objects contexts
		 *
		 * This class is used as a base for all the contexts that could
		 * be sent to plugin objects. It may have common interfaces that
		 * all the plugins would need.
		 *
		 * \sa OpenViBE::Plugins::IBoxAlgorithmContext
		 * \sa OpenViBE::Plugins::IBoxBehaviorContext
		 * \sa OpenViBE::Plugins::IPluginObjectLoadContext
		 * \sa OpenViBE::Plugins::IPluginObjectSaveContext
		 */
		class OV_API IPluginObjectContext : virtual public OpenViBE::IObject
		{
		public:

			// TODO

			_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassId_Plugins_PluginObjectContext)
		};
	};
};

#endif // __OpenViBE_Plugins_IPluginObjectContext_H__
