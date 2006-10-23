#ifndef __OpenViBEKernel_Kernel_CBoxContext_H__
#define __OpenViBEKernel_Kernel_CBoxContext_H__

#include "ovkTPluginObjectContext.h"
#include "simulated-objects/ovkPsSimulatedBox.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class CBoxContext : virtual public OpenViBE::Kernel::TPluginObjectContext<OpenViBE::Plugins::IBoxContext>
		{
		public:

			class CInput : virtual public OpenViBE::Kernel::TPluginObjectContext<OpenViBE::Plugins::IBoxContext::IInput>
			{
			public:
				CInput(::PsSimulatedBox& rSimulatedBox, const OpenViBE::Kernel::IBox& rBox, const uint32 ui32InputIndex);

				virtual OpenViBE::CString getName(void);
				virtual OpenViBE::CIdentifier getTypeIdentifier(void);

				virtual OpenViBE::uint32 getChunkCount(void);
				virtual OpenViBE::boolean getChunk(
					const OpenViBE::uint32 ui32Index,
					OpenViBE::uint64& rStartTime,
					OpenViBE::uint64& rEndTime,
					OpenViBE::uint64& rChunkSize,
					const OpenViBE::uint8*& rpChunkBuffer);
				virtual OpenViBE::boolean markAsDeprecated(
					const OpenViBE::uint32 ui32Index);
#if 0
				virtual OpenViBE::boolean getSource(
					OpenViBE::CIdentifier& rBoxIdentifier,
					OpenViBE::uint32& rBoxOutputIndex);
#endif

				virtual OpenViBE::boolean setInputIndex(
					const OpenViBE::uint32 ui32InputIndex);

				_IsDerivedFromClass_Final_(OpenViBE::Kernel::TPluginObjectContext<OpenViBE::Plugins::IBoxContext::IInput>, OVK_ClassId_Kernel_BoxContextInput)

			protected:

				::PsSimulatedBox& m_rSimulatedBox;
				const OpenViBE::Kernel::IBox& m_rBox;
				OpenViBE::uint32 m_ui32InputIndex;
			};

			class COutput : virtual public OpenViBE::Kernel::TPluginObjectContext<OpenViBE::Plugins::IBoxContext::IOutput>
			{
			public:
				COutput(::PsSimulatedBox& rSimulatedBox, const OpenViBE::Kernel::IBox& rBox, const uint32 ui32OutputIndex);

				virtual OpenViBE::CString getName(void);
				virtual OpenViBE::CIdentifier getTypeIdentifier(void);

				virtual OpenViBE::uint64 getChunkSize(void);
				virtual OpenViBE::boolean setChunkSize(
					const OpenViBE::uint64 ui64Size,
					const OpenViBE::boolean bDiscard=true);
				virtual OpenViBE::uint8* getChunkBuffer(void);
				virtual OpenViBE::boolean markAsReadyToSend(
					const OpenViBE::uint64 ui64StartTime,
					const OpenViBE::uint64 ui64EndTime);
#if 0
				virtual OpenViBE::uint32 getTargetCount(void);
				virtual OpenViBE::boolean getTarget(
					const OpenViBE::uint32 ui32TargetIndex,
					OpenViBE::CIdentifier& rBoxIdentifier,
					OpenViBE::uint32& rBoxInputIndex);
#endif

				virtual OpenViBE::boolean setOutputIndex(
					const OpenViBE::uint32 ui32OutputIndex);

				_IsDerivedFromClass_Final_(OpenViBE::Kernel::TPluginObjectContext<OpenViBE::Plugins::IBoxContext::IOutput>, OVK_ClassId_Kernel_BoxContextOutput)

			protected:

				::PsSimulatedBox& m_rSimulatedBox;
				const OpenViBE::Kernel::IBox& m_rBox;
				OpenViBE::uint32 m_ui32OutputIndex;
			};

			CBoxContext(::PsSimulatedBox& rSimulatedBox, const OpenViBE::Kernel::IBox& rBox);

			virtual OpenViBE::CIdentifier getIdentifier(void) const;
			virtual OpenViBE::CString getName(void) const;

			virtual OpenViBE::uint32 getInputCount(void) const;
			virtual OpenViBE::Plugins::IBoxContext::IInput* getInput(
				const OpenViBE::uint32 ui32InputIndex);

			virtual OpenViBE::uint32 getOutputCount(void) const;
			virtual OpenViBE::Plugins::IBoxContext::IOutput* getOutput(
				const OpenViBE::uint32 ui32OutputIndex);

			virtual OpenViBE::uint32 getSettingCount(void) const;
			virtual OpenViBE::boolean getSettingType(
				const OpenViBE::uint32 ui32SettingIndex,
				OpenViBE::CIdentifier& rTypeIdentifier) const;
			virtual OpenViBE::boolean getSettingValue(
				const OpenViBE::uint32 ui32SettingIndex,
				OpenViBE::CString& rValue) const;

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TPluginObjectContext<OpenViBE::Plugins::IBoxContext>, OVK_ClassId_Kernel_BoxContext)

		protected:

			::PsSimulatedBox& m_rSimulatedBox;
			const OpenViBE::Kernel::IBox& m_rBox;
			OpenViBE::Kernel::CBoxContext::CInput m_oInput;
			OpenViBE::Kernel::CBoxContext::COutput m_oOutput;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_CBoxContext_H__
