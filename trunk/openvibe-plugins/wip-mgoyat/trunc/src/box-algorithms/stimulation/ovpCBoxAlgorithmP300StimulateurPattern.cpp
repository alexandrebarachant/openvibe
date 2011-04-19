#include "ovpCBoxAlgorithmP300StimulateurPattern.h"

#include <list>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cstdio>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Stimulation;


namespace
{
	class _AutoCast_
	{
	public:
		_AutoCast_(IBox& rBox, IConfigurationManager& rConfigurationManager, const uint32 ui32Index) : m_rConfigurationManager(rConfigurationManager) { rBox.getSettingValue(ui32Index, m_sSettingValue); }
		operator uint64 (void) { return m_rConfigurationManager.expandAsUInteger(m_sSettingValue); }
		operator int64 (void) { return m_rConfigurationManager.expandAsInteger(m_sSettingValue); }
		operator float64 (void) { return m_rConfigurationManager.expandAsFloat(m_sSettingValue); }
		operator boolean (void) { return m_rConfigurationManager.expandAsBoolean(m_sSettingValue); }
		operator const CString (void) { return m_sSettingValue; }
	protected:
		IConfigurationManager& m_rConfigurationManager;
		CString m_sSettingValue;
	};
};

uint64 CBoxAlgorithmP300StimulateurPattern::getClockFrequency(void)
{
	return 128LL<<32;
}

boolean CBoxAlgorithmP300StimulateurPattern::initialize(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();

	m_ui64FlashCountInRepetition=_AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 0);
	m_ui64RepetitionCountInTrial=_AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 1);
	m_ui64TrialCount            =_AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 2);
	m_ui64BlocCount             =_AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 3);
	m_bSequenceRowThenColumn	=_AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 4);

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------

	m_pStimulationEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamEncoder));
	m_pStimulationEncoder->initialize();
	ip_pStimulationToEncode.initialize(m_pStimulationEncoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_InputParameterId_StimulationSet));
    op_pEncodedMemoryBuffer.initialize(m_pStimulationEncoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_OutputParameterId_EncodedMemoryBuffer));

	m_pStimulationDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
	m_pStimulationDecoder->initialize();

	m_bHeaderSent=false;
	m_bStartReceived=false;


	return true;
}

boolean CBoxAlgorithmP300StimulateurPattern::uninitialize(void)
{
	m_pStimulationDecoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationDecoder);

	ip_pStimulationToEncode.uninitialize();
	op_pEncodedMemoryBuffer.uninitialize();
	m_pStimulationEncoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationEncoder);

	return true;
}

boolean CBoxAlgorithmP300StimulateurPattern::processInput(uint32 ui32InputIndex)
{
	// IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	for(uint32 i=0; i<l_rDynamicBoxContext.getInputChunkCount(0); i++)
	{
		if(!m_bStartReceived)
		{
			TParameterHandler < const IMemoryBuffer* > ip_pMemoryBuffer(m_pStimulationDecoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
			TParameterHandler < IStimulationSet* > op_pStimulationSet(m_pStimulationDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
			ip_pMemoryBuffer=l_rDynamicBoxContext.getInputChunk(0, i);
			m_pStimulationDecoder->process();

			if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedHeader))
			{
			}

			if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
			{
				for(uint32 j=0; j<op_pStimulationSet->getStimulationCount(); j++)
				{
					if(op_pStimulationSet->getStimulationIdentifier(j) != 0)
					{
						m_ui64TrialStartTime=op_pStimulationSet->getStimulationDate(j);
						m_bStartReceived=true;
						break;
					}
				}
			}

			if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedEnd))
			{
			}
		}

		l_rDynamicBoxContext.markInputAsDeprecated(0, i);
	}

	return true;
}

boolean CBoxAlgorithmP300StimulateurPattern::processClock(IMessageClock& rMessageClock)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}

