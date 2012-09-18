/*
 * ovasCDriverBrainmasterDiscovery.cpp
 *
 * Copyright (c) 2012, Yann Renard. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */

#if defined TARGET_HAS_ThirdPartyBrainmasterCodeMakerAPI

#include "ovasCDriverBrainmasterDiscovery.h"
#include "ovasCConfigurationBrainmasterDiscovery.h"
#include "../ovasCConfigurationBuilder.h"

#include <openvibe-toolkit/ovtk_all.h>

#include "ovas_defines_brainmaster_discovery.h"

using namespace OpenViBEAcquisitionServer;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;

#define boolean OpenViBE::boolean

extern "C"
{
	int OvAtlLoginDevice(const char* sSerialNumber, const char* sPassKey);
}

namespace
{
	float32 decode_24(unsigned char*& rpBuffer)
	{
		int32 l_i32Result = rpBuffer[0] + rpBuffer[1]*0x100 + rpBuffer[2]*0x10000;
		if(l_i32Result > 0x800000)
		{
			l_i32Result=l_i32Result - 0x1000000;
		}
		rpBuffer+=3;
		return l_i32Result*0.01163f;
	}

	float32 decode_16(unsigned char*& rpBuffer)
	{
		int32 l_i32Result = rpBuffer[0] + rpBuffer[1]*0x100;
		if(l_i32Result > 0x8000)
		{
			l_i32Result=l_i32Result - 0x10000;
		}
		rpBuffer+=2;
		return l_i32Result*0.1983f;
	}

	float32 decode_8(unsigned char*& rpBuffer)
	{
		int32 l_i32Result = rpBuffer[0];
		l_i32Result-=0x80;
		rpBuffer+=1;
		return l_i32Result*100/128.f;
	}

	uint32 next_sync(uint32 ui32Sync)
	{
		ui32Sync++;
		ui32Sync&=7;
		return ui32Sync?ui32Sync:1;
	}
}

//___________________________________________________________________//
//                                                                   //

