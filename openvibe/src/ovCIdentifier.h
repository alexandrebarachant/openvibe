#ifndef __OpenViBE_CIdentifier_H__
#define __OpenViBE_CIdentifier_H__

#include "ov_base.h"

namespace OpenViBE
{
	/**
	 * \class CIdentifier
	 * \author Yann Renard (INRIA/IRISA)
	 * \date 2006-06-16
	 * \brief Globally used identifiaction class
	 *
	 * This class is the basic class to use in order to identify
	 * objects in the §OpenViBE§ platform. It can be used for class
	 * identification, for object identification and any user needed
	 * identification process. 
	 *
	 * The identification of the §OpenViBE§ platform is based on
	 * 64 bits integers. This allows more than
	 * 1 800 000 000 000 000 000 identifiers to exist... I won't
	 * write that this should be enough ;) (this is (c) Bill Gates)
	 * but at least... it may fit our today needs !
	 *
	 * This class is heavily used in the OpenViBE::IObject class. Also,
	 * the §OpenViBE§ specification gives serveral already defined
	 * class identifiers the developper should know of. For this,
	 * let you have a look to the documentation of ov_defines.h !
	 *
	 * \sa ov_defines.h
	 */
	class OV_API CIdentifier
	{
	public:

		/** \name Constructors */
		//@{

		/**
		 * \brief Default constructor
		 *
		 * Builds up the 64 bits identifier with 32 bits random values.
		 */
		CIdentifier(void);
		/**
		 * \brief 32 bits integer based constructor
		 * \param ui32Identifier1 [in] : the first part of the identifier
		 * \param ui32Identifier2 [in] : the second part of the identifier
		 *
		 * Builds up the 64 bits identifier given its two 32 bits
		 * components.
		 */
		CIdentifier(
			const OpenViBE::uint32 ui32Identifier1, 
			const OpenViBE::uint32 ui32Identifier2);
		/**
		 * \brief Copy constructor
		 * \param rIdentifier [in] : the identifier to initialize
		 *        this identifier from
		 *
		 * Builds up the 64 bits identifier exacly the same as
		 * given rIdentifier parameter.
		 */
		CIdentifier(
			const OpenViBE::CIdentifier& rIdentifier);

		//@}
		/** \name Operators */
		//@{

		/**
		 * \brief Affectation operator
		 * \param rIdentifier [in] : the identifier to initialize
		 *        this identifier from
		 *
		 * Reinitializes the 64 bits identifier exactly the same as
		 * given rIdentifier parameter.
		 */
		const OpenViBE::CIdentifier& operator=(
			const OpenViBE::CIdentifier& rIdentifier);
		/**
		 * \brief Equality test operator
		 * \param rIdentifier1 [in] : the first identifier to compare
		 * \param rIdentifier2 [in] : the second identifier to compare
		 * \return \e true if the two identifiers are equal,
		 *         \e false if the two identifiers are different
		 *
		 * Compares both 32 bits parts of the two identifiers and
		 * checks if those are equal or not.
		 *
		 * \sa operator!=
		 */
		friend OpenViBE::boolean operator==(
			const OpenViBE::CIdentifier& rIdentifier1, 
			const OpenViBE::CIdentifier& rIdentifier2);
		/**
		 * \brief Difference test operator
		 * \param rIdentifier1 [in] : the first identifier to compare
		 * \param rIdentifier2 [in] : the second identifier to compare
		 * \return \e true if the two identifiers are different,
		 *         \e false if the two identifiers are equal
		 *
		 * Compares both 32 bits parts of the two identifiers and
		 * checks if those are equal or not.
		 *
		 * \sa operator==
		 */
		friend OpenViBE::boolean operator!=(
			const OpenViBE::CIdentifier& rIdentifier1, 
			const OpenViBE::CIdentifier& rIdentifier2);

		//@}

		std::string toString(void) const;

	protected:

		OpenViBE::uint32 m_ui32Identifier1; ///< First 32 bits part of the identifier
		OpenViBE::uint32 m_ui32Identifier2; ///< Second 32 bits part of the identifier
	};
};

#endif // __OpenViBE_CIdentifier_H__