boolean CBoxAlgorithmP300StimulateurPattern::process(void)
{
	// IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();


	if(m_bStartReceived && !m_bHeaderSent)
	{
		std::vector<int> l_vSequence;
		CStimulationSet l_stimSet;
		uint64 l_ui64EndTime=0;
		l_stimSet.appendStimulation(OVTK_StimulationId_ExperimentStart,l_ui64EndTime,0);
		l_stimSet.appendStimulation(OVTK_StimulationId_RestStart,l_ui64EndTime,0);
		for(int i=0; i<m_ui64BlocCount*m_ui64TrialCount; i++)
		{
			l_ui64EndTime++;
			l_stimSet.appendStimulation(OVTK_StimulationId_RestStop,l_ui64EndTime,0);
			l_stimSet.appendStimulation(OVTK_StimulationId_TrialStart,l_ui64EndTime,0);
			for(int k=0; k<m_ui64RepetitionCountInTrial ; k++)
			  {
				l_stimSet.appendStimulation(OVTK_StimulationId_SegmentStart,l_ui64EndTime,0);
				std::vector<int> vect=generateSequenceRepetition();
				for(uint32 j=0; j<vect.size(); j++)
				  {
					l_vSequence.push_back(vect[j]);
					l_stimSet.appendStimulation(OVTK_StimulationId_Label_01+vect[j],l_ui64EndTime,0);
					l_stimSet.appendStimulation(OVTK_StimulationId_VisualStimulationStart,l_ui64EndTime,0);
					l_ui64EndTime++;
					l_stimSet.appendStimulation(OVTK_StimulationId_VisualStimulationStop,l_ui64EndTime,0);
					l_ui64EndTime++;
				  }
				l_stimSet.appendStimulation(OVTK_StimulationId_SegmentStop,l_ui64EndTime,0);
			  }
			l_stimSet.appendStimulation(OVTK_StimulationId_TrialStop,l_ui64EndTime,0);
			if(i!=m_ui64BlocCount*m_ui64TrialCount-1)
			  {l_stimSet.appendStimulation(OVTK_StimulationId_RestStart,l_ui64EndTime,0);}
		}
		l_ui64EndTime++;
		l_stimSet.appendStimulation(OVTK_StimulationId_ExperimentStop,l_ui64EndTime,0);



		uint32 l_ui32OutputIdx=0;
		ip_pStimulationToEncode=&l_stimSet;
		op_pEncodedMemoryBuffer=l_rDynamicBoxContext.getOutputChunk(l_ui32OutputIdx);
		uint64 l_ui64StartTime=0;
		
		//
		m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeHeader);
		l_rDynamicBoxContext.markOutputAsReadyToSend(l_ui32OutputIdx, l_ui64StartTime, l_ui64EndTime);
		//
		m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeBuffer);
		l_rDynamicBoxContext.markOutputAsReadyToSend(l_ui32OutputIdx, l_ui64StartTime, l_ui64EndTime);
		//
		m_bHeaderSent=true;
		
		//copy en fichier :  
		FILE * pFile;
		pFile = fopen ("config_P300FlashSequence.txt" , "w");
		if (pFile == NULL) perror ("Error opening file");
		else
		  {
			for(unsigned int i=0; i<l_vSequence.size(); i++)
			  {
				std::stringstream out;
				out << l_vSequence.at(i)<<"\n";
				std::string str = out.str();
				fwrite (str.c_str() , 1 , str.size() , pFile );
			  }
			fclose (pFile);
		  }
		  
	}

	return true;
}

std::vector<int> CBoxAlgorithmP300StimulateurPattern::generateSequenceRepetition()
{
 std::vector<int> vect;

 if(!m_bSequenceRowThenColumn)
   {
	std::vector < uint32 > l_vFlash;
	for(uint32 i=0; i<m_ui64FlashCountInRepetition; i++)
	{
		l_vFlash.push_back(i);
	}
	for(uint32 i=0; i<m_ui64FlashCountInRepetition; i++)
	{
		uint32 j=rand()%l_vFlash.size();
		vect.push_back(l_vFlash[j]);
		l_vFlash.erase(l_vFlash.begin()+j);
	}
   }
else
  {
  	std::vector < uint32 > l_vFlash;
	for(uint32 i=0; i<m_ui64FlashCountInRepetition/2; i++)
	{
		l_vFlash.push_back(i);
	}
	for(uint32 i=0; i<m_ui64FlashCountInRepetition/2; i++)
	{
		uint32 j=rand()%l_vFlash.size();
		vect.push_back(l_vFlash[j]);
		l_vFlash.erase(l_vFlash.begin()+j);
	}

	l_vFlash.clear();
	for(uint32 i=m_ui64FlashCountInRepetition/2; i<m_ui64FlashCountInRepetition; i++)
	{
		l_vFlash.push_back(i);
	}
	for(uint32 i=m_ui64FlashCountInRepetition/2; i<m_ui64FlashCountInRepetition; i++)
	{
		uint32 j=rand()%l_vFlash.size();
		vect.push_back(l_vFlash[j]);
		l_vFlash.erase(l_vFlash.begin()+j);
	}

  }
  
 return vect;
}
