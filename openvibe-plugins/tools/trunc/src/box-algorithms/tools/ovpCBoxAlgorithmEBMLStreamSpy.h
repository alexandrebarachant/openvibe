#ifndef __OpenViBEPlugins_BoxAlgorithm_EBMLStreamSpy_H__
#define __OpenViBEPlugins_BoxAlgorithm_EBMLStreamSpy_H__

#include "../../ovp_defines.h"

#include <openvibe-toolkit/ovtk_all.h>

#include <ebml/IReader.h>
#include <ebml/IReaderHelper.h>

#include <stack>
#include <map>
#include <string>

#define OVP_ClassId_BoxAlgorithm_EBMLStreamSpy     OpenViBE::CIdentifier(0x0ED76695, 0x01A69CC3)
#define OVP_ClassId_BoxAlgorithm_EBMLStreamSpyDesc OpenViBE::CIdentifier(0x354A6864, 0x06BC570C)

namespace OpenViBEPlugins
{
	namespace Tools
	{
		class CBoxAlgorithmEBMLStreamSpy : public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>, virtual public EBML::IReaderCallback
		{
		public:

			CBoxAlgorithmEBMLStreamSpy(void);

			virtual void release(void);

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);

			virtual EBML::boolean isMasterChild(const EBML::CIdentifier& rIdentifier);
			virtual void openChild(const EBML::CIdentifier& rIdentifier);
			virtual void processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize);
			virtual void closeChild(void);

			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>, OVP_ClassId_BoxAlgorithm_EBMLStreamSpy)

		protected:

			std::stack<EBML::CIdentifier> m_vNodes;
			std::map<EBML::CIdentifier, std::string> m_vName;
			std::map<EBML::CIdentifier, std::string> m_vType;
			OpenViBE::Kernel::ELogLevel m_eLogLevel;
			EBML::IReader* m_pReader;
			EBML::IReaderHelper* m_pReaderHelper;
		};

		class CBoxAlgorithmEBMLStreamSpyDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("EBML stream spy"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("EBML stream tree viewer"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("This sample EBML stream analyzer prints the EBML tree structure to the console"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Tools"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-info"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_EBMLStreamSpy; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Tools::CBoxAlgorithmEBMLStreamSpy(); }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rPrototype) const
			{
				// Adds box inputs
				rPrototype.addInput("Spied EBML stream 1", OV_TypeId_EBMLStream);
				rPrototype.addInput("Spied EBML stream 2", OV_TypeId_EBMLStream);

				// Adds box outputs

				// Adds box settings
				rPrototype.addSetting("EBML nodes description", OV_TypeId_Filename, "../share/openvibe-plugins/tools/config-ebml-stream-spy.txt");
				rPrototype.addSetting("Log level to use", OV_TypeId_LogLevel, "Debug");

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_EBMLStreamSpyDesc)
		};
	};
};

#endif // __OpenViBEPlugins_Tools_CEBMLStreamSpy_H__
