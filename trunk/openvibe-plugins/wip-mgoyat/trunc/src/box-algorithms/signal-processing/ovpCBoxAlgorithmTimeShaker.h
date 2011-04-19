#ifndef __OpenViBEPlugins_SignalProcessing_BoxAlgorithms_CTimeShaker_H__
#define __OpenViBEPlugins_SignalProcessing_BoxAlgorithms_CTimeShaker_H__

#include "../../ovp_defines.h"

#include <openvibe-toolkit/ovtk_all.h>

#define OVP_ClassId_BoxAlgorithm_TimeMover      OpenViBE::CIdentifier(0x3F570B74, 0x000B426D)
#define OVP_ClassId_BoxAlgorithm_TimeMoverDesc  OpenViBE::CIdentifier(0x1CFF113E, 0x512E6B4B)

namespace OpenViBEPlugins
{
	namespace SignalProcessing
	{
		class CBoxTimeMover : public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);

			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>, OVP_ClassId_BoxAlgorithm_TimeMover)

		protected:

			OpenViBE::Kernel::IAlgorithmProxy* m_pStreamDecoder;
			OpenViBE::Kernel::IAlgorithmProxy* m_pStreamEncoder;
			
			OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > m_oInputMemoryBufferHandle;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > m_oOutputMemoryBufferHandle;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > iop_pSamplingRate;
			
		private:
			OpenViBE::float64 m_foffStart;
			OpenViBE::float64 m_foffEnd;
			OpenViBE::float64 m_fDilatation;
		
			OpenViBE::uint64 TranslationTime3232(OpenViBE::uint64, OpenViBE::float64 times);
			OpenViBE::uint64 DilatationTime3232(OpenViBE::uint64, OpenViBE::float64 factor);
		};

		class CBoxTimeMoverDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Time shaker"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Matthieu Goyat"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("Gipsa-lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Dilate and offset chunk time"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Signal processing/Epoching"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-missing-image"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_TimeMover; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SignalProcessing::CBoxTimeMover(); }
	
			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rPrototype) const
			{
				rPrototype.addInput  ("Input Signals",	OV_TypeId_Signal);
				rPrototype.addOutput ("Ouput Signals",  OV_TypeId_Signal);
				rPrototype.addSetting ("Dilatation value",   OV_TypeId_Float, "1");
				rPrototype.addSetting ("Offset chunk start", OV_TypeId_Float, "0");
				rPrototype.addSetting ("Offset chunk end",   OV_TypeId_Float, "0");
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_TimeMoverDesc)
		};
	};
};

#endif // __OpenViBEPlugins_SignalProcessing_BoxAlgorithms_CTimeMover_H__
