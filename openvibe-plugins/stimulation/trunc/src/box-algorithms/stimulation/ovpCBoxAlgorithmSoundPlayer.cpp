#include "ovpCBoxAlgorithmSoundPlayer.h"

#if defined OVP_OS_Linux
 #include <unistd.h>
#endif

#include <string>
#include <cstdlib>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Stimulation;

using namespace std;

/*
uint64 CBoxAlgorithmSoundPlayer::getClockFrequency(void)
{
	return 0; // the box clock frequency
}
*/

boolean CBoxAlgorithmSoundPlayer::initialize(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();

	m_pStreamDecoder=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
	m_pStreamDecoder->initialize();

	CString l_sSettingValue;
	l_rStaticBoxContext.getSettingValue(0, l_sSettingValue);
	l_rStaticBoxContext.getSettingValue(1, m_sSoundFilename);
	m_ui64StimulationId=getTypeManager().getEnumerationEntryValueFromName(OV_TypeId_Stimulation, l_sSettingValue);

	return true;
}

boolean CBoxAlgorithmSoundPlayer::uninitialize(void)
{
	m_pStreamDecoder->uninitialize();
	getAlgorithmManager().releaseAlgorithm(*m_pStreamDecoder);

	return true;
}

/*
boolean CBoxAlgorithmSoundPlayer::processEvent(IMessageEvent& rMessageEvent)
{
	// ...

	// getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}
*/

/*
boolean CBoxAlgorithmSoundPlayer::processSignal(IMessageSignal& rMessageSignal)
{
	// ...

	// getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}
*/

/*
boolean CBoxAlgorithmSoundPlayer::processClock(IMessageClock& rMessageClock)
{
	// ...

	// getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}
*/

boolean CBoxAlgorithmSoundPlayer::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}

boolean CBoxAlgorithmSoundPlayer::process(void)
{
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	for(uint32 i=0; i<l_rDynamicBoxContext.getInputChunkCount(0); i++)
	{
		TParameterHandler < const IMemoryBuffer* > l_ipMemoryBuffer(m_pStreamDecoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
		l_ipMemoryBuffer=l_rDynamicBoxContext.getInputChunk(0, i);
		m_pStreamDecoder->process();
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedHeader))
		{
		}
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
		{
			TParameterHandler < IStimulationSet* > l_opStimulationSet(m_pStreamDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
			for(uint32 j=0; j<l_opStimulationSet->getStimulationCount(); j++)
			{
				if(l_opStimulationSet->getStimulationIdentifier(j)==m_ui64StimulationId)
				{
					getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Trace << "Beep !\n";
#if defined OVP_OS_Linux
					string l_sCommand;
					l_sCommand+="cat ";
					l_sCommand+=m_sSoundFilename.toASCIIString();
					l_sCommand+=" > /dev/dsp &";
					int l_iResult=::system(l_sCommand.c_str());
#else
					getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Warning << "Sound player not yet implemented for this OS\n";
#endif
				}
			}
		}
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedEnd))
		{
		}

		l_rDynamicBoxContext.markInputAsDeprecated(0, i);
	}

	return true;
}
