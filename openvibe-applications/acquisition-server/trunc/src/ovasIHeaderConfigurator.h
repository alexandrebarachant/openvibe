#ifndef __OpenViBE_AcquisitionServer_IHeaderConfigurator_H__
#define __OpenViBE_AcquisitionServer_IHeaderConfigurator_H__

#include "ovas_base.h"

namespace OpenViBEAcquisitionServer
{
	class IHeader;

	/**
	 * \class IHeaderConfigurator
	 * \author Yann Renard (INRIA/IRISA)
	 * \date 2007-04-01
	 * \brief Base class for partial header fillers
	 *
	 * This base class could be used by drivers as a base
	 * for objects that fill a header (GUI for examples)
	 * when such information are missing from the hardware.
	 *
	 * \sa IHeader
	 * \sa IDriver
	 */
	class IHeaderConfigurator
	{
	public:

		/** \name General purpose functions */
		//@{

		/**
		 * \brief Releases this header
		 *
		 * When called, this function should release the header. It tells
		 * it won't be used any more in the future and any allocated memory
		 * should be freed.
		 */
		virtual void release(void)=0;

		//@}
		/** \name */
		//@{

		/**
		 * \brief Request a configuration for a header
		 * \param rHeader [in|out] : the header to configure
		 * \return \e true in case of success.
		 * \return \e false in case of error.
		 */
		virtual OpenViBEAcquisitionServer::boolean configure(
			OpenViBEAcquisitionServer::IHeader& rHeader)=0;

		//@}

	protected:

		/**
		 * \brief Destructor
		 */
		virtual ~IHeaderConfigurator(void) { }
	};
};

#endif // __OpenViBE_AcquisitionServer_IHeaderConfigurator_H__
