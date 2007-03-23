#include "ovtkIBoxAlgorithmSignalOutputWriter.h"
#include "ovtkIStreamedMatrixWriterHelper.h"
#include "ovtkTBoxAlgorithmOutputWriter.h"

#include <iostream>

#define OVTK_Version_SignalOutputWriter 1

using namespace OpenViBE;
using namespace OpenViBEToolkit;

// ________________________________________________________________________________________________________________
//

namespace OpenViBEToolkit
{
	class CBoxAlgorithmSignalOutputWriter : virtual public TBoxAlgorithmOutputWriter<IBoxAlgorithmSignalOutputWriter>
	{
	public:

		CBoxAlgorithmSignalOutputWriter(void);
		virtual ~CBoxAlgorithmSignalOutputWriter(void);

		virtual boolean setSamplingRate(const uint32 ui32SamplingRate);
		virtual boolean setChannelCount(const uint32 ui32ChannelCount);
		virtual boolean setChannelName(const uint32 ui32ChannelIndex, const char* sChannelName);
		virtual boolean setSampleCount(const uint32 ui32SampleCount);
		virtual boolean setSampleBuffer(const float64* pSampleBuffer);

		virtual boolean writeHeader(EBML::IWriter& rWriter);
		virtual boolean writeBuffer(EBML::IWriter& rWriter);

		_IsDerivedFromClass_Final_(TBoxAlgorithmOutputWriter<IBoxAlgorithmSignalOutputWriter>, OVTK_ClassId_);

	protected:

		const float64* m_pSampleBuffer;
		uint32 m_ui32SamplingRate;
		IStreamedMatrixWriterHelper* m_pStreamedMatrixWriterHelper;
	};
};

// ________________________________________________________________________________________________________________
//

CBoxAlgorithmSignalOutputWriter::CBoxAlgorithmSignalOutputWriter(void)
	:m_pSampleBuffer(NULL)
	,m_ui32SamplingRate(0)
	,m_pStreamedMatrixWriterHelper(NULL)
{
	m_pStreamedMatrixWriterHelper=createStreamedMatrixWriterHelper();
	m_pStreamedMatrixWriterHelper->setDimmensionCount(2);
}

CBoxAlgorithmSignalOutputWriter::~CBoxAlgorithmSignalOutputWriter(void)
{
	releaseStreamedMatrixWriterHelper(m_pStreamedMatrixWriterHelper);
	m_pStreamedMatrixWriterHelper=NULL;
}

// ________________________________________________________________________________________________________________
//

boolean CBoxAlgorithmSignalOutputWriter::setSamplingRate(const uint32 ui32SamplingRate)
{
	m_ui32SamplingRate=ui32SamplingRate;
	return m_ui32SamplingRate!=0;
}

boolean CBoxAlgorithmSignalOutputWriter::setChannelCount(const uint32 ui32ChannelCount)
{
	return m_pStreamedMatrixWriterHelper->setDimmensionSize(0, ui32ChannelCount);
}

boolean CBoxAlgorithmSignalOutputWriter::setChannelName(const uint32 ui32ChannelIndex, const char* sChannelName)
{
	return m_pStreamedMatrixWriterHelper->setDimmensionLabel(0, ui32ChannelIndex, sChannelName);
}

boolean CBoxAlgorithmSignalOutputWriter::setSampleCount(const uint32 ui32SampleCount)
{
	return m_pStreamedMatrixWriterHelper->setDimmensionSize(1, ui32SampleCount);
}

// ________________________________________________________________________________________________________________
//

boolean CBoxAlgorithmSignalOutputWriter::setSampleBuffer(const float64* pSampleBuffer)
{
	return m_pStreamedMatrixWriterHelper->setBuffer(pSampleBuffer);
}

// ________________________________________________________________________________________________________________
//

boolean CBoxAlgorithmSignalOutputWriter::writeHeader(EBML::IWriter& rWriter)
{
	m_pWriterHelper->connect(&rWriter);
	 m_pWriterHelper->openChild(OVTK_NodeId_Signal_Header);
	  m_pWriterHelper->openChild(OVTK_NodeId_StreamVersion);
	   m_pWriterHelper->setUIntegerAsChildData(OVTK_Version_SignalOutputWriter);
	  m_pWriterHelper->closeChild();
	  m_pWriterHelper->openChild(OVTK_NodeId_Signal_SamplingRate);
	   m_pWriterHelper->setUIntegerAsChildData(m_ui32SamplingRate);
	  m_pWriterHelper->closeChild();
	  m_pStreamedMatrixWriterHelper->writeHeader(rWriter);
	 m_pWriterHelper->closeChild();
	m_pWriterHelper->disconnect();
	return true;
}

boolean CBoxAlgorithmSignalOutputWriter::writeBuffer(EBML::IWriter& rWriter)
{
	m_pWriterHelper->connect(&rWriter);
	 m_pWriterHelper->openChild(OVTK_NodeId_Signal_Buffer);
	  m_pStreamedMatrixWriterHelper->writeBuffer(rWriter);
	 m_pWriterHelper->closeChild();
	m_pWriterHelper->disconnect();

	return true;
}

// ________________________________________________________________________________________________________________
//

IBoxAlgorithmSignalOutputWriter* OpenViBEToolkit::createBoxAlgorithmSignalOutputWriter(void)
{
	return new CBoxAlgorithmSignalOutputWriter();
}

void OpenViBEToolkit::releaseBoxAlgorithmSignalOutputWriter(IBoxAlgorithmSignalOutputWriter* pOutputWriter)
{
	delete pOutputWriter;
}

// ________________________________________________________________________________________________________________
//
