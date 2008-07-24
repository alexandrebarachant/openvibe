#ifndef __OpenViBEToolkit_TBoxAlgorithm_H__
#define __OpenViBEToolkit_TBoxAlgorithm_H__

#include "../ovtkIObject.h"

namespace OpenViBEToolkit
{
	template <class CBoxAlgorithmParentClass>
	class TBoxAlgorithm : public CBoxAlgorithmParentClass
	{
	public:

		TBoxAlgorithm(void)
			:m_pBoxAlgorithmContext(NULL)
		{
		}

// ====================================================================================================================================

	private:

		virtual OpenViBE::uint64 getClockFrequency(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext)
		{
			CScopedBoxAlgorithm l_oScopedBoxAlgorithm(m_pBoxAlgorithmContext, &rBoxAlgorithmContext);
			return getClockFrequency();
		}

		virtual OpenViBE::boolean initialize(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext)
		{
			CScopedBoxAlgorithm l_oScopedBoxAlgorithm(m_pBoxAlgorithmContext, &rBoxAlgorithmContext);
			return initialize();
		}

		virtual OpenViBE::boolean uninitialize(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext)
		{
			CScopedBoxAlgorithm l_oScopedBoxAlgorithm(m_pBoxAlgorithmContext, &rBoxAlgorithmContext);
			return uninitialize();
		}

		virtual OpenViBE::boolean processEvent(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext, OpenViBE::CMessageEvent& rMessageEvent)
		{
			CScopedBoxAlgorithm l_oScopedBoxAlgorithm(m_pBoxAlgorithmContext, &rBoxAlgorithmContext);
			return processEvent(rMessageEvent);
		}

		virtual OpenViBE::boolean processSignal(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext, OpenViBE::CMessageSignal& rMessageSignal)
		{
			CScopedBoxAlgorithm l_oScopedBoxAlgorithm(m_pBoxAlgorithmContext, &rBoxAlgorithmContext);
			return processSignal(rMessageSignal);
		}

		virtual OpenViBE::boolean processClock(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext, OpenViBE::CMessageClock& rMessageClock)
		{
			CScopedBoxAlgorithm l_oScopedBoxAlgorithm(m_pBoxAlgorithmContext, &rBoxAlgorithmContext);
			return processClock(rMessageClock);
		}

		virtual OpenViBE::boolean processInput(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext, OpenViBE::uint32 ui32InputIndex)
		{
			CScopedBoxAlgorithm l_oScopedBoxAlgorithm(m_pBoxAlgorithmContext, &rBoxAlgorithmContext);
			return processInput(ui32InputIndex);
		}

		virtual OpenViBE::boolean process(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext)
		{
			CScopedBoxAlgorithm l_oScopedBoxAlgorithm(m_pBoxAlgorithmContext, &rBoxAlgorithmContext);
			return process();
		}

// ====================================================================================================================================

	public:

		virtual OpenViBE::uint64 getClockFrequency(void)                                  { return 0; }
		virtual OpenViBE::boolean initialize(void)                                        { return true; }
		virtual OpenViBE::boolean uninitialize(void)                                      { return true; }
		virtual OpenViBE::boolean processEvent(OpenViBE::CMessageEvent& rMessageEvent)    { return false; }
		virtual OpenViBE::boolean processSignal(OpenViBE::CMessageSignal& rMessageSignal) { return false; }
		virtual OpenViBE::boolean processClock(OpenViBE::CMessageClock& rMessageClock)    { return false; }
		virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex)           { return false; }
		virtual OpenViBE::boolean process(void)=0;

// ====================================================================================================================================

		virtual OpenViBE::Kernel::IBoxAlgorithmContext* getBoxAlgorithmContext(void) { return m_pBoxAlgorithmContext; }
		// virtual OpenViBE::Kernel::IBoxAlgorithmContext& getBoxAlgorithmContext(void) { return *m_pBoxAlgorithmContext; } this one should replace !
		virtual OpenViBE::Kernel::IBox& getStaticBoxContext(void) { return *m_pBoxAlgorithmContext->getStaticBoxContext(); }
		virtual OpenViBE::Kernel::IBoxIO& getDynamicBoxContext(void) { return *m_pBoxAlgorithmContext->getDynamicBoxContext(); }
		virtual OpenViBE::Kernel::IPlayerContext& getPlayerContext(void) { return *m_pBoxAlgorithmContext->getPlayerContext(); }
		virtual OpenViBE::Kernel::IVisualisationContext& getVisualisationContext(void) { return *m_pBoxAlgorithmContext->getVisualisationContext(); }

		virtual OpenViBE::Kernel::IAlgorithmManager& getAlgorithmManager(void) { return getPlayerContext().getAlgorithmManager(); }
		virtual OpenViBE::Kernel::ILogManager& getLogManager(void) { return getPlayerContext().getLogManager(); }
		virtual OpenViBE::Kernel::IScenarioManager& getScenarioManager(void) { return getPlayerContext().getScenarioManager(); }
		virtual OpenViBE::Kernel::ITypeManager& getTypeManager(void) { return getPlayerContext().getTypeManager(); }

// ====================================================================================================================================

		virtual void appendOutputChunkData(const OpenViBE::uint32 ui32OutputIndex, const void* pBuffer, const OpenViBE::uint64 ui64BufferSize)
		{
			OpenViBE::Kernel::IBoxAlgorithmContext* l_pBoxAlgorithmContext=this->getBoxAlgorithmContext();
			if(l_pBoxAlgorithmContext)
			{
				OpenViBE::Kernel::IBoxIO* l_pDynamicBoxContext=l_pBoxAlgorithmContext->getDynamicBoxContext();
				if(l_pDynamicBoxContext)
				{
					l_pDynamicBoxContext->appendOutputChunkData(ui32OutputIndex, static_cast<const OpenViBE::uint8*>(pBuffer), ui64BufferSize);
				}
			}
		}

		template <OpenViBE::uint32 ui32OutputIndex>
		void appendOutputChunkData(const void* pBuffer, const OpenViBE::uint64 ui64BufferSize)
		{
			appendOutputChunkData(ui32OutputIndex, pBuffer, ui64BufferSize);
		}

		_IsDerivedFromClass_(CBoxAlgorithmParentClass, OVTK_ClassId_);

	private:

		class CScopedBoxAlgorithm
		{
		public:
			CScopedBoxAlgorithm(OpenViBE::Kernel::IBoxAlgorithmContext*& rpBoxAlgorithmContext, OpenViBE::Kernel::IBoxAlgorithmContext* pBoxAlgorithmContext)
				:m_rpBoxAlgorithmContext(rpBoxAlgorithmContext)
			{
				m_rpBoxAlgorithmContext=pBoxAlgorithmContext;
			}
			virtual ~CScopedBoxAlgorithm(void)
			{
				m_rpBoxAlgorithmContext=NULL;
			}
		protected:
			OpenViBE::Kernel::IBoxAlgorithmContext*& m_rpBoxAlgorithmContext;
		};

		OpenViBE::Kernel::IBoxAlgorithmContext* m_pBoxAlgorithmContext;
	};
};

#endif // __OpenViBEToolkit_TBoxAlgorithm_H__
