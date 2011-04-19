#ifndef __OpenViBEPlugins_BoxAlgorithm_NbStimFilter_H__
#define __OpenViBEPlugins_BoxAlgorithm_NbStimFilter_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <vector>

#define OVP_ClassId_BoxAlgorithm_NbStimFilter  		   		OpenViBE::CIdentifier(0x77420AE0, 0x442F46F7)
#define OVP_ClassId_BoxAlgorithm_NbStimFilterDesc 			OpenViBE::CIdentifier(0x50F260BE, 0x3A151E09)

namespace OpenViBEPlugins
{
	namespace Stimulation
	{
		class CBoxAlgorithmNbStimFilter : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32Index);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_NbStimFilter);

		protected:

			OpenViBE::int64 m_i64KeepingCount;
			OpenViBE::int64 m_i64AvoidBetween;
			OpenViBE::int64 m_i64AvoidStart;

		private:

			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationEncoder;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pEncodedMemoryBuffer;
			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationDecoder;
			OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pMemoryBufferToDecode;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > iop_pStimulationSet;

			OpenViBE::int64 m_i64keepingCountCurrent;
			OpenViBE::int64 m_i64avoidBetweenCurrent;
			OpenViBE::int64 m_i64avoidStartCurrent;
		};

		class CBoxAlgorithmNbStimFilterDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("stimulation Filter on number"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Matthieu Goyat"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("Gipsa-lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Stimulation"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-select-font"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_NbStimFilter; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Stimulation::CBoxAlgorithmNbStimFilter; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("Incoming stimulations",           OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addOutput ("Produced stimulations",           OV_TypeId_Stimulations);

				rBoxAlgorithmPrototype.addSetting("Number of Stimulation to keep",  			 OV_TypeId_Integer, "1");
				rBoxAlgorithmPrototype.addSetting("Number of Stimulation to avoid between",  	 OV_TypeId_Integer, "1");
				rBoxAlgorithmPrototype.addSetting("Number of Stimulation to avoid at begining",  OV_TypeId_Integer, "0");
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_NbStimFilterDesc);
		};
	};
};

#endif
