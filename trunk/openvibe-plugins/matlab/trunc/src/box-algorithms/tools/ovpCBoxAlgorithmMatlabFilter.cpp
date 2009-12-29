#include "ovpCBoxAlgorithmMatlabFilter.h"

#if defined TARGET_HAS_ThirdPartyMatlab

#include <system/Memory.h>

#include <mex.h>
#include <engine.h>

#define m_pMatlabEngine ((Engine*)m_pMatlabEngineHandle)
#define m_pMatlabStimulation ((mxArray*)m_pMatlabStimulationHandle)
#define m_pMatlabMatrix ((mxArray*)m_pMatlabMatrixHandle)
#define m_pMatlabBCIContext ((mxArray*)m_pMatlabBCIContextHandle)

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Local;

boolean CBoxAlgorithmMatlabFilter::initialize(void)
{
	CString l_sSettingValue;

	this->getLogManager() << LogLevel_Trace << "Trying to open Matlab engine\n";
#if defined OVP_OS_Linux
	getStaticBoxContext().getSettingValue(0, l_sSettingValue);
	m_pMatlabEngineHandle=::engOpen(l_sSettingValue.toASCIIString());
#elif defined OVP_OS_Windows
	m_pMatlabEngineHandle=::engOpen(NULL);
#else
#endif
	if(!m_pMatlabEngine)
	{
		this->getLogManager() << LogLevel_Error << "Could not open Matlab engine\n";
	}

	getStaticBoxContext().getSettingValue(1, l_sSettingValue);
	l_sSettingValue=CString("cd ")+l_sSettingValue;
	::engEvalString(m_pMatlabEngine, l_sSettingValue.toASCIIString());

	m_pMatlabStimulationHandle=NULL;
	m_pMatlabMatrixHandle=NULL;
	m_pMatlabBCIContextHandle=::mxCreateDoubleMatrix(
		2,
		1,
		mxREAL);

	m_pStimulationDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
	m_pStreamedMatrixDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StreamedMatrixStreamDecoder));
	m_pStreamedMatrixEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StreamedMatrixStreamEncoder));

	m_pStimulationDecoder->initialize();
	m_pStreamedMatrixDecoder->initialize();
	m_pStreamedMatrixEncoder->initialize();

	op_pStimulationSet.initialize(m_pStimulationDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
	op_pMatrix.initialize(m_pStreamedMatrixDecoder->getOutputParameter(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputParameterId_Matrix));
	ip_pMatrix.initialize(m_pStreamedMatrixEncoder->getInputParameter(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_InputParameterId_Matrix));

	m_ui64LatestStimulationChunkEndTime=0;
	return true;
}

boolean CBoxAlgorithmMatlabFilter::uninitialize(void)
{
	ip_pMatrix.uninitialize();
	op_pMatrix.uninitialize();
	op_pStimulationSet.uninitialize();

	m_pStreamedMatrixEncoder->uninitialize();
	m_pStreamedMatrixDecoder->uninitialize();
	m_pStimulationDecoder->uninitialize();

	this->getAlgorithmManager().releaseAlgorithm(*m_pStreamedMatrixEncoder);
	this->getAlgorithmManager().releaseAlgorithm(*m_pStreamedMatrixDecoder);
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationDecoder);

	if(m_pMatlabMatrix)
	{
		::mxDestroyArray(m_pMatlabMatrix);
	}

	::mxDestroyArray(m_pMatlabBCIContext);

	if(m_pMatlabEngine)
	{
		if(::engClose(m_pMatlabEngine)!=0)
		{
			this->getLogManager() << LogLevel_ImportantWarning << "Could not close Matlab engine\n";
		}
	}

	return true;
}

boolean CBoxAlgorithmMatlabFilter::processInput(uint32 ui32InputIndex)
{
	if(!m_pMatlabEngine)
	{
		return true;
	}

	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}

