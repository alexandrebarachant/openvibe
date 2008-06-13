#ifndef __OpenViBEKernel_Kernel_Player_CSimulatedBox_H__
#define __OpenViBEKernel_Kernel_Player_CSimulatedBox_H__

#include "../ovkTKernelObject.h"
#include "ovkCBuffer.h"

#include <system/CChrono.h>

#include <vector>
#include <string>
#include <list>

namespace OpenViBE
{
	namespace Kernel
	{
		class CScheduler;

		class CChunk
		{
		public:

			CChunk(void)
				:m_ui64StartTime(0)
				,m_ui64EndTime(0)
				,m_bIsDeprecated(false)
			{
			}

			CChunk(const OpenViBE::Kernel::CChunk& rChunk)
				:m_oBuffer(rChunk.m_oBuffer)
				,m_ui64StartTime(rChunk.m_ui64StartTime)
				,m_ui64EndTime(rChunk.m_ui64EndTime)
				,m_bIsDeprecated(false)
			{
			}

			const OpenViBE::Kernel::CBuffer& getBuffer(void) const
			{
				return m_oBuffer;
			}

			OpenViBE::uint64 getStartTime(void) const
			{
				return m_ui64StartTime;
			}

			OpenViBE::uint64 getEndTime(void) const
			{
				return m_ui64EndTime;
			}

			OpenViBE::boolean isDeprecated(void) const
			{
				return m_bIsDeprecated;
			}

			OpenViBE::Kernel::CBuffer& getBuffer(void)
			{
				return m_oBuffer;
			}

			OpenViBE::boolean setStartTime(OpenViBE::uint64 ui64StartTime)
			{
				m_ui64StartTime=ui64StartTime;
				return true;
			}

			OpenViBE::boolean setEndTime(OpenViBE::uint64 ui64EndTime)
			{
				m_ui64EndTime=ui64EndTime;
				return true;
			}

			OpenViBE::boolean markAsDeprecated(OpenViBE::boolean bIsDeprecated)
			{
				m_bIsDeprecated=bIsDeprecated;
				return true;
			}

		protected:

			OpenViBE::Kernel::CBuffer m_oBuffer;
			OpenViBE::uint64 m_ui64StartTime;
			OpenViBE::uint64 m_ui64EndTime;
			OpenViBE::boolean m_bIsDeprecated;
		};

		class CSimulatedBox : public OpenViBE::Kernel::TKernelObject < OpenViBE::Kernel::IBoxIO >
		{
		public:

			CSimulatedBox(const OpenViBE::Kernel::IKernelContext& rKernelContext, OpenViBE::Kernel::CScheduler& rScheduler);
			virtual ~CSimulatedBox() ;

			virtual OpenViBE::boolean setScenarioIdentifier(const OpenViBE::CIdentifier& rScenarioIdentifier);
			virtual OpenViBE::boolean setBoxIdentifier(const OpenViBE::CIdentifier& rBoxIdentifier);

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);

			virtual OpenViBE::boolean processClock(void);
			virtual OpenViBE::boolean processInput(const OpenViBE::uint32 ui32InputIndex, const OpenViBE::Kernel::CChunk& rChunk);
			virtual OpenViBE::boolean process(void);
			virtual OpenViBE::boolean isReadyToProcess(void) const;

			virtual OpenViBE::uint64 getCurrentTime() const;
			virtual OpenViBE::CString getName(void) const;
			virtual const OpenViBE::Kernel::IScenario& getScenario(void) const;

			// IBoxIO - inputs
			virtual OpenViBE::uint32 getInputChunkCount(
				const OpenViBE::uint32 ui32InputIndex) const;
			virtual OpenViBE::boolean getInputChunk(
				const OpenViBE::uint32 ui32InputIndex,
				const OpenViBE::uint32 ui32ChunkIndex,
				OpenViBE::uint64& rStartTime,
				OpenViBE::uint64& rEndTime,
				OpenViBE::uint64& rChunkSize,
				const OpenViBE::uint8*& rpChunkBuffer) const;
			virtual const OpenViBE::IMemoryBuffer* getInputChunk(
				const OpenViBE::uint32 ui32InputIndex,
				const OpenViBE::uint32 ui32ChunkIndex) const;
			virtual OpenViBE::uint64 getInputChunkStartTime(
				const OpenViBE::uint32 ui32InputIndex,
				const OpenViBE::uint32 ui32ChunkIndex) const;
			virtual OpenViBE::uint64 getInputChunkEndTime(
				const OpenViBE::uint32 ui32InputIndex,
				const OpenViBE::uint32 ui32ChunkIndex) const;
			virtual OpenViBE::boolean markInputAsDeprecated(
				const OpenViBE::uint32 ui32InputIndex,
				const OpenViBE::uint32 ui32ChunkIndex);

			// IBoxIO - outputs
			virtual OpenViBE::uint64 getOutputChunkSize(
				const OpenViBE::uint32 ui32OutputIndex) const;
			virtual OpenViBE::boolean setOutputChunkSize(
				const OpenViBE::uint32 ui32OutputIndex,
				const OpenViBE::uint64 ui64Size,
				const OpenViBE::boolean bDiscard=true);
			virtual OpenViBE::uint8* getOutputChunkBuffer(
				const OpenViBE::uint32 ui32OutputIndex);
			virtual OpenViBE::boolean appendOutputChunkData(
				const OpenViBE::uint32 ui32OutputIndex,
				const OpenViBE::uint8* pBuffer,
				const OpenViBE::uint64 ui64BufferSize);
			virtual OpenViBE::IMemoryBuffer* getOutputChunk(
				const OpenViBE::uint32 ui32OutputIndex);
			virtual OpenViBE::boolean markOutputAsReadyToSend(
				const OpenViBE::uint32 ui32OutputIndex,
				const OpenViBE::uint64 ui64StartTime,
				const OpenViBE::uint64 ui64EndTime);

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TKernelObject < OpenViBE::Kernel::IBoxIO >, OVK_ClassId_Kernel_Player_SimulatedBox);

		private:

			virtual void handleCrash(const char* sHintName="");

		protected:

			OpenViBE::uint32 m_ui32CrashCount;
			OpenViBE::boolean m_bReadyToProcess;
			OpenViBE::boolean m_bActive;

			OpenViBE::Plugins::IBoxAlgorithm* m_pBoxAlgorithm;
			const OpenViBE::Kernel::IScenario* m_pScenario;
			const OpenViBE::Kernel::IBox* m_pBox;
			OpenViBE::Kernel::CScheduler& m_rScheduler;

			OpenViBE::uint64 m_ui64LastClockActivationDate;
			OpenViBE::uint64 m_ui64ClockFrequency;
			OpenViBE::uint64 m_ui64ClockActivationStep;

		private:

			System::CChrono m_oBenchmarkChronoProcessClock;
			System::CChrono m_oBenchmarkChronoProcessInput;
			System::CChrono m_oBenchmarkChronoProcess;

		public:

			std::vector < std::list < OpenViBE::Kernel::CChunk > > m_vInput;
			std::vector < std::list < OpenViBE::Kernel::CChunk > > m_vOutput;
			std::vector < OpenViBE::Kernel::CChunk > m_vCurrentOutput;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_Player_CSimulatedBox_H__
