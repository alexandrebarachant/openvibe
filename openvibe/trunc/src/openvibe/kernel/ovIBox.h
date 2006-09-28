#ifndef __OpenViBE_Kernel_IBox_H__
#define __OpenViBE_Kernel_IBox_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IBoxAlgorithmDesc;
		class IBoxBehaviorDesc;

		/**
		 * \class IBox
		 * \author Yann Renard (IRISA/INRIA)
		 * \date 2006-08-16
		 * \brief Complete §OpenViBE§ box interface
		 *
		 * This interface can be used in order to fully describe an
		 * §OpenViBE§ black box. It describes its identification values,
		 * its inputs, its outputs and its settings.
		 */
		class OV_API IBox : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

#if 0
			/** \name Initialization */
			//@{

			/**
			 * \brief Description based initialisation
			 * \param rBoxAlgorithmDesc [in] : The description of
			 *        the algorithm this box uses
			 * \param rBoxBehaviorDesc [in] : The description of
			 *        the behavior this box uses
			 * \return \e true in case of success.
			 * \return \e false in case of error. In such case,
			 *         the box is simply considered as empty, with
			 *         no input, no output, and no settings.
			 */
			virtual OpenViBE::boolean initialize(
				OpenViBE::Kernel::IBoxAlgorithmDesc& rBoxAlgorithmDesc,
				OpenViBE::Kernel::IBoxBehaviorDesc& rBoxBehaviorDesc)=0;

			//@}
