#ifndef __OpenViBE_AcquisitionServer_IHeaderConfigurator_H__
#define __OpenViBE_AcquisitionServer_IHeaderConfigurator_H__

#include "ovas_base.h"

namespace OpenViBEAcquisitionServer
{
	class IHeader;

	class IHeaderConfigurator
	{
	public:

		virtual void release(void)=0;

		virtual OpenViBEAcquisitionServer::boolean configure(
			OpenViBEAcquisitionServer::IHeader& rHeader)=0;

	protected:

		virtual ~IHeaderConfigurator(void) { }
	};
};

#endif // __OpenViBE_AcquisitionServer_IHeaderConfigurator_H__
