#include "ovasIHeader.h"

#include <map>
#include <string>

#define _NoValueI_ 0xffffffff

using namespace OpenViBEAcquisitionServer;
using namespace std;

//___________________________________________________________________//
//                                                                   //

namespace OpenViBEAcquisitionServer
{
	class CHeader : virtual public IHeader
	{
	public:

		CHeader(void);
		virtual ~CHeader(void);
		virtual void release(void);
		virtual void reset(void);

		// Experimentation information
		virtual OpenViBEAcquisitionServer::boolean setExperimentIdentifier(const OpenViBEAcquisitionServer::uint32 ui32ExperimentIdentifier);
		virtual OpenViBEAcquisitionServer::boolean setSubjectAge(const OpenViBEAcquisitionServer::uint32 ui32SubjectAge);
		virtual OpenViBEAcquisitionServer::boolean setSubjectSex(const OpenViBEAcquisitionServer::uint32 ui32SubjectSex);

		virtual OpenViBEAcquisitionServer::uint32 getExperimentIdentifier(void) const;
		virtual OpenViBEAcquisitionServer::uint32 getSubjectAge(void) const;
		virtual OpenViBEAcquisitionServer::uint32 getSubjectSex(void) const;

		virtual OpenViBEAcquisitionServer::boolean isExperimentIdentifierSet(void) const;
		virtual OpenViBEAcquisitionServer::boolean isSubjectAgeSet(void) const;
		virtual OpenViBEAcquisitionServer::boolean isSubjectSexSet(void) const;

		// Chanel information
		virtual OpenViBEAcquisitionServer::boolean setChannelCount(const OpenViBEAcquisitionServer::uint32 ui32ChannelCount);
		virtual OpenViBEAcquisitionServer::boolean setChannelName(const OpenViBEAcquisitionServer::uint32 ui32ChannelIndex, const char* sChannelName);
		virtual OpenViBEAcquisitionServer::boolean setChannelGain(const OpenViBEAcquisitionServer::uint32 ui32ChannelIndex, const OpenViBEAcquisitionServer::float32 f32ChannelGain);
		// virtual OpenViBEAcquisitionServer::boolean setChannelLocation(const OpenViBEAcquisitionServer::uint32 ui32ChannelIndex, const OpenViBEAcquisitionServer::float32 ui32ChannelLocationX, const OpenViBEAcquisitionServer::float32 ui32ChannelLocationY, const OpenViBEAcquisitionServer::float32 ui32ChannelLocationZ);

		virtual OpenViBEAcquisitionServer::uint32 getChannelCount(void) const;
		virtual const char* getChannelName(const OpenViBEAcquisitionServer::uint32 ui32ChannelIndex) const;
		virtual OpenViBEAcquisitionServer::float32 getChannelGain(const OpenViBEAcquisitionServer::uint32 ui32ChannelIndex) const;
		// virtual getChannelLocation(const OpenViBEAcquisitionServer::uint32 ui32ChannelIndex) const;

		virtual OpenViBEAcquisitionServer::boolean isChannelCountSet(void) const;
		virtual OpenViBEAcquisitionServer::boolean isChannelNameSet(void) const;
		virtual OpenViBEAcquisitionServer::boolean isChannelGainSet(void) const;
		// virtual OpenViBEAcquisitionServer::boolean isChannelLocationSet(void) const;

		// Samples information
		virtual OpenViBEAcquisitionServer::boolean setSamplingFrequency(const OpenViBEAcquisitionServer::uint32 ui32SamplingFrequency);

		virtual OpenViBEAcquisitionServer::uint32 getSamplingFrequency(void) const;

		virtual OpenViBEAcquisitionServer::boolean isSamplingFrequencySet(void) const;

	protected:

		// Experimentation information
		OpenViBEAcquisitionServer::uint32 m_ui32ExperimentIdentifier;
		OpenViBEAcquisitionServer::uint32 m_ui32SubjectAge;
		OpenViBEAcquisitionServer::uint32 m_ui32SubjectSex;

		// Chanel information
		OpenViBEAcquisitionServer::uint32 m_ui32ChannelCount;
		std::map<OpenViBEAcquisitionServer::uint32, std::string> m_vChannelName;
		std::map<OpenViBEAcquisitionServer::uint32, OpenViBEAcquisitionServer::float32> m_vChannelGain;

		// Samples information
		OpenViBEAcquisitionServer::uint32 m_ui32SamplingFrequency;
	};
};

//___________________________________________________________________//
//                                                                   //

CHeader::CHeader(void)
	:m_ui32ExperimentIdentifier(_NoValueI_)
	,m_ui32SubjectAge(_NoValueI_)
	,m_ui32SubjectSex(_NoValueI_)
	,m_ui32ChannelCount(_NoValueI_)
	,m_ui32SamplingFrequency(_NoValueI_)
{
}

CHeader::~CHeader(void)
{
}

void CHeader::release(void)
{
	delete this;
}