#endif

			/** \name Box naming and identification */
			//@{

			/**
			 * \brief Gets the identifier of this box
			 * \return The identifier of this §OpenViBE§ box.
			 */
			virtual const OpenViBE::CIdentifier& getIdentifier(void) const=0;
			/**
			 * \brief Gets the display name of this box
			 * \return The name of this §OpenViBE§ box.
			 */
			virtual const OpenViBE::CString& getName(void) const=0;
			/**
			 * \brief Changes the identifier of this box
			 * \param rIdentifier [in] : The new identifier
			 *        this box should take.
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean setIdentifier(
				const OpenViBE::CIdentifier& rIdentifier)=0;
			/**
			 * \brief Renames this box
			 * \param sName [in] : The name this box should take
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean setName(
				const OpenViBE::CString& sName)=0;
			/**
			 * \brief Gets the behavior class identifier
			 * \return This box' behavior class identifier
			 */
			virtual const OpenViBE::CIdentifier& getBehaviorClassIdentifier(void) const=0;
			/**
			 * \brief Gets the algorithm class identifier
			 * \return This box' algorithm class identifier
			 */
			virtual const OpenViBE::CIdentifier& getAlgorithmClassIdentifier(void) const=0;

			//@}
			/** \name Input management */
			//@{

			/**
			 * \brief Adds an input to this box
			 * \param sName [in] : The input name
			 * \param rTypeIdentifier [in] : The
			 *        input type identifier
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 *
			 * The input is always added after the last
			 * already existing input.
			 */
			virtual OpenViBE::boolean addInput(
				const OpenViBE::CString& sName,
				const OpenViBE::CIdentifier& rTypeIdentifier)=0;
			/**
			 * \brief Removes an input for this box
			 * \param ui32InputIndex [in] : The index
			 *        of the input to be removed
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 *
			 * Inputs coming after the removed input
			 * have their indices changing after this,
			 * they all decrease by 1.
			 */
			virtual OpenViBE::boolean removeInput(
				const OpenViBE::uint32 ui32InputIndex)=0;
			/**
			 * \brief Gets the number of inputs for this box
			 * \return The number of inputs for this box.
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
			/**
			 * \brief Sets an input type identifier
			 * \param ui32InputIndex [in] : The input index
			 * \param rTypeIdentifier [in] : The type identifier
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean setInputType(
				const OpenViBE::uint32 ui32InputIndex,
				const OpenViBE::CIdentifier& rTypeIdentifier)=0;
			/**
			 * \brief Sets an input name
			 * \param ui32InputIndex [in] : The input index
			 * \param rName [in] : The name of this input
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean setInputName(
				const OpenViBE::uint32 ui32InputIndex,
				const OpenViBE::CString& rName)=0;

			//@}
			/** \name Output management */
			//@{

			/**
			 * \brief Adds an output to this box
			 * \param sName [in] : The output name
			 * \param rTypeIdentifier [in] : The
			 *        output type idenfitier
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 *
			 * The output is always added after the last
			 * already existing output.
			 */
			virtual OpenViBE::boolean addOutput(
				const OpenViBE::CString& sName,
				const OpenViBE::CIdentifier& rTypeIdentifier)=0;
			/**
			 * \brief Removes an output for this box
			 * \param ui32OutputIndex [in] : The index
			 *        of the output to remove
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 *
			 * Outputs coming after the removed output
			 * have their indices changing after this,
			 * they all decrease by 1.
			 */
			virtual OpenViBE::boolean removeOutput(
				const OpenViBE::uint32 ui32OutputIndex)=0;
			/**
			 * \brief Gets the number of outputs for this box
			 * \return The number of outputs for this box.
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
			/**
			 * \brief Sets an output type identifier
			 * \param ui32OutputIndex [in] : The output index
			 * \param rTypeIdentifier [in] : The type identifier
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean setOutputType(
				const OpenViBE::uint32 ui32OutputIndex,
				const OpenViBE::CIdentifier& rTypeIdentifier)=0;
			/**
			 * \brief Sets an output name
			 * \param ui32OutputIndex [in] : The output index
			 * \param rName [in] : The name of this output
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean setOutputName(
				const OpenViBE::uint32 ui32OutputIndex,
				const OpenViBE::CString& rName)=0;

			//@}
			/** \name Setting management */
			//@{

			/**
			 * \brief Adds a setting to this box
			 * \param sName [in] : The setting name
			 * \param rTypeIdentifier [in] : The
			 *        setting type identifier
			 * \param sDefaultValue [in] : The default
			 *        value for this setting
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 *
			 * The setting is always added after the last
			 * already existing setting.
			 */
			virtual OpenViBE::boolean addSetting(
				const OpenViBE::CString& sName,
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::CString& sDefaultValue)=0;
			/**
			 * \brief Removes a setting for this box
			 * \param ui32SettingIndex [in] : The index
			 *        of the setting to remove
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 *
			 * Settings coming after the removed setting
			 * have their indices changing after this,
			 * they all decrease by 1.
			 */
			virtual OpenViBE::boolean removeSetting(
				const OpenViBE::uint32 ui32Index)=0;
			/**
			 * \brief Gets the number of settings for this box
			 * \return The number of settings for this box.
			 */
			virtual OpenViBE::uint32 getSettingCount(void) const=0;
			/**
			 * \brief Gets a seting type identifier
			 * \param ui32SettingIndex [in] : The setting index
			 * \param rTypeIdentifier [out] : The type identifier
			 * \return \e true in case of success.
			 * \return \e false in case of error. In such case,
			 *         \c rTypeIdentifier remains unchanged.
			 */
			virtual OpenViBE::boolean getSettingType(
				const OpenViBE::uint32 ui32SettingIndex,
				OpenViBE::CIdentifier& rTypeIdentifier) const=0;
			/**
			 * \brief Gets a setting name
			 * \param ui32SettingIndex [in] : The setting index
			 * \param rName [out] : The name of this setting
			 * \return \e true in case of success.
			 * \return \e false in case of error. In such case,
			 *         \c rName remains unchanged.
			 */
			virtual OpenViBE::boolean getSettingName(
				const OpenViBE::uint32 ui32SettingIndex,
				OpenViBE::CString& rName) const=0;
			/**
			 * \brief Gets the default setting value
			 * \param ui32SettingIndex [in] : The setting index
			 * \param rDefaultValue [out] : The default value
			 * \return \e true in case of success.
			 * \return \e false in case of error. In such case,
			 *         \c rDefaultValue remains unchanged.
			 */
			virtual OpenViBE::boolean getSettingDefaultValue(
				const OpenViBE::uint32 ui32SettingIndex,
				OpenViBE::CString& rDefaultValue) const=0;
			/**
			 * \brief Gets the setting value
			 * \param ui32SettingIndex [in] : The setting index
			 * \param rValue [out] : The value
			 * \return \e true in case of success.
			 * \return \e false in case of error. In such case,
			 *         \c rValue remains unchanged.
			 */
			virtual OpenViBE::boolean getSettingValue(
				const OpenViBE::uint32 ui32SettingIndex,
				OpenViBE::CString& rValue) const=0;
			/**
			 * \brief Sets a setting type identifier
			 * \param ui32SettingIndex [in] : The setting index
			 * \param rTypeIdentifier [in] : The type identifier
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean setSettingType(
				const OpenViBE::uint32 ui32SettingIndex,
				const OpenViBE::CIdentifier& rTypeIdentifier)=0;
			/**
			 * \brief Sets an setting name
			 * \param ui32SettingIndex [in] : The setting index
			 * \param rName [in] : The name of this setting
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean setSettingName(
				const OpenViBE::uint32 ui32SettingIndex,
				const OpenViBE::CString& rName)=0;
			/**
			 * \brief Sets the default setting value
			 * \param ui32SettingIndex [in] : The setting index
			 * \param rValue [in] : The default value
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean setSettingDefaultValue(
				const OpenViBE::uint32 ui32SettingIndex,
				const OpenViBE::CString& rDefaultValue)=0;
			/**
			 * \brief Sets the setting value
			 * \param ui32SettingIndex [in] : The setting index
			 * \param rDefaultValue [in] : The value
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean setSettingValue(
				const OpenViBE::uint32 ui32SettingIndex,
				const OpenViBE::CString& rValue)=0;

			//@}

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Box)
		};
	};
};

#endif // __OpenViBE_Kernel_IBox_H__
