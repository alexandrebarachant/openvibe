#ifndef __OpenViBE_IStimulationSet_H__
#define __OpenViBE_IStimulationSet_H__

#include "ovIObject.h"

namespace OpenViBE
{
	class OV_API IStimulationSet : virtual public OpenViBE::IObject
	{
	public:

		virtual const OpenViBE::uint64 getStimulationCount(void) const=0;
		virtual const OpenViBE::uint64 getStimulationIdentifier(const OpenViBE::uint64 ui64StimulationIndex) const=0;
		virtual const OpenViBE::uint64 getStimulationDate(const OpenViBE::uint64 ui64StimulationIndex) const=0;
		virtual const OpenViBE::uint64 getStimulationDuration(const OpenViBE::uint64 ui64StimulationIndex) const=0;

		virtual OpenViBE::boolean setStimulationCount(const OpenViBE::uint64 ui64StimulationCount)=0;
		virtual OpenViBE::boolean setStimulationIdentifier(const OpenViBE::uint64 ui64StimulationIndex, const OpenViBE::uint64 ui64StimulationIdentifier)=0;
		virtual OpenViBE::boolean setStimulationDate(const OpenViBE::uint64 ui64StimulationIndex, const OpenViBE::uint64 ui64StimulationDate)=0;
		virtual OpenViBE::boolean setStimulationDuration(const OpenViBE::uint64 ui64StimulationIndex, const OpenViBE::uint64 ui64StimulationDuration)=0;

		_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassId_StimulationSet);
	};
};

#endif // __OpenViBE_IStimulationSet_H__
