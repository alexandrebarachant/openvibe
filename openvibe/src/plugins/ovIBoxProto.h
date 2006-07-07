#ifndef __OpenViBE_Plugins_IBoxProto_H__
#define __OpenViBE_Plugins_IBoxProto_H__

#include "../ovIObject.h"

namespace OpenViBE
{
	namespace Plugins
	{
		/**
		 * \class IBoxProto
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2006-07-05
		 * \brief §OpenViBE§ box prototype
		 *
		 * This class is used by a plugin algorithm descriptor
		 * to let the §OpenViBE§ platform know what an algorithm
		 * box looks like. It declares several things, like
		 * it input types, output types and settings.
		 *
		 * \sa OpenViBE::Plugins::IBoxAlgorithmDesc
		 */
		class OV_API IBoxProto : virtual public OpenViBE::IObject
		{
		public:

			/**
			 * \brief Adds an input to the box
			 * \param sName [in] : the name of the input to add
			 * \param rTypeIdentifier [in] : the type of the input
			 * \param bDiscrete [in] : a flag telling whether this
			 *        input is discrete or not
			 * \return The created input index.
			 */
			virtual OpenViBE::uint32 addInput(
				const std::string& sName,
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::boolean bDiscrete=false)=0;
			/**
			 * \brief Adds an output to the box
			 * \param sName [in] : the name of the output to add
			 * \param rTypeIdentifier [in] : the type of the output
			 * \param bDiscrete [in] : a flag telling whether this
			 *        output is discrete or not
			 * \return The created output index.
			 */
			virtual OpenViBE::uint32 addOutput(
				const std::string& sName,
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const OpenViBE::boolean bDiscrete=false)=0;
			/**
			 * \brief Adds an setting to the box
			 * \param sName [in] : the name of the setting to add
			 * \param rTypeIdentifier [in] : the type of the setting
			 * \param sDefaultValue [in] : the default value of this
			 *        setting (used to initialize the box itself)
			 * \return The created setting index.
			 */
			virtual OpenViBE::uint32 addSetting(
				const std::string& sName,
				const OpenViBE::CIdentifier& rTypeIdentifier,
				const std::string& sDefaultValue)=0;

			/* TODO add flags etc... */

			_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassIdentifier_BoxProto)
		};
	};
};

#endif // __OpenViBE_Plugins_IBoxProto_H__
