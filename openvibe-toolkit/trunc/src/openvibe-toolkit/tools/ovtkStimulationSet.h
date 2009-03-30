#ifndef __OpenViBEToolkit_Tools_StimulationSet_H__
#define __OpenViBEToolkit_Tools_StimulationSet_H__

#include "../ovtk_base.h"

namespace OpenViBEToolkit
{
	namespace Tools
	{
		namespace StimulationSet
		{
			OVTK_API OpenViBE::boolean copy(OpenViBE::IStimulationSet& rDestinationStimulationSet, const OpenViBE::IStimulationSet& rSourceStimulationSet, const OpenViBE::uint64 ui64TimeShift=0);
			OVTK_API OpenViBE::boolean shift(OpenViBE::IStimulationSet& rStimulationSet, const OpenViBE::uint64 ui64TimeShift);
		};
	};
};

#endif // __OpenViBEToolkit_Tools_StimulationSet_H__
