#ifndef __OpenViBE_CStimulationSet_H__
#define __OpenViBE_CStimulationSet_H__

#include "ovIStimulationSet.h"

namespace OpenViBE
{
	class OV_API CStimulationSet : virtual public OpenViBE::IStimulationSet
	{
	public:

		CStimulationSet(void);
		virtual ~CStimulationSet(void);

		virtual const OpenViBE::uint64 getStimulationCount(void) const;
		virtual const OpenViBE::uint64 getStimulationIdentifier(const OpenViBE::uint64 ui64StimulationIndex) const;
		virtual const OpenViBE::uint64 getStimulationDate(const OpenViBE::uint64 ui64StimulationIndex) const;
		virtual const OpenViBE::uint64 getStimulationDuration(const OpenViBE::uint64 ui64StimulationIndex) const;

		virtual OpenViBE::boolean setStimulationCount(const OpenViBE::uint64 ui64StimulationCount);
		virtual OpenViBE::boolean setStimulationIdentifier(const OpenViBE::uint64 ui64StimulationIndex, const OpenViBE::uint64 ui64StimulationIdentifier);
		virtual OpenViBE::boolean setStimulationDate(const OpenViBE::uint64 ui64StimulationIndex, const OpenViBE::uint64 ui64StimulationDate);
		virtual OpenViBE::boolean setStimulationDuration(const OpenViBE::uint64 ui64StimulationIndex, const OpenViBE::uint64 ui64StimulationDuration);

		_IsDerivedFromClass_Final_(OpenViBE::IStimulationSet, OV_ClassId_StimulationSetBridge);

	private:

		OpenViBE::IStimulationSet* m_pStimulationSetImpl;
	};
};

#endif // __OpenViBE_CStimulationSet_H__
