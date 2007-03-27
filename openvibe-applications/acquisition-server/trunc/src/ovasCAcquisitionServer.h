#ifndef __OpenViBE_AcquisitionServer_CAcquisitionServer_H__
#define __OpenViBE_AcquisitionServer_CAcquisitionServer_H__

#include "ovas_base.h"
#include "ovasIDriver.h"
#include "ovasIHeader.h"

#include <ebml/defines.h>
#include <ebml/IWriter.h>
#include <ebml/IWriterHelper.h>

#include <socket/IConnectionServer.h>

#include <glade/glade.h>

#include <gtk/gtk.h>

#include <string>
#include <vector>
#include <list>

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

	class CAcquisitionServer : virtual OpenViBEAcquisitionServer::IDriverCallback
	{
	public:

		CAcquisitionServer(void);
		virtual ~CAcquisitionServer(void);

		virtual OpenViBEAcquisitionServer::boolean initialize(void);

		// GTK idle callback
		virtual void idleCB(void);

		// GTK button callbacks
		virtual void buttonConfigurePressedCB(::GtkButton* pButton);
		virtual void buttonConnectToggledCB(::GtkToggleButton* pButton);
		virtual void buttonStartPressedCB(::GtkButton* pButton);
		virtual void buttonStopPressedCB(::GtkButton* pButton);
		virtual void comboBoxDriverChanged(::GtkComboBox* pComboBox);

		// Driver samples information callback
		virtual void setSamples(const OpenViBEAcquisitionServer::float32* pSample);

	protected :

		::GladeXML* m_pGladeInterface;

		std::list<OpenViBEAcquisitionServer::CConnectionHandler*> m_vConnectionHandler;
		Socket::IConnectionServer* m_pConnectionServer;

		std::vector<OpenViBEAcquisitionServer::IDriver*> m_vDriver;
		OpenViBEAcquisitionServer::boolean m_bInitialized;
		OpenViBEAcquisitionServer::boolean m_bStarted;
		OpenViBEAcquisitionServer::uint32 m_ui32IdleCallbackId;
		OpenViBEAcquisitionServer::uint32 m_ui32SampleCountPerSentBlock;

		OpenViBEAcquisitionServer::uint8* m_pSampleBuffer;
		OpenViBEAcquisitionServer::boolean m_bGotData;

		OpenViBEAcquisitionServer::IDriver* m_pDriver;
	};
};

#endif // __OpenViBE_AcquisitionServer_CAcquisitionServer_H__