CDriverBrainmasterDiscovery::CDriverBrainmasterDiscovery(IDriverContext& rDriverContext)
	:IDriver(rDriverContext)
	,m_pCallback(NULL)
	,m_ui32SampleCountPerSentBlock(0)
	,m_ui32Port(0)
	,m_ui32Preset(Preset_Discovery_24)
	,m_ui32Type(Type_Discovery)
	,m_ui32BaudRate(0)
	,m_ui32SamplingRate(0)
	,m_ui32BitDepth(0)
	,m_ui32NotchFilters(0)
{
	m_sDeviceSerial=rDriverContext.getConfigurationManager().expand("${AcquisitionServer_Driver_BrainmasterDeviceSerial}").toASCIIString();
	m_sDevicePasskey=rDriverContext.getConfigurationManager().expand("${AcquisitionServer_Driver_BrainmasterDevicePasskey}").toASCIIString();
	m_sFrameDumpFilename=rDriverContext.getConfigurationManager().expand("${AcquisitionServer_Driver_BrainmasterFrameDumpFilename}").toASCIIString();

	m_vBaudRate[BaudRate_9600]=0x30; // 9600
	m_vBaudRate[BaudRate_115200]=0x20; // 115200
	m_vBaudRate[BaudRate_460800]=0x10; // 460800

	m_vBaudRateValue[BaudRate_9600]=9600; // 9600
	m_vBaudRateValue[BaudRate_115200]=115200; // 115200
	m_vBaudRateValue[BaudRate_460800]=460800; // 460800

	m_vNotchFilters[NotchFilter_Default]=0; // Off
	m_vNotchFilters[NotchFilter_Off]=0; // Off
	m_vNotchFilters[NotchFilter_50]=2; // 50 Hz
	m_vNotchFilters[NotchFilter_60]=3; // 60 Hz

	m_vNotchFiltersValue[NotchFilter_Default]=0; // Off
	m_vNotchFiltersValue[NotchFilter_Off]=0; // Off
	m_vNotchFiltersValue[NotchFilter_50]=50; // 50 Hz
	m_vNotchFiltersValue[NotchFilter_60]=60; // 60 Hz

	m_vBitDepth[BitDepth_8]=1; // 8 bits
	m_vBitDepth[BitDepth_16]=2; // 16 bits
	m_vBitDepth[BitDepth_24]=3; // 24 bits

	m_vBitDepthValue[BitDepth_8]=8; // 8 bits
	m_vBitDepthValue[BitDepth_16]=16; // 16 bits
	m_vBitDepthValue[BitDepth_24]=24; // 24 bits

	// 0xcc for 2 EEG, 0xff for 2 EEG + 2 AUX
	m_vChannelSelectionMask[24]=0xffffff; // Type_Discovery
	m_vChannelSelectionMask[4]=0xf; // Type_Atlantis 2x2
	m_vChannelSelectionMask[8]=0xff; // Type_Atlantis 4x4

	m_vBitDepthDecoder[ChannelType_EEG][BitDepth_8]=::decode_8;  // 8 bits
	m_vBitDepthDecoder[ChannelType_EEG][BitDepth_16]=::decode_16; // 16 bits
	m_vBitDepthDecoder[ChannelType_EEG][BitDepth_24]=::decode_24; // 24 bits

	m_vBitDepthDecoder[ChannelType_AUX][BitDepth_8]=::decode_8;  // 8 bits
	m_vBitDepthDecoder[ChannelType_AUX][BitDepth_16]=::decode_16; // 16 bits
	m_vBitDepthDecoder[ChannelType_AUX][BitDepth_24]=::decode_16; // 24 bits

	m_vStartModule[Type_Discovery]=::DiscStartModule;
	m_vStartModule[Type_Atlantis]=::AtlStartModule;

	m_vStopModule[Type_Discovery]=::DiscStopModule;
	m_vStopModule[Type_Atlantis]=::AtlStopModule;

	m_oHeader.setSamplingFrequency(256);
	m_oHeader.setChannelCount(24);
	m_oHeader.setChannelName(0, "Fp1"/*"FP1"*/);
	m_oHeader.setChannelName(1, "F3");
	m_oHeader.setChannelName(2, "C3");
	m_oHeader.setChannelName(3, "P3");
	m_oHeader.setChannelName(4, "O1");
	m_oHeader.setChannelName(5, "F7");
	m_oHeader.setChannelName(6, "T3");
	m_oHeader.setChannelName(7, "T5");
	m_oHeader.setChannelName(8, "Fz");
	m_oHeader.setChannelName(9, "Fp2"/*"FP2"*/);
	m_oHeader.setChannelName(10, "F4");
	m_oHeader.setChannelName(11, "C4");
	m_oHeader.setChannelName(12, "P4");
	m_oHeader.setChannelName(13, "O2");
	m_oHeader.setChannelName(14, "F8");
	m_oHeader.setChannelName(15, "T4");
	m_oHeader.setChannelName(16, "T6");
	m_oHeader.setChannelName(17, "Cz");
	m_oHeader.setChannelName(18, "Pz");
	m_oHeader.setChannelName(19, "A2");
	m_oHeader.setChannelName(20, "Fpz"/*"FPz"*/);
	m_oHeader.setChannelName(21, "Oz");
	m_oHeader.setChannelName(22, "AUX1");
	m_oHeader.setChannelName(23, "AUX2");
}

void CDriverBrainmasterDiscovery::release(void)
{
	delete this;
}

const char* CDriverBrainmasterDiscovery::getName(void)
{
	return "Brainmaster Discovery and Atlantis";
}

//___________________________________________________________________//
//                                                                   //

