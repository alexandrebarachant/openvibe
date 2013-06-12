#ifndef __OpenViBE_Plugins_IServerExtensionDesc_H__
#define __OpenViBE_Plugins_IServerExtensionDesc_H__

#include "ovIPluginObjectDesc.h"

namespace OpenViBE
{
	namespace Plugins
	{
		/**
		  * \class IServerExtensionDesc
		  * \author Jozef Legény (Inria)
		  * \date 2013-06-11
		  * \brief Server Extension plugin descriptor
		  *
		  * This class should be derived by any plugin which is an Acquisition
		  * Server extension.
		  *
		  */


		class OV_API IServerExtensionDesc : public OpenViBE::Plugins::IPluginObjectDesc
		{
		public:

			/**
			  * \brief Gets the prototype for this extension
			  * \param rServerExtensionPrototype [out] : the prototype to fill
			  * \return \e true in case of success \e false in other cases
			  *
			  * After calling this function the returned prototype should be filled by the
			  * descriptor so the Acquisition Server knows the structure of the extension
			  * (its parameters).
			  *
			  * \sa OpenViBE::Plugins::IServerExtensionProto
			  */
			virtual OpenViBE::boolean getServerExtentionPrototype(
					OpenViBE::Kernel::IServerExtensionProto& rServerExtensionPrototype) const = 0;

			_IsDerivedFromClass_(OpenViBE::Plugins::IPluginObjectDesc, OV_ClassId_Plugins_ServerExtensionDesc)
		};
	}
}

#endif // __OpenViBE_Plugins_IServerPluginExtensionDesc_H__
