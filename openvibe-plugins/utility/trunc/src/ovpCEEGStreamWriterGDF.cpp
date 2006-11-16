#include "ovpCEEGStreamWriterGDF.h"

#include <system/Memory.h>

#include <iostream>
#include <vector>
#include <stack>

#include <stdio.h>
#include <errno.h>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Utility;

using namespace std;

namespace OpenViBEPlugins
{
	namespace Utility
	{
		namespace GDF
		{
			class CFixedGDFHeader
			{
			public:
				CFixedGDFHeader(void) { memset(this, 0x20, sizeof(*this)); }
				char m_sVersionId[8];
				char m_sPatientId[80];
				char m_sRecordingId[80];
				char m_sStartDateAndTimeOfRecording[16];
				int64 m_i64NumberOfBytesInHeaderRecord;
				uint64 m_ui64EquipmentProviderId;
				uint64 m_ui64LaboratoryId;
				uint64 m_ui64TechnicianId;
				char m_sReservedSerialNumber[20];
				int64 m_i64NumberOfDataRecords;
				uint32 m_ui32DurationOfADataRecordNumerator;
				uint32 m_ui32DurationOfADataRecordDenominator;
				uint32 m_ui32NumberOfSignals;
			};

			class CVariableGDFHeaderPerChannel
			{
			public:
				CVariableGDFHeaderPerChannel(void)
				{
					memset(this, 0x20, sizeof(*this));
				}
				char m_sLabel[16];
				char m_sTranducerType[80];
				char m_sPhysicalDimension[8];
				float64 m_f64PhysicalMinimum;
				float64 m_f64PhysicalMaximum;
				int64 m_i64DigitalMinimum;
				int64 m_i64DigitalMaximum;
				char m_sPreFiltering[80];
				uint32 m_ui32NumberOfSamplesInEachRecord;
				uint32 m_ui32ChannelType;
				char m_sReserved[32];
			};

			class CGDFEventTable
			{
			public:
				CGDFEventTable(void) { memset(this, 0x20, sizeof(*this)); }
				uint8 m_ui8EventTableMode;
				uint8 m_ui8SampleRate1;
				uint8 m_ui8SampleRate2;
				uint8 m_ui8SampleRate3;
				uint32 m_ui32NumberOfEvents;
			};

			class CGDFEvent
			{
			public:
				CGDFEvent(void) { memset(this, 0x20, sizeof(*this)); }
				uint32 m_ui32Position;
				uint16 m_ui16Type;
				uint16 m_ui16Channel; // only if mode 3
				uint32 m_ui32Duration; // only if mode 3
			};
		};

		class CEEGStreamWriterGDF_ReaderCallBack : virtual public EBML::IReaderCallBack
		{
		public:

			CEEGStreamWriterGDF_ReaderCallBack(CEEGStreamWriterGDF& rPlugin)
				:m_rPlugin(rPlugin)
				,m_pReaderHelper(NULL)
				,m_pFile(NULL)
				,m_bFirstTime(true)
			{
				m_pReaderHelper=EBML::createReaderHelper();
			}

			virtual ~CEEGStreamWriterGDF_ReaderCallBack()
			{
				fclose(m_pFile);
				m_pReaderHelper->release();
			}

