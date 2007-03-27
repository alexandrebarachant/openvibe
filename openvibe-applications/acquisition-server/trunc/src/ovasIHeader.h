#ifndef __OpenViBE_AcquisitionServer_IHeader_H__
#define __OpenViBE_AcquisitionServer_IHeader_H__

#include "ovas_base.h"

namespace OpenViBEAcquisitionServer
{
	class IHeader
	{
	public:

		virtual void release(void)=0;
		virtual void reset(void)=0;

		// Experimentation information
		virtual OpenViBEAcquisitionServer::boolean setExperimentIdentifier(const OpenViBEAcquisitionServer::uint32 ui32ExperimentIdentifier)=0;
		virtual OpenViBEAcquisitionServer::boolean setSubjectAge(const OpenViBEAcquisitionServer::uint32 ui32SubjectAge)=0;
		virtual OpenViBEAcquisitionServer::boolean setSubjectSex(const OpenViBEAcquisitionServer::uint32 ui32SubjectSex)=0;

		virtual OpenViBEAcquisitionServer::uint32 getExperimentIdentifier(void) const=0;
		virtual OpenViBEAcquisitionServer::uint32 getSubjectAge(void) const=0;
		virtual OpenViBEAcquisitionServer::uint32 getSubjectSex(void) const=0;

		virtual OpenViBEAcquisitionServer::boolean isExperimentIdentifierSet(void) const=0;
		virtual OpenViBEAcquisitionServer::boolean isSubjectAgeSet(void) const=0;
		virtual OpenViBEAcquisitionServer::boolean isSubjectSexSet(void) const=0;

		// Chanel information
		virtual OpenViBEAcquisitionServer::boolean setChannelCount(const OpenViBEAcquisitionServer::uint32 ui32ChannelCount)=0;
		virtual OpenViBEAcquisitionServer::boolean setChannelName(const OpenViBEAcquisitionServer::uint32 ui32ChannelIndex, const char* sChannelName)=0;
		virtual OpenViBEAcquisitionServer::boolean setChannelGain(const OpenViBEAcquisitionServer::uint32 ui32ChannelIndex, const OpenViBEAcquisitionServer::float32 f32ChannelGain)=0;
		// virtual OpenViBEAcquisitionServer::boolean setChannelLocation(const OpenViBEAcquisitionServer::uint32 ui32ChannelIndex, const OpenViBEAcquisitionServer::float32 ui32ChannelLocationX, const OpenViBEAcquisitionServer::float32 ui32ChannelLocationY, const OpenViBEAcquisitionServer::float32 ui32ChannelLocationZ)=0;

		virtual OpenViBEAcquisitionServer::uint32 getChannelCount(void) const=0;
		virtual const char* getChannelName(const OpenViBEAcquisitionServer::uint32 ui32ChannelIndex) const=0;
		virtual OpenViBEAcquisitionServer::float32 getChannelGain(const OpenViBEAcquisitionServer::uint32 ui32ChannelIndex) const=0;
		// virtual getChannelLocation(const OpenViBEAcquisitionServer::uint32 ui32ChannelIndex) const=0;

		virtual OpenViBEAcquisitionServer::boolean isChannelCountSet(void) const=0;
		virtual OpenViBEAcquisitionServer::boolean isChannelNameSet(void) const=0;
		virtual OpenViBEAcquisitionServer::boolean isChannelGainSet(void) const=0;
		// virtual OpenViBEAcquisitionServer::boolean isChannelLocationSet(void) const=0;

		// Samples information
		virtual OpenViBEAcquisitionServer::boolean setSamplingFrequency(const OpenViBEAcquisitionServer::uint32 ui32SamplingFrequency)=0;

		virtual OpenViBEAcquisitionServer::uint32 getSamplingFrequency(void) const=0;

		virtual OpenViBEAcquisitionServer::boolean isSamplingFrequencySet(void) const=0;

	protected:

		virtual ~IHeader(void) { }
	};

	extern OpenViBEAcquisitionServer::IHeader* createHeader(void);
};

#endif // __OpenViBE_AcquisitionServer_IHeader_H__
