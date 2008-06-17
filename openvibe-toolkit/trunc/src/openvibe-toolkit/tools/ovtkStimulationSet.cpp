#include "ovtkStimulationSet.h"

using namespace OpenViBE;

boolean OpenViBEToolkit::Tools::StimulationSet::copy(IStimulationSet& rDestinationStimulationSet, const IStimulationSet& rSourceStimulationSet, const uint64 ui64TimeShift)
{
	uint64 l_ui64StimulationCount=rSourceStimulationSet.getStimulationCount();
	rDestinationStimulationSet.setStimulationCount(l_ui64StimulationCount);
	for(uint64 i=0; i<l_ui64StimulationCount; i++)
	{
		rDestinationStimulationSet.setStimulationIdentifier(i, rSourceStimulationSet.getStimulationIdentifier(i));
		rDestinationStimulationSet.setStimulationDate(i, rSourceStimulationSet.getStimulationDate(i)+ui64TimeShift);
		rDestinationStimulationSet.setStimulationDuration(i, rSourceStimulationSet.getStimulationDuration(i));
	}
	return true;
}

boolean OpenViBEToolkit::Tools::StimulationSet::shift(IStimulationSet& rStimulationSet, const uint64 ui64TimeShift)
{
	uint64 l_ui64StimulationCount=rStimulationSet.getStimulationCount();
	for(uint64 i=0; i<l_ui64StimulationCount; i++)
	{
		rStimulationSet.setStimulationDate(i, rStimulationSet.getStimulationDate(i)+ui64TimeShift);
	}
	return true;
}
