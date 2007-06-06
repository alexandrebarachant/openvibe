#ifndef __OpenViBEToolkit_TBoxAlgorithm_H__
#define __OpenViBEToolkit_TBoxAlgorithm_H__

#include "ovtkIObject.h"

namespace OpenViBEToolkit
{
	class OVTK_API IBoxAlgorithmContextHandler : virtual public OpenViBEToolkit::IObject
	{
	public:

		virtual OpenViBE::boolean push(OpenViBE::Kernel::IBoxAlgorithmContext* pBoxAlgorithmContext)=0;
		virtual OpenViBE::boolean pop(void)=0;
		virtual OpenViBE::Kernel::IBoxAlgorithmContext* top(void)=0;

		_IsDerivedFromClass_(OpenViBEToolkit::IObject, OVTK_ClassId_);
	};

	extern OVTK_API OpenViBEToolkit::IBoxAlgorithmContextHandler* createBoxAlgorithmContextHandler(void);
	extern OVTK_API void releaseBoxAlgorithmContextHandler(OpenViBEToolkit::IBoxAlgorithmContextHandler* pBoxAlgorithmContextHandler);

	template <class CBoxAlgorithmParentClass>
	class TBoxAlgorithm : virtual public CBoxAlgorithmParentClass
	{
	public:

		TBoxAlgorithm(void)
		{
			m_pBoxAlgorithmContextHandler=createBoxAlgorithmContextHandler();
		}

		virtual ~TBoxAlgorithm(void)
		{
			releaseBoxAlgorithmContextHandler(m_pBoxAlgorithmContextHandler);
		}

// ====================================================================================================================================

		virtual OpenViBE::uint32 getClockFrequency(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext)
		{
			CScopedBoxAlgorithm l_oScopedBoxAlgorithm(m_pBoxAlgorithmContextHandler, &rBoxAlgorithmContext);
			return getClockFrequency();
		}

		virtual OpenViBE::boolean initialize(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext)
		{
			CScopedBoxAlgorithm l_oScopedBoxAlgorithm(m_pBoxAlgorithmContextHandler, &rBoxAlgorithmContext);
			return initialize();
		}

		virtual OpenViBE::boolean uninitialize(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext)
		{
			CScopedBoxAlgorithm l_oScopedBoxAlgorithm(m_pBoxAlgorithmContextHandler, &rBoxAlgorithmContext);
			return uninitialize();
		}

		virtual OpenViBE::boolean processEvent(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext, OpenViBE::CMessageEvent& rMessageEvent)
		{
			CScopedBoxAlgorithm l_oScopedBoxAlgorithm(m_pBoxAlgorithmContextHandler, &rBoxAlgorithmContext);
			return processEvent(rMessageEvent);
		}

		virtual OpenViBE::boolean processSignal(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext, OpenViBE::CMessageSignal& rMessageSignal)
		{
			CScopedBoxAlgorithm l_oScopedBoxAlgorithm(m_pBoxAlgorithmContextHandler, &rBoxAlgorithmContext);
			return processSignal(rMessageSignal);
		}

		virtual OpenViBE::boolean processClock(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext, OpenViBE::CMessageClock& rMessageClock)
		{
			CScopedBoxAlgorithm l_oScopedBoxAlgorithm(m_pBoxAlgorithmContextHandler, &rBoxAlgorithmContext);
			return processClock(rMessageClock);
		}

		virtual OpenViBE::boolean processInput(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext, OpenViBE::uint32 ui32InputIndex)
		{
			CScopedBoxAlgorithm l_oScopedBoxAlgorithm(m_pBoxAlgorithmContextHandler, &rBoxAlgorithmContext);
			return processInput(ui32InputIndex);
		}

		virtual OpenViBE::boolean process(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext)
		{
			CScopedBoxAlgorithm l_oScopedBoxAlgorithm(m_pBoxAlgorithmContextHandler, &rBoxAlgorithmContext);
			return process();
		}

// ====================================================================================================================================

		virtual OpenViBE::uint32 getClockFrequency(void)                                  { return 0; }
		virtual OpenViBE::boolean initialize(void)                                        { return true; }
		virtual OpenViBE::boolean uninitialize(void)                                      { return true; }
		virtual OpenViBE::boolean processEvent(OpenViBE::CMessageEvent& rMessageEvent)    { return false; }
		virtual OpenViBE::boolean processSignal(OpenViBE::CMessageSignal& rMessageSignal) { return false; }
		virtual OpenViBE::boolean processClock(OpenViBE::CMessageClock& rMessageClock)    { return false; }
		virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex)           { return false; }
		virtual OpenViBE::boolean process(void)=0;

// ====================================================================================================================================

		virtual OpenViBE::Kernel::IBoxAlgorithmContext* getBoxAlgorithmContext(void)
		{
			return m_pBoxAlgorithmContextHandler?m_pBoxAlgorithmContextHandler->top():NULL;
		}

// ====================================================================================================================================

		template <OpenViBE::uint32 ui32OutputIndex>
		OpenViBE::boolean appendOutputChunkData(const void* pBuffer, const OpenViBE::uint64 ui64BufferSize)
		{
			OpenViBE::Kernel::IBoxAlgorithmContext* l_pBoxAlgorithmContext=getBoxAlgorithmContext();
			if(l_pBoxAlgorithmContext)
			{
				OpenViBE::Kernel::IBoxIO* l_pDynamicBoxContext=l_pBoxAlgorithmContext->getDynamicBoxContext();
				if(l_pDynamicBoxContext)
				{
					return l_pDynamicBoxContext->appendOutputChunkData(ui32OutputIndex, static_cast<const OpenViBE::uint8*>(pBuffer), ui64BufferSize);
				}
			}
			return false;
		}

		_IsDerivedFromClass_(CBoxAlgorithmParentClass, OVTK_ClassId_);

	private:

		class CScopedBoxAlgorithm
		{
		public:
			CScopedBoxAlgorithm(OpenViBEToolkit::IBoxAlgorithmContextHandler* pBoxAlgorithmContextHandler, OpenViBE::Kernel::IBoxAlgorithmContext* pBoxAlgorithmContext)
				:m_pBoxAlgorithmContextHandler(pBoxAlgorithmContextHandler)
			{
				m_pBoxAlgorithmContextHandler->push(pBoxAlgorithmContext);
			}
			virtual ~CScopedBoxAlgorithm(void)
			{
				m_pBoxAlgorithmContextHandler->pop();
			}
		protected:
			OpenViBEToolkit::IBoxAlgorithmContextHandler* m_pBoxAlgorithmContextHandler;
		};

		mutable OpenViBEToolkit::IBoxAlgorithmContextHandler* m_pBoxAlgorithmContextHandler;
	};
};

#endif // __OpenViBEToolkit_TBoxAlgorithm_H__
