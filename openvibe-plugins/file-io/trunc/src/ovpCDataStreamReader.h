#ifndef __OpenViBEPlugins_FileIO_ovpCDataStreamReader_H__
#define __OpenViBEPlugins_FileIO_ovpCDataStreamReader_H__

#include "ovp_defines.h"

#include <system/Memory.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <fstream>

namespace OpenViBEPlugins
{
	namespace FileIO
	{
		/**
		 * This class is used to read a file produced by the Data stream writer, and
		 * playback the saved data streams.
		 */
		class CDataStreamReader : virtual public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>
		{
		public:

			CDataStreamReader(void);
			virtual void release(void);

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);

			virtual OpenViBE::boolean processClock(OpenViBE::CMessageClock &rMessageClock);

			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>, OVP_ClassId_DataStreamReader)

		protected:
			//!Input file handle
			std::ifstream m_oFile;

			//! The name of the input file
			OpenViBE::CString m_sFileName;

		};

		class CDataStreamReaderDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Generic stream reader"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Bruno Renier"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Data stream reader"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("Reads saved data streams from the input file and plays them on its outputs"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("File reading and writing/Generic"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.5"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_DataStreamReader; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::FileIO::CDataStreamReader(); }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Plugins::IBoxProto& rPrototype) const
			{
				// Adds box outputs
				rPrototype.addOutput("Output Stream 1", OV_UndefinedIdentifier);
				rPrototype.addOutput("Output Stream 2", OV_UndefinedIdentifier);

				// Adds box settings
				rPrototype.addSetting("Input file", OV_TypeId_String, "../../streamdata.bin");

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_DataStreamReaderDesc)
		};
	};
};

#endif // __OpenViBEPlugins_FileIO_ovpCDataStreamReader_H__
