#ifndef __OpenViBEPlugins_BoxAlgorithm_SignalMerger_H__
#define __OpenViBEPlugins_BoxAlgorithm_SignalMerger_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <vector>
#include <cstdio>
#include <string>

#define OVP_ClassId_BoxAlgorithm_SignalMerger 	 	OpenViBE::CIdentifier(0x4BF9326F, 0x75603102)
#define OVP_ClassId_BoxAlgorithm_SignalMergerDesc 	OpenViBE::CIdentifier(0x7A684C44, 0x23BA70A5)

namespace OpenViBEPlugins
{
	namespace Streaming
	{
		class CBoxAlgorithmSignalMerger : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_SignalMerger);

		protected:

			OpenViBE::Kernel::IAlgorithmProxy* m_pSignalDecoder;
			OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pMemoryBufferToDecode1;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > op_pDecodedMatrix1;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > op_ui64SamplingRate1;

			OpenViBE::Kernel::IAlgorithmProxy* m_pSignalEncoder;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > ip_pMatrixToEncode;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pEncodedMemoryBuffer;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > ip_ui64SamplingRate;

			std::vector < OpenViBE::uint32> m_vChannelCount;
			std::vector < std::string > m_vChannelName;
		};

		class CBoxAlgorithmSignalMergerListener : public OpenViBEToolkit::TBoxListener < OpenViBE::Plugins::IBoxListener >
		{
		public:

			virtual OpenViBE::boolean onInputAdded(OpenViBE::Kernel::IBox& rBox, const OpenViBE::uint32 ui32Index)
			{
				char l_sName[1024];
				OpenViBE::uint32 i;

				rBox.setInputType(ui32Index, OV_TypeId_Signal);
				for(i=0; i<rBox.getInputCount(); i++)
				{
					sprintf(l_sName, "Input Signal %u", i+1);
					rBox.setInputName(i, l_sName);
				}

				return true;
			};

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxListener < OpenViBE::Plugins::IBoxListener >, OV_UndefinedIdentifier);
		};

		class CBoxAlgorithmSignalMergerDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Signal Merger"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Guillaume Lio - Matthieu Goyat"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("GIPSA-Lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Channels concatenation of two signals."); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("Converts some signal in one signal through channel addition. Signals must have the same epoch and the same sampling rate."); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Streaming"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-sort-ascending"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_SignalMerger; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Streaming::CBoxAlgorithmSignalMerger; }
			virtual OpenViBE::Plugins::IBoxListener* createBoxListener(void) const               { return new CBoxAlgorithmSignalMergerListener; }
			virtual void releaseBoxListener(OpenViBE::Plugins::IBoxListener* pBoxListener) const { delete pBoxListener; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("Input Signal 1", OV_TypeId_Signal);
				rBoxAlgorithmPrototype.addInput  ("Input Signal 2", OV_TypeId_Signal);
				rBoxAlgorithmPrototype.addOutput ("Merged Signal", OV_TypeId_Signal);
				rBoxAlgorithmPrototype.addFlag   (OpenViBE::Kernel::BoxFlag_CanAddInput);

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_SignalMergerDesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_SignalMerger_H__
