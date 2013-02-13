#ifndef __SamplePlugin_CHelloWorld_H__
#define __SamplePlugin_CHelloWorld_H__

#include "ovp_defines.h"
#include <openvibe-toolkit/ovtk_all.h>
#include <cstdio>

namespace OpenViBEPlugins
{
	namespace Samples
	{
		class CHelloWorld : public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>
		{
		public:

			virtual void release(void);
			virtual OpenViBE::uint64 getClockFrequency(void);
			virtual OpenViBE::boolean processClock(OpenViBE::Kernel::IMessageClock& /* rMessageClock */);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_HelloWorld)
		};

		class CHelloWorldListener : public OpenViBEToolkit::TBoxListener < OpenViBE::Plugins::IBoxListener >
		{
		public:

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxListener < OpenViBE::Plugins::IBoxListener >, OV_UndefinedIdentifier);
		};

		class CHelloWorldDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("HelloWorld"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Prints \"Hello World!\" to the log"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("Using several copies of this friendly box (with different names) can be used to e.g. examine box execution order"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Samples"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-copy"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_HelloWorld; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Samples::CHelloWorld(); }
			virtual OpenViBE::Plugins::IBoxListener* createBoxListener(void) const               { return new CHelloWorldListener; }
			virtual void releaseBoxListener(OpenViBE::Plugins::IBoxListener* pBoxListener) const { delete pBoxListener; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rPrototype) const
			{
				rPrototype.addSetting("Frequency (Hz)",			OV_TypeId_Float, "1.0");
				rPrototype.addSetting("My greeting",			OV_TypeId_String, "Hello World!");

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_HelloWorldDesc)
		};
	};
};

#endif // __SamplePlugin_CHelloWorld_H__
