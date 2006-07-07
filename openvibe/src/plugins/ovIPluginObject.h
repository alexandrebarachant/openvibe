#ifndef __OpenViBE_Plugins_IPluginObject_H__
#define __OpenViBE_Plugins_IPluginObject_H__

#include "../ovIObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IPluginObjectLoadContext;
		class IPluginObjectSaveContext;
	}

	namespace Plugins
	{
		/**
		 * \class IPluginObject
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2006-06-19
		 * \brief Base class for plugin object
		 *
		 * This class is the base class for all the concrete plugin
		 * classes that extend the §OpenViBE§ platform. It comes with
		 * several basic functions each plugin should have in order
		 * to work in the §OpenViBE§ platform.
		 *
		 * Each plugin class comes with a plugin descriptor class
		 * (which you can write deriving the IPluginObjectDesc
		 * interface).
		 *
		 * \sa IBoxAlgorithm
		 * \sa IBoxBehavior
		 * \sa IScenarioImporter
		 * \sa IScenarioExporter
		 * \sa IPluginObjectDesc
		 *
		 * \todo details about building new plugins
		 */
		class OV_API IPluginObject : virtual public OpenViBE::IObject
		{
		public:

			/** \name Memory management */
			//@{

			/**
			 * \brief Informs the plugin object it won't be used anymore
			 *
			 * This is called by the §OpenViBE§ platform to inform
			 * the plugin object it is not usefull anymore. The plugin
			 * can chose whether to delete itself or to stay in
			 * memory until it decides is is ok to be deleted.
			 * However, the §OpenViBE§ platform will not call any
			 * of the plugin functions after release is called.
			 */
			virtual void release(void)=0;

			//@}
			/** \name Input / Output */
			//@{

			/**
			 * \brief Loads this plugin object state
			 * \param rLoadContext [in] : the object to use to perform load operations
			 * \return \e true on success, \e false when something went wrong.
			 *
			 * This method is used so this plugin object state can
			 * be loaded from a scenario and restored to its last
			 * saved state. The rLoadContext instance should be used
			 * to effectively read data from the source.
			 *
			 * Default implementation simply returns \e true.
			 *
			 * \sa save
			 * \sa OpenViBE::Kernel::IPluginObjectLoadContext
			 *
			 * \todo add discussion on what is done when \e false is returned
			 */
			virtual OpenViBE::boolean load(OpenViBE::Kernel::IPluginObjectLoadContext& rLoadContext);
			/**
			 * \brief Saves this plugins object state
			 * \param rSaveContext [in] : the object to use to perform save operations
			 * \return \e true on success, \e false when someting went wrong.
			 *
			 * This method is used so this plugin object state can
			 * be saved and stored into a scenario in order to be
			 * restored later by a load function. The rSaveContext
			 * instance should be used to effectively write data
			 * to the target.
			 *
			 * Default implementation simply returns \e true.
			 *
			 * \sa load
			 * \sa OpenViBE::Kernel::IPluginObjectSaveContext
			 *
			 * \todo add discussion on what is done when \e false is returned
			 */
			virtual OpenViBE::boolean save(OpenViBE::Kernel::IPluginObjectSaveContext& rSaveContext);

			//@}

			_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassIdentifier_PluginObject)
		};
	};
};

#endif // __OpenViBE_Plugins_IPluginObject_H__
