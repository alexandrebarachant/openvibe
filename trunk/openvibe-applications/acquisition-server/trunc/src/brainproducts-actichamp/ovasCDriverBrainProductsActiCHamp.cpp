/*
 * ovasCDriverBrainProductsActiCHamp.cpp
 *
 * Copyright (c) 2012, Mensia Technologies SA. All rights reserved.
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

#if defined TARGET_HAS_ThirdPartyActiCHampAPI

#include "ovasCDriverBrainProductsActiCHamp.h"
#include "ovasCConfigurationBrainProductsActiCHamp.h"

#include <openvibe-toolkit/ovtk_all.h>

#include <actichamp.h>

using namespace OpenViBEAcquisitionServer;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
#define boolean OpenViBE::boolean

//___________________________________________________________________//
//                                                                   //

namespace
{
	typedef union
	{
		t_champDataModelAux ModelAux;
		t_champDataModel32 Model32;
		t_champDataModel64 Model64;
		t_champDataModel96 Model96;
		t_champDataModel128 Model128;
		t_champDataModel160 Model160;
	} UBuffer;

	const uint32 NB_MAX_BUFFER_IN_CACHE = 1000;
	UBuffer Buffer[NB_MAX_BUFFER_IN_CACHE];
	
	// Low pass FIR filters before downsampling
	//
	// Computed with python and scipy
	// References :
	// http://docs.scipy.org/doc/numpy/reference/generated/numpy.ndarray.tofile.html
	// http://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.firwin.html
	//
	// The following filters have roughly 50ms delay as described in "2.1.4 What is the
	// delay of a linear-phase FIR?" at http://www.dspguru.com/book/export/html/3 :
	// n =   500, Fs =   5000, delay = (n-1)/(2*Fs) = 0.04995
	// n =  1000, Fs =  10000, delay = (n-1)/(2*Fs) = 0.04995
	// n =  2500, Fs =  25000, delay = (n-1)/(2*Fs) = 0.04995
	// n =  5000, Fs =  50000, delay = (n-1)/(2*Fs) = 0.04995
	// n = 10000, Fs = 100000, delay = (n-1)/(2*Fs) = 0.04995
	//
	// In order to correct this delay, filtering should be done as a two steps process with a forward filter
	// followed by a backward filter. However, this leads to an n square complexity where a linear complexity is
	// sufficient in forward only filtering (using 100kHz input signal, n=10000 taps !)
	//
	// To avoid such complexity, it is chosen to antedate acquired samples by 50ms cheating the drift correction
	// process. Indeed, the acquisition server application monitors the drifting of the acquisition process and
	// corrects this drift upon demand. It is up to the driver to require this correction and it can be chosen not
	// to fit the 0 drift, but to fit an arbitrary fixed drift instead.
	//
	// The offset for this correction is stored in the m_i64DriftOffsetSampleCount variable

/* ---------------------------------------------------------------------------------------------------------------
	from scipy import signal
	N=500
	signal.firwin(N, cutoff=2048./(0.5*5000.), window='hamming').tofile("c:/openvibe/f64_5k_4096.bin")
	signal.firwin(N, cutoff=1024./(0.5*5000.), window='hamming').tofile("c:/openvibe/f64_5k_2048.bin")
	signal.firwin(N, cutoff= 512./(0.5*5000.), window='hamming').tofile("c:/openvibe/f64_5k_1024.bin")
	signal.firwin(N, cutoff= 256./(0.5*5000.), window='hamming').tofile("c:/openvibe/f64_5k_512.bin")
	signal.firwin(N, cutoff= 128./(0.5*5000.), window='hamming').tofile("c:/openvibe/f64_5k_256.bin")
	signal.firwin(N, cutoff=  64./(0.5*5000.), window='hamming').tofile("c:/openvibe/f64_5k_128.bin")

	N=1000
	signal.firwin(N, cutoff=2048./(0.5*10000.), window='hamming').tofile("c:/openvibe/f64_10k_4096.bin")
	signal.firwin(N, cutoff=1024./(0.5*10000.), window='hamming').tofile("c:/openvibe/f64_10k_2048.bin")
	signal.firwin(N, cutoff= 512./(0.5*10000.), window='hamming').tofile("c:/openvibe/f64_10k_1024.bin")
	signal.firwin(N, cutoff= 256./(0.5*10000.), window='hamming').tofile("c:/openvibe/f64_10k_512.bin")
	signal.firwin(N, cutoff= 128./(0.5*10000.), window='hamming').tofile("c:/openvibe/f64_10k_256.bin")
	signal.firwin(N, cutoff=  64./(0.5*10000.), window='hamming').tofile("c:/openvibe/f64_10k_128.bin")

	N=2500
	signal.firwin(N, cutoff=2048./(0.5*25000.), window='hamming').tofile("c:/openvibe/f64_25k_4096.bin")
	signal.firwin(N, cutoff=1024./(0.5*25000.), window='hamming').tofile("c:/openvibe/f64_25k_2048.bin")
	signal.firwin(N, cutoff= 512./(0.5*25000.), window='hamming').tofile("c:/openvibe/f64_25k_1024.bin")
	signal.firwin(N, cutoff= 256./(0.5*25000.), window='hamming').tofile("c:/openvibe/f64_25k_512.bin")
	signal.firwin(N, cutoff= 128./(0.5*25000.), window='hamming').tofile("c:/openvibe/f64_25k_256.bin")
	signal.firwin(N, cutoff=  64./(0.5*25000.), window='hamming').tofile("c:/openvibe/f64_25k_128.bin")

	N=5000
	signal.firwin(N, cutoff=2048./(0.5*50000.), window='hamming').tofile("c:/openvibe/f64_50k_4096.bin")
	signal.firwin(N, cutoff=1024./(0.5*50000.), window='hamming').tofile("c:/openvibe/f64_50k_2048.bin")
	signal.firwin(N, cutoff= 512./(0.5*50000.), window='hamming').tofile("c:/openvibe/f64_50k_1024.bin")
	signal.firwin(N, cutoff= 256./(0.5*50000.), window='hamming').tofile("c:/openvibe/f64_50k_512.bin")
	signal.firwin(N, cutoff= 128./(0.5*50000.), window='hamming').tofile("c:/openvibe/f64_50k_256.bin")
	signal.firwin(N, cutoff=  64./(0.5*50000.), window='hamming').tofile("c:/openvibe/f64_50k_128.bin")

	N=10000
	signal.firwin(N, cutoff=2048./(0.5*100000.), window='hamming').tofile("c:/openvibe/f64_100k_4096.bin")
	signal.firwin(N, cutoff=1024./(0.5*100000.), window='hamming').tofile("c:/openvibe/f64_100k_2048.bin")
	signal.firwin(N, cutoff= 512./(0.5*100000.), window='hamming').tofile("c:/openvibe/f64_100k_1024.bin")
	signal.firwin(N, cutoff= 256./(0.5*100000.), window='hamming').tofile("c:/openvibe/f64_100k_512.bin")
	signal.firwin(N, cutoff= 128./(0.5*100000.), window='hamming').tofile("c:/openvibe/f64_100k_256.bin")
	signal.firwin(N, cutoff=  64./(0.5*100000.), window='hamming').tofile("c:/openvibe/f64_100k_128.bin")
--------------------------------------------------------------------------------------------------------------- */

	static bool loadFilter(const char* sFilename, std::vector < float64 >& vFilter)
	{
		FILE* l_pFile=::fopen(sFilename, "rb");
		if(!l_pFile)
		{
			vFilter.clear();
			vFilter.push_back(1);
			return false;
		}
		else
		{
			::fseek(l_pFile, 0, SEEK_END);
			size_t len=::ftell(l_pFile);
			::fseek(l_pFile, 0, SEEK_SET);
			vFilter.resize(len/sizeof(float64));
			::fread(&vFilter[0], len, 1, l_pFile);
			::fclose(l_pFile);
		}
		return true;
	}
};

