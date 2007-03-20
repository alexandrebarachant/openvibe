#include "GDFHelpers.h"

#include <math.h>
		
#include <iostream>
using namespace std;

#define _NoValueI_ 0xffffffff
#define _NoValueS_ "_unspecified_"

namespace OpenViBEPlugins
{
	namespace Acquisition
	{
		
		//CFIXEDHEADER methods
		OpenViBE::uint64 GDF::CFixedGDFHeader::getSubjectIdentifier()
		{
			return _NoValueI_;
		}
		
		std::string GDF::CFixedGDFHeader::getSubjectName()
		{
			return _NoValueS_;
		}
		
		OpenViBE::uint64 GDF::CFixedGDFHeader::getSubjectSex()
		{
			return _NoValueI_;
		}
		
		OpenViBE::uint64 GDF::CFixedGDFHeader::getSubjectAge()
		{
			return _NoValueI_;
		}
		
		OpenViBE::uint64 GDF::CFixedGDFHeader::getExperimentIdentifier()
		{
			return _NoValueI_;
		}
		
		std::string GDF::CFixedGDFHeader::getExperimentDate()
		{
			return _NoValueS_;
		}
		
		OpenViBE::uint64 GDF::CFixedGDFHeader::getLaboratoryIdentifier()
		{
			return _NoValueI_;
		}
		
		OpenViBE::uint64 GDF::CFixedGDFHeader::getTechnicianIdentifier()
		{
			return _NoValueI_;
		}
		
		std::string GDF::CFixedGDFHeader::getLaboratoryName()
		{
			return _NoValueS_;
		}
		
		std::string GDF::CFixedGDFHeader::getTechnicianName()
		{
			return _NoValueS_;
		}

		
		OpenViBE::boolean GDF::CFixedGDF1Header::read(std::ifstream& oFile)
		{
			OpenViBE::uint8 l_pHeaderBuffer[72];
			
			oFile.read(m_sPatientId, 80);
			oFile.read(m_sRecordingId, 80);
			oFile.read(m_sStartDateAndTimeOfRecording, 16);
			
			oFile.read(reinterpret_cast<char*>(l_pHeaderBuffer), 72);
			
			System::Memory::littleEndianToHost(l_pHeaderBuffer,reinterpret_cast<OpenViBE::uint64*>(&m_i64NumberOfBytesInHeaderRecord));
			System::Memory::littleEndianToHost(l_pHeaderBuffer+8,&m_ui64EquipmentProviderId);
			System::Memory::littleEndianToHost(l_pHeaderBuffer+16,&m_ui64LaboratoryId);
			System::Memory::littleEndianToHost(l_pHeaderBuffer+24,&m_ui64TechnicianId);
			
			System::Memory::littleEndianToHost(l_pHeaderBuffer+52,reinterpret_cast<OpenViBE::uint64*>( &m_i64NumberOfDataRecords));
			System::Memory::littleEndianToHost(l_pHeaderBuffer+60,&m_ui32DurationOfADataRecordNumerator);
			System::Memory::littleEndianToHost(l_pHeaderBuffer+64,&m_ui32DurationOfADataRecordDenominator);
			System::Memory::littleEndianToHost(l_pHeaderBuffer+68,&m_ui32NumberOfSignals);
			
			if(oFile.bad())
			{
				return false;
			}
			return true;
		}

		
		
		std::string GDF::CFixedGDF1Header::getSubjectName()
		{
			// extracts the PID and Patient name from the m_sPatientId
			char * l_pToken = strtok(m_sPatientId, " ");
			if(l_pToken)
			{
				//The PId is not a numerical value in GDF, it is useless for us
				l_pToken = strtok(NULL, " ");
			}
			
			if(l_pToken)
			{
				return l_pToken;
			}
			else
			{
				return _NoValueS_;
			}
		}
		
		OpenViBE::uint64 GDF::CFixedGDF1Header::getLaboratoryIdentifier()
		{
			OpenViBE::uint8 * l_pTemp = reinterpret_cast<OpenViBE::uint8*>(&m_ui64LaboratoryId);
			OpenViBE::boolean l_bBlank = true;
			for(int i=0 ; i<8 && l_bBlank; i++)
			{
				if(l_pTemp[i] != 0x20)
				{
					l_bBlank = false;
				}
			}
			
			if(l_bBlank)
			{
				return _NoValueI_;
			}
			else
			{
				return m_ui64LaboratoryId;
			}
			
		}
		
