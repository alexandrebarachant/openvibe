#ifndef __SamplePlugin_CWidget_H__
#define __SamplePlugin_CWidget_H__

#include "ovp_defines.h"

#include <iostream>

#include <glade/glade.h>
#include <gtk/gtk.h>

namespace OpenViBEPlugins
{
	namespace Samples
	{
		class CWidget : virtual public OpenViBE::Plugins::IBoxAlgorithm
		{
		public:

			virtual void release(void)
			{
				delete this;
			}

			virtual OpenViBE::boolean initialize(
				OpenViBE::Plugins::IBoxAlgorithmContext& rContext)
			{
				m_pGladeInterface=glade_xml_new("../share/openvibe-acquisition-server-interface.glade", NULL, NULL);
				glade_xml_signal_autoconnect(m_pGladeInterface);
				gtk_widget_show(glade_xml_get_widget(m_pGladeInterface, "openvibe-acquisition-server"));

				return true;
			}

			virtual OpenViBE::boolean uninitialize(
				OpenViBE::Plugins::IBoxAlgorithmContext& rContext)
			{
				gtk_widget_destroy(glade_xml_get_widget(m_pGladeInterface, "openvibe-acquisition-server"));
				m_pGladeInterface=NULL;

				return true;
			}

			virtual OpenViBE::boolean processClock(
				OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext,
				OpenViBE::CMessageClock& rMessageClock)
			{
				rBoxAlgorithmContext.markAlgorithmAsReadyToProcess();
				return true;
			}

			virtual OpenViBE::boolean processInput(
				OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext,
				OpenViBE::uint32 ui32InputIndex)
			{
				rBoxAlgorithmContext.markAlgorithmAsReadyToProcess();
				return true;
			}

			virtual OpenViBE::boolean process(OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext)
			{
/*
				OpenViBE::Plugins::IStaticBoxContext* l_pStaticBoxContext=rBoxAlgorithmContext.getStaticBoxContext();
				OpenViBE::Plugins::IDynamicBoxContext* l_pDynamicBoxContext=rBoxAlgorithmContext.getDynamicBoxContext();

				OpenViBE::uint64 l_ui64StartTime=0;
				OpenViBE::uint64 l_ui64EndTime=0;
				OpenViBE::uint64 l_ui64ChunkSize=0;
				const OpenViBE::uint8* l_pChunkBuffer=NULL;

				for(OpenViBE::uint32 i=0; i<l_pStaticBoxContext->getInputCount(); i++)
				{
					for(OpenViBE::uint32 j=0; j<l_pDynamicBoxContext->getInputChunkCount(i); j++)
					{
						l_pDynamicBoxContext->getInputChunk(i, j, l_ui64StartTime, l_ui64EndTime, l_ui64ChunkSize, l_pChunkBuffer);
						l_pDynamicBoxContext->appendOutputChunkData(i, l_pChunkBuffer, l_ui64ChunkSize);
						l_pDynamicBoxContext->markOutputAsReadyToSend(i, l_ui64StartTime, l_ui64EndTime);
						l_pDynamicBoxContext->markInputAsDeprecated(i, j);
					}
				}
*/
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_Widget)

		protected:

			::GladeXML* m_pGladeInterface;
		};

		class CWidgetDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Widget"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Samples"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_Widget; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Samples::CWidget(); }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Plugins::IBoxProto& rPrototype) const
			{
				rPrototype.addInput("an input", OV_UndefinedIdentifier);

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_WidgetDesc)
		};
	};
};

#endif // __SamplePlugin_CWidget_H__