boolean CDriverBrainmasterDiscovery::initialize(
	const uint32 ui32SampleCountPerSentBlock,
	IDriverCallback& rCallback)
{
	uint32 i;

	if(m_rDriverContext.isConnected()) { return false; }

	if(m_sDeviceSerial=="" || m_sDevicePasskey=="")
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "The device serial or passkey were not configured\n";
		m_rDriverContext.getLogManager() << LogLevel_Error << "You can set their value from the configuration pannel or from the following configuration tokens :\n";
		m_rDriverContext.getLogManager() << LogLevel_Error << " - " << CString("AcquisitionServer_Driver_BrainmasterDeviceSerial") << "\n";
		m_rDriverContext.getLogManager() << LogLevel_Error << " - " << CString("AcquisitionServer_Driver_BrainmasterDevicePasskey") << "\n";
		return false;
	}

	switch(m_ui32Type)
	{
		default:
		case Type_Discovery:
			m_ui32BaudRateReal=(m_ui32BaudRate!=BaudRate_Default?m_ui32BaudRate:BaudRate_460800);
			m_ui32SamplingRateReal=m_ui32SamplingRate;
			m_ui32BitDepthReal=BitDepth_24;
			m_ui32NotchFiltersReal=(m_ui32NotchFilters!=NotchFilter_Default?m_ui32NotchFilters:NotchFilter_Default);
			m_ui32FrameSize=3;
			m_ui32DataOffset=3;
			break;

		case Type_Atlantis:
			m_ui32BaudRateReal=(m_ui32BaudRate!=BaudRate_Default?m_ui32BaudRate:BaudRate_115200);
			m_ui32SamplingRateReal=m_ui32SamplingRate;
			m_ui32BitDepthReal=(m_ui32BitDepth!=BitDepth_Default?m_ui32BitDepth:BitDepth_8);
			m_ui32NotchFiltersReal=(m_ui32NotchFilters!=NotchFilter_Default?m_ui32NotchFilters:NotchFilter_Default);
			m_ui32FrameSize=1;
			m_ui32DataOffset=1;
			break;
	}

	switch(m_ui32BitDepthReal)
	{
		default:
		case BitDepth_24:
			for(i=0; i<m_oHeader.getChannelCount(); i++)
			{
				switch(m_vChannelType[i])
				{
					default:
					case ChannelType_EEG:
						m_ui32FrameSize+=3;
						break;

					case ChannelType_AUX:
						m_ui32FrameSize+=2;
						break;
				}
			}
			break;

		case BitDepth_16:
			m_ui32FrameSize+=m_oHeader.getChannelCount()*2;
			break;

		case BitDepth_8:
			m_ui32FrameSize+=m_oHeader.getChannelCount();
			break;
	}

	m_rDriverContext.getLogManager() << LogLevel_Trace << "Computed frame size of " << m_ui32FrameSize << " bytes\n";

	m_vSample.resize(m_oHeader.getChannelCount());
	m_vBuffer.resize(m_ui32FrameSize);

	m_pCallback=&rCallback;
	m_ui32SampleCountPerSentBlock=ui32SampleCountPerSentBlock;

	// Sets/Autodetects port
	m_ui32PortReal=m_ui32Port;
	if(m_ui32PortReal==0)
	{
		m_rDriverContext.getLogManager() << LogLevel_Trace << "Auto detecting COM port...\n";

		m_ui32PortReal=autoDetectPort();
		if(m_ui32PortReal==uint32(-1))
		{
			m_rDriverContext.getLogManager() << LogLevel_Error << "Port is not set and could not be auto detected, please configure the driver\n";
			return false;
		}

		m_rDriverContext.getLogManager() << LogLevel_Trace << "Found COM port " << m_ui32PortReal << " !\n";
	}

	// Opens device handle at default speed
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Opening COM port " << m_ui32PortReal << " at 9600 bauds\n";
	if(!::AtlOpenPort(m_ui32PortReal, 9600, NULL))
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not open port " << m_ui32PortReal << "\n";
		return false;
	}

	// Writes baud rate
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Setting baud rate to " << m_vBaudRateValue[m_ui32BaudRateReal] << " bauds (code " << m_vBaudRate[m_ui32BaudRateReal] << ")\n";
	if(!::AtlSetBaudRate(m_vBaudRate[m_ui32BaudRateReal]))
	{
		::AtlClosePort(m_ui32PortReal);
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not set baud rate\n";
		return false;
	}

	// Closes port (which was at default speed)
	::AtlClosePort(m_ui32PortReal);

	// Opens device handle at targeted speed
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Re-opening COM port " << m_ui32PortReal << " at " << m_vBaudRateValue[m_ui32BaudRateReal] << " bauds (code " << m_vBaudRate[m_ui32BaudRateReal] << ")\n";
	if(!::AtlOpenPort(m_ui32PortReal, m_vBaudRateValue[m_ui32BaudRateReal], NULL))
	// if(!::AtlOpenPort(m_ui32PortReal, 9600, NULL))
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not open port " << m_ui32PortReal << "\n";
		return false;
	}

	// Logs in the device
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Logging in the device...\n";
	int code=::OvAtlLoginDevice(m_sDeviceSerial.c_str(), m_sDevicePasskey.c_str());
	if(!code)
	{
		::AtlSetBaudRate(m_vBaudRate[BaudRate_9600]);
		::AtlClosePort(m_ui32PortReal);

		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not log in device (error code was " << uint32(code) << ")\n";
		m_rDriverContext.getLogManager() << LogLevel_Error << "Please check the device serial and passkey configuration\n";
		m_rDriverContext.getLogManager() << LogLevel_Error << "You can set their value from the configuration pannel or from the following configuration tokens :\n";
		m_rDriverContext.getLogManager() << LogLevel_Error << " - " << CString("AcquisitionServer_Driver_BrainmasterDeviceSerial") << " (was set to [" << m_sDeviceSerial.c_str() << "])\n";
		m_rDriverContext.getLogManager() << LogLevel_Error << " - " << CString("AcquisitionServer_Driver_BrainmasterDevicePasskey") << " (was set to [" << m_sDevicePasskey.c_str() << "])\n";

		if(!checkDeviceSerial(m_sDeviceSerial))   m_rDriverContext.getLogManager() << LogLevel_Error << "By the way, serial seems to be malformed, it is supposed to be XXXXX with each X be a digit\n";
		if(!checkDevicePasskey(m_sDevicePasskey)) m_rDriverContext.getLogManager() << LogLevel_Error << "By the way, passkey seems to be malformed, it is supposed to be XXXX-XXXX-XXXX with each X be a digit or a letter\n";

		return false;
	}

	// Reads firmware version
	m_rDriverContext.getLogManager() << LogLevel_Info << "Successfully opened device on port " << m_ui32PortReal << " | Firware nr : " << uint32(::AtlQueryFirmware(0)) << "\n";

	// Writes notch filters
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Setting notch filters to " << m_vNotchFiltersValue[m_ui32NotchFiltersReal] << " (code " << m_vNotchFilters[m_ui32NotchFiltersReal] << ")...\n";
	if(!::AtlSetNotchFilters(m_vNotchFilters[m_ui32NotchFiltersReal]))
	{
		::AtlSetBaudRate(m_vBaudRate[BaudRate_9600]);
		::AtlClosePort(m_ui32PortReal);
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not set notch filters\n";
		return false;
	}

	// Writes bit depth
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Setting bit depth to " << m_vBitDepthValue[m_ui32BitDepthReal] << " bits (code " << m_vBitDepth[m_ui32BitDepthReal] << ")...\n";
	if(!::AtlSetBytesPerSample(m_vBitDepth[m_ui32BitDepthReal]))
	{
		::AtlSetBaudRate(m_vBaudRate[BaudRate_9600]);
		::AtlClosePort(m_ui32PortReal);
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not set bytes per sample\n";
		return false;
	}

	// Selects all the channels
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Selecting all channels with channel mask " << m_vChannelSelectionMask[m_oHeader.getChannelCount()] << "...\n";
	if(!::AtlSelectChannels(m_vChannelSelectionMask[m_oHeader.getChannelCount()]))
	{
		::AtlSetBaudRate(m_vBaudRate[BaudRate_9600]);
		::AtlClosePort(m_ui32PortReal);
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not select all the channels\n";
		return false;
	}

	// Clears special data
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Clearing specials...\n";
	if(!::AtlClearSpecials())
	{
		::AtlSetBaudRate(m_vBaudRate[BaudRate_9600]);
		::AtlClosePort(m_ui32PortReal);
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not clear special data selection\n";
		return false;
	}

	// serialn_p ::AtlQuerySerialNumber(int auth)

	// Actually starts the acquisition
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Starting module !\n";
	if(!m_vStartModule[m_ui32Type]())
	{
		::AtlSetBaudRate(m_vBaudRate[BaudRate_9600]);
		::AtlClosePort(m_ui32PortReal);
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not start module\n";
		return false;
	}

	m_ui32SyncByte=1;

	// Preparing optional frame dump
	if(m_sFrameDumpFilename=="")
	{
		m_rDriverContext.getLogManager() << LogLevel_Trace << "Won't dump frames\n";
		m_bFrameDumpFlag=false;
	}
	else
	{
		FILE* l_pDumpFile=fopen(m_sFrameDumpFilename.c_str(), "wb");
		if(l_pDumpFile)
		{
			m_bFrameDumpFlag=true;
			fclose(l_pDumpFile);
			m_rDriverContext.getLogManager() << LogLevel_Trace << "Will dump frames in [" << CString(m_sFrameDumpFilename.c_str()) << "]\n";
		}
		else
		{
			m_bFrameDumpFlag=false;
			m_rDriverContext.getLogManager() << LogLevel_Warning << "Should have dumped frames in [" << CString(m_sFrameDumpFilename.c_str()) << "] but file can't be opened for writing !\n";
		}
	}


	return true;
}

