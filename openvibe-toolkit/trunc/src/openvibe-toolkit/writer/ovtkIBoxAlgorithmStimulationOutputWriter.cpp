#include "ovtkIBoxAlgorithmStimulationOutputWriter.h"
#include "ovtkTBoxAlgorithmOutputWriter.h"

#include <map>

#define OVTK_Version_StimulationOutputWriter 1

using namespace OpenViBE;
using namespace OpenViBEToolkit;
using namespace std;

// ________________________________________________________________________________________________________________
//

namespace OpenViBEToolkit
{
	class CBoxAlgorithmStimulationOutputWriter : virtual public TBoxAlgorithmOutputWriter<IBoxAlgorithmStimulationOutputWriter>
	{
	public:

		CBoxAlgorithmStimulationOutputWriter(void);
		virtual ~CBoxAlgorithmStimulationOutputWriter(void);

		virtual boolean setStimulationCount(const uint32 ui32StimulationCount);
		virtual boolean setStimulation(const uint32 ui32StimulationIndex, const uint32 ui32StimulationIdentifier, const uint64 ui64StimulationDate);

		virtual boolean writeHeader(EBML::IWriter& rWriter);
		virtual boolean writeBuffer(EBML::IWriter& rWriter);

		_IsDerivedFromClass_Final_(TBoxAlgorithmOutputWriter<IBoxAlgorithmStimulationOutputWriter>, OVTK_ClassId_);

	protected:

		uint32 m_ui32SitmulationCount;
		map< uint32, pair< uint32, uint64 > > m_vStimulation;
	};
};

// ________________________________________________________________________________________________________________
//

CBoxAlgorithmStimulationOutputWriter::CBoxAlgorithmStimulationOutputWriter(void)
	:m_ui32SitmulationCount(0)
{
}

CBoxAlgorithmStimulationOutputWriter::~CBoxAlgorithmStimulationOutputWriter(void)
{
}

// ________________________________________________________________________________________________________________
//

boolean CBoxAlgorithmStimulationOutputWriter::setStimulationCount(const uint32 ui32StimulationCount)
{
	m_ui32SitmulationCount=ui32StimulationCount;
	return true;
}

boolean CBoxAlgorithmStimulationOutputWriter::setStimulation(const uint32 ui32StimulationIndex, const uint32 ui32StimulationIdentifier, const uint64 ui64StimulationDate)
{
	m_vStimulation[ui32StimulationIndex]=pair<uint32, uint64>(ui32StimulationIdentifier, ui64StimulationDate);
	return true;
}

// ________________________________________________________________________________________________________________
//

boolean CBoxAlgorithmStimulationOutputWriter::writeHeader(EBML::IWriter& rWriter)
{
	m_pWriterHelper->connect(&rWriter);
	 m_pWriterHelper->openChild(OVTK_NodeId_Stimulation_Header);
	  m_pWriterHelper->openChild(OVTK_NodeId_StreamVersion);
	   m_pWriterHelper->setUIntegerAsChildData(OVTK_Version_StimulationOutputWriter);
	  m_pWriterHelper->closeChild();
	 m_pWriterHelper->closeChild();
	m_pWriterHelper->disconnect();
	return true;
}

boolean CBoxAlgorithmStimulationOutputWriter::writeBuffer(EBML::IWriter& rWriter)
{
	m_pWriterHelper->connect(&rWriter);
	 m_pWriterHelper->openChild(OVTK_NodeId_Stimulation_Buffer);
	  m_pWriterHelper->openChild(OVTK_NodeId_Stimulation_NumberOfStimulations);
	   m_pWriterHelper->setUIntegerAsChildData(m_ui32SitmulationCount);
	  m_pWriterHelper->closeChild();
		for(uint32 i=0; i<m_ui32SitmulationCount; i++)
		{
			m_pWriterHelper->openChild(OVTK_NodeId_Stimulation_Stimulation);
			 m_pWriterHelper->openChild(OVTK_NodeId_Stimulation_Stimulation_Identifier);
			  m_pWriterHelper->setUIntegerAsChildData(m_vStimulation[i].first);
			 m_pWriterHelper->closeChild();
			 m_pWriterHelper->openChild(OVTK_NodeId_Stimulation_Stimulation_Date);
			  m_pWriterHelper->setUIntegerAsChildData(m_vStimulation[i].second);
			 m_pWriterHelper->closeChild();
			m_pWriterHelper->closeChild();
		}
	 m_pWriterHelper->closeChild();
	m_pWriterHelper->disconnect();
	return true;
}

// ________________________________________________________________________________________________________________
//

IBoxAlgorithmStimulationOutputWriter* OpenViBEToolkit::createBoxAlgorithmStimulationOutputWriter(void)
{
	return new CBoxAlgorithmStimulationOutputWriter();
}

void OpenViBEToolkit::releaseBoxAlgorithmStimulationOutputWriter(IBoxAlgorithmStimulationOutputWriter* pOutputWriter)
{
	delete pOutputWriter;
}

// ________________________________________________________________________________________________________________
//