			virtual void save(boolean bHeader)
			{
				if(!m_pFile)
				{
					if(!(m_pFile=fopen(m_rPlugin.m_sFileName, "wb")))
					{
						return;
					}
				}

				fseek(m_pFile, 0, SEEK_SET);
				strcpy(m_oFixedHeader.m_sVersionId, "GDF 0.12");
				strcpy(m_oFixedHeader.m_sStartDateAndTimeOfRecording, "2006101100000000");
				memset(m_oFixedHeader.m_sPatientId, ' ', sizeof(m_oFixedHeader.m_sPatientId));
				memset(m_oFixedHeader.m_sRecordingId, ' ', sizeof(m_oFixedHeader.m_sRecordingId));
				m_oFixedHeader.m_i64NumberOfBytesInHeaderRecord=(m_oFixedHeader.m_ui32NumberOfSignals+1)*256;
				m_oFixedHeader.m_i64NumberOfDataRecords=m_vSampleCount[0];
				m_oFixedHeader.m_ui32DurationOfADataRecordNumerator=1;
				m_oFixedHeader.m_ui32DurationOfADataRecordDenominator=(uint32)m_ui64SamplingFrequency;
				fwrite(&m_oFixedHeader, sizeof(m_oFixedHeader), 1, m_pFile);

				if(bHeader)
				{
					vector< GDF::CVariableGDFHeaderPerChannel >::iterator i;
					#define __dump1(f,i,m) { fwrite(i->m, sizeof(i->m), 1, f); }
					#define __dump2(f,i,m) { fwrite(&i->m, sizeof(i->m), 1, f); }
					for(i=m_vVariableHeader.begin(); i!=m_vVariableHeader.end(); i++) __dump1(m_pFile, i, m_sLabel);
					for(i=m_vVariableHeader.begin(); i!=m_vVariableHeader.end(); i++) __dump1(m_pFile, i, m_sTranducerType);
					for(i=m_vVariableHeader.begin(); i!=m_vVariableHeader.end(); i++) __dump1(m_pFile, i, m_sPhysicalDimension);
					for(i=m_vVariableHeader.begin(); i!=m_vVariableHeader.end(); i++) __dump2(m_pFile, i, m_f64PhysicalMinimum);
					for(i=m_vVariableHeader.begin(); i!=m_vVariableHeader.end(); i++) __dump2(m_pFile, i, m_f64PhysicalMaximum);
					for(i=m_vVariableHeader.begin(); i!=m_vVariableHeader.end(); i++) __dump2(m_pFile, i, m_i64DigitalMinimum);
					for(i=m_vVariableHeader.begin(); i!=m_vVariableHeader.end(); i++) __dump2(m_pFile, i, m_i64DigitalMaximum);
					for(i=m_vVariableHeader.begin(); i!=m_vVariableHeader.end(); i++) __dump1(m_pFile, i, m_sPreFiltering);
					for(i=m_vVariableHeader.begin(); i!=m_vVariableHeader.end(); i++) __dump2(m_pFile, i, m_ui32NumberOfSamplesInEachRecord);
					for(i=m_vVariableHeader.begin(); i!=m_vVariableHeader.end(); i++) __dump2(m_pFile, i, m_ui32ChannelType);
					for(i=m_vVariableHeader.begin(); i!=m_vVariableHeader.end(); i++) __dump1(m_pFile, i, m_sReserved);
					#undef __dump2
					#undef __dump1
				}

				if(!bHeader)
				{
					size_t i,j;
					fseek(m_pFile, 0, SEEK_END);
					for(i=0; i<m_vSamples[0].size(); i++)
					{
						for(j=0; j<m_vSamples.size(); j++)
						{
							uint16 v=m_vSamples[j][i];
							fwrite(&v, sizeof(v), 1, m_pFile);
						}
					}
					for(j=0; j<m_vSamples.size(); j++)
					{
						m_vSamples[j].clear();
					}
				}
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
				if(rIdentifier==EEG_NodeId_ChannelNames)           { m_ui32ChannelIndex=0; }
				if(rIdentifier==EEG_NodeId_GainFactors)            { m_ui32ChannelIndex=0; }
				if(rIdentifier==EEG_NodeId_ChannelLocations)       { m_ui32ChannelIndex=0; }
				if(rIdentifier==EEG_NodeId_Samples)                { m_ui32ChannelIndex=0; }
				m_vCurrentIdentifier.push(rIdentifier);
			}

			virtual void processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize)
			{
				EBML::CIdentifier l_oIdentifier=m_vCurrentIdentifier.top();

				if(l_oIdentifier==EEG_NodeId_ExperimentId)
				{
					EBML::uint64 l_ui64Id=m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize);
					EBML::uint64 l_ui64Id1=l_ui64Id>>32;
					EBML::uint64 l_ui64Id2=l_ui64Id&0xffffffff;
					sprintf(m_oFixedHeader.m_sRecordingId, "0x%08X, 0x%08x", (int)l_ui64Id1, (int) l_ui64Id2);
				}
				else if(l_oIdentifier==EEG_NodeId_SubjectAge) { /* nothing to do */ }
				else if(l_oIdentifier==EEG_NodeId_SubjectSex) { /* nothing to do */ }
				else if(l_oIdentifier==EEG_NodeId_ChannelCount)
				{
					m_oFixedHeader.m_ui32NumberOfSignals=(uint32)m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize);
					m_vVariableHeader.resize(m_oFixedHeader.m_ui32NumberOfSignals);
					m_vSamples.resize(m_oFixedHeader.m_ui32NumberOfSignals);
					m_vSampleCount.resize(m_oFixedHeader.m_ui32NumberOfSignals);
				}
				else if(l_oIdentifier==EEG_NodeId_SamplingFrequency)
				{
					m_ui64SamplingFrequency=m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize);
				}
				else if(l_oIdentifier==EEG_NodeId_ChannelName)
				{
					sprintf(m_vVariableHeader[m_ui32ChannelIndex].m_sLabel, m_pReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize));
					m_vVariableHeader[m_ui32ChannelIndex].m_ui32ChannelType=3; // int16
					m_vVariableHeader[m_ui32ChannelIndex].m_ui32NumberOfSamplesInEachRecord=1;
					m_vVariableHeader[m_ui32ChannelIndex].m_f64PhysicalMinimum=0;
					m_vVariableHeader[m_ui32ChannelIndex].m_f64PhysicalMaximum=1;
					m_vVariableHeader[m_ui32ChannelIndex].m_i64DigitalMinimum=-0x7fff;
					m_vVariableHeader[m_ui32ChannelIndex].m_i64DigitalMaximum=0x7fff;
					memcpy(m_vVariableHeader[m_ui32ChannelIndex].m_sPhysicalDimension, "uV", sizeof("uV"));

					m_ui32ChannelIndex++;
				}
				else if(l_oIdentifier==EEG_NodeId_GainFactor) { /* nothing to do */ }
				else if(l_oIdentifier==EEG_NodeId_ChannelLocation) { /* nothing to do */ }
				else if(l_oIdentifier==EEG_NodeId_SamplesPerChannelCount) { /* nothing to do */ }
				else if(l_oIdentifier==EEG_NodeId_SampleBlock)
				{
					const uint8* l_pBuffer=static_cast<const uint8*>(pBuffer);
					float32 l_f32Sample=0;
					uint32 l_ui32SampleCount=(uint32)(ui64BufferSize>>2);
					for(uint32 i=0; i<l_ui32SampleCount; i++)
					{
						System::Memory::littleEndianToHost(l_pBuffer+(i<<2), &l_f32Sample);
						m_vSamples[m_ui32ChannelIndex].push_back((int16)(l_f32Sample*5000));
					}
					m_vSampleCount[m_ui32ChannelIndex]+=l_ui32SampleCount;
					m_ui32ChannelIndex++;
				}
			}

			virtual void closeChild(void)
			{
				EBML::CIdentifier l_oIdentifier=m_vCurrentIdentifier.top();
				if(l_oIdentifier==EEG_NodeId_Header) save(true);
				if(l_oIdentifier==EEG_NodeId_Buffer) save(false);
				m_vCurrentIdentifier.pop();
			}

		public:

			CEEGStreamWriterGDF& m_rPlugin;
			stack< EBML::CIdentifier > m_vCurrentIdentifier;
			EBML::IReaderHelper* m_pReaderHelper;
			FILE* m_pFile;
			boolean m_bFirstTime;

			GDF::CFixedGDFHeader m_oFixedHeader;
			vector< GDF::CVariableGDFHeaderPerChannel > m_vVariableHeader;
			vector< vector< int16 > > m_vSamples;
			vector< int64 > m_vSampleCount;
			GDF::CGDFEventTable m_oEventTable;
			vector< GDF::CGDFEvent > m_vEvent;

			uint32 m_ui32ChannelIndex;
			uint64 m_ui64SamplingFrequency;
		};
	};
};

