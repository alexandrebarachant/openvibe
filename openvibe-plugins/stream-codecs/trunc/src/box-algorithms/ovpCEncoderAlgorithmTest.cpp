#include "ovpCEncoderAlgorithmTest.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::StreamCodecs;

boolean CEncoderAlgorithmTest::initialize(void)
{
	m_pStreamEncoder[0]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_ExperimentInformationEncoder));
	m_pStreamEncoder[1]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_FeatureVectorEncoder));
	m_pStreamEncoder[2]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_SignalEncoder));
	m_pStreamEncoder[3]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_SpectrumEncoder));
	m_pStreamEncoder[4]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_StimulationEncoder));
	m_pStreamEncoder[5]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_StreamedMatrixEncoder));

	for(uint32 i=0; i<6; i++)
	{
		m_pStreamEncoder[i]->initialize();
		m_oMemoryBufferHandle[i].initialize(m_pStreamEncoder[i]->getOutputParameter(OVP_EBMLEncoder_MemoryBuffer_OutParameterId));
	}

	m_oMatrix1.setDimensionCount(2);
	m_oMatrix1.setDimensionSize(0, 16);
	m_oMatrix1.setDimensionSize(1, 16);
	OpenViBEToolkit::Tools::MatrixManipulation::clearContent(m_oMatrix1);

	m_oMatrix2.setDimensionCount(2);
	m_oMatrix2.setDimensionSize(0, 16);
	m_oMatrix2.setDimensionSize(1, 2);
	OpenViBEToolkit::Tools::MatrixManipulation::clearContent(m_oMatrix2);

	uint64 m_ui64SamplingRate=16;

	IMatrix* l_pMatrix1=&m_oMatrix1;
	IMatrix* l_pMatrix2=&m_oMatrix2;
	IStimulationSet* l_pStimulationSet=&m_oStimulationSet;

	m_pStreamEncoder[1]->getInputParameter(OVP_StreamedMatrixEncoder_Matrix_InParameterId)->setValue(&l_pMatrix1);
	m_pStreamEncoder[2]->getInputParameter(OVP_StreamedMatrixEncoder_Matrix_InParameterId)->setValue(&l_pMatrix1);
	m_pStreamEncoder[2]->getInputParameter(OVP_SignalEncoder_SamplingRate_InParameterId)->setValue(&m_ui64SamplingRate);
	m_pStreamEncoder[3]->getInputParameter(OVP_StreamedMatrixEncoder_Matrix_InParameterId)->setValue(&l_pMatrix1);
	m_pStreamEncoder[3]->getInputParameter(OVP_SpectrumEncoder_FrequencyBandMinMax_InParameterId)->setValue(&l_pMatrix2);
	m_pStreamEncoder[4]->getInputParameter(OVP_StimulationEncoder_StimulationSet_InParameterId)->setValue(&l_pStimulationSet);
	m_pStreamEncoder[5]->getInputParameter(OVP_StreamedMatrixEncoder_Matrix_InParameterId)->setValue(&l_pMatrix1);

	m_bHasSentHeader=false;
	m_ui64StartTime=0;
	m_ui64EndTime=0;

	return true;
}

boolean CEncoderAlgorithmTest::uininitialize(void)
{
	for(uint32 i=0; i<6; i++)
	{
		m_oMemoryBufferHandle[i].uninitialize();
		m_pStreamEncoder[i]->uninitialize();
		getAlgorithmManager().releaseAlgorithm(*m_pStreamEncoder[i]);
		m_pStreamEncoder[i]=NULL;
	}

	return true;
}

boolean CEncoderAlgorithmTest::processClock(IMessageClock& rMessageClock)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CEncoderAlgorithmTest::process(void)
{
	IBoxIO& l_rDynamicBoxContext=getDynamicBoxContext();
	IBox& l_rStaticBoxContext=getStaticBoxContext();
	IPlayerContext& l_rPlayerContext=getPlayerContext();

	if(!m_bHasSentHeader)
	{
		m_ui64StartTime=0;
		m_ui64EndTime=0;
		for(uint32 i=0; i<l_rStaticBoxContext.getOutputCount(); i++)
		{
			m_oMemoryBufferHandle[i]=l_rDynamicBoxContext.getOutputChunk(i);
			m_pStreamEncoder[i]->process(OVP_EBMLEncoder_WriteHeader_TriggerId);
		}
		m_bHasSentHeader=true;
	}
	else
	{
		for(uint32 i=0; i<l_rStaticBoxContext.getOutputCount(); i++)
		{
			m_oMemoryBufferHandle[i]=l_rDynamicBoxContext.getOutputChunk(i);
			m_pStreamEncoder[i]->process(OVP_EBMLEncoder_WriteBuffer_TriggerId);
		}
	}

	for(uint32 i=0; i<l_rStaticBoxContext.getOutputCount(); i++)
	{
		l_rDynamicBoxContext.markOutputAsReadyToSend(i, m_ui64StartTime, m_ui64EndTime);
	}

	m_ui64StartTime=m_ui64EndTime;
	m_ui64EndTime=l_rPlayerContext.getCurrentTime();

	return true;
}
