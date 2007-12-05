#include "ovpCSignalEncoder.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::StreamCodecs;

boolean CSignalEncoder::initialize(void)
{
	CStreamedMatrixEncoder::initialize();

	m_oSamplingRateHandle.initialize(getInputParameter(OVP_SignalEncoder_SamplingRate_InParameterId));

	return true;
}

boolean CSignalEncoder::uninitialize(void)
{
	m_oSamplingRateHandle.uninitialize();

	CStreamedMatrixEncoder::uninitialize();

	return true;
}

// ________________________________________________________________________________________________________________
//

boolean CSignalEncoder::processHeader(void)
{
	CStreamedMatrixEncoder::processHeader();

	m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_Signal);
	 m_pEBMLWriterHelper->openChild(OVTK_NodeId_Header_Signal_SamplingRate);
	  m_pEBMLWriterHelper->setUIntegerAsChildData(m_oSamplingRateHandle);
	 m_pEBMLWriterHelper->closeChild();
	m_pEBMLWriterHelper->closeChild();

	return true;
}
