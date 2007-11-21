#ifndef __OpenViBEPlugins_FileIO_ovpCGenericStreamReader_H__
#define __OpenViBEPlugins_FileIO_ovpCGenericStreamReader_H__

#include "ovp_defines.h"

#include <system/Memory.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <fstream>

namespace OpenViBEPlugins
{
	namespace FileIO
	{
		class CGenericChunk
		{
			public :
			OpenViBE::uint32 m_ui32CurrentInput;
			OpenViBE::uint64 m_ui64StartTime;
			OpenViBE::uint64 m_ui64EndTime;
			OpenViBE::uint64 m_ui64ChunkSize;
			OpenViBE::uint8 * m_pChunkBuffer;

			OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>& m_oPlugin;

			CGenericChunk(OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>& oPlugin) : 
				m_ui32CurrentInput(0), 
				m_ui64StartTime(0),
				m_ui64EndTime(0),
				m_ui64ChunkSize(0),
				m_pChunkBuffer(NULL),
				m_oPlugin(oPlugin)
			{}

			OpenViBE::boolean read(std::ifstream& oFile);

		};


		/**
		 * This class is used to read a file produced by the Data stream writer, and
		 * playback the saved data streams.
		 */
		class CGenericStreamReader : virtual public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>
		{
		public:

			CGenericStreamReader(void);
			virtual void release(void);

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);

			/* TODO maximum clock frequency */ 
			virtual OpenViBE::uint64 getClockFrequency(){ return (100LL<<32); }

			virtual OpenViBE::boolean processClock(OpenViBE::CMessageClock &rMessageClock);

			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>, OVP_ClassId_GenericStreamReader)

		protected:
			//!Input file handle
			std::ifstream m_oFile;

			//! The name of the input file
			OpenViBE::CString m_sFileName;

			CGenericChunk m_oCurrentChunk;

			OpenViBE::uint64 m_ui64CurrentTime;

			OpenViBE::boolean m_bError;

		};

		class CGenericStreamReaderDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
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

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_GenericStreamReader; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::FileIO::CGenericStreamReader(); }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rPrototype) const
			{
				// Adds box outputs
				rPrototype.addOutput("Output Stream 1", OV_UndefinedIdentifier);
				rPrototype.addOutput("Output Stream 2", OV_UndefinedIdentifier);

				// Adds box settings
				rPrototype.addSetting("Input file", OV_TypeId_String, "../../streamdata.bin");

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_GenericStreamReaderDesc)
		};
	};
};

#endif // __OpenViBEPlugins_FileIO_ovpCGenericStreamReader_H__
