#ifndef __OpenViBE_Plugins_IPluginObjectDesc_H__
#define __OpenViBE_Plugins_IPluginObjectDesc_H__

#include "../ovIObject.h"

namespace OpenViBE
{
	namespace Plugins
	{
		class IPluginObject;

		/**
		 * \class IPluginObjectDesc
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2006-06-19
		 * \brief Base class for plugin descriptor
		 *
		 * This class is the base class for all the plugin description
		 * classes. It contains basic functions that could be used for
		 * each plugin description. Derived plugin descriptions will
		 * be used as a prototype of what they can create.
		 *
		 * \sa IBoxAlgorithmDesc
		 * \sa IBoxBehaviorDesc
		 * \sa IScenarioImporterDesc
		 * \sa IScenarioExporterDesc
		 * \sa IPluginObject
		 *
		 * \todo details about building new plugins
		 */
		class OV_API IPluginObjectDesc : virtual public OpenViBE::IObject
		{
		public:
			/** \name Memory management */
			//@{

			/**
			 * \brief Informs the plugin description it won't be used any more
			 *
			 * This is called by the §OpenViBE§ platform to inform
			 * the plugin description it is not usefull anymore.
			 * The plugin can chose whether to delete itself or
			 * to stay in memory until it decides it is ok to be
			 * deleted. However, the §OpenViBE§ platform will not
			 * call any of the plugin functions after release is
			 * called.
			 */
			virtual void release(void)=0;

			//@}
			/** \name Creation process */
			//@{

			/**
			 * \brief Gives a tip on what this plugin descriptor is able to create
			 *
			 * This may inform the §OpenViBE§ platform about what
			 * kind of plugin can be created using this plugin
			 * descriptor. It should return the concrete class
			 * identifier of the plugin object itself.
			 */
			virtual OpenViBE::CIdentifier getCreatedClass(void) const=0;
			/**
			 * \brief Creates the plugin object itself
			 * \return the created object.
			 *
			 * This method creates the plugin object itself and returns
			 * it with the lowest level interface. The §OpenVIBE§
			 * platform then uses the IObject::isDerivedFromClass
			 * method to use the plugin correctly.
			 */
			virtual OpenViBE::Plugins::IPluginObject* create(void)=0;

			//@}
			/** \name Textual plugin object description and information */
			//@{

			/**
			 * \brief Gets the plugin name
			 * \return The plugin name.
			 *
			 * Default implementation simply returns empty string.
			 */
			virtual OpenViBE::CString getName(void) const;
			/**
			 * \brief Gets the author name for this plugin
			 * \return The author name for this plugin.
			 *
			 * Default implementation simply returns "no name".
			 */
			virtual OpenViBE::CString getAuthorName(void) const;
			/**
			 * \brief Gets the author company name for this plugin
			 * \return The author company name for this plugin.
			 *
			 * Default implementation simply returns "unknown".
			 */
			virtual OpenViBE::CString getAuthorCompanyName(void) const;
			/**
			 * \brief Gets a short description of the plugin
			 * \return A short description of the plugin.
			 *
			 * Default implementation simply returns "unknown".
			 */
			virtual OpenViBE::CString getShortDescription(void) const;
			/**
			 * \brief Gets a detailed description of the plugin
			 * \return A detailed description of the plugin.
			 *
			 * Default implementation simply returns empty string.
			 *
			 * \note You can use std::endl to have the description
			 *       on several lines when needed.
			 */
			virtual OpenViBE::CString getDetailedDescription(void) const;
			/**
			 * \brief Gets a basic category of the plugin
			 * \return the category tokens of the plugin
			 *
			 * The tokens should be separated with '/' characters
			 * in order to create sub categories.
			 *
			 * Default implementation returns "unknown".
			 */
			virtual OpenViBE::CString getCategory(void) const;
			/**
			 * \brief Gets the version of the plugin
			 * \return the version of the plugin.
			 *
			 * Default implementation simply returns "unknown".
			 */
			virtual OpenViBE::CString getVersion(void) const;

			//@}

			_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassId_Plugins_PluginObjectDesc)
		};
	};
};

#endif // __OpenViBE_Plugins_IPluginObjectDesc_H__
