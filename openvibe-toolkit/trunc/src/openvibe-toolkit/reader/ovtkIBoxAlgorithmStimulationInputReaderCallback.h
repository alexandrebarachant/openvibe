#ifndef __OpenViBEToolkit_Reader_IBoxAlgorithmStimulationInputReaderCallback_H__
#define __OpenViBEToolkit_Reader_IBoxAlgorithmStimulationInputReaderCallback_H__

#include "ovtkIBoxAlgorithmInputReaderCallback.h"

namespace OpenViBEToolkit
{
	class OVTK_API IBoxAlgorithmStimulationInputReaderCallback : virtual public OpenViBEToolkit::IBoxAlgorithmInputReaderCallback
	{
	public:

		class OVTK_API ICallback
		{
		public:
			virtual void setStimulationCount(const OpenViBE::uint32 ui32StimulationCount)=0;
			virtual void setStimulation(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint64 ui64StimulationIdentifier, const OpenViBE::uint64 ui64StimulationDate)=0;
			virtual ~ICallback(void) { }
		};

		template <class COwnerClass>
		class OVTK_API TCallbackProxy1 : virtual public OpenViBEToolkit::IBoxAlgorithmStimulationInputReaderCallback::ICallback
		{
		public:
			TCallbackProxy1(
				COwnerClass& rOwnerObject,
				void (COwnerClass::*mfpSetStimulationCount)(const OpenViBE::uint32 ui32StimulationCount),
				void (COwnerClass::*mfpSetStimulation)(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint64 ui64StimulationIdentifier, const OpenViBE::uint64 ui64StimulationDate))
				:m_rOwnerObject(rOwnerObject)
				,m_mfpSetStimulationCount(mfpSetStimulationCount)
				,m_mfpSetStimulation(mfpSetStimulation)
			{
			}
			virtual void setStimulationCount(const OpenViBE::uint32 ui32StimulationCount)
			{
				if(m_mfpSetStimulationCount)
				{
					(m_rOwnerObject.*m_mfpSetStimulationCount)(ui32StimulationCount);
				}
			}
			virtual void setStimulation(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint64 ui64StimulationIdentifier, const OpenViBE::uint64 ui64StimulationDate)
			{
				if(m_mfpSetStimulation)
				{
					(m_rOwnerObject.*m_mfpSetStimulation)(ui32StimulationIndex, ui64StimulationIdentifier, ui64StimulationDate);
				}
			}
		protected:
			COwnerClass& m_rOwnerObject;
			void (COwnerClass::*m_mfpSetStimulationCount)(const OpenViBE::uint32 ui32StimulationCount);
			void (COwnerClass::*m_mfpSetStimulation)(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint64 ui64StimulationIdentifier, const OpenViBE::uint64 ui64StimulationDate);
		};

		template <class COwnerClass, void (COwnerClass::*mfpSetStimulationCount)(const OpenViBE::uint32 ui32StimulationCount), void (COwnerClass::*mfpSetStimulation)(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint64 ui64StimulationIdentifier, const OpenViBE::uint64 ui64StimulationDate)>
		class OVTK_API TCallbackProxy2 : virtual public OpenViBEToolkit::IBoxAlgorithmStimulationInputReaderCallback::ICallback
		{
		public:
			TCallbackProxy2(
				COwnerClass& rOwnerObject)
				:m_rOwnerObject(rOwnerObject)
				,m_mfpSetStimulationCount(mfpSetStimulationCount)
				,m_mfpSetStimulation(mfpSetStimulation)
			{
			}
			virtual void setStimulationCount(const OpenViBE::uint32 ui32StimulationCount)
			{
				if(m_mfpSetStimulationCount)
				{
					(m_rOwnerObject.*m_mfpSetStimulationCount)(ui32StimulationCount);
				}
			}
			virtual void setStimulation(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint64 ui64StimulationIdentifier, const OpenViBE::uint64 ui64StimulationDate)
			{
				if(m_mfpSetStimulation)
				{
					(m_rOwnerObject.*m_mfpSetStimulation)(ui32StimulationIndex, ui64StimulationIdentifier, ui64StimulationDate);
				}
			}
		protected:
			COwnerClass& m_rOwnerObject;
			void (COwnerClass::*m_mfpSetStimulationCount)(const OpenViBE::uint32 ui32StimulationCount);
			void (COwnerClass::*m_mfpSetStimulation)(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint64 ui64StimulationIdentifier, const OpenViBE::uint64 ui64StimulationDate);
		};

		_IsDerivedFromClass_(OpenViBEToolkit::IBoxAlgorithmInputReaderCallback, OVTK_ClassId_);
	};

	extern OVTK_API OpenViBEToolkit::IBoxAlgorithmStimulationInputReaderCallback* createBoxAlgorithmStimulationInputReaderCallback(OpenViBEToolkit::IBoxAlgorithmStimulationInputReaderCallback::ICallback& rCallback);
	extern OVTK_API void releaseBoxAlgorithmStimulationInputReaderCallback(OpenViBEToolkit::IBoxAlgorithmStimulationInputReaderCallback* pBoxAlgorithmStimulationInputReaderCallback);
};

#endif // __OpenViBEToolkit_Reader_IBoxAlgorithmStimulationInputReaderCallback_H__