boolean CDriverBrainmasterDiscovery::start(void)
{
	if(!m_rDriverContext.isConnected()) { return false; }
	if(m_rDriverContext.isStarted()) { return false; }
	return true;
}

boolean CDriverBrainmasterDiscovery::loop(void)
{
	if(!m_rDriverContext.isConnected()) { return false; }
	if(!m_rDriverContext.isStarted()) { return true; }
	uint32 i, j;
	boolean l_bGotData=false;

	for(i=0; i<m_ui32SampleCountPerSentBlock; i++)
	{
		// Updates Sync bits
		m_ui32SyncByte=next_sync(m_ui32SyncByte);

		// Searches for Sync bits
		this->read(&m_vBuffer[0], 1);
		if(((m_vBuffer[0]>>5)&7)!=m_ui32SyncByte)
		{
			// Needs resync
			uint32 l_ui32Sync=this->sync();
			if(l_ui32Sync!=uint32(-1))
			{
				m_rDriverContext.getLogManager() << LogLevel_Info << "Resynced by " << l_ui32Sync << " byte(s) !\n";
			}
			else
			{
				m_rDriverContext.getLogManager() << LogLevel_ImportantWarning << "Could not resync, did not find the sync bytes !\n";
			}

			m_bWasLastFrameCorrect=false;
		}
		else
		{
			// Sends samples if last frame was correct
			if(m_bWasLastFrameCorrect)
			{
				m_pCallback->setSamples(&m_vSample[0], 1);
				l_bGotData=true;
			}

			// Assumed synced stream - Reads leading bytes
			this->read(&m_vBuffer[1], m_vBuffer.size()-1);

			// Converts new buffer to samples
			unsigned char* l_pBuffer=&m_vBuffer[m_ui32DataOffset];

			for(j=0; j<m_vSample.size(); j++)
			{
				m_vSample[j]=m_vBitDepthDecoder[m_vChannelType[j]][m_ui32BitDepthReal](l_pBuffer);
			}

			m_bWasLastFrameCorrect=true;
		}
	}

	if(l_bGotData)
	{
		m_rDriverContext.correctDriftSampleCount(m_rDriverContext.getSuggestedDriftCorrectionSampleCount());
	}

	return true;
}

