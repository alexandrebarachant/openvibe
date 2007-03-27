#ifndef __OpenViBE_AcquisitionServer_IDriver_H__
#define __OpenViBE_AcquisitionServer_IDriver_H__

#include "ovas_base.h"

namespace OpenViBEAcquisitionServer
{
	class IHeader;

	class IDriverCallback
	{
	public:

		virtual void setSamples(
			const OpenViBEAcquisitionServer::float32* pSample)=0;

		virtual ~IDriverCallback(void) { }
	};

	class IDriver
	{
	public:

		virtual void release(void)=0;
		virtual const char* getName(void)=0;

		/* Should only return when getHeader si able to return correct header ! */
		virtual OpenViBEAcquisitionServer::boolean initialize(
			const OpenViBEAcquisitionServer::uint32 ui32SampleCountPerChannel,
			OpenViBEAcquisitionServer::IDriverCallback& rCallback)=0;
		virtual OpenViBEAcquisitionServer::boolean uninitialize(void)=0;

		virtual OpenViBEAcquisitionServer::boolean start(void)=0;
		virtual OpenViBEAcquisitionServer::boolean stop(void)=0;
		virtual OpenViBEAcquisitionServer::boolean loop(void)=0;

		virtual OpenViBEAcquisitionServer::boolean isConfigurable(void)=0;
		virtual OpenViBEAcquisitionServer::boolean configure(void)=0;
		virtual const OpenViBEAcquisitionServer::IHeader* getHeader(void)=0;

	protected:

		virtual ~IDriver(void) { }
	};
};

#endif // __OpenViBE_AcquisitionServer_IDriver_H__
