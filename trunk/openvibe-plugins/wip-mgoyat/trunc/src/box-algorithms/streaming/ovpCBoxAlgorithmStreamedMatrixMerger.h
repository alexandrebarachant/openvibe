#ifndef __OpenViBEPlugins_BoxAlgorithm_StreamedMatrixMerger_H__
#define __OpenViBEPlugins_BoxAlgorithm_StreamedMatrixMerger_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <vector>
#include <cstdio>
#include <string>
#include <fstream>

#define OVP_ClassId_BoxAlgorithm_StreamedMatrixMerger 	 	OpenViBE::CIdentifier(0x3C6E2AC6, 0x3B866C1E)
#define OVP_ClassId_BoxAlgorithm_StreamedMatrixMergerDesc 	OpenViBE::CIdentifier(0x0F3D2EDE, 0x7DF26BFF)

namespace OpenViBEPlugins
{
	namespace Streaming
	{
		class CBoxAlgorithmStreamedMatrixMerger : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_StreamedMatrixMerger);

		protected:

			OpenViBE::Kernel::IAlgorithmProxy* m_pSignalDecoder;
			OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pMemoryBufferToDecode1;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > op_pDecodedMatrix1;

			OpenViBE::Kernel::IAlgorithmProxy* m_pSignalEncoder;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > ip_pMatrixToEncode;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pEncodedMemoryBuffer;

			OpenViBE::uint32 m_ui32MergingDimension;
			std::vector < OpenViBE::uint32> m_vMergingDimensionCount;
			std::vector < std::string > m_vChannelName;
			
			FILE * pFile;
		};

		class CBoxAlgorithmStreamedMatrixMergerListener : public OpenViBEToolkit::TBoxListener < OpenViBE::Plugins::IBoxListener >
		{
		public:

			virtual OpenViBE::boolean onInputAdded(OpenViBE::Kernel::IBox& rBox, const OpenViBE::uint32 ui32Index)
			{
				char l_sName[1024];
				OpenViBE::uint32 i;

				rBox.setInputType(ui32Index, OV_TypeId_StreamedMatrix);
				for(i=0; i<rBox.getInputCount(); i++)
				{
					sprintf(l_sName, "Input Signal %u", i+1);
					rBox.setInputName(i, l_sName);
				}

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxListener < OpenViBE::Plugins::IBoxListener >, OV_UndefinedIdentifier);
		};

		class CBoxAlgorithmStreamedMatrixMergerDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Streamed Matrix Merger"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Matthieu Goyat"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("GIPSA-Lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Dimension concatenation of two Matrix."); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Streaming"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-sort-ascending"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_StreamedMatrixMerger; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Streaming::CBoxAlgorithmStreamedMatrixMerger; }
			virtual OpenViBE::Plugins::IBoxListener* createBoxListener(void) const               { return new CBoxAlgorithmStreamedMatrixMergerListener; }
			virtual void releaseBoxListener(OpenViBE::Plugins::IBoxListener* pBoxListener) const { delete pBoxListener; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("Input Signal 1", OV_TypeId_StreamedMatrix);
				rBoxAlgorithmPrototype.addInput  ("Input Signal 2", OV_TypeId_StreamedMatrix);
				rBoxAlgorithmPrototype.addOutput ("Merged Signal", OV_TypeId_StreamedMatrix);
				rBoxAlgorithmPrototype.addSetting ("Merging dimension", OV_TypeId_Integer, "0");
				rBoxAlgorithmPrototype.addFlag   (OpenViBE::Kernel::BoxFlag_CanAddInput);

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_StreamedMatrixMergerDesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_StreamedMatrixMerger_H__
