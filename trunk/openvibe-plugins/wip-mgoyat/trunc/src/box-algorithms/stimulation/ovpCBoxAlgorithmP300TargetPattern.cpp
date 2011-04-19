#include "ovpCBoxAlgorithmP300TargetPattern.h"

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

uint64 CBoxAlgorithmP300TargetPattern::getClockFrequency(void)
{
	return 128LL<<32;
}

boolean CBoxAlgorithmP300TargetPattern::initialize(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();

	m_ui64FlashCountRow		=_AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 0);
	m_ui64FlashCountColumn	=_AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 1);
	m_ui64TrialCount        =_AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 2);
	m_ui64BlocCount         =_AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 3);

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

boolean CBoxAlgorithmP300TargetPattern::uninitialize(void)
{
	m_pStimulationDecoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationDecoder);

	ip_pStimulationToEncode.uninitialize();
	op_pEncodedMemoryBuffer.uninitialize();
	m_pStimulationEncoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationEncoder);

	return true;
}

boolean CBoxAlgorithmP300TargetPattern::processInput(uint32 ui32InputIndex)
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
						//m_ui64TrialStartTime=op_pStimulationSet->getStimulationDate(j);
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

boolean CBoxAlgorithmP300TargetPattern::processClock(IMessageClock& rMessageClock)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}

boolean CBoxAlgorithmP300TargetPattern::process(void)
{
	// IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();


	if(m_bStartReceived && !m_bHeaderSent)
	{
		std::vector<std::pair<int,int> > l_vSequence;
		CStimulationSet l_stimSet;
		uint64 l_ui64EndTime=0;
		for(uint32 i=0; i<m_ui64BlocCount*m_ui64TrialCount ; i++)
		{
			std::vector<int> vect=generateTarget();
			for(uint32 j=0; j<vect.size(); j++)
			  {
				int row=vect[j];
				l_stimSet.appendStimulation(OVTK_StimulationId_Label_00,l_ui64EndTime,0);
				l_stimSet.appendStimulation(OVTK_StimulationId_Label_01+row,l_ui64EndTime,0);
				j++;
				if(j>=vect.size()) {continue;}
				int col=vect[j];
				l_stimSet.appendStimulation(OVTK_StimulationId_Label_07+col,l_ui64EndTime,0);
				//
				l_vSequence.push_back(std::pair<int,int>(row,col));
				//
				l_ui64EndTime++;
			  }
		}
		l_stimSet.appendStimulation(OVTK_StimulationId_Train,l_ui64EndTime,0);



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
		pFile = fopen ("config_P300TargetSequence.txt" , "w");
		if (pFile == NULL) perror ("Error opening file");
		else
		  {
			for(unsigned int i=0; i<l_vSequence.size(); i++)
			  {
				std::stringstream out;
				out << l_vSequence.at(i).first <<" "<<l_vSequence.at(i).second<<"\n";
				std::string str = out.str();
				fwrite (str.c_str() , 1 , str.size() , pFile );
			  }
			fclose (pFile);
		  }
		
	}

	return true;
}

std::vector<int> CBoxAlgorithmP300TargetPattern::generateTarget()
{
 std::vector<int> vect;

	std::vector < uint32 > l_vFlashRow;
	for(uint32 i=0; i<m_ui64FlashCountRow; i++)
	{
		l_vFlashRow.push_back(i);
	}
	std::vector < uint32 > l_vFlashColumn;
	for(uint32 i=0; i<m_ui64FlashCountColumn; i++)
	{
		l_vFlashColumn.push_back(i);
	}

	uint32 j=rand()%l_vFlashRow.size();
	vect.push_back(l_vFlashRow[j]);
	//
	j=rand()%l_vFlashColumn.size();
	vect.push_back(l_vFlashColumn[j]);

 return vect;
}
