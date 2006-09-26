#ifndef __OpenViBE_Plugins_IPluginObjectLoadContext_H__
#define __OpenViBE_Plugins_IPluginObjectLoadContext_H__

#include "../ov_base.h"
#include "ovIPluginObjectContext.h"

namespace OpenViBE
{
	namespace Plugins
	{
		/**
		 * \class IPluginObjectLoadContext
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2006-07-07
		 * \brief Plugin objects load interface
		 *
		 * This context is given to each plugin object that
		 * has to be loaded from something to allow it to
		 * be loaded.
		 */
		class OV_API IPluginObjectLoadContext : virtual public OpenViBE::Plugins::IPluginObjectContext
		{
		public:

			// TODO

			_IsDerivedFromClass_(OpenViBE::Plugins::IPluginObjectContext, OV_ClassIdentifier_PluginObjectLoadContext)

		};
	};
};

#endif // __OpenViBE_Plugins_IPluginObjectLoadContext_H__