CDriverBrainProductsActiCHamp::CDriverBrainProductsActiCHamp(IDriverContext& rDriverContext)
	:IDriver(rDriverContext)
	,m_pHandle(NULL)
	,m_pCallback(NULL)
	,m_ui32DeviceId(0)
	,m_ui32Mode(CHAMP_MODE_ACTIVE_SHIELD)
	,m_ui32PhysicalSampleRate(CHAMP_RATE_10KHZ)
	,m_ui32ADCDataFilter(CHAMP_ADC_NATIVE)
	,m_ui32ADCDataDecimation(CHAMP_DECIMATION_2)
	,m_ui32ActiveShieldGain(5) // default value from pyCorder = 5/100
	,m_ui32ModuleEnabled(0xffffffff)
	,m_uiLastTriggers(0)
	,m_ui64LastSampleDate(0)
	,m_ui32PhysicalSampleRateHz(0)
	,m_ui64SampleCount(0)
	,m_bUseAuxChannels(false)
	,m_ui32BadImpedanceLimit(10000)
	,m_ui32GoodImpedanceLimit(5000)
{
	t_champVersion l_oVersion;
	::champGetVersion(NULL, &l_oVersion);

	m_rDriverContext.getLogManager() << LogLevel_Trace << "Got actiCHamp version (Dll:" << uint32(l_oVersion.Dll) << ")\n";
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Number of attached devices : " << uint32(::champGetCount()) << "\n";

	m_oHeader.setSamplingFrequency(512);
}

void CDriverBrainProductsActiCHamp::release(void)
{
	delete this;
}

const char* CDriverBrainProductsActiCHamp::getName(void)
{
	return "Brain Products actiCHamp";
}