boolean CEEGStreamWriterGDFDesc::getBoxPrototype(
	IBoxProto& rPrototype) const
{
	// Adds box inputs
	rPrototype.addInput("Incoming stream", OV_UndefinedIdentifier);

	// Adds box outputs

	// Adds box settings
	rPrototype.addSetting("Filename", OV_TypeId_String, "eeg_stream_writer.gdf");

	return true;
}

uint32 CEEGStreamWriterGDFDesc::getClockFrequency(IStaticBoxContext& rStaticBoxContext) const
{
	return 0;
}

CEEGStreamWriterGDF::CEEGStreamWriterGDF(void)
	:m_pReaderCallBack(NULL)
	,m_pReader(NULL)
{
}

boolean CEEGStreamWriterGDF::initialize(
	const IBoxAlgorithmContext& rBoxAlgorithmContext)
{
	const IStaticBoxContext* l_pBoxContext=rBoxAlgorithmContext.getStaticBoxContext();

	// Prepares EBML reader
	m_pReaderCallBack=new CEEGStreamWriterGDF_ReaderCallBack(*this);
	m_pReader=EBML::createReader(*m_pReaderCallBack);

	// Parses box settings to find filename
	l_pBoxContext->getSettingValue(0, m_sFileName);

	return true;
}

boolean CEEGStreamWriterGDF::uninitialize(
	const IBoxAlgorithmContext& rBoxAlgorithmContext)
{
	const IStaticBoxContext* l_pBoxContext=rBoxAlgorithmContext.getStaticBoxContext();

	// Cleans up EBML reader
	m_pReader->release();
	m_pReader=NULL;
	delete m_pReaderCallBack;
	m_pReaderCallBack=NULL;

	return true;
}

boolean CEEGStreamWriterGDF::processInput(
	IBoxAlgorithmContext& rBoxAlgorithmContext,
	uint32 ui32InputIndex)
{
	// Only one input, should be the good one :)
	rBoxAlgorithmContext.markAlgorithmAsReadyToProcess();

	return true;
}

boolean CEEGStreamWriterGDF::process(
	IBoxAlgorithmContext& rBoxAlgorithmContext)
{
	IDynamicBoxContext* l_pDynamicBoxContext=rBoxAlgorithmContext.getDynamicBoxContext();
	for(uint32 i=0; i<l_pDynamicBoxContext->getInputChunkCount(0); i++)
	{
		uint64 l_ui64StartTime;
		uint64 l_ui64EndTime;
		uint64 l_ui64ChunkSize;
		const uint8* l_pChunkBuffer=NULL;
		if(l_pDynamicBoxContext->getInputChunk(0, i, l_ui64StartTime, l_ui64EndTime, l_ui64ChunkSize, l_pChunkBuffer))
		{
			m_pReader->processData(l_pChunkBuffer, l_ui64ChunkSize);
		}
		l_pDynamicBoxContext->markInputAsDeprecated(0, i);
	}

	return true;
}
