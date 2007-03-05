#ifndef __SamplePlugin_CIdentity_H__
#define __SamplePlugin_CIdentity_H__

#include "ovp_defines.h"

#include <iostream>

namespace OpenViBEPlugins
{
	namespace Samples
	{
		class CIdentity : virtual public OpenViBE::Plugins::IBoxAlgorithm
		{
		public:

			virtual void release(void)
			{
			}

			virtual OpenViBE::boolean processInput(
				OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext,
				OpenViBE::uint32 ui32InputIndex)
			{
				rBoxAlgorithmContext.markAlgorithmAsReadyToProcess();
				return true;
			}

			virtual OpenViBE::boolean process(OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext)
			{
				OpenViBE::Plugins::IDynamicBoxContext* l_pDynamicBoxContext=rBoxAlgorithmContext.getDynamicBoxContext();
				for(OpenViBE::uint32 i=0; i<l_pDynamicBoxContext->getInputChunkCount(0); i++)
				{
					OpenViBE::uint64 l_ui64StartTime;
					OpenViBE::uint64 l_ui64EndTime;
					OpenViBE::uint64 l_ui64ChunkSize;
					const OpenViBE::uint8* l_pChunkBuffer;
					l_pDynamicBoxContext->getInputChunk(0, i, l_ui64StartTime, l_ui64EndTime, l_ui64ChunkSize, l_pChunkBuffer);
					l_pDynamicBoxContext->markInputAsDeprecated(0, i);
					l_pDynamicBoxContext->setOutputChunkSize(0, 0);
					l_pDynamicBoxContext->appendOutputChunkData(0, l_pChunkBuffer, l_ui64ChunkSize);
					l_pDynamicBoxContext->markOutputAsReadyToSend(0, 0, 0);
				}
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_Identity)
		};

		class CIdentityDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Identity"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Samples"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_Identity; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Samples::CIdentity(); }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Plugins::IBoxProto& rPrototype) const
			{
				rPrototype.addInput("Input stream", OV_UndefinedIdentifier);

				rPrototype.addOutput("Output stream", OV_UndefinedIdentifier);

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_IdentityDesc)
		};
	};
};

#endif // __SamplePlugin_CIdentity_H__