//___________________________________________________________________//
//                                                                   //

boolean CDriverBrainProductsActiCHamp::initialize(
	const uint32 ui32SampleCountPerSentBlock,
	IDriverCallback& rCallback)
{
	if(m_rDriverContext.isConnected()) { return false; }
	// Opens device
	if((m_pHandle=::champOpen(m_ui32DeviceId))==NULL)
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "Can not open actiCHamp device id " << m_ui32DeviceId << "\n";
		return false;
	}

	// Gets versions
	t_champVersion l_oVersion;
	::champGetVersion(m_pHandle, &l_oVersion);
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Got actiCHamp version (Dll:" << uint32(l_oVersion.Dll) << ", Driver:" << uint32(l_oVersion.Driver) << ", Cypress:" << uint32(l_oVersion.Cypress) << ", Fpga:" << uint32(l_oVersion.Fpga) << ", Msp430:" << uint32(l_oVersion.Msp430) << ")\n";

	// Gets battery voltages
	t_champVoltages l_oVoltage;
	if(::champGetVoltages(m_pHandle, &l_oVoltage))
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not get voltages - Got error [" << getError() << "]\n";
	}
	else
	{
		int l_iState=0;
		if(l_oVoltage.VDC < 5.5) l_iState=1;
		if(l_oVoltage.VDC < 5.2) l_iState=2;
		switch(l_iState)
		{
			case 0:
				m_rDriverContext.getLogManager() << LogLevel_Trace << "Battery level is fine (" << float32(l_oVoltage.VDC) << ")\n";
				break;
			case 1:
				m_rDriverContext.getLogManager() << LogLevel_Warning << "Battery level is critical (" << float32(l_oVoltage.VDC) << ")\n";
				break;
			case 2:
				m_rDriverContext.getLogManager() << LogLevel_ImportantWarning << "Battery level is bad (" << float32(l_oVoltage.VDC) << ")\n";
				break;
			default:
				break;
		}
	}

	// Gets and sets modules status
	t_champModules l_oModules;
	::champGetModules(m_pHandle, &l_oModules);
	l_oModules.Enabled=m_ui32ModuleEnabled;
	::champSetModules(m_pHandle, &l_oModules);
	::champGetModules(m_pHandle, &l_oModules);
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Got modules status : Present=" << l_oModules.Present << " / Enabled=" << l_oModules.Enabled << "\n";

	// Active shield
	if(::champSetActiveShieldGain(m_pHandle, m_ui32ActiveShieldGain))
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not set active shield gain - Got error [" << getError() << "]\n";
		::champClose(m_pHandle);
		m_pHandle=NULL;
		return false;
	}
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Active Shield Gain set to ["<<m_ui32ActiveShieldGain<<"]\n";

	// Sets settings to acquisition state
	t_champSettingsEx l_oSettings;
	l_oSettings.Mode=t_champMode(m_ui32Mode);
	l_oSettings.Rate=t_champRate(m_ui32PhysicalSampleRate);
	l_oSettings.AdcFilter=t_champAdcFilter(m_ui32ADCDataFilter);
	l_oSettings.Decimation=t_champDecimation(m_ui32ADCDataDecimation);
	if(::champSetSettingsEx(m_pHandle, &l_oSettings))
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not set settings - Got error [" << getError() << "]\n";
		return false;
	}
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Acquisition Settings set to [Mode: "<<m_ui32Mode<<"] [PhysicalSamplingRate: "<<m_ui32PhysicalSampleRate<<"] [ADCDataFilter: "<<m_ui32ADCDataFilter<<"] [ADCDataDecimation: "<<m_ui32ADCDataDecimation<<"]\n";

	// Gets properties
	t_champProperty l_oProperties;
	if(::champGetProperty(m_pHandle, &l_oProperties))
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not get properties - Got error [" << getError() << "]\n";
		::champClose(m_pHandle);
		m_pHandle=NULL;
		return false;
	}

	// Sets channel count
	// as the modules were set before according to the configuration, the property structure
	// contains the right number of channels.
	m_ui32CountEEG=l_oProperties.CountEeg;
	m_ui32CountAux=(m_bUseAuxChannels ? l_oProperties.CountAux : 0);
	m_ui32ChannelCount=m_ui32CountEEG+m_ui32CountAux;
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Channel counts are  [EEG: "<<m_ui32CountEEG<<"] [AUX: "<<m_ui32CountAux<<"]\n";


	m_oHeader.setChannelCount(m_ui32ChannelCount);

	m_vImpedance.resize(m_ui32CountEEG+1);
	m_vSample.resize(m_ui32ChannelCount);
	m_vResolution.resize(m_ui32ChannelCount);
	
	m_ui32PhysicalSampleRateHz = (uint32)l_oProperties.Rate;
	m_ui64CounterStep=(uint64(m_oHeader.getSamplingFrequency())<<32) / uint64(m_ui32PhysicalSampleRateHz);
	
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Sampling rate in Hz  [Physical: "<<m_ui32PhysicalSampleRateHz<<"] [Driver: "<<m_oHeader.getSamplingFrequency()<<"]\n";

	uint32 i,j=0;
	for(i=0; i<m_ui32CountEEG; i++, j++) m_vResolution[j]=l_oProperties.ResolutionEeg*1E6f; // converts to µV
	for(i=0; i<m_ui32CountAux; i++, j++) m_vResolution[j]=l_oProperties.ResolutionAux*1E6f; // converts to µV

	// Sets data pointers
	// the amplifier model is depending on the number of channels, always including AUX
	switch((m_bUseAuxChannels ? m_ui32ChannelCount : m_ui32ChannelCount+8))
	{
		case 8:
			m_rDriverContext.getLogManager() << LogLevel_Trace << "Data Model is [AUX]\n";
			m_pAux=Buffer[0].ModelAux.Aux;
			m_pEEG=NULL; // Buffer[0].ModelAux.Main;
			m_pTriggers=&Buffer[0].ModelAux.Triggers;
			m_uiSize=sizeof(Buffer[0].ModelAux);
			break;

		case 32+8:
			m_rDriverContext.getLogManager() << LogLevel_Trace << "Data Model is [Model32]\n";
			m_pAux=Buffer[0].Model32.Aux;
			m_pEEG=Buffer[0].Model32.Main;
			m_pTriggers=&Buffer[0].Model32.Triggers;
			m_uiSize=sizeof(Buffer[0].Model32);
			break;

		case 64+8:
			m_rDriverContext.getLogManager() << LogLevel_Trace << "Data Model is [Model64]\n";
			m_pAux=Buffer[0].Model64.Aux;
			m_pEEG=Buffer[0].Model64.Main;
			m_pTriggers=&Buffer[0].Model64.Triggers;
			m_uiSize=sizeof(Buffer[0].Model64);
			break;

		case 96+8:
			m_rDriverContext.getLogManager() << LogLevel_Trace << "Data Model is [Model96]\n";
			m_pAux=Buffer[0].Model96.Aux;
			m_pEEG=Buffer[0].Model96.Main;
			m_pTriggers=&Buffer[0].Model96.Triggers;
			m_uiSize=sizeof(Buffer[0].Model96);
			break;

		case 128+8:
			m_rDriverContext.getLogManager() << LogLevel_Trace << "Data Model is [Model128]\n";
			m_pAux=Buffer[0].Model128.Aux;
			m_pEEG=Buffer[0].Model128.Main;
			m_pTriggers=&Buffer[0].Model128.Triggers;
			m_uiSize=sizeof(Buffer[0].Model128);
			break;

		case 160+8:
			m_rDriverContext.getLogManager() << LogLevel_Trace << "Data Model is [Model160]\n";
			m_pAux=Buffer[0].Model160.Aux;
			m_pEEG=Buffer[0].Model160.Main;
			m_pTriggers=&Buffer[0].Model160.Triggers;
			m_uiSize=sizeof(Buffer[0].Model160);
			break;

		default:
			m_rDriverContext.getLogManager() << LogLevel_Trace << "Data Model unknown.\n";
			m_pAux=NULL;
			m_pEEG=NULL;
			m_pTriggers=NULL;
			m_uiSize=0;
			break;
	}

	if(m_uiSize==0)
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "Unknown device model " << m_ui32CountEEG << " EEG channels / " << m_ui32CountAux << " Auxiliary channels\n";
		::champClose(m_pHandle);
		m_pHandle=NULL;
		return false;
	}
	else
	{
		m_rDriverContext.getLogManager() << LogLevel_Trace << "Device model " << m_ui32CountEEG << " EEG channels / " << m_ui32CountAux << " Auxiliary channels\n";
	}

	// Prepares low pass filter

