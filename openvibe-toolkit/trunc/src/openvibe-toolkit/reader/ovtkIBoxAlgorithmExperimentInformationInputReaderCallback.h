#ifndef __OpenViBEToolkit_Reader_IBoxAlgorithmExperimentInformationInputReaderCallback_H__
#define __OpenViBEToolkit_Reader_IBoxAlgorithmExperimentInformationInputReaderCallback_H__

#include "ovtkIBoxAlgorithmInputReaderCallback.h"

namespace OpenViBEToolkit
{
	class OVTK_API IBoxAlgorithmExperimentInformationInputReaderCallback : virtual public OpenViBEToolkit::IBoxAlgorithmInputReaderCallback
	{
	public:

		enum
		{
			Value_ExperimentIdentifier, // unsigned integer
			Value_SubjectIdentifier,    // unsigned integer
			Value_SubjectAge,           // unsigned integer
			Value_SubjectSex,           // unsigned integer
			Value_LaboratoryIdentifier, // unsigned integer
			Value_TechnicianIdentifier, // unsigned integer

			Value_ExperimentDate,       // string
			Value_SubjectName,          // string
			Value_LaboratoryName,       // string
			Value_TechnicianName,       // string
		};

		class OVTK_API ICallback
		{
		public:
			virtual void setValue(const OpenViBE::uint32 ui32ValueIdentifier, const OpenViBE::uint32 ui32Value)=0;
			virtual void setValue(const OpenViBE::uint32 ui32ValueIdentifier, const char* sValue)=0;
			virtual ~ICallback(void) { }
		};

		template <class COwnerClass>
		class OVTK_API TCallbackProxy1 : virtual public OpenViBEToolkit::IBoxAlgorithmExperimentInformationInputReaderCallback::ICallback
		{
		public:
			TCallbackProxy1(
				COwnerClass& rOwnerObject,
				void (COwnerClass::*mfpSetValueI)(const OpenViBE::uint32 ui32ValueIdentifier, const OpenViBE::uint32 ui32Value),
				void (COwnerClass::*mfpSetValueS)(const OpenViBE::uint32 ui32ValueIdentifier, const char* sValue))
				:m_rOwnerObject(rOwnerObject)
				,m_mfpSetValueI(mfpSetValueI)
				,m_mfpSetValueS(mfpSetValueS)
			{
			}
			virtual void mfpSetValue(const OpenViBE::uint32 ui32ValueIdentifier, const OpenViBE::uint32 ui32Value)
			{
				if(m_mfpSetValueI)
				{
					(m_rOwnerObject.*m_mfpSetValueI)(ui32ValueIdentifier, ui32Value);
				}
			}
			virtual void mfpSetValue(const OpenViBE::uint32 ui32ValueIdentifier, const char* sValue)
			{
				if(m_mfpSetValueS)
				{
					(m_rOwnerObject.*m_mfpSetValueS)(ui32ValueIdentifier, sValue);
				}
			}
		protected:
			COwnerClass& m_rOwnerObject;
			void (COwnerClass::*m_mfpSetValueI)(const OpenViBE::uint32 ui32ValueIdentifier, const OpenViBE::uint32 ui32Value);
			void (COwnerClass::*m_mfpSetValueS)(const OpenViBE::uint32 ui32ValueIdentifier, const char* sValue);
		};

		template <class COwnerClass, void (COwnerClass::*mfpSetValueI)(const OpenViBE::uint32 ui32ValueIdentifier, const OpenViBE::uint32 ui32Value), void (COwnerClass::*mfpSetValueS)(const OpenViBE::uint32 ui32ValueIdentifier, const char* sValue)>
		class OVTK_API TCallbackProxy2 : virtual public OpenViBEToolkit::IBoxAlgorithmExperimentInformationInputReaderCallback::ICallback
		{
		public:
			TCallbackProxy2(
				COwnerClass& rOwnerObject)
				:m_rOwnerObject(rOwnerObject)
				,m_mfpSetValueI(mfpSetValueI)
				,m_mfpSetValueS(mfpSetValueS)
			{
			}
			virtual void mfpSetValue(const OpenViBE::uint32 ui32ValueIdentifier, const OpenViBE::uint32 ui32Value)
			{
				if(m_mfpSetValueI)
				{
					(m_rOwnerObject.*m_mfpSetValueI)(ui32ValueIdentifier, ui32Value);
				}
			}
			virtual void mfpSetValue(const OpenViBE::uint32 ui32ValueIdentifier, const char* sValue)
			{
				if(m_mfpSetValueS)
				{
					(m_rOwnerObject.*m_mfpSetValueS)(ui32ValueIdentifier, sValue);
				}
			}
		protected:
			COwnerClass& m_rOwnerObject;
			void (COwnerClass::*m_mfpSetValueI)(const OpenViBE::uint32 ui32ValueIdentifier, const OpenViBE::uint32 ui32Value);
			void (COwnerClass::*m_mfpSetValueS)(const OpenViBE::uint32 ui32ValueIdentifier, const char* sValue);
		};

		_IsDerivedFromClass_(OpenViBEToolkit::IBoxAlgorithmInputReaderCallback, OVTK_ClassId_);
	};

	extern OVTK_API OpenViBEToolkit::IBoxAlgorithmExperimentInformationInputReaderCallback* createBoxAlgorithmExperimentInformationInputReaderCallback(OpenViBEToolkit::IBoxAlgorithmExperimentInformationInputReaderCallback::ICallback& rCallback);
	extern OVTK_API void releaseBoxAlgorithmExperimentInformationInputReaderCallback(OpenViBEToolkit::IBoxAlgorithmExperimentInformationInputReaderCallback* pBoxAlgorithmExperimentInformationInputReaderCallback);
};

#endif // __OpenViBEToolkit_Reader_IBoxAlgorithmExperimentInformationInputReaderCallback_H__
