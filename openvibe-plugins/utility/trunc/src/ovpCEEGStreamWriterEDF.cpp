#include "ovpCEEGStreamWriterEDF.h"

#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Utility;

using namespace std;

namespace OpenViBEPlugins
{
	namespace Utility
	{
		class CEEGStreamWriterEDF_ReaderCallBack : virtual public EBML::IReaderCallBack
		{
		public:

			CEEGStreamWriterEDF_ReaderCallBack(CEEGStreamWriterEDF& rPlugin)
				:m_rPlugin(rPlugin)
				,m_pReaderHelper(NULL)
				,m_bFirstTime(true)
			{
				m_pReaderHelper=EBML::createReaderHelper();
			}

			virtual ~CEEGStreamWriterEDF_ReaderCallBack()
			{
				m_pReaderHelper->release();
			}

			virtual EBML::boolean isMasterChild(const EBML::CIdentifier& rIdentifier)
			{
				if(rIdentifier==EEG_NodeId_Header)                 return true;
				if(rIdentifier==EEG_NodeId_AcquisitionInformation) return true;
				if(rIdentifier==EEG_NodeId_ChannelNames)           return true;
				if(rIdentifier==EEG_NodeId_GainFactors)            return true;
				if(rIdentifier==EEG_NodeId_ChannelLocations)       return true;
				if(rIdentifier==EEG_NodeId_Buffer)                 return true;
				if(rIdentifier==EEG_NodeId_Samples)                return true;
				if(rIdentifier==EEG_NodeId_Stimulations)           return true;
				if(rIdentifier==EEG_NodeId_Stimulation)            return true;
				return false;
			}

			virtual void openChild(const EBML::CIdentifier& rIdentifier)
			{
				m_oCurrentIdentifier=rIdentifier;
			}

			virtual void processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize)
			{
				FILE* l_pFile=fopen(m_rPlugin.m_sFileName, m_bFirstTime?"wt":"at");
				m_bFirstTime=false;

				if(!l_pFile)
				{
					return;
				}

				if(m_oCurrentIdentifier==EEG_NodeId_Header)
				{
					// TODO
				}
				else if(m_oCurrentIdentifier==EEG_NodeId_AcquisitionInformation)
				{
					// TODO
				}
				else if(m_oCurrentIdentifier==EEG_NodeId_ExperimentId)
				{
					fprintf(l_pFile, "ExperimentID=0x%08x\n", m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize));
				}
				else if(m_oCurrentIdentifier==EEG_NodeId_SubjectAge)
				{
					fprintf(l_pFile, "SubjectAge=%i\n", m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize));
				}
				else if(m_oCurrentIdentifier==EEG_NodeId_SubjectSex)
				{
					fprintf(l_pFile, "SubjectSex=%s\n", m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize)==0?"Male":"Female");
				}
				else if(m_oCurrentIdentifier==EEG_NodeId_ChannelCount)
				{
					fprintf(l_pFile, "ChannelCount=%i\n", m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize));
				}
				else if(m_oCurrentIdentifier==EEG_NodeId_SamplingFrequency)
				{
					fprintf(l_pFile, "SamplingFrequency=%i\n", m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize));
				}
				else if(m_oCurrentIdentifier==EEG_NodeId_ChannelName)
				{
					fprintf(l_pFile, "ChannelName=%s\n", m_pReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize));
				}
				else if(m_oCurrentIdentifier==EEG_NodeId_GainFactor)
				{
					fprintf(l_pFile, "ChannelGainFactor=%f\n", m_pReaderHelper->getFloatFromChildData(pBuffer, ui64BufferSize));
				}
				else if(m_oCurrentIdentifier==EEG_NodeId_ChannelLocation)
				{
					// TODO
				}
				else if(m_oCurrentIdentifier==EEG_NodeId_Buffer)
				{
					// TODO
				}
				else if(m_oCurrentIdentifier==EEG_NodeId_Samples)
				{
					// TODO
				}
				else if(m_oCurrentIdentifier==EEG_NodeId_SampleBlock)
				{
					fprintf(l_pFile, "SampleBlock=");
					for(int i=0; i<ui64BufferSize>>2; i++)
					{
						fprintf(l_pFile, "%f ", ((float32*)pBuffer)[i]);
					}
					fprintf(l_pFile, "\n");
				}
				else if(m_oCurrentIdentifier==EEG_NodeId_SamplesPerChannelCount)
				{
					fprintf(l_pFile, "SamplesPerChannel=%i\n", m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize));
				}
				else if(m_oCurrentIdentifier==EEG_NodeId_SampleBlock)
				{
					// TODO
				}
				else if(m_oCurrentIdentifier==EEG_NodeId_StimulationsCount)
				{
					// TODO
				}
				else if(m_oCurrentIdentifier==EEG_NodeId_Stimulation)
				{
					// TODO
				}
				else if(m_oCurrentIdentifier==EEG_NodeId_StimulationSampleIndex)
				{
					// TODO
				}
				else if(m_oCurrentIdentifier==EEG_NodeId_StimulationIdentifier)
				{
					// TODO
				}
				else
				{
				}

				fclose(l_pFile);
			}

			virtual void closeChild(void)
			{
			}

		public:

			CEEGStreamWriterEDF& m_rPlugin;
			EBML::CIdentifier m_oCurrentIdentifier;
			EBML::IReaderHelper* m_pReaderHelper;
			boolean m_bFirstTime;
		};
	};
};