#define __set_filter__(f,f_decim) \
	if(m_ui32ADCDataDecimation==CHAMP_DECIMATION_0)	loadFilter(OpenViBE::Directories::getDataDir() + "/openvibe-applications/acquisition-server/filters/"f".bin", m_vFilter); \
	else loadFilter(OpenViBE::Directories::getDataDir() + "/openvibe-applications/acquisition-server/filters/"f_decim".bin", m_vFilter);\
	l_bValid=true;

	m_rDriverContext.getLogManager() << LogLevel_Trace << "Setting up the FIR filter for signal decimation (physical rate > driver rate).\n";

	boolean l_bValid=false;
	switch(m_oHeader.getSamplingFrequency())
	{
		case 128:
			switch(m_ui32PhysicalSampleRate)
			{
				case CHAMP_RATE_10KHZ:  __set_filter__("f64_10k_128","f64_5k_128"); break;
				case CHAMP_RATE_50KHZ:  __set_filter__("f64_50k_128","f64_25k_128");  break;
				case CHAMP_RATE_100KHZ: __set_filter__("f64_100k_128","f64_50k_128"); break;
				default: break;
			}
			break;

		case 256:
			switch(m_ui32PhysicalSampleRate)
			{
				case CHAMP_RATE_10KHZ:  __set_filter__("f64_10k_256","f64_5k_256");  break;
				case CHAMP_RATE_50KHZ:  __set_filter__("f64_50k_256","f64_25k_256");  break;
				case CHAMP_RATE_100KHZ: __set_filter__("f64_100k_256","f64_50k_256"); break;
				default: break;
			}
			break;

		case 512:
			switch(m_ui32PhysicalSampleRate)
			{
				case CHAMP_RATE_10KHZ:  __set_filter__("f64_10k_512","f64_5k_512");  break;
				case CHAMP_RATE_50KHZ:  __set_filter__("f64_50k_512","f64_25k_512");  break;
				case CHAMP_RATE_100KHZ: __set_filter__("f64_100k_512","f64_50k_512"); break;
				default: break;
			}
			break;

		case 1024:
			switch(m_ui32PhysicalSampleRate)
			{
				case CHAMP_RATE_10KHZ:  __set_filter__("f64_10k_1024","f64_5k_1024");  break;
				case CHAMP_RATE_50KHZ:  __set_filter__("f64_50k_1024","f64_25k_1024");  break;
				case CHAMP_RATE_100KHZ: __set_filter__("f64_100k_1024","f64_50k_1024"); break;
				default: break;
			}
			break;

		case 2048:
			switch(m_ui32PhysicalSampleRate)
			{
				case CHAMP_RATE_10KHZ:  __set_filter__("f64_10k_2048","f64_5k_2048");  break;
				case CHAMP_RATE_50KHZ:  __set_filter__("f64_50k_2048","f64_25k_2048");  break;
				case CHAMP_RATE_100KHZ: __set_filter__("f64_100k_2048","f64_50k_2048"); break;
				default: break;
			}
			break;

		case 4096:
			switch(m_ui32PhysicalSampleRate)
			{
				case CHAMP_RATE_10KHZ:  __set_filter__("f64_10k_4096","f64_5k_4096");  break;
				case CHAMP_RATE_50KHZ:  __set_filter__("f64_50k_4096","f64_25k_4096");  break;
				case CHAMP_RATE_100KHZ: __set_filter__("f64_100k_4096","f64_50k_4096"); break;
				default: break;
			}
			break;

		default:
			break;
	}

