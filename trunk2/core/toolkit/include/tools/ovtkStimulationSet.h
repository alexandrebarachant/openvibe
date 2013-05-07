#ifndef __OpenViBEToolkit_Tools_StimulationSet_H__
#define __OpenViBEToolkit_Tools_StimulationSet_H__

#include "../ovtk_base.h"

namespace OpenViBEToolkit
{
	namespace Tools
	{
		namespace StimulationSet
		{
			OV_API OpenViBE::boolean shift(OpenViBE::IStimulationSet& rStimulationSet, const OpenViBE::uint64 ui64TimeShift);
			OV_API OpenViBE::boolean copy(OpenViBE::IStimulationSet& rDestinationStimulationSet, const OpenViBE::IStimulationSet& rSourceStimulationSet, const OpenViBE::uint64 ui64TimeShift=0);
			OV_API OpenViBE::boolean append(OpenViBE::IStimulationSet& rDestinationStimulationSet, const OpenViBE::IStimulationSet& rSourceStimulationSet, const OpenViBE::uint64 ui64TimeShift=0);
			OV_API OpenViBE::boolean appendRange(OpenViBE::IStimulationSet& rDestinationStimulationSet, const OpenViBE::IStimulationSet& rSourceStimulationSet, const OpenViBE::uint64 ui64SourceStartTime, const OpenViBE::uint64 ui64SourceEndTime, const OpenViBE::uint64 ui64TimeShift=0);
			OV_API OpenViBE::boolean removeRange(OpenViBE::IStimulationSet& rStimulationSet, const OpenViBE::uint64 ui64StartTime, const OpenViBE::uint64 ui64EndTime);
		};
	};
};

#endif // __OpenViBEToolkit_Tools_StimulationSet_H__