		OpenViBE::uint64 GDF::CFixedGDF1Header::getTechnicianIdentifier()
		{
			OpenViBE::uint8 * l_pTemp = reinterpret_cast<OpenViBE::uint8*>(&m_ui64TechnicianId);
			OpenViBE::boolean l_bBlank = true;
			for(int i=0 ; i<8 && l_bBlank; i++)
			{
				if(l_pTemp[i] != 0x20)
				{
					l_bBlank = false;
				}
			}
			
			if(l_bBlank)
			{
				return _NoValueI_;
			}
			else
			{
				return m_ui64TechnicianId;
			}
		}
		
		OpenViBE::float64 GDF::CFixedGDF1Header::getDataRecordDuration()
		{
			return static_cast<OpenViBE::float64>(m_ui32DurationOfADataRecordNumerator) / static_cast<OpenViBE::float64>(m_ui32DurationOfADataRecordDenominator);
		}
		
		OpenViBE::uint64 GDF::CFixedGDF1Header::getNumberOfDataRecords()
		{
			return m_i64NumberOfDataRecords;
		}
		
		OpenViBE::uint64 GDF::CFixedGDF1Header::getChannelCount()
		{
			return m_ui32NumberOfSignals;
		}
		
		
		
		
		
		OpenViBE::boolean GDF::CFixedGDF2Header::read(std::ifstream& oFile)
		{
			OpenViBE::uint8 l_pHeaderBuffer[104];
					
			oFile.read(m_sPatientId, 66);
					
			oFile.read(reinterpret_cast<char*>(l_pHeaderBuffer), 14);
			m_ui8HealthInformation = l_pHeaderBuffer[10];
			m_ui8Weight = l_pHeaderBuffer[11];
			m_ui8Height = l_pHeaderBuffer[12];
			m_ui8SubjectInformation = l_pHeaderBuffer[13];
					
			oFile.read(m_sRecordingId, 64);
					
			oFile.read(reinterpret_cast<char*>(l_pHeaderBuffer), 102);
					
			for(int i=0 ; i<4 ; i++)
			{
				System::Memory::littleEndianToHost(l_pHeaderBuffer+i*sizeof(OpenViBE::uint32), &m_ui32RecordingLocation[i]);
			}
					
			System::Memory::littleEndianToHost(l_pHeaderBuffer+16, &m_ui32StartDateAndTimeOfRecording[0]);
			System::Memory::littleEndianToHost(l_pHeaderBuffer+20, &m_ui32StartDateAndTimeOfRecording[1]);
					
			System::Memory::littleEndianToHost(l_pHeaderBuffer+24, &m_ui32Birthday[0]);
			System::Memory::littleEndianToHost(l_pHeaderBuffer+28, &m_ui32Birthday[1]);
					
			System::Memory::littleEndianToHost(l_pHeaderBuffer+32, &m_ui16NumberOfBlocksInHeader);
					
			System::Memory::littleEndianToHost(l_pHeaderBuffer+40, &m_ui64EquipmentProviderId);
					
			for(int i=0 ; i<6 ; i++)
			{
				m_ui8IPAdress[i] = l_pHeaderBuffer[48+i];
			}
					
			for(int i=0 ; i<3 ; i++)
			{
				System::Memory::littleEndianToHost(l_pHeaderBuffer+54+i*sizeof(OpenViBE::uint16), &m_ui16HeadSize[i]);
						
				System::Memory::littleEndianToHost(l_pHeaderBuffer+60+i*sizeof(OpenViBE::float32), &m_f32PositionReferenceElectrode[i]);
						
				System::Memory::littleEndianToHost(l_pHeaderBuffer+72+i*sizeof(OpenViBE::float32), &m_f32GroundElectrode[i]);
			}
					
			System::Memory::littleEndianToHost(l_pHeaderBuffer+84, reinterpret_cast<OpenViBE::uint64*>(&m_i64NumberOfDataRecords));
			System::Memory::littleEndianToHost(l_pHeaderBuffer+92, &m_ui32DurationOfADataRecordNumerator);
			System::Memory::littleEndianToHost(l_pHeaderBuffer+96, &m_ui32DurationOfADataRecordDenominator);
			System::Memory::littleEndianToHost(l_pHeaderBuffer+100, &m_ui16NumberOfSignals);
					
			if(oFile.bad())
			{
				return false;
			}
			return true;
		}
		
