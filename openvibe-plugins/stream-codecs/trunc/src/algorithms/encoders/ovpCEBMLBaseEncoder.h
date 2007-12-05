#ifndef __SamplePlugin_Algorithms_CEBMLBaseEncoder_H__
#define __SamplePlugin_Algorithms_CEBMLBaseEncoder_H__

#include "../../ovp_defines.h"

#include <openvibe/ov_all.h>

#include <openvibe-toolkit/ovtk_all.h>

#include <ebml/IWriter.h>
#include <ebml/IWriterHelper.h>
#include <ebml/TWriterCallbackProxy.h>

namespace OpenViBEPlugins
{
	namespace StreamCodecs
	{
		class CEBMLBaseEncoder : virtual public OpenViBEToolkit::TAlgorithm < OpenViBE::Plugins::IAlgorithm >
		{
		public:

			CEBMLBaseEncoder(void);

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean process(void);
			virtual OpenViBE::boolean processHeader(void) { return true; }
			virtual OpenViBE::boolean processBuffer(void) { return true; }
			virtual OpenViBE::boolean processEnd(void)    { return true; }

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TAlgorithm < OpenViBE::Plugins::IAlgorithm >, OVP_ClassId_Algorithm_EBMLBaseEncoder);

			// ebml callbacks
			virtual void write(const void* pBuffer, const EBML::uint64 ui64BufferSize);

		protected:

			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > m_oMemoryBufferHandle;

			EBML::IWriterHelper* m_pEBMLWriterHelper;
			EBML::IWriter* m_pEBMLWriter;
			EBML::TWriterCallbackProxy1 < OpenViBEPlugins::StreamCodecs::CEBMLBaseEncoder > m_oEBMLWriterCallbackProxy;
		};

		class CEBMLBaseEncoderDesc : virtual public OpenViBE::Plugins::IAlgorithmDesc
		{
		public:

			virtual OpenViBE::boolean getAlgorithmPrototype(
				OpenViBE::Kernel::IAlgorithmProto& rAlgorithmPrototype) const
			{
				rAlgorithmPrototype.addOutputParameter(OVP_EBMLEncoder_MemoryBuffer_OutParameterId, "Encoded memory buffer", OpenViBE::Kernel::ParameterType_MemoryBuffer);

				rAlgorithmPrototype.addInputTrigger(OVP_EBMLEncoder_WriteHeader_TriggerId, "Encode header");
				rAlgorithmPrototype.addInputTrigger(OVP_EBMLEncoder_WriteBuffer_TriggerId, "Encode buffer");
				rAlgorithmPrototype.addInputTrigger(OVP_EBMLEncoder_WriteEnd_TriggerId, "Encode end");

				return true;
			}
		};
	};
};

#endif // __SamplePlugin_Algorithms_CEBMLBaseEncoder_H__
