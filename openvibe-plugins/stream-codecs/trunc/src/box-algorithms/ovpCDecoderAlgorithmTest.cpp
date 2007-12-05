#include "ovpCDecoderAlgorithmTest.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::StreamCodecs;

ILogManager& operator << (ILogManager& rLogManager, IMatrix& rMatrix)
{
	rLogManager << "Matrix :\n";
	rLogManager << " | Dimension count : " << rMatrix.getDimensionCount() << "\n";
	for(uint64 i=0; i<rMatrix.getDimensionCount(); i++)
	{
		rLogManager << " |   Dimmesion size " << i << " : " << rMatrix.getDimensionSize(i) << "\n";
		for(uint64 j=0; j<rMatrix.getDimensionSize(i); j++)
		{
			rLogManager << " |     Dimmesion label " << i << " " << j << " : " << rMatrix.getDimensionLabel(i, j) << "\n";
		}
	}
	return rLogManager;
}


ILogManager& operator << (ILogManager& rLogManager, IStimulationSet& rStimulationSet)
{
	rLogManager << "Stimulation set :\n";
	rLogManager << " | Number of elements : " << rStimulationSet.getStimulationCount() << "\n";
	for(uint64 i=0; i<rStimulationSet.getStimulationCount(); i++)
	{
		rLogManager << " |   Stimulation " << i << " : "
			<< "id="       << rStimulationSet.getStimulationIdentifier(i) << " "
			<< "date="     << rStimulationSet.getStimulationDate(i) << " "
			<< "duration=" << rStimulationSet.getStimulationDuration(i) << "\n";
	}
	return rLogManager;
}

CDecoderAlgorithmTest::CDecoderAlgorithmTest(void)
{
}

CDecoderAlgorithmTest::~CDecoderAlgorithmTest(void)
{
}

boolean CDecoderAlgorithmTest::initialize(void)
{
	m_pStreamDecoder[0]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_ExperimentInformationDecoder));
	m_pStreamDecoder[1]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_FeatureVectorDecoder));
	m_pStreamDecoder[2]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_SignalDecoder));
	m_pStreamDecoder[3]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_SpectrumDecoder));
	m_pStreamDecoder[4]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_StimulationDecoder));
	m_pStreamDecoder[5]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_StreamedMatrixDecoder));

	for(uint32 i=0; i<6; i++)
	{
		m_pStreamDecoder[i]->initialize();
		m_oEBMLMemoryBufferHandle[i].initialize(m_pStreamDecoder[i]->getInputParameter(OVP_EBMLDecoder_EBMLMemoryBuffer_InParameterId));
	}

	return true;
}

boolean CDecoderAlgorithmTest::uininitialize(void)
{
	for(uint32 i=0; i<6; i++)
	{
		m_oEBMLMemoryBufferHandle[i].uninitialize();
		m_pStreamDecoder[i]->uninitialize();
		getAlgorithmManager().releaseAlgorithm(*m_pStreamDecoder[i]);
		m_pStreamDecoder[i]=NULL;
	}

	return true;
}

boolean CDecoderAlgorithmTest::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CDecoderAlgorithmTest::process(void)
{
	IBoxIO& l_rDynamicBoxContext=getDynamicBoxContext();
	IBox& l_rStaticBoxContext=getStaticBoxContext();

	for(uint32 i=0; i<l_rStaticBoxContext.getInputCount(); i++)
	{
		for(uint32 j=0; j<l_rDynamicBoxContext.getInputChunkCount(i); j++)
		{
			m_oEBMLMemoryBufferHandle[i]=l_rDynamicBoxContext.getInputChunk(i, j);
			m_pStreamDecoder[i]->process();

			if(m_pStreamDecoder[i]->isOutputTriggerActive(OVP_EBMLDecoder_GotHeader_TriggerId))
			{
				{
					TParameterHandler < IMatrix* > l_oHandle(m_pStreamDecoder[i]->getOutputParameter(OVP_StreamedMatrixDecoder_Matrix_OutParameterId));
					if(l_oHandle.exists())
					{
						getLogManager() << LogLevel_Warning << *l_oHandle << "\n";
					}
				}

				{
					TParameterHandler < IMatrix* > l_oHandle(m_pStreamDecoder[i]->getOutputParameter(OVP_SpectrumDecoder_FrequencyBandMinMax_OutParameterId));
					if(l_oHandle.exists())
					{
						getLogManager() << LogLevel_Warning << *l_oHandle << "\n";
					}
				}

				{
					TParameterHandler < uint64 > l_oHandle(m_pStreamDecoder[i]->getOutputParameter(OVP_SignalDecoder_SamplingRate_OutParameterId));
					if(l_oHandle.exists())
					{
						getLogManager() << LogLevel_Warning << l_oHandle << "\n";
					}
				}
			}

			if(m_pStreamDecoder[i]->isOutputTriggerActive(OVP_EBMLDecoder_GotBuffer_TriggerId))
			{
				{
					TParameterHandler < IStimulationSet* > l_oHandle(m_pStreamDecoder[i]->getOutputParameter(OVP_StimulationDecoder_StimulationSet_OutParameterId));
					if(l_oHandle.exists())
					{
						getLogManager() << LogLevel_Warning << *l_oHandle << "\n";
					}
				}
			}

			l_rDynamicBoxContext.markInputAsDeprecated(i, j);
		}
	}

	return true;
}
