/*
 * ovas_defines_brainmaster_discovery.h
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

#ifndef __OVAS_DEFINES_BRAINMASTER_DISCOVERY__
#define __OVAS_DEFINES_BRAINMASTER_DISCOVERY__

#if defined TARGET_HAS_ThirdPartyBrainmasterCodeMakerAPI

#include "../ovasIDriver.h"

#include "CMKRDLLU.H"
#include <string>

namespace OpenViBEAcquisitionServer
{
	typedef enum
	{
		Preset_Custom=0,
		Preset_Discovery_24=1,
		Preset_Atlantis_4x4=2,
		Preset_Atlantis_2x2=3,

		Type_Discovery=0,
		Type_Atlantis=1,

		ChannelType_EEG=0,
		ChannelType_AUX=1,

		BaudRate_Default=0,
		BaudRate_9600=1,
		BaudRate_115200=2,
		BaudRate_460800=3,

		NotchFilter_Default=0,
		NotchFilter_Off=1,
		NotchFilter_50=2,
		NotchFilter_60=3,

		BitDepth_Default=0,
		BitDepth_8=1,
		BitDepth_16=2,
		BitDepth_24=3,

		SamplingFrequency_256=0,
		SamplingFrequency_512=1,
		SamplingFrequency_1024=2,
		SamplingFrequency_2048=3,
	} EParameter;

	inline OpenViBE::uint32 autoDetectPort(OpenViBE::uint32 ui32StartPort=1, OpenViBE::uint32 ui32StopPort=16, OpenViBE::uint32 ui32Fallback=OpenViBE::uint32(-1))
	{
		for(OpenViBE::uint32 i=ui32StartPort; i<=ui32StopPort; i++)
		{
			BOOL l_bResult=::AtlOpenPort(i, /*m_ui32BaudRate*/ 0, NULL);
			::AtlClosePort(i);
			if(l_bResult)
			{
				return i;
			}
		}
		return ui32Fallback;
	}

	// returns true on success
	// returns false on error
	inline OpenViBE::boolean checkDeviceSerial(const std::string& sDeviceSerial)
	{
		OpenViBE::uint32 i;

		// Checks length
		if(sDeviceSerial.length()!=5)
		{
			return false;
		}

		// Checks digits
		for(i=0; i<sDeviceSerial.length(); i++)
		{
			if(sDeviceSerial[i]<'0' || sDeviceSerial[i]>'9')
			{
				return false;
			}
		}

		// First digit gives device kind
		switch(sDeviceSerial[0])
		{
			case '3': // Atlantis
			case '4': // Atlantis
			case '6': // Discovery
			default:
				break;
		}

		// Everything's fine
		return true;
	}

	// returns true on success
	// returns false on error
	inline OpenViBE::boolean checkDevicePasskey(const std::string& sDevicePasskey)
	{
		OpenViBE::uint32 i, j, k;

		// Checks length -- other checks are hardcoded assuming length is 15
		if(sDevicePasskey.length()!=14)
		{
			return false;
		}

		// Checks separators
		if(sDevicePasskey[ 4]!='-') return false;
		if(sDevicePasskey[ 9]!='-') return false;

		// Checks alpha-nums
		for(i=0; i<3; i++)
		{
			for(j=0; j<4; j++)
			{
				k=i*5+j;
				if((sDevicePasskey[k]<'0' || sDevicePasskey[k]>'9')
				&& (sDevicePasskey[k]<'a' || sDevicePasskey[k]>'z')
				&& (sDevicePasskey[k]<'A' || sDevicePasskey[k]>'Z'))
				{
					return false;
				}
			}
		}

		// Everything's fine
		return true;
	}
};

#endif // TARGET_HAS_ThirdPartyBrainmasterCodeMakerAPI

#endif // __OVAS_DEFINES_BRAINMASTER_DISCOVERY__