boolean CEEGStreamWriterEDFDesc::getBoxPrototype(
	IBoxProto& rPrototype) const
{
	// Adds box inputs
	rPrototype.addInput("Incoming stream", OV_UndefinedIdentifier);

	// Adds box outputs

	// Adds box settings
	rPrototype.addSetting("Filename", OV_TypeId_String, "dumped_stream_edf.txt");

	return true;
}

uint32 CEEGStreamWriterEDFDesc::getClockFrequency(void) const
{
	return 0;
}

CEEGStreamWriterEDF::CEEGStreamWriterEDF(void)
	:m_pReaderCallBack(NULL)
	,m_pReader(NULL)
	,m_pInput(NULL)
{
}

boolean CEEGStreamWriterEDF::initialize(
	const IPluginObjectContext& rContext)
{
	const IBoxContext& rBoxContext=dynamic_cast<const IBoxContext&>(rContext);

	// Prepares EBML reader
	m_pReaderCallBack=new CEEGStreamWriterEDF_ReaderCallBack(*this);
	m_pReader=EBML::createReader(*m_pReaderCallBack);

	// Parses box settings to find filename
	rBoxContext.getSettingValue(0, m_sFileName);

	return true;
}

boolean CEEGStreamWriterEDF::uninitialize(
	const IPluginObjectContext& rContext)
{
	const IBoxContext& rBoxContext=dynamic_cast<const IBoxContext&>(rContext);

	// Cleans up EBML reader
	m_pReader->release();
	m_pReader=NULL;
	delete m_pReaderCallBack;
	m_pReaderCallBack=NULL;

	return true;
}

boolean CEEGStreamWriterEDF::processInput(
	IBoxAlgorithmContext& rBoxAlgorithmContext,
	uint32 ui32InputIndex)
{
	// Only one input, should be the good one :)
	rBoxAlgorithmContext.markAlgorithmAsReadyToProcess();

	return true;
}

boolean CEEGStreamWriterEDF::process(
	IBoxAlgorithmContext& rBoxAlgorithmContext)
{
	m_pInput=rBoxAlgorithmContext.getBoxContext().getInput(0);

	for(uint32 i=0; i<m_pInput->getChunkCount(); i++)
	{
		uint64 l_ui64StartTime;
		uint64 l_ui64EndTime;
		uint64 l_ui64ChunkSize;
		const uint8* l_pChunkBuffer=NULL;
		if(m_pInput->getChunk(i, l_ui64StartTime, l_ui64EndTime, l_ui64ChunkSize, l_pChunkBuffer))
		{
			m_pReader->processData(l_pChunkBuffer, l_ui64ChunkSize);
		}
		m_pInput->markAsDeprecated(i);
	}

	m_pInput=NULL;

	return true;
}