void CHeader::reset(void)
{
	m_ui32ExperimentIdentifier=_NoValueI_;
	m_ui32SubjectAge=_NoValueI_;
	m_ui32SubjectSex=_NoValueI_;
	m_ui32ChannelCount=_NoValueI_;
	m_vChannelName.clear();
	m_vChannelGain.clear();
	m_ui32SamplingFrequency=_NoValueI_;
}

//___________________________________________________________________//
//                                                                   //

// Experimentation information
boolean CHeader::setExperimentIdentifier(const uint32 ui32ExperimentIdentifier)
{
	m_ui32ExperimentIdentifier=ui32ExperimentIdentifier;
	return m_ui32ExperimentIdentifier!=_NoValueI_;
}

boolean CHeader::setSubjectAge(const uint32 ui32SubjectAge)
{
	m_ui32SubjectAge=ui32SubjectAge;
	return m_ui32SubjectAge!=_NoValueI_;
}

boolean CHeader::setSubjectSex(const uint32 ui32SubjectSex)
{
	m_ui32SubjectSex=ui32SubjectSex;
	return m_ui32SubjectSex!=_NoValueI_;
}

uint32 CHeader::getExperimentIdentifier(void) const
{
	return m_ui32ExperimentIdentifier;
}

uint32 CHeader::getSubjectAge(void) const
{
	return m_ui32SubjectAge;
}

uint32 CHeader::getSubjectSex(void) const
{
	return m_ui32SubjectSex;
}

boolean CHeader::isExperimentIdentifierSet(void) const
{
	return m_ui32ExperimentIdentifier!=_NoValueI_;
}

boolean CHeader::isSubjectAgeSet(void) const
{
	return m_ui32SubjectAge!=_NoValueI_;
}

boolean CHeader::isSubjectSexSet(void) const
{
	return m_ui32SubjectSex!=_NoValueI_;
}

//___________________________________________________________________//
//                                                                   //

// Chanel information

boolean CHeader::setChannelCount(const uint32 ui32ChannelCount)
{
	m_ui32ChannelCount=ui32ChannelCount;
	m_vChannelName.clear();
	m_vChannelGain.clear();
	return m_ui32ChannelCount!=_NoValueI_;
}

boolean CHeader::setChannelName(const uint32 ui32ChannelIndex, const char* sChannelName)
{
	m_vChannelName[ui32ChannelIndex]=sChannelName;
	return ui32ChannelIndex<m_ui32ChannelCount;
}

boolean CHeader::setChannelGain(const uint32 ui32ChannelIndex, const float32 f32ChannelGain)
{
	m_vChannelGain[ui32ChannelIndex]=f32ChannelGain;
	return ui32ChannelIndex<m_ui32ChannelCount;
}

// boolean CHeader::setChannelLocation(const uint32 ui32ChannelIndex, const float32 ui32ChannelLocationX, const float32 ui32ChannelLocationY, const float32 ui32ChannelLocationZ);

uint32 CHeader::getChannelCount(void) const
{
	return m_ui32ChannelCount;
}

const char* CHeader::getChannelName(const uint32 ui32ChannelIndex) const
{
	map<uint32, string>::const_iterator i=m_vChannelName.find(ui32ChannelIndex);
	if(i==m_vChannelName.end())
	{
		return "";
	}
	return i->second.c_str();
}

float32 CHeader::getChannelGain(const uint32 ui32ChannelIndex) const
{
	map<uint32, float32>::const_iterator i=m_vChannelGain.find(ui32ChannelIndex);
	if(i==m_vChannelGain.end())
	{
		return(ui32ChannelIndex<m_ui32ChannelCount?1:0);
	}
	return i->second;
}

// CHeader::getChannelLocation(const uint32 ui32ChannelIndex) const;

boolean CHeader::isChannelCountSet(void) const
{
	return m_ui32ChannelCount!=_NoValueI_ && m_ui32ChannelCount!=0;
}

boolean CHeader::isChannelNameSet(void) const
{
	return isChannelCountSet();
}

boolean CHeader::isChannelGainSet(void) const
{
	return isChannelCountSet();
}

// boolean CHeader::isChannelLocationSet(void) const

//___________________________________________________________________//
//                                                                   //

// Samples information
boolean CHeader::setSamplingFrequency(const uint32 ui32SamplingFrequency)
{
	m_ui32SamplingFrequency=ui32SamplingFrequency;
	return m_ui32SamplingFrequency!=_NoValueI_;
}

uint32 CHeader::getSamplingFrequency(void) const
{
	return m_ui32SamplingFrequency;
}

boolean CHeader::isSamplingFrequencySet(void) const
{
	return m_ui32SamplingFrequency!=_NoValueI_;
}

//___________________________________________________________________//
//                                                                   //

IHeader* OpenViBEAcquisitionServer::createHeader(void)
{
	return new CHeader();
}

//___________________________________________________________________//
//                                                                   //