boolean CDriverBrainmasterDiscovery::read(unsigned char* pFrame, uint32 ui32FrameSize)
{
	uint32 l_ui32ByteCount=ui32FrameSize;
	uint32 l_ui32ByteIndex=0;

	while(l_ui32ByteIndex!=l_ui32ByteCount)
	{
		int iCode=::AtlReadData(pFrame+l_ui32ByteIndex, l_ui32ByteCount-l_ui32ByteIndex);
		if(iCode>=0)
		{
			l_ui32ByteIndex+=iCode;
		}
	}

	return true;
}

uint32 CDriverBrainmasterDiscovery::sync(void)
{
	std::vector < unsigned char > l_vBuffer;
	l_vBuffer.resize(m_vBuffer.size()*4);

	boolean l_bFinished=false;
	uint32 l_ui32SyncCount=0;
	uint32 i=0;

	while(!l_bFinished && i<16)
	{
		i++;
		this->read(&l_vBuffer[0], l_vBuffer.size());
		for(uint32 i=0; i<m_vBuffer.size() && !l_bFinished; i++)
		{
			uint32 l_pSyncCandidate[4];
			l_pSyncCandidate[0]=l_vBuffer[i+0*m_vBuffer.size()]>>5;
			l_pSyncCandidate[1]=l_vBuffer[i+1*m_vBuffer.size()]>>5;
			l_pSyncCandidate[2]=l_vBuffer[i+2*m_vBuffer.size()]>>5;
			l_pSyncCandidate[3]=l_vBuffer[i+3*m_vBuffer.size()]>>5;
			if(next_sync(l_pSyncCandidate[0])==l_pSyncCandidate[1]
			&& next_sync(l_pSyncCandidate[1])==l_pSyncCandidate[2]
			&& next_sync(l_pSyncCandidate[2])==l_pSyncCandidate[3])
			{
				this->read(&l_vBuffer[0], i);
				m_ui32SyncByte=l_pSyncCandidate[3];
				l_ui32SyncCount=i;
				l_bFinished=true;
			}
		}
	}

#if 1
	// For debug purpose
	if(m_bFrameDumpFlag)
	{
		FILE* l_pFile=::fopen(m_sFrameDumpFilename.c_str(), "ab");
		if(l_pFile)
		{
			::fprintf(l_pFile, "Syncing...\n");
			for(uint32 i=0; i<l_vBuffer.size(); i++)
			{
				::fprintf(l_pFile, "0x%02x ", l_vBuffer[i]);
				if(((i+1)%m_vBuffer.size())==0)
				{
					::fprintf(l_pFile, "\n");
				}
			}
			
			::fclose(l_pFile);
		}
		else
		{
			m_rDriverContext.getLogManager() << LogLevel_Warning << "Unexpected: Could not open dump file [" << CString(m_sFrameDumpFilename.c_str()) << "] for writing\n";
			m_bFrameDumpFlag=false;
		}
	}
#endif

	return l_bFinished?l_ui32SyncCount:uint32(-1);
}

