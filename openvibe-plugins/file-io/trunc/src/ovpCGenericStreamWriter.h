#ifndef __OpenViBEPlugins_FileIO_ovpCGenericStreamWriter_H__
#define __OpenViBEPlugins_FileIO_ovpCGenericStreamWriter_H__

#include "ovp_defines.h"

#include <system/Memory.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <vector>
#include <fstream>

namespace OpenViBEPlugins
{
	namespace FileIO
	{
		/**
		 * This class is used to save the incoming data streams into a file. This file
		 * can later be read by the Data stream reader plugin. It saves all the incoming
		 * data from the different inputs and then allows the user to analyse it or "play"
		 * it again.
		 */
		class CGenericStreamWriter : public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>
		{
		public:

			CGenericStreamWriter(void);
			virtual void release(void);

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);

			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>, OVP_ClassId_GenericStreamWriter)

		protected:
			//!Vector of indexes of the inputs which have pending data
			std::vector<OpenViBE::uint32> m_vInputWithPendingData;

			//!The output file's name
			OpenViBE::CString m_sFileName;

			//!The output file's handle
			std::ofstream m_oFile;
		};

		class CGenericStreamWriterListener : public OpenViBEToolkit::TBoxListener < OpenViBE::Plugins::IBoxListener >
		{
		public:

			OpenViBE::boolean check(OpenViBE::Kernel::IBox& rBox)
			{
				char l_sName[1024];
				OpenViBE::uint32 i;
				for(i=0; i<rBox.getInputCount(); i++)
				{
					sprintf(l_sName, "Input stream %u", i+1);
					rBox.setInputName(i, l_sName);
				}
				return true;
			}

			virtual OpenViBE::boolean onInputAdded(OpenViBE::Kernel::IBox& rBox, const OpenViBE::uint32 ui32Index)
			{
				this->check(rBox);
				return true;
			}

			virtual OpenViBE::boolean onInputRemoved(OpenViBE::Kernel::IBox& rBox, const OpenViBE::uint32 ui32Index)
			{
				this->check(rBox);
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxListener < OpenViBE::Plugins::IBoxListener >, OV_UndefinedIdentifier);
		};

		class CGenericStreamWriterDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Generic stream writer"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Bruno Renier"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Data stream writer"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("This plugin reads the data stream from its inputs then writes it to a file so it can be played again later"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("File reading and writing/Generic"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.5"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-save"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_GenericStreamWriter; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::FileIO::CGenericStreamWriter(); }
			virtual OpenViBE::Plugins::IBoxListener* createBoxListener(void) const               { return new CGenericStreamWriterListener; }
			virtual void releaseBoxListener(OpenViBE::Plugins::IBoxListener* pBoxListener) const { delete pBoxListener; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rPrototype) const
			{
				rPrototype.addInput  ("Input Stream 1", OV_UndefinedIdentifier);
				rPrototype.addSetting("Output file",    OV_TypeId_String, "../../streamdata.bin");
				rPrototype.addFlag   (OpenViBE::Kernel::BoxFlag_CanAddInput);
				rPrototype.addFlag   (OpenViBE::Kernel::BoxFlag_CanModifyInput);
				rPrototype.addFlag   (OpenViBE::Kernel::BoxFlag_IsUnstable);
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_GenericStreamWriterDesc)
		};
	};
};

#endif // __OpenViBEPlugins_FileIO_ovpCGenericStreamWriter_H__
