#ifndef __OpenViBEKernel_Kernel_Scenario_CBoxListenerContext_H__
#define __OpenViBEKernel_Kernel_Scenario_CBoxListenerContext_H__

#include "../ovkTKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class CBoxListenerContext : public OpenViBE::Kernel::TKernelObject < OpenViBE::Kernel::IBoxListenerContext >
		{
		public:

			class CLogManagerBridge : public TKernelObject<ILogManager>
			{
			public:

				CLogManagerBridge(const IKernelContext& rKernelContext, const IBox& rBox) : TKernelObject<ILogManager>(rKernelContext), m_rBox(rBox) { }

				virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), void, log, , const uint64, ui64Value)
				virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), void, log, , const uint32, ui32Value)
				virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), void, log, , const uint16, ui16Value)
				virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), void, log, , const uint8, ui8Value)

				virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), void, log, , const int64, i64Value)
				virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), void, log, , const int32, i32Value)
				virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), void, log, , const int16, i16Value)
				virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), void, log, , const int8, i8Value)

				virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), void, log, , const float32, f32Value)
				virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), void, log, , const float64, f64Value)

				virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), void, log, , const boolean, bValue)

				virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), void, log, , const CIdentifier&, rValue)
				virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), void, log, , const CString&, rValue);
				virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), void, log, , const char*, rValue);

				// virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), void, log, , const ELogLevel, eLogLevel)
				virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), void, log, , const ELogColor, eLogColor)

				virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), boolean, addListener, , ILogListener*, pListener)
				virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), boolean, removeListener, , ILogListener*, pListener)

				virtual __BridgeBindFunc1__(getKernelContext().getLogManager(), boolean, isActive, , ELogLevel, eLogLevel)
				virtual __BridgeBindFunc2__(getKernelContext().getLogManager(), boolean, activate, , ELogLevel, eLogLevel, boolean, bActive)

				virtual void log(const ELogLevel eLogLevel)
				{
					getKernelContext().getLogManager()
						<< eLogLevel
						<< "<"
						<< LogColor_PushStateBit
						<< LogColor_ForegroundBlue
						<< "Box listener"
						<< LogColor_PopStateBit
						<< "::"
						<< m_rBox.getName()
						<< "> ";
				}

				_IsDerivedFromClass_Final_(TKernelObject<ILogManager>, OV_UndefinedIdentifier);

			protected:

				const IBox& m_rBox;
			};

			CBoxListenerContext(const OpenViBE::Kernel::IKernelContext& rKernelContext, OpenViBE::Kernel::IBox& rBox, OpenViBE::uint32 ui32Index) : TKernelObject<IBoxListenerContext>(rKernelContext), m_oLogManager(rKernelContext, rBox), m_rBox(rBox), m_ui32Index(ui32Index) {}

			virtual OpenViBE::Kernel::IAlgorithmManager& getAlgorithmManager(void) const { return this->getKernelContext().getAlgorithmManager(); }
			virtual OpenViBE::Kernel::IPlayerManager& getPlayerManager(void) const { return this->getKernelContext().getPlayerManager(); }
			virtual OpenViBE::Kernel::IPluginManager& getPluginManager(void) const { return this->getKernelContext().getPluginManager(); }
			virtual OpenViBE::Kernel::IScenarioManager& getScenarioManager(void) const { return this->getKernelContext().getScenarioManager(); }
			virtual OpenViBE::Kernel::ITypeManager& getTypeManager(void) const { return this->getKernelContext().getTypeManager(); }
			virtual OpenViBE::Kernel::ILogManager& getLogManager(void) const { return m_oLogManager; }
			virtual OpenViBE::Kernel::IVisualisationManager& getVisualisationManager(void) const { return this->getKernelContext().getVisualisationManager(); }

			virtual OpenViBE::Kernel::IBox& getBox(void) const { return m_rBox; }
			virtual OpenViBE::Kernel::IScenario& getScenario(void) const
			{
				this->getKernelContext().getLogManager() << OpenViBE::Kernel::LogLevel_Fatal << "Getting scenario from box listener context is not yet implemented\n"; // $$$
				return *((IScenario*)NULL);
			}
			virtual OpenViBE::uint32 getIndex(void) const { return m_ui32Index; }

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::IKernelObject, OVK_ClassId_Kernel_Scenario_BoxListenerContext)

		protected:

			mutable CLogManagerBridge m_oLogManager;
			OpenViBE::Kernel::IBox& m_rBox;
			OpenViBE::uint32 m_ui32Index;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_Scenario_CBoxListenerContext_H__