#if 0
	__set_filter__("f64_none");
	__set_filter__("f64_10k_12");
	__set_filter__("f64_10k_256");
#endif

#undef __set_filter__

	if(!l_bValid)
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "Unhandled soft sampling frequency / physical sampling frequency configuration [" << m_oHeader.getSamplingFrequency() << ", " << m_ui32PhysicalSampleRate << "]\n";
		::champClose(m_pHandle);
		m_pHandle=NULL;
		return false;
	}

	// Prepares cache for filtering
	m_vSampleCache.clear();
	for(i=0; i<m_vFilter.size(); i++)
	{
		m_vSampleCache.push_back(m_vSample);
	}

	m_pCallback=&rCallback;
	m_ui64Counter=0;
	
	// Setting the inner latency as described in the beginning of the file
	m_i64DriftOffsetSampleCount=int64(m_oHeader.getSamplingFrequency()*50)/1000;
	m_rDriverContext.setInnerLatencySampleCount(-m_i64DriftOffsetSampleCount);
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Driver inner latency set to 50ms.\n";

	if(m_rDriverContext.isImpedanceCheckRequested())
	{
		// Sets settings manually for impedance check
		t_champSettingsEx l_oSettings;
		l_oSettings.Mode=CHAMP_MODE_IMPEDANCE;
		l_oSettings.Rate=CHAMP_RATE_10KHZ;
		l_oSettings.AdcFilter=CHAMP_ADC_NATIVE;
		l_oSettings.Decimation=CHAMP_DECIMATION_0;
		if(::champSetSettingsEx(m_pHandle, &l_oSettings))
		{
			m_rDriverContext.getLogManager() << LogLevel_Error << "Could not set settings - Got error [" << getError() << "]\n";
			::champClose(m_pHandle);
			m_pHandle=NULL;
			return false;
		}
		m_rDriverContext.getLogManager() << LogLevel_Trace << "Impedance check is requested. Setting the acquisition to [CHAMP_MODE_IMPEDANCE].\n";
		
		// Sets impedance setup manually
		t_champImpedanceSetup l_oImpedanceSetup;
		l_oImpedanceSetup.Good=m_ui32GoodImpedanceLimit;// 5000; // 5kOhm
		l_oImpedanceSetup.Bad=m_ui32BadImpedanceLimit; //10000; // 10kOhm
		l_oImpedanceSetup.LedsDisable=0;
		l_oImpedanceSetup.TimeOut=5;
		if(::champImpedanceSetSetup(m_pHandle, &l_oImpedanceSetup))
		{
			m_rDriverContext.getLogManager() << LogLevel_Error << "Could not set impedance setup - Got error [" << getError() << "]\n";
			::champClose(m_pHandle);
			m_pHandle=NULL;
			return false;
		}
		m_rDriverContext.getLogManager() << LogLevel_Trace << "Impedance limits are set to [GREEN < "<<m_ui32GoodImpedanceLimit<<"Ohm < YELLOW < "<<m_ui32BadImpedanceLimit<<"Ohm < RED].\n";
		
		// Gets/Sets impedance mode
		t_champImpedanceMode l_oImpedanceMode;
		if(::champImpedanceGetMode(m_pHandle, &l_oImpedanceMode))
		{
			m_rDriverContext.getLogManager() << LogLevel_Error << "Could not get impedance mode - Got error [" << getError() << "]\n";
			::champClose(m_pHandle);
			m_pHandle=NULL;
			return false;
		}
		
		l_oImpedanceMode.Splitter=l_oImpedanceMode.Splitters;
		if(::champImpedanceSetMode(m_pHandle, &l_oImpedanceMode))
		{
			m_rDriverContext.getLogManager() << LogLevel_Error << "Could not set impedance mode - Got error [" << getError() << "]\n";
			::champClose(m_pHandle);
			m_pHandle=NULL;
			return false;
		}
		m_rDriverContext.getLogManager() << LogLevel_Trace << "Impedance splitters set to [" << uint32(l_oImpedanceMode.Splitters) << "]\n";

		// Starts impedance check
		if(::champStart(m_pHandle))
		{
			m_rDriverContext.getLogManager() << LogLevel_Error << "Can not start device id " << m_ui32DeviceId << " - Got error [" << getError() << "]\n";
			::champClose(m_pHandle);
			m_pHandle=NULL;
			return false;
		}
		m_rDriverContext.getLogManager() << LogLevel_Trace << "Impedance check started...\n";
	}

	return true;
}

