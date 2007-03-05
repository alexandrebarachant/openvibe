#ifndef __OpenViBE_AcquisitionServer_CAcquisitionServer_H__
#define __OpenViBE_AcquisitionServer_CAcquisitionServer_H__

#include "ovas_base.h"
#include "ovas_driver.h"

#include <ebml/defines.h>
#include <ebml/IWriter.h>
#include <ebml/IWriterHelper.h>

#include <socket/IConnectionServer.h>

#include <glade/glade.h>

#include <gtk/gtk.h>

#include <string>
#include <list>
#include <map>

/******************************************************************************/

#define EBML_Identifier_HEADER                        EBML::CIdentifier(0x04239)
#define EBML_Identifier_INFORMATION                   EBML::CIdentifier(0x04240)
#define EBML_Identifier_EXPERIMENT_ID                 EBML::CIdentifier(0x04241)
#define EBML_Identifier_SUBJECT_AGE                   EBML::CIdentifier(0x04242)
#define EBML_Identifier_SUBJECT_SEX                   EBML::CIdentifier(0x04243)
#define EBML_Identifier_CHANNEL_NUMBER                EBML::CIdentifier(0x04244)
#define EBML_Identifier_SAMPLING_FREQUENCY            EBML::CIdentifier(0x04245)
#define EBML_Identifier_CHANNEL_NAME_LIST             EBML::CIdentifier(0x04246)
#define EBML_Identifier_CHANNEL_NAME                  EBML::CIdentifier(0x04247)

#define EBML_Identifier_BUFFER                        EBML::CIdentifier(0x05A)
#define EBML_Identifier_SAMPLES                       EBML::CIdentifier(0x05B)
#define EBML_Identifier_NUMBER_OF_SAMPLE_PER_CHANNELS EBML::CIdentifier(0x05C)
#define EBML_Identifier_SAMPLE_BLOC                   EBML::CIdentifier(0x05D)

/******************************************************************************/

namespace OpenViBEAcquisitionServer
{
	class CConnectionHandler : virtual public EBML::IWriterCallback
	{
	public:

		CConnectionHandler(Socket::IConnection& rConnection);
		virtual ~CConnectionHandler(void);

		// Accessors
		Socket::IConnection& getConnection(void);
		EBML::IWriter& getWriter(void);
		EBML::IWriterHelper& getWriterHelper(void);

		// EBML writer callback
		virtual void write(const void* pBuffer, const EBML::uint64 ui64BufferSize);

	protected:

		Socket::IConnection& m_rConnection;
		EBML::IWriter* m_pWriter;
		EBML::IWriterHelper* m_pWriterHelper;
	};

	class CAcquisitionServer
	{
	public:

		CAcquisitionServer(void);
		virtual ~CAcquisitionServer(void);

		virtual OpenViBEAcquisitionServer::boolean initialize(void);

		// Acquisition server driver callback
		void driverProcessDataCB(const OpenViBEAcquisitionServer::float32* pSample);

		// GTK idle callback
		virtual void idleCB(void);

		// GTK button callbacks
		virtual void buttonConnectToggledCB(::GtkToggleButton* pButton);
		virtual void buttonStartPressedCB(::GtkButton* pButton);
		virtual void buttonStopPressedCB(::GtkButton* pButton);
		virtual void buttonShowElectrodeNamesCB(::GtkButton* pButton);
		virtual void buttonApplyElectrodeNameCB(::GtkButton* pButton);
		virtual void buttonRemoveElectrodeNameCB(::GtkButton* pButton);

	protected :

		::GladeXML* m_pGladeInterface;
		::GtkListStore* m_pElectrodeNameListStore;
		::GtkListStore* m_pChannelNameListStore;

		std::map<OpenViBEAcquisitionServer::uint32, std::string> m_vChannelName;
		std::list<OpenViBEAcquisitionServer::CConnectionHandler*> m_vConnectionHandler;
		Socket::IConnectionServer* m_pConnectionServer;

		OpenViBEAcquisitionServer::boolean m_bInitialized;
		OpenViBEAcquisitionServer::boolean m_bStarted;
		OpenViBEAcquisitionServer::uint32 m_ui32IdleCallbackId;

		OpenViBEAcquisitionServer::SDriver m_oDriver;

		OpenViBEAcquisitionServer::uint32 m_ui32ExperimentIdentifier;
		OpenViBEAcquisitionServer::uint32 m_ui32SubjectAge;
		OpenViBEAcquisitionServer::uint32 m_ui32ChannelCount;
		OpenViBEAcquisitionServer::uint32 m_ui32SamplingFrequency;
		OpenViBEAcquisitionServer::uint32 m_ui32SubjectSex;
		OpenViBEAcquisitionServer::uint32 m_ui32SampleCountPerSentBlock;

		OpenViBEAcquisitionServer::uint8* m_pSampleBuffer;
		OpenViBEAcquisitionServer::boolean m_bGotData;
	};
};

#endif // __OpenViBE_AcquisitionServer_CAcquisitionServer_H__
