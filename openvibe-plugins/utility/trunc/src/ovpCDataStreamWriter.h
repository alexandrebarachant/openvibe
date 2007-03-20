#ifndef __UtilityPlugin_ovpCDataStreamWriter_H__
#define __UtilityPlugin_ovpCDataStreamWriter_H__

#include "ovp_defines.h"

#include <system/Memory.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <vector>
#include <fstream>

namespace OpenViBEPlugins
{
	namespace Utility
	{
		/**
		 * This class is used to save the incoming data streams into a file. This file
		 * can later be read by the Data stream reader plugin. It saves all the incoming
		 * data from the different inputs and then allows the user to analyse it or "play"
		 * it again.
		 */
		class CDataStreamWriter : virtual public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>
		{
		public:

			CDataStreamWriter(void);
			virtual void release(void);

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);

			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>, OVP_ClassId_DataStreamWriter)
			
		protected:
			//!Vector of indexes of the inputs which have pending data
			std::vector<OpenViBE::uint32> m_vInputWithPendingData;
			
			//!The output file's name
			OpenViBE::CString m_sFileName;

			//!The output file's handle
			std::ofstream m_oFile;

			
		};

		class CDataStreamWriterDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Data stream writer"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Bruno Renier"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Data stream writer"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("This plugin reads the data stream from its inputs then writes it to a file so it can be played again later"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Utility"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.5"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_DataStreamWriter; } 
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Utility::CDataStreamWriter(); }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Plugins::IBoxProto& rPrototype) const
			{
				// Adds box inputs
				rPrototype.addInput("Input Stream 1", OV_UndefinedIdentifier);
				rPrototype.addInput("Input Stream 2", OV_UndefinedIdentifier);

				// Adds box settings
				rPrototype.addSetting("Output file", OV_TypeId_String, "../../streamdata.bin");

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_DataStreamWriterDesc)
		};
	};
};

#endif // __UtilityPlugin_ovpCDataStreamWriter_H__