boolean CDriverBrainProductsActiCHamp::start(void)
{
	if(!m_rDriverContext.isConnected()) { return false; }
	if(m_rDriverContext.isStarted()) { return false; }

	if(m_rDriverContext.isImpedanceCheckRequested())
	{
		::champStop(m_pHandle);
	}

	// Sets settings to acquisition state
	t_champSettingsEx l_oSettings;
	l_oSettings.Mode=t_champMode(m_ui32Mode);
	l_oSettings.Rate=t_champRate(m_ui32PhysicalSampleRate);
	l_oSettings.AdcFilter=t_champAdcFilter(m_ui32ADCDataFilter);
	l_oSettings.Decimation=t_champDecimation(m_ui32ADCDataDecimation);
	if(::champSetSettingsEx(m_pHandle, &l_oSettings))
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "Could not set settings - Got error [" << getError() << "]\n";
		return false;
	}
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Acquisition Settings set to [Mode: "<<m_ui32Mode<<"] [PhysicalSamplingRate: "<<m_ui32PhysicalSampleRate<<"] [ADCDataFilter: "<<m_ui32ADCDataFilter<<"] [ADCDataDecimation: "<<m_ui32ADCDataDecimation<<"]\n";

	// Starts acquisition
	if(::champStart(m_pHandle))
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "Can not start device id " << m_ui32DeviceId << " - Got error [" << getError() << "]\n";
		return false;
	}

	m_rDriverContext.getLogManager() << LogLevel_Trace << "Acquisition started...\n";

	return true;
}

