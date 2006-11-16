#ifndef __OpenViBE_Plugins_IStaticBoxContext_H__
#define __OpenViBE_Plugins_IStaticBoxContext_H__

#include "ovIPluginObjectContext.h"

namespace OpenViBE
{
	namespace Plugins
	{
		/**
		 * \class IStaticBoxContext
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2006-06-21
		 * \brief Main §OpenViBE§ static box interface to communicate with the kernel
		 *
		 * This box context is given to plugin objects such as box 
		 * algorithm so they can access the "seen" box interface.
		 */
		class OV_API IStaticBoxContext : virtual public OpenViBE::Plugins::IPluginObjectContext
		{
		public:

			/** \name Box identification */
			//@{

			/**
			 * \brief Gets the identifier of the related box
			 * \return The identifier of the related box.
			 *
			 * This function allows the caller to know the identifier
			 * of the box it's working on. Each box has a unique
			 * instance identifier which is used when connecting
			 * boxes together and whend sending messages between
			 * boxes.
			 */
			virtual OpenViBE::CIdentifier getIdentifier(void) const=0;
			/**
			 * \brief Gets the name of the related box
			 * \return The name of the related box.
			 */
			virtual OpenViBE::CString getName(void) const=0;

			//@}
			/** \name Input management */
			//@{

			/**
			 * \brief Gets the input count for the related box
			 * \return The input count for the related box.
			 */
			virtual OpenViBE::uint32 getInputCount(void) const=0;
			/**
			 * \brief Gets an input type identifier
			 * \param ui32InputIndex [in] : The input index
			 * \param rTypeIdentifier [out] : The type identifier
			 * \return \e true in case of success.
			 * \return \e false in case of error. In such case,
			 *         \c rTypeIdentifier remains unchanged.
			 */
			virtual OpenViBE::boolean getInputType(
				const OpenViBE::uint32 ui32InputIndex,
				OpenViBE::CIdentifier& rTypeIdentifier) const=0;
			/**
			 * \brief Gets an input name
			 * \param ui32InputIndex [in] : The input index
			 * \param rName [out] : The name of this input
			 * \return \e true in case of success.
			 * \return \e false in case of error. In such case,
			 *         \c rName remains unchanged.
			 */
			virtual OpenViBE::boolean getInputName(
				const OpenViBE::uint32 ui32InputIndex,
				OpenViBE::CString& rName) const=0;

			//@}
			/* \name Output management */
			//@{

			/**
			 * \brief Gets the output count for the related box
			 * \return The output count for the related box.
			 */
			virtual OpenViBE::uint32 getOutputCount(void) const=0;
			/**
			 * \brief Gets an output type identifier
			 * \param ui32OutputIndex [in] : The output index
			 * \param rTypeIdentifier [out] : The type identifier
			 * \return \e true in case of success.
			 * \return \e false in case of error. In such case,
			 *         \c rTypeIdentifier remains unchanged.
			 */
			virtual OpenViBE::boolean getOutputType(
				const OpenViBE::uint32 ui32OutputIndex,
				OpenViBE::CIdentifier& rTypeIdentifier) const=0;
			/**
			 * \brief Gets an output name
			 * \param ui32OutputIndex [in] : The output index
			 * \param rName [out] : The name of this output
			 * \return \e true in case of success.
			 * \return \e false in case of error. In such case,
			 *         \c rName remains unchanged.
			 */
			virtual OpenViBE::boolean getOutputName(
				const OpenViBE::uint32 ui32OutputIndex,
				OpenViBE::CString& rName) const=0;

			//@}
			/* \name Setting management */
			//@{

			/**
			 * \brief Gets the setting count for the related box
			 * \return The setting count for the related box.
			 */
			virtual OpenViBE::uint32 getSettingCount(void) const=0;
			/**
			 * \brief Gets the setting type for the related box
			 * \param ui32SettingIndex [in] : the setting index
			 * \param rTypeIdentifier [out] : the identifier of the
			 *        requested setting.
			 * \return \e true in case of success, \e false in other cases.
			 */
			virtual OpenViBE::boolean getSettingType(
				const OpenViBE::uint32 ui32SettingIndex,
				OpenViBE::CIdentifier& rTypeIdentifier) const=0;
			/**
			 * \brief Gets the setting value for the related box
			 * \param ui32SettingIndex [in] : the setting index
			 * \param rValue [out] : the setting value
			 * \return \e true in case of success, \e false in other cases.
			 */
			virtual OpenViBE::boolean getSettingValue(
				const OpenViBE::uint32 ui32SettingIndex,
				OpenViBE::CString& sValue) const=0;

			//@}

			_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassId_Plugins_StaticBoxContext)
		};
	};
};

#endif // __OpenViBE_Plugins_IStaticBoxContext_H__
