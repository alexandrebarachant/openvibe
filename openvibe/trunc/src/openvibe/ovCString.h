#ifndef __OpenViBE_CString_H__
#define __OpenViBE_CString_H__

#include "ov_base.h"

namespace OpenViBE
{
	typedef struct CStringImpl CStringImpl;

	/**
	 * \class CString
	 * \author Yann Renard (INRIA/IRISA)
	 * \date 2006-08-10
	 * \brief String class to avoid std::string in the interface
	 *
	 * This class helps avoiding std::string being present in exposed
	 * C++ interface, eventually resulting in compile/link errors when
	 * dynamically loading modules.
	 *
	 * \note The implementation uses std::string of course :)
	 */
	class OV_API CString
	{
	public:

		/** \name Constructor / Destructor */
		//@{

		/**
		 * \brief Default constructor
		 *
		 * Initializes the string to an empty string.
		 */
		CString(void);
		/**
		 * \brief Copy constructor
		 * \param rString [in] : The string to copy
		 *
		 * Copies the content of \c rString into the new string.
		 */
		CString(const OpenViBE::CString& rString);
		/**
		 * \brief Constructor based on ASCII strings
		 * \param pString [in] : The string to copy
		 *
		 * Copies the content of \c pString into the new string.
		 */
		CString(const char* pString);
		/**
		 * \brief Destructor
		 *
		 * The destructor releases the std::string implementation !
		 */
		virtual ~CString(void);

		//@}
		/** \name Operators */
		//@{

		/**
		 * \brief ASCII string cast operator
		 * \return The string formated as standard ASCII string used in C.
		 *
		 * The implementation simply calls \c c_str().
		 */
		operator const char* (void) const;
		/**
		 * \brief Affectation operator (copy)
		 * \param rString [in] : The string to copy
		 * \return This string.
		 */
		OpenViBE::CString& operator=(const OpenViBE::CString& rString);
		/**
		 * \brief Addition operator
		 * \param rString1 [in] : The first part of the resulting string
		 * \param rString2 [in] : The second part of the resulting string
		 * \return The concatenation of \c rString1 and \c rString2.
		 */
		friend OV_API const OpenViBE::CString operator+(const OpenViBE::CString& rString1, const OpenViBE::CString& rString2);
		/**
		 * \brief Equality comparison operator
		 * \param rString1 [in] : The first part of the resulting string
		 * \param rString2 [in] : The second part of the resulting string
		 * \return \e true is \c rString1 is exactly rString2.
		 * \return \e false in other case.
		 * \note This is case sensitive !
		 */
		friend OV_API OpenViBE::boolean operator==(const OpenViBE::CString& rString1, const OpenViBE::CString& rString2);
		/**
		 * \brief Inequality comparison operator
		 * \param rString1 [in] : The first part of the resulting string
		 * \param rString2 [in] : The second part of the resulting string
		 * \return \e false is \c rString1 is exactly rString2.
		 * \return \e true in other case.
		 * \note This is case sensitive !
		 */
		friend OV_API OpenViBE::boolean operator!=(const OpenViBE::CString& rString1, const OpenViBE::CString& rString2);

		//@}

		virtual OpenViBE::boolean set(const OpenViBE::CString& rString);
		virtual OpenViBE::boolean set(const char* pString);
		virtual const char* toCString(void) const;
		virtual const char* toASCIIString(void) const { return this->toCString(); }

	protected:

		OpenViBE::CStringImpl* m_pStringImpl; ///< The string implementation
	};
};

#endif // __OpenViBE_CString_H__