boolean CDriverBrainProductsActiCHamp::loop(void)
{
	if(!m_rDriverContext.isConnected()) { return false; }

	if(m_rDriverContext.isStarted())
	{
		t_champDataStatus l_oDataStatus;
		::champGetDataStatus(m_pHandle, &l_oDataStatus);
		//m_rDriverContext.getLogManager() << LogLevel_Trace << "Status : Samples:" << uint32(l_oDataStatus.Samples) << " Errors:" << uint32(l_oDataStatus.Errors) << " Rate:" << float32(l_oDataStatus.Rate) << " Speed:" << float32(l_oDataStatus.Speed) << "\n";

		// Reads all the data. 
		// Buffers are aligned : with one call to champGetData we get as much buffers as possible
		int iCode;
		while((iCode = ::champGetData(m_pHandle, &Buffer[0], m_uiSize*NB_MAX_BUFFER_IN_CACHE)) > 0)
		{
			m_rDriverContext.getLogManager() << LogLevel_Debug << "Received " << ((float32)iCode)/m_uiSize << " new buffers with champGetData.\n";
			
			for(int32 buf=0; buf < (int32)(iCode/m_uiSize); buf++)
			{
				uint32 i, j=0;

				// we find the good pointers (m_pEEG and m_pAux being on the first buffer)
				// Size of an union in memory is the size of the biggest possible type
				signed int * l_pAux = m_pAux+buf*(m_uiSize/sizeof(signed int));
				signed int * l_pEEG = m_pEEG+buf*(m_uiSize/sizeof(signed int));
				// m_rDriverContext.getLogManager() << LogLevel_Debug << "AUX ptr = " << (uint32)l_pAux << " EEG ptr = "<< (uint32)l_pEEG <<"\n";
				
				// Scales data according to respective resolution
				for(i=0; i<m_ui32CountEEG; i++, j++) m_vSample[j]=m_vResolution[j]*l_pEEG[i];
				if(m_bUseAuxChannels)
				{
					for(i=0; i<m_ui32CountAux; i++, j++) m_vSample[j]=m_vResolution[j]*l_pAux[i];
				}
				//m_rDriverContext.getLogManager() << LogLevel_Debug << "Samples saved in cache.\n";

				// Updates cache
				//m_vSampleCache.erase(m_vSampleCache.begin());
				m_vSampleCache.pop_front();
				m_vSampleCache.push_back(m_vSample);
				m_ui64SampleCount++;

				// Downsamples and sends samples only when relevant
				m_ui64Counter+=m_ui64CounterStep;
				if(m_ui64Counter>=(1LL<<32))
				{
					//m_rDriverContext.getLogManager() << LogLevel_Trace << "Enough samples received to build a new buffer. Applying FIR filter.\n";
					m_ui64Counter-=(1LL<<32);

					// Filters last samples
					std::deque<std::vector < OpenViBE::float32 > >::iterator it;
					for(i=0; i<m_ui32ChannelCount; i++)
					{
						m_vSample[i]=0;
						it = m_vSampleCache.begin();
						for(j=0; j<m_vFilter.size(); j++)
						{
							//m_vSample[i]+=m_vFilter[j]*m_vSampleCache[j][i];
							m_vSample[i]+=(float32)(m_vFilter[j]*(*it)[i]);
							it++;
						}
					}

					//m_rDriverContext.getLogManager() << LogLevel_Trace << "Samples ready for sending.\n";
					m_pCallback->setSamples(&m_vSample[0], 1);
					m_pCallback->setStimulationSet(m_oStimulationSet);
					m_oStimulationSet.clear();

					// Saves the date of this sample for further use with the triggers
					m_ui64LastSampleDate += (uint64)((1LL<<32)/m_oHeader.getSamplingFrequency()) ;
					m_ui64SampleCount = 0;
					//m_rDriverContext.getLogManager() << LogLevel_Debug << "Sending one sample.\n";
				}

				//m_rDriverContext.getLogManager() << LogLevel_Debug << "Getting trigger...\n";

				// we save the MyButton state and detect if it switched
				boolean l_bSwitch = m_bMyButtonstate;
				unsigned int * l_pTrigger = m_pTriggers+buf*(m_uiSize/sizeof(unsigned int));
				m_bMyButtonstate = ((*l_pTrigger & 0x80000000) >> 31) == 1;
				l_bSwitch = (l_bSwitch ? !m_bMyButtonstate : m_bMyButtonstate);

				// Decomposes the trigger bytes:
				// Digital inputs (bits 0 - 7) + output (bits 8 - 15) state + 15 MSB reserved bits + MyButton bit (bit 31)
				// apply a mask to silence any modification on the output and reserved parts
				(*l_pTrigger) &= 0x800000ff;
			

				// Now we can test for modification on the interesting part (input triggers and MyButton)
				if(*l_pTrigger != m_uiLastTriggers)
				{
					m_rDriverContext.getLogManager() << LogLevel_Trace << "Current input triggers state [";
					m_rDriverContext.getLogManager() << ((*l_pTrigger & 0x00000001)) << " ";
					m_rDriverContext.getLogManager() << ((*l_pTrigger & 0x00000002) >> 1) << " ";
					m_rDriverContext.getLogManager() << ((*l_pTrigger & 0x00000004) >> 2) << " ";
					m_rDriverContext.getLogManager() << ((*l_pTrigger & 0x00000008) >> 3) << " ";
					m_rDriverContext.getLogManager() << ((*l_pTrigger & 0x00000010) >> 4) << " ";
					m_rDriverContext.getLogManager() << ((*l_pTrigger & 0x00000020) >> 5) << " ";
					m_rDriverContext.getLogManager() << ((*l_pTrigger & 0x00000040) >> 6) << " ";
					m_rDriverContext.getLogManager() << ((*l_pTrigger & 0x00000080) >> 7) << "] MyButton [";
					m_rDriverContext.getLogManager() << m_bMyButtonstate << "]\n";
				
					// Every time that a trigger has changed, we send a stimulation.
					// code of stimulation = OVTK_StimulationId_LabelStart + value of the trigger bytes.
					// This supposes that the user knows the structure of the triggers bit by bit and can process it in a Lua box for example.
					// The value received is in [0-255]
					// The MyButton bit is set to 0 to ensure this range.
				

					// The date is relative to the last buffer start time (cf the setSamples before setStimulationSet)
					uint64 l_ui64Date = m_ui64SampleCount * (1LL<<32) / m_ui32PhysicalSampleRateHz;
					m_oStimulationSet.appendStimulation(OVTK_StimulationId_Label((*l_pTrigger)&0x000000ff), l_ui64Date, 0);

					// special case : MyButton
					// The MyButton trigger bit is too far in the trigger bytes to have a valid value
					// We use 2 dedicated stims OVTK_StimulationId_Button1_Pressed and OVTK_StimulationId_Button1_Released
					if(l_bSwitch)
					{
						m_oStimulationSet.appendStimulation((m_bMyButtonstate ? OVTK_StimulationId_Button1_Pressed : OVTK_StimulationId_Button1_Released), l_ui64Date, 0);
					}
				}
				m_uiLastTriggers = * l_pTrigger;
			}
		}

		// As described in at the begining of this file, the drift
		// correction process is altered to consider the ~ 50ms delay
		// of the single way filtering process
		// Inner latency was set accordingly.
		m_rDriverContext.correctDriftSampleCount(m_rDriverContext.getSuggestedDriftCorrectionSampleCount());
	}
	else
	{
		if(m_rDriverContext.isImpedanceCheckRequested())
		{
			// Reads impedances
			if(::champImpedanceGetData(m_pHandle, &m_vImpedance[0], m_vImpedance.size()*sizeof(unsigned int)))
			{
				m_rDriverContext.getLogManager() << LogLevel_Warning << "Can not read impedances on device id " << m_ui32DeviceId << " - Got error [" << getError() << "]\n";
			}
			else
			{
				// Updates impedances
				m_rDriverContext.getLogManager() << LogLevel_Trace << "Impedances are [ ";
				for(uint32 j=0; j<m_ui32CountEEG; j++)
				{
					m_rDriverContext.updateImpedance(j, m_vImpedance[j]);
					m_rDriverContext.getLogManager() << uint32(m_vImpedance[j]) << " ";
				}
				m_rDriverContext.getLogManager() << "\n";

				// Drops data
				while((::champGetData(m_pHandle, &Buffer, m_uiSize)) > 0);
			}
		}
	}

	return true;
}

