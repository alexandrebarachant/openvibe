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
			virtual void setStimulation(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint32 ui32StimulationIdentifier, const OpenViBE::uint32 ui32SampleIndex)=0;
			virtual ~ICallback(void) { }
		};

		template <class COwnerClass>
		class OVTK_API TCallbackProxy1
		{
		public:
			TCallbackProxy1(
				COwnerClass& rOwnerObject,
				void (COwnerClass::*mfpSetStimulationCount)(const OpenViBE::uint32 ui32StimulationCount),
				void (COwnerClass::*mfpSetStimulation)(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint32 ui32StimulationIdentifier, const OpenViBE::uint32 ui32SampleIndex))
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
			virtual void setStimulation(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint32 ui32StimulationIdentifier, const OpenViBE::uint32 ui32SampleIndex)
			{
				if(m_mfpSetStimulation)
				{
					(m_rOwnerObject.*m_mfpSetStimulation)(ui32StimulationIndex, ui32StimulationIdentifier, ui32SampleIndex);
				}
			}
		protected:
			COwnerClass& m_rOwnerObject;
			void (COwnerClass::*m_mfpSetStimulationCount)(const OpenViBE::uint32 ui32StimulationCount);
			void (COwnerClass::*m_mfpSetStimulation)(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint32 ui32StimulationIdentifier, const OpenViBE::uint32 ui32SampleIndex);
		};

		template <class COwnerClass, void (COwnerClass::*mfpSetStimulationCount)(const OpenViBE::uint32 ui32StimulationCount), void (COwnerClass::*mfpSetStimulation)(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint32 ui32StimulationIdentifier, const OpenViBE::uint32 ui32SampleIndex)>
		class OVTK_API TCallbackProxy2
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
			virtual void setStimulation(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint32 ui32StimulationIdentifier, const OpenViBE::uint32 ui32SampleIndex)
			{
				if(m_mfpSetStimulation)
				{
					(m_rOwnerObject.*m_mfpSetStimulation)(ui32StimulationIndex, ui32StimulationIdentifier, ui32SampleIndex);
				}
			}
		protected:
			COwnerClass& m_rOwnerObject;
			void (COwnerClass::*m_mfpSetStimulationCount)(const OpenViBE::uint32 ui32StimulationCount);
			void (COwnerClass::*m_mfpSetStimulation)(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint32 ui32StimulationIdentifier, const OpenViBE::uint32 ui32SampleIndex);
		};

		_IsDerivedFromClass_(OpenViBEToolkit::IBoxAlgorithmInputReaderCallback, OVTK_ClassId_);
	};

	extern OVTK_API OpenViBEToolkit::IBoxAlgorithmStimulationInputReaderCallback* createBoxAlgorithmStimulationInputReaderCallback(OpenViBEToolkit::IBoxAlgorithmStimulationInputReaderCallback::ICallback& rCallback);
	extern OVTK_API void releaseBoxAlgorithmStimulationInputReaderCallback(OpenViBEToolkit::IBoxAlgorithmStimulationInputReaderCallback* pBoxAlgorithmStimulationInputReaderCallback);
};

#endif // __OpenViBEToolkit_Reader_IBoxAlgorithmStimulationInputReaderCallback_H__
