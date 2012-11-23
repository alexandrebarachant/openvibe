#ifndef __OpenViBE_AcquisitionServer_CDriverBrainProductsActiCHamp_H__
#define __OpenViBE_AcquisitionServer_CDriverBrainProductsActiCHamp_H__

#if defined TARGET_HAS_ThirdPartyActiCHampAPI

#include "../ovasIDriver.h"
#include "../ovasCHeader.h"

#include <deque>

namespace OpenViBEAcquisitionServer
{
	class CDriverBrainProductsActiCHamp : public OpenViBEAcquisitionServer::IDriver
	{
	public:

		CDriverBrainProductsActiCHamp(OpenViBEAcquisitionServer::IDriverContext& rDriverContext);
		virtual void release(void);
		virtual const char* getName(void);

		virtual OpenViBE::boolean initialize(
			const OpenViBE::uint32 ui32SampleCountPerSentBlock,
			OpenViBEAcquisitionServer::IDriverCallback& rCallback);
		virtual OpenViBE::boolean uninitialize(void);

		virtual OpenViBE::boolean start(void);
		virtual OpenViBE::boolean stop(void);
		virtual OpenViBE::boolean loop(void);

		virtual OpenViBE::boolean isConfigurable(void);
		virtual OpenViBE::boolean configure(void);
		virtual const OpenViBEAcquisitionServer::IHeader* getHeader(void) { return &m_oHeader; }

		OpenViBE::CString getError(int* pCode=NULL);

	protected:

		void* m_pHandle;

		OpenViBEAcquisitionServer::IDriverCallback* m_pCallback;
		OpenViBEAcquisitionServer::CHeader m_oHeader;

		std::vector < unsigned int > m_vImpedance;
		std::vector < OpenViBE::float32 > m_vSample;
		std::vector < OpenViBE::float32 > m_vResolution;
		//std::vector < std::vector < OpenViBE::float32 > > m_vSampleCache;
		std::deque < std::vector < OpenViBE::float32 > > m_vSampleCache;
		OpenViBE::uint32 m_ui32SampleCacheIndex;
		std::vector < OpenViBE::float64 > m_vFilter;

		OpenViBE::CStimulationSet m_oStimulationSet;

		OpenViBE::uint32 m_ui32DeviceId;
		OpenViBE::uint32 m_ui32Mode;
		OpenViBE::uint32 m_ui32PhysicalSampleRate;
		OpenViBE::uint32 m_ui32ADCDataFilter;
		OpenViBE::uint32 m_ui32ADCDataDecimation;
		OpenViBE::uint32 m_ui32ActiveShieldGain;
		OpenViBE::uint32 m_ui32ModuleEnabled;
		OpenViBE::boolean m_bUseAuxChannels;
		
		OpenViBE::uint32 m_ui32PhysicalSampleRateHz;

		OpenViBE::uint32 m_ui32ChannelCount;
		OpenViBE::uint32 m_ui32CountEEG;
		OpenViBE::uint32 m_ui32CountAux;
		OpenViBE::uint32 m_ui32CountTriggersIn;

		OpenViBE::uint64 m_ui64Counter;
		OpenViBE::uint64 m_ui64CounterStep;
		OpenViBE::uint64 m_ui64SampleCount;
		OpenViBE::int64 m_i64DriftOffsetSampleCount;

		OpenViBE::uint32 m_ui32GoodImpedanceLimit;
		OpenViBE::uint32 m_ui32BadImpedanceLimit;

		signed int* m_pAux;
		signed int* m_pEEG;
		unsigned int* m_pTriggers;
		unsigned int m_uiSize;

		unsigned int m_uiLastTriggers;
		OpenViBE::uint64 m_ui64LastSampleDate;

		OpenViBE::boolean m_bMyButtonstate;
	};
};

#endif // TARGET_HAS_ThirdPartyActiCHampAPI

#endif // __OpenViBE_AcquisitionServer_CDriverBrainProductsActiCHamp_H__
