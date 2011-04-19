#ifndef __OpenViBEPlugins_BoxAlgorithm_StreamSynchro_H__
#define __OpenViBEPlugins_BoxAlgorithm_StreamSynchro_H__

#include "ovpCSynchroEngine.h"

//Signal synchronization boxAlgo
#define OVP_ClassId_BoxAlgorithm_StreamSynchro				OpenViBE::CIdentifier(0x575B753B, 0x40F909D1)
#define OVP_ClassId_BoxAlgorithm_StreamSynchroDesc			OpenViBE::CIdentifier(0x162F39E7, 0x3BC35A58)

namespace OpenViBEPlugins
{
	namespace SignalProcessing
	{
		class CBoxAlgorithmStreamSynchro : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_StreamSynchro);

		protected:

			// ...
			OpenViBE::Kernel::IAlgorithmProxy* m_pSignalDecoder1;
            OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pMemoryBufferToDecode1;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > op_pDecodedMatrix1;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > op_ui64SamplingRate1;

			OpenViBE::Kernel::IAlgorithmProxy* m_pSignalDecoder2;
            OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pMemoryBufferToDecode2;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > op_pDecodedMatrix2;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > op_ui64SamplingRate2;


			OpenViBE::Kernel::IAlgorithmProxy* m_pSignalEncoder;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > ip_pMatrixToEncode;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pEncodedMemoryBuffer;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > ip_ui64SamplingRate;

		protected:

			CSynchroEngine		m_engine;
			OpenViBE::boolean	m_headerSent;


			OpenViBE::uint32	m_OutputChunkSize;
			OpenViBE::float64	m_Chunk_sent_count;
			OpenViBE::float64	m_Chunk_time;

			OpenViBE::uint32	m_2ChunksFlag_s1;
			OpenViBE::uint32	m_2ChunksFlag_s2;
		};

		class CBoxAlgorithmStreamSynchroDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Stream Synchronization"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Guillaume Lio"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("GIPSA-Lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("This box make the synchronization of two signal streams according to their reference channels."); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("This box make the synchronization of two signal streams according to their reference channels. The reference channel is the last channel of each signal stream, and must be a binary periodic signal sent by a third device to the two devices that must be synchronized."); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Signal processing/Basic"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-connect"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_StreamSynchro; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SignalProcessing::CBoxAlgorithmStreamSynchro; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("input Min1", OV_TypeId_Signal);
				rBoxAlgorithmPrototype.addInput  ("input Min2", OV_TypeId_Signal);
				rBoxAlgorithmPrototype.addOutput  ("output Mout", OV_TypeId_Signal);

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_StreamSynchroDesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_StreamSynchro_H__