boolean CDriverBrainProductsActiCHamp::stop(void)
{
	if(!m_rDriverContext.isConnected()) { return false; }
	if(!m_rDriverContext.isStarted()) { return false; }

	if(!m_rDriverContext.isImpedanceCheckRequested())
	{
		::champStop(m_pHandle);
	}

	return true;
}

boolean CDriverBrainProductsActiCHamp::uninitialize(void)
{
	if(!m_rDriverContext.isConnected()) { return false; }
	if(m_rDriverContext.isStarted()) { return false; }

	if(m_rDriverContext.isImpedanceCheckRequested())
	{
		::champStop(m_pHandle);
	}

	::champClose(m_pHandle);
	m_pHandle=NULL;

	return true;
}

//___________________________________________________________________//
//                                                                   //

boolean CDriverBrainProductsActiCHamp::isConfigurable(void)
{
	return true;
}

boolean CDriverBrainProductsActiCHamp::configure(void)
{
	CConfigurationBrainProductsActiCHamp m_oConfiguration(OpenViBE::Directories::getDataDir() + "/openvibe-applications/acquisition-server/interface-BrainProducts-ActiCHamp.ui",
		m_ui32DeviceId,
		m_ui32Mode,
		m_ui32PhysicalSampleRate,
		m_ui32ADCDataFilter,
		m_ui32ADCDataDecimation,
		m_ui32ActiveShieldGain,
		m_ui32ModuleEnabled,
		m_bUseAuxChannels,
		m_ui32GoodImpedanceLimit,
		m_ui32BadImpedanceLimit);
	return m_oConfiguration.configure(m_oHeader);
}

//___________________________________________________________________//
//                                                                   //

CString CDriverBrainProductsActiCHamp::getError(int* pCode)
{
	int l_iCode=0;
	int* l_pCode=(pCode?pCode:&l_iCode);
	if(!m_pHandle) return "";
	char l_sMessage[1024];
	l_sMessage[0]='\0';
	::champGetError(m_pHandle, l_pCode, l_sMessage, sizeof(l_sMessage));
	return l_sMessage;
}

#endif // TARGET_HAS_ThirdPartyActiCHampAPI
