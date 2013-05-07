#ifndef __OpenViBEToolkit_Reader_IBoxAlgorithmStreamedMatrixInputReaderCallback_H__
#define __OpenViBEToolkit_Reader_IBoxAlgorithmStreamedMatrixInputReaderCallback_H__

#include "ovtkIBoxAlgorithmEBMLInputReaderCallback.h"

namespace OpenViBEToolkit
{
	class OV_API IBoxAlgorithmStreamedMatrixInputReaderCallback : public OpenViBEToolkit::IBoxAlgorithmEBMLInputReaderCallback
	{
	public:

		class OV_API ICallback
		{
		public:
			virtual void setMatrixDimensionCount(const OpenViBE::uint32 ui32DimensionCount)=0;
			virtual void setMatrixDimensionSize(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionSize)=0;
			virtual void setMatrixDimensionLabel(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionEntryIndex, const char* sDimensionLabel)=0;
			virtual void setMatrixBuffer(const OpenViBE::float64* pBuffer)=0;
			virtual ~ICallback(void) { }
		};

		template <class COwnerClass>
		class TCallbackProxy1 : public OpenViBEToolkit::IBoxAlgorithmStreamedMatrixInputReaderCallback::ICallback
		{
		public:
			TCallbackProxy1(
				COwnerClass& rOwnerObject,
				void (COwnerClass::*fpSetMatrixDimensionCount)(const OpenViBE::uint32 ui32DimensionCount),
				void (COwnerClass::*fpSetMatrixDimensionSize)(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionSize),
				void (COwnerClass::*fpSetMatrixDimensionLabel)(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionEntryIndex, const char* sDimensionLabel),
				void (COwnerClass::*fpSetMatrixBuffer)(const OpenViBE::float64* pBuffer))
				:m_rOwnerObject(rOwnerObject)
				,m_fpSetMatrixDimensionCount(fpSetMatrixDimensionCount)
				,m_fpSetMatrixDimensionSize(fpSetMatrixDimensionSize)
				,m_fpSetMatrixDimensionLabel(fpSetMatrixDimensionLabel)
				,m_fpSetMatrixBuffer(fpSetMatrixBuffer)
			{
			}
			virtual void setMatrixDimensionCount(const OpenViBE::uint32 ui32DimensionCount)
			{
				if(m_fpSetMatrixDimensionCount)
				{
					(m_rOwnerObject.*m_fpSetMatrixDimensionCount)(ui32DimensionCount);
				}
			}
			virtual void setMatrixDimensionSize(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionSize)
			{
				if(m_fpSetMatrixDimensionSize)
				{
					(m_rOwnerObject.*m_fpSetMatrixDimensionSize)(ui32DimensionIndex, ui32DimensionSize);
				}
			}
			virtual void setMatrixDimensionLabel(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionEntryIndex, const char* sDimensionLabel)
			{
				if(m_fpSetMatrixDimensionLabel)
				{
					(m_rOwnerObject.*m_fpSetMatrixDimensionLabel)(ui32DimensionIndex, ui32DimensionEntryIndex, sDimensionLabel);
				}
			}
			virtual void setMatrixBuffer(const OpenViBE::float64* pBuffer)
			{
				if(m_fpSetMatrixBuffer)
				{
					(m_rOwnerObject.*m_fpSetMatrixBuffer)(pBuffer);
				}
			}
		protected:
			COwnerClass& m_rOwnerObject;
			void (COwnerClass::*m_fpSetMatrixDimensionCount)(const OpenViBE::uint32 ui32DimensionCount);
			void (COwnerClass::*m_fpSetMatrixDimensionSize)(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionSize);
			void (COwnerClass::*m_fpSetMatrixDimensionLabel)(const OpenViBE::uint32 ui32DimensionIndex, const OpenViBE::uint32 ui32DimensionEntryIndex, const char* sDimensionLabel);
			void (COwnerClass::*m_fpSetMatrixBuffer)(const OpenViBE::float64* pBuffer);
		};

		_IsDerivedFromClass_(OpenViBEToolkit::IBoxAlgorithmEBMLInputReaderCallback, OVTK_ClassId_);
	};

	OV_API OpenViBEToolkit::IBoxAlgorithmStreamedMatrixInputReaderCallback* createBoxAlgorithmStreamedMatrixInputReaderCallback(OpenViBEToolkit::IBoxAlgorithmStreamedMatrixInputReaderCallback::ICallback& rCallback);
	OV_API void releaseBoxAlgorithmStreamedMatrixInputReaderCallback(OpenViBEToolkit::IBoxAlgorithmStreamedMatrixInputReaderCallback* pStreamedMatrixInputReaderCallback);
};

#endif // __OpenViBEToolkit_Reader_IBoxAlgorithmStreamedMatrixInputReaderCallback_H__