boolean CDriverBrainmasterDiscovery::stop(void)
{
	if(!m_rDriverContext.isConnected()) { return false; }
	if(!m_rDriverContext.isStarted()) { return false; }

	return true;
}

boolean CDriverBrainmasterDiscovery::uninitialize(void)
{
	if(!m_rDriverContext.isConnected()) { return false; }
	if(m_rDriverContext.isStarted()) { return false; }

	// Stops acquisition
	if(!m_vStopModule[m_ui32Type]())
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could stop module\n";
		return false;
	}

	// Writes baud rate back to 9600
	if(!::AtlSetBaudRate(m_vBaudRate[BaudRate_9600]))
	{
		::AtlClosePort(m_ui32PortReal);
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not set baud rate\n";
		return false;
	}

	// Closes device handle
	if(!::AtlClosePort(m_ui32PortReal))
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not close port " << m_ui32PortReal << "\n";
		return false;
	}

	return true;
}

//___________________________________________________________________//
//                                                                   //

boolean CDriverBrainmasterDiscovery::isConfigurable(void)
{
	return true;
}

boolean CDriverBrainmasterDiscovery::configure(void)
{
	CConfigurationBrainmasterDiscovery m_oConfiguration("../share/openvibe-applications/acquisition-server/interface-Brainmaster-Discovery.ui", m_ui32Port, m_ui32Preset, m_ui32Type, m_ui32BaudRate, m_ui32SamplingRate, m_ui32BitDepth, m_ui32NotchFilters, m_vChannelType, m_sDeviceSerial, m_sDevicePasskey);
	if(!m_oConfiguration.configure(m_oHeader))
	{
		return false;
	}
	return true;
}

#endif // TARGET_HAS_ThirdPartyBrainmasterCodeMakerAPI
