/*
 * ovasCDriverBrainmasterDiscovery.h
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

#ifndef __OpenViBE_AcquisitionServer_CDriverBrainmasterDiscovery_H__
#define __OpenViBE_AcquisitionServer_CDriverBrainmasterDiscovery_H__

#if defined TARGET_HAS_ThirdPartyBrainmasterCodeMakerAPI

#include "../ovasIDriver.h"
#include "../ovasCHeader.h"

#include <vector>
#include <map>
#include <string>

namespace OpenViBEAcquisitionServer
{
	class CDriverBrainmasterDiscovery : public OpenViBEAcquisitionServer::IDriver
	{
	public:

		CDriverBrainmasterDiscovery(OpenViBEAcquisitionServer::IDriverContext& rDriverContext);
		virtual void release(void);
		virtual const char* getName(void);

		virtual OpenViBE::boolean initialize(
			const OpenViBE::uint32 ui32SampleCountPerSentBlock,
			OpenViBEAcquisitionServer::IDriverCallback& rCallback);
		virtual OpenViBE::boolean uninitialize(void);

		virtual OpenViBE::boolean start(void);
		virtual OpenViBE::boolean stop(void);
		virtual OpenViBE::boolean loop(void);
		virtual OpenViBE::boolean read(unsigned char* pFrame, OpenViBE::uint32 ui32Count);
		virtual OpenViBE::uint32 sync(void);

		virtual OpenViBE::boolean isConfigurable(void);
		virtual OpenViBE::boolean configure(void);
		virtual const OpenViBEAcquisitionServer::IHeader* getHeader(void) { return &m_oHeader; }

	protected:

		OpenViBEAcquisitionServer::IDriverCallback* m_pCallback;
		OpenViBEAcquisitionServer::CHeader m_oHeader;
		OpenViBE::uint32 m_ui32SampleCountPerSentBlock;

		std::string m_sDeviceSerial;
		std::string m_sDevicePasskey;
		std::string m_sFrameDumpFilename;
		OpenViBE::boolean m_bFrameDumpFlag;

		OpenViBE::uint32 m_ui32Port;
		OpenViBE::uint32 m_ui32PortReal;

		OpenViBE::uint32 m_ui32Preset;
		OpenViBE::uint32 m_ui32Type;
		OpenViBE::uint32 m_ui32BaudRate;
		OpenViBE::uint32 m_ui32BaudRateReal;
		OpenViBE::uint32 m_ui32SamplingRate;
		OpenViBE::uint32 m_ui32SamplingRateReal;
		OpenViBE::uint32 m_ui32BitDepth;
		OpenViBE::uint32 m_ui32BitDepthReal;
		OpenViBE::uint32 m_ui32NotchFilters;
		OpenViBE::uint32 m_ui32NotchFiltersReal;

		OpenViBE::uint32 m_ui32FrameSize;
		OpenViBE::uint32 m_ui32DataOffset;

		std::map < OpenViBE::uint32, OpenViBE::uint32 > m_vBaudRate;
		std::map < OpenViBE::uint32, OpenViBE::uint32 > m_vBaudRateValue;
		std::map < OpenViBE::uint32, OpenViBE::uint32 > m_vNotchFilters;
		std::map < OpenViBE::uint32, OpenViBE::uint32 > m_vNotchFiltersValue;
		std::map < OpenViBE::uint32, OpenViBE::uint32 > m_vBitDepth;
		std::map < OpenViBE::uint32, OpenViBE::uint32 > m_vBitDepthValue;
		std::map < OpenViBE::uint32, OpenViBE::uint32 > m_vChannelType;
		std::map < OpenViBE::uint32, OpenViBE::uint32 > m_vChannelSelectionMask;
		std::map < OpenViBE::uint32, std::map < OpenViBE::uint32, OpenViBE::float32 (*)(unsigned char*&) > > m_vBitDepthDecoder;
		std::map < OpenViBE::uint32, int (*)(void) > m_vStartModule;
		std::map < OpenViBE::uint32, int (*)(void) > m_vStopModule;

		std::vector < OpenViBE::float32 > m_vSample;
		std::vector < unsigned char > m_vBuffer;

		OpenViBE::uint32 m_ui32SyncByte;
		OpenViBE::boolean m_bWasLastFrameCorrect;
	};
};

#endif // TARGET_HAS_ThirdPartyBrainmasterCodeMakerAPI

#endif // __OpenViBE_AcquisitionServer_CDriverBrainmasterDiscovery_H__