boolean CBoxAlgorithmMatlabFilter::process(void)
{
	// IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();
	uint32 i,j;

	TParameterHandler < IMemoryBuffer* > op_pMemoryBuffer(m_pStreamedMatrixEncoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_OutputParameterId_EncodedMemoryBuffer));

	for(i=0; i<l_rDynamicBoxContext.getInputChunkCount(0); i++)
	{
		TParameterHandler < const IMemoryBuffer* > ip_pMemoryBuffer(m_pStimulationDecoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
		ip_pMemoryBuffer=l_rDynamicBoxContext.getInputChunk(0, i);
		m_pStimulationDecoder->process();
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedHeader))
		{
		}
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
		{
			for(uint32 j=0; j<op_pStimulationSet->getStimulationCount(); j++)
			{
				m_oPendingStimulationSet.appendStimulation(
					op_pStimulationSet->getStimulationIdentifier(j),
					op_pStimulationSet->getStimulationDate(j),
					op_pStimulationSet->getStimulationDuration(j));
			}
		}
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedEnd))
		{
		}
		m_ui64LatestStimulationChunkEndTime=l_rDynamicBoxContext.getInputChunkEndTime(0, i);
		l_rDynamicBoxContext.markInputAsDeprecated(0, i);
	}

	for(i=0; i<l_rDynamicBoxContext.getInputChunkCount(1); i++)
	{
		uint64 l_ui64StartTime=l_rDynamicBoxContext.getInputChunkStartTime(1, i);
		uint64 l_ui64EndTime=l_rDynamicBoxContext.getInputChunkEndTime(1, i);

		((float64*)::mxGetPr(m_pMatlabBCIContext))[0]=(l_ui64StartTime >> 16) / 65536.0;
		((float64*)::mxGetPr(m_pMatlabBCIContext))[1]=(l_ui64EndTime >> 16) / 65536.0;

		if(l_ui64EndTime<=m_ui64LatestStimulationChunkEndTime)
		{
			TParameterHandler < const IMemoryBuffer* > ip_pMemoryBuffer(m_pStreamedMatrixDecoder->getInputParameter(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_InputParameterId_MemoryBufferToDecode));
			ip_pMemoryBuffer=l_rDynamicBoxContext.getInputChunk(1, i);
			m_pStreamedMatrixDecoder->process();
			if(m_pStreamedMatrixDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputTriggerId_ReceivedHeader))
			{
				if(op_pMatrix->getDimensionCount()!=2)
				{
					this->getLogManager() << LogLevel_ImportantWarning << "Streamed matrix dimension count is not 2\n";
				}
				else
				{
					m_pMatlabMatrixHandle=::mxCreateDoubleMatrix(
						op_pMatrix->getDimensionSize(1),
						op_pMatrix->getDimensionSize(0),
						mxREAL);
					if(!m_pMatlabMatrix)
					{
						this->getLogManager() << LogLevel_ImportantWarning << "Could not create Matlab matrix\n";
					}

					::engPutVariable(m_pMatlabEngine, "bci_context", m_pMatlabBCIContext);
					::engPutVariable(m_pMatlabEngine, "matrix", m_pMatlabMatrix);
					::engEvalString(m_pMatlabEngine, "result = bci_Initialize(bci_context, matrix)");

					::mxArray* l_pMatlabResult=::engGetVariable(m_pMatlabEngine, "result");
					if(l_pMatlabResult)
					{
						if(!::mxIsDouble(l_pMatlabResult))
						{
							this->getLogManager() << LogLevel_Warning << "Expected double matrix output from " << CString("bci_Initialize") << "\n";
						}
						else
						{
							ip_pMatrix->setDimensionCount(2);
							ip_pMatrix->setDimensionSize(0, ::mxGetDimensions(l_pMatlabResult)[1]);
							ip_pMatrix->setDimensionSize(1, ::mxGetDimensions(l_pMatlabResult)[0]);

							op_pMemoryBuffer=l_rDynamicBoxContext.getOutputChunk(0);
							m_pStreamedMatrixEncoder->process(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_InputTriggerId_EncodeHeader);
							l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_rDynamicBoxContext.getInputChunkStartTime(1, i), l_rDynamicBoxContext.getInputChunkEndTime(1, i));
						}
						::mxDestroyArray(l_pMatlabResult);
					}
				}
			}
			if(m_pStreamedMatrixDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputTriggerId_ReceivedBuffer))
			{
				CStimulationSet l_oStimulationSet;
				for(j=0; j<m_oPendingStimulationSet.getStimulationCount(); )
				{
					if(m_oPendingStimulationSet.getStimulationDate(j)<l_ui64StartTime)
					{
						m_oPendingStimulationSet.removeStimulation(j);
					}
					else
					{
						if(m_oPendingStimulationSet.getStimulationDate(j)<l_ui64EndTime)
						{
							l_oStimulationSet.appendStimulation(
								m_oPendingStimulationSet.getStimulationIdentifier(j),
								m_oPendingStimulationSet.getStimulationDate(j),
								m_oPendingStimulationSet.getStimulationDuration(j));
						}
						j++;
					}
				}

				m_pMatlabStimulationHandle=::mxCreateDoubleMatrix(l_oStimulationSet.getStimulationCount(), 3, mxREAL);
				float64* l_pMatlabStimulationBuffer=(float64*)::mxGetPr(m_pMatlabStimulation);
				for(j=0; j<l_oStimulationSet.getStimulationCount(); j++)
				{
					l_pMatlabStimulationBuffer[j*3+0]=l_oStimulationSet.getStimulationIdentifier(j);
					l_pMatlabStimulationBuffer[j*3+1]=((l_oStimulationSet.getStimulationDate(j)-l_ui64StartTime) >> 16) / 65536.0;
					l_pMatlabStimulationBuffer[j*3+2]=( l_oStimulationSet.getStimulationDuration(j) >> 16) / 65536.0;
				}

				System::Memory::copy(::mxGetPr(m_pMatlabMatrix), op_pMatrix->getBuffer(), op_pMatrix->getBufferElementCount()*sizeof(float64));

				::engPutVariable(m_pMatlabEngine, "bci_context", m_pMatlabBCIContext);
				::engPutVariable(m_pMatlabEngine, "stimulation_set", m_pMatlabStimulation);
				::engPutVariable(m_pMatlabEngine, "matrix", m_pMatlabMatrix);
				::engEvalString(m_pMatlabEngine, "result = bci_Process(bci_context, stimulation_set, matrix)");
				::mxDestroyArray(m_pMatlabStimulation);

					::mxArray* l_pMatlabResult=::engGetVariable(m_pMatlabEngine, "result");
					if(l_pMatlabResult)
					{
						if(!::mxIsDouble(l_pMatlabResult))
						{
							this->getLogManager() << LogLevel_Warning << "Expected double matrix output from " << CString("bci_Process") << "\n";
						}
						else
						{
/*
							ip_pMatrix->setDimensionCount(2);
							ip_pMatrix->setDimensionSize(0, ::mxGetDimensions(l_pMatlabResult, 0));
							ip_pMatrix->setDimensionSize(1, ::mxGetDimensions(l_pMatlabResult, 1));
*/
							System::Memory::copy(ip_pMatrix->getBuffer(), ::mxGetPr(l_pMatlabResult), ip_pMatrix->getBufferElementCount()*sizeof(float64));

							op_pMemoryBuffer=l_rDynamicBoxContext.getOutputChunk(0);
							m_pStreamedMatrixEncoder->process(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_InputTriggerId_EncodeBuffer);
							l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_rDynamicBoxContext.getInputChunkStartTime(1, i), l_rDynamicBoxContext.getInputChunkEndTime(1, i));
						}
						::mxDestroyArray(l_pMatlabResult);
					}
			}
			if(m_pStreamedMatrixDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputTriggerId_ReceivedEnd))
			{
				m_pStreamedMatrixEncoder->process(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_InputTriggerId_EncodeEnd);
			}
			l_rDynamicBoxContext.markInputAsDeprecated(1, i);
		}
	}

	return true;
}

#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines
