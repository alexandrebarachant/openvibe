#ifndef __OpenViBEPlugins_BoxAlgorithm_EMA_H__
#define __OpenViBEPlugins_BoxAlgorithm_EMA_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#define OVP_ClassId_BoxAlgorithm_EMA                                                   OpenViBE::CIdentifier(0x42A270B9, 0x3F5C416F)
#define OVP_ClassId_BoxAlgorithm_EMADesc                                               OpenViBE::CIdentifier(0x024D1B04, 0x0FEC7F80)

namespace OpenViBEPlugins
{
	namespace SignalProcessing
	{
		class CBoxAlgorithmEMA : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_EMA);

		protected:

			// ...
			OpenViBE::Kernel::IAlgorithmProxy* m_pSignalDecoder;
			OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pMemoryBufferToDecode;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > op_pDecodedMatrix;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > op_ui64SamplingRate;

			OpenViBE::Kernel::IAlgorithmProxy* m_pSignalEncoder;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > ip_pMatrixToEncode;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pEncodedMemoryBuffer;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > ip_ui64SamplingRate;

			OpenViBE::Kernel::IAlgorithmProxy* m_pSommeAlgorithm;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > ip_pMatrixAlgo;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > op_pMatrixAlgo;

			OpenViBE::Kernel::TParameterHandler < OpenViBE::float64 > ip_pParameter;
		};

		class CBoxAlgorithmEMADesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("EMA"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Matthieu Goyat"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("GIPSA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Exponential Moving Average : S(t)=alpha*X(t)-(1-alpha)*S(t-1)"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Signal processing/GIPSA"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-about"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_EMA; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SignalProcessing::CBoxAlgorithmEMA; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("input Min", OV_TypeId_Signal);
				rBoxAlgorithmPrototype.addOutput  ("output Mout", OV_TypeId_Signal);
				rBoxAlgorithmPrototype.addSetting("forgetting (alpha)",    OV_TypeId_Float, "0.5");

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_EMADesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_EMA_H__
