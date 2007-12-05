#include "ovpCSpectrumEncoder.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::StreamCodecs;

boolean CSpectrumEncoder::initialize(void)
{
	CStreamedMatrixEncoder::initialize();

	m_oFrequencyBandMinMaxHandle.initialize(getInputParameter(OVP_SpectrumEncoder_FrequencyBandMinMax_InParameterId));

	return true;
}

boolean CSpectrumEncoder::uninitialize(void)
{
	m_oFrequencyBandMinMaxHandle.uninitialize();

	CStreamedMatrixEncoder::uninitialize();

	return true;
}

// ________________________________________________________________________________________________________________
//

boolean CSpectrumEncoder::processHeader(void)
{
	// m_oFrequencyBandMinMaxHandle dimmension count should be 2
	// m_oFrequencyBandMinMaxHandle dimmension size 0 should be the same as streamed matrix dimmension size 0
	// m_oFrequencyBandMinMaxHandle dimmension size 1 should be 2

	CStreamedMatrixEncoder::processHeader();

	IMatrix* l_pFrequencyBandMinMax=m_oFrequencyBandMinMaxHandle;

	m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_Spectrum);
	 for(uint32 i=0; i<l_pFrequencyBandMinMax->getDimensionSize(0); i++)
	 {
	  m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_Spectrum_FrequencyBand);
	   m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_Spectrum_FrequencyBand_Start);
	    m_pEBMLWriterHelper->setFloat64AsChildData(l_pFrequencyBandMinMax->getBuffer()[i*2+0]);
	   m_pEBMLWriterHelper->closeChild();
	   m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_Spectrum_FrequencyBand_Stop);
	    m_pEBMLWriterHelper->setFloat64AsChildData(l_pFrequencyBandMinMax->getBuffer()[i*2+1]);
	   m_pEBMLWriterHelper->closeChild();
	  m_pEBMLWriterHelper->closeChild();
	 }
	m_pEBMLWriterHelper->closeChild();

	return true;
}
