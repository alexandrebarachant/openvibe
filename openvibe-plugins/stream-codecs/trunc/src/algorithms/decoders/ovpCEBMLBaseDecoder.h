#ifndef __SamplePlugin_Algorithms_CEBMLBaseDecoder_H__
#define __SamplePlugin_Algorithms_CEBMLBaseDecoder_H__

#include "../../ovp_defines.h"

#include <openvibe/ov_all.h>

#include <openvibe-toolkit/ovtk_all.h>

#include <ebml/IReader.h>
#include <ebml/IReaderHelper.h>
#include <ebml/TReaderCallbackProxy.h>

#include <stack>

namespace OpenViBEPlugins
{
	namespace StreamCodecs
	{
		class CEBMLBaseDecoder : virtual public OpenViBEToolkit::TAlgorithm < OpenViBE::Plugins::IAlgorithm >
		{
		public:

			CEBMLBaseDecoder(void);

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TAlgorithm < OpenViBE::Plugins::IAlgorithm >, OVP_ClassId_Algorithm_EBMLBaseDecoder);

			// ebml callbacks
			virtual EBML::boolean isMasterChild(const EBML::CIdentifier& rIdentifier);
			virtual void openChild(const EBML::CIdentifier& rIdentifier);
			virtual void processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize);
			virtual void closeChild(void);

		protected:

			EBML::IReaderHelper* m_pEBMLReaderHelper;
			EBML::IReader* m_pEBMLReader;
			EBML::TReaderCallbackProxy1 < OpenViBEPlugins::StreamCodecs::CEBMLBaseDecoder > m_oEBMLReaderCallbackProxy;

			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > m_oEBMLMemoryBufferHandle;
		};

		class CEBMLBaseDecoderDesc : virtual public OpenViBE::Plugins::IAlgorithmDesc
		{
		public:

			virtual OpenViBE::boolean getAlgorithmPrototype(
				OpenViBE::Kernel::IAlgorithmProto& rAlgorithmPrototype) const
			{
				rAlgorithmPrototype.addInputParameter(OVP_EBMLDecoder_EBMLMemoryBuffer_InParameterId, "Memory buffer to decode", OpenViBE::Kernel::ParameterType_MemoryBuffer);

				rAlgorithmPrototype.addOutputTrigger(OVP_EBMLDecoder_GotHeader_TriggerId, "Received header");
				rAlgorithmPrototype.addOutputTrigger(OVP_EBMLDecoder_GotBuffer_TriggerId, "Received buffer");
				rAlgorithmPrototype.addOutputTrigger(OVP_EBMLDecoder_GotEnd_TriggerId, "Received end");

				return true;
			}
		};
	};
};

#endif // __SamplePlugin_Algorithms_CEBMLBaseDecoder_H__
