#include "ovpCEncoderAlgorithmTest.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::StreamCodecs;

boolean CEncoderAlgorithmTest::initialize(void)
{
	m_pStreamEncoder[0]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_ExperimentInformationStreamEncoder));
	m_pStreamEncoder[1]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_FeatureVectorStreamEncoder));
	m_pStreamEncoder[2]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_SignalStreamEncoder));
	m_pStreamEncoder[3]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_SpectrumStreamEncoder));
	m_pStreamEncoder[4]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_StimulationStreamEncoder));
	m_pStreamEncoder[5]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_StreamedMatrixStreamEncoder));

	for(uint32 i=0; i<6; i++)
	{
		m_pStreamEncoder[i]->initialize();
		op_pMemoryBuffer[i].initialize(m_pStreamEncoder[i]->getOutputParameter(OVP_Algorithm_EBMLStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
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

	m_pStreamEncoder[1]->getInputParameter(OVP_Algorithm_StreamedMatrixStreamEncoder_InputParameterId_Matrix)->setValue(&l_pMatrix1);
	m_pStreamEncoder[2]->getInputParameter(OVP_Algorithm_StreamedMatrixStreamEncoder_InputParameterId_Matrix)->setValue(&l_pMatrix1);
	m_pStreamEncoder[2]->getInputParameter(OVP_Algorithm_SignalStreamEncoder_InputParameterId_SamplingRate)->setValue(&m_ui64SamplingRate);
	m_pStreamEncoder[3]->getInputParameter(OVP_Algorithm_StreamedMatrixStreamEncoder_InputParameterId_Matrix)->setValue(&l_pMatrix1);
	m_pStreamEncoder[3]->getInputParameter(OVP_Algorithm_SpectrumStreamEncoder_InputParameterId_MinMaxFrequencyBands)->setValue(&l_pMatrix2);
	m_pStreamEncoder[4]->getInputParameter(OVP_Algorithm_StimulationStreamEncoder_InputParameterId_StimulationSet)->setValue(&l_pStimulationSet);
	m_pStreamEncoder[5]->getInputParameter(OVP_Algorithm_StreamedMatrixStreamEncoder_InputParameterId_Matrix)->setValue(&l_pMatrix1);

	m_bHasSentHeader=false;
	m_ui64StartTime=0;
	m_ui64EndTime=0;

	return true;
}

boolean CEncoderAlgorithmTest::uininitialize(void)
{
	for(uint32 i=0; i<6; i++)
	{
		op_pMemoryBuffer[i].uninitialize();
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
			op_pMemoryBuffer[i]=l_rDynamicBoxContext.getOutputChunk(i);
			m_pStreamEncoder[i]->process(OVP_Algorithm_EBMLStreamEncoder_InputTriggerId_EncodeHeader);
		}
		m_bHasSentHeader=true;
	}
	else
	{
		for(uint32 i=0; i<l_rStaticBoxContext.getOutputCount(); i++)
		{
			op_pMemoryBuffer[i]=l_rDynamicBoxContext.getOutputChunk(i);
			m_pStreamEncoder[i]->process(OVP_Algorithm_EBMLStreamEncoder_InputTriggerId_EncodeBuffer);
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