		std::string GDF::CFixedGDF2Header::getSubjectName()
		{
			// extracts the PID and Patient name from the m_sPatientId
			char * l_pToken = strtok(m_sPatientId, " ");
			if(l_pToken)
			{
				//The PId is not a numerical value in GDF, it is useless for us
				l_pToken = strtok(NULL, " ");
			}
			
			if(l_pToken)
			{
				return l_pToken;
			}
			else
			{
				return _NoValueS_;
			}
		}
		
		OpenViBE::uint64 GDF::CFixedGDF2Header::getSubjectSex()
		{		
			return m_ui8SubjectInformation & 0x03;
		}
		
		
		std::string GDF::CFixedGDF2Header::getExperimentDate()
		{
			//computes the experiment date
			OpenViBE::uint64 l_ui32TempDate= *(reinterpret_cast<OpenViBE::uint64*>(m_ui32StartDateAndTimeOfRecording));
					
			time_t l_sStartDateAndTimeOfRecordingInSeconds = ((l_ui32TempDate/2^32) - 719529) * (3600*24);
			tm * l_sStartDateAndTimeOfRecording = gmtime(&l_sStartDateAndTimeOfRecordingInSeconds);
					
			//TODO check how date is coded in openvibe Date not good?
			//(l_sStartDateAndTimeOfRecording->mon+1)<<8 + (l_sStartDateAndTimeOfRecording->day)

			return _NoValueS_;
		}
		
		
		OpenViBE::uint64 GDF::CFixedGDF2Header::getSubjectAge()
		{
			return static_cast<OpenViBE::uint64>( floor(static_cast<double>((m_ui32StartDateAndTimeOfRecording[1] - m_ui32Birthday[1]) / 365.242189813)));
		}

		OpenViBE::float64 GDF::CFixedGDF2Header::getDataRecordDuration()
		{
			return static_cast<OpenViBE::float64>(m_ui32DurationOfADataRecordNumerator) / static_cast<OpenViBE::float64>(m_ui32DurationOfADataRecordDenominator);
		}
		
		OpenViBE::uint64 GDF::CFixedGDF2Header::getNumberOfDataRecords()
		{
			return m_i64NumberOfDataRecords;
		}
		
		OpenViBE::uint64 GDF::CFixedGDF2Header::getChannelCount()
		{
			return m_ui16NumberOfSignals;
		}
		
		
		
		//HELPER METHODS
		OpenViBE::uint16 GDF::GDFDataSize(OpenViBE::uint32 ui32ChannelType)
		{
			OpenViBE::uint16 l_ui16Size;
			switch(ui32ChannelType)
			{
				case GDF::ChannelType_int8 : l_ui16Size = 1; break;
				case GDF::ChannelType_uint8 : l_ui16Size = 1; break;	
				case GDF::ChannelType_int16 : l_ui16Size = 2; break;
				case GDF::ChannelType_uint16 : l_ui16Size = 2; break;
				case GDF::ChannelType_int32 : l_ui16Size = 4; break;
				case GDF::ChannelType_uint32 : l_ui16Size = 4; break;	
				case GDF::ChannelType_int64 : l_ui16Size = 8; break;
				case GDF::ChannelType_uint64 : l_ui16Size = 8; break;	
								
				case GDF::ChannelType_float32 : l_ui16Size = 4; break;
				case GDF::ChannelType_float64 : l_ui16Size = 8; break;
				case GDF::ChannelType_float128 : l_ui16Size = 16; break;
							
				case GDF::ChannelType_int24 : l_ui16Size = 3; break;
				case GDF::ChannelType_uint24 : l_ui16Size = 3; break;
		
			}
			return l_ui16Size;
		}
		
	}
}
