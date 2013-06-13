#ifndef __OpenViBE_AcquisitionServer_CAcquisitionServerGUI_H__
#define __OpenViBE_AcquisitionServer_CAcquisitionServerGUI_H__

#include "ovas_base.h"
#include "ovasIDriver.h"
#include "ovasIHeader.h"
#include "ovasCHeader.h"


#include <socket/IConnectionServer.h>

#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/version.hpp>
#include <boost/algorithm/string.hpp>

#include <gtk/gtk.h>

namespace OpenViBEAcquisitionServer
{
	class CAcquisitionServer;
	class CAcquisitionServerThread;
	class IAcquisitionServerPlugin;
	struct PluginSetting;

	class CAcquisitionServerGUI
	{
	public:

		CAcquisitionServerGUI(const OpenViBE::Kernel::IKernelContext& rKernelContext);
		virtual ~CAcquisitionServerGUI(void);

		virtual OpenViBE::boolean initialize(void);

		OpenViBEAcquisitionServer::IDriver& getDriver(void);
		OpenViBE::uint32 getSampleCountPerBuffer(void);
		OpenViBE::uint32 getTCPPort(void);
		OpenViBEAcquisitionServer::IHeader& getHeaderCopy(void);
		void setClientCount(OpenViBE::uint32 ui32ClientCount);
		void setDrift(OpenViBE::float64 f64Drift);
		void setImpedance(OpenViBE::uint32 ui32ChannelIndex, OpenViBE::float64 f64Impedance);
		void disconnect(void);

		// GTK button callbacks
		virtual void buttonPreferencePressedCB(::GtkButton* pButton);
		virtual void buttonConfigurePressedCB(::GtkButton* pButton);
		virtual void buttonConnectToggledCB(::GtkToggleButton* pButton);
		virtual void buttonStartPressedCB(::GtkButton* pButton);
		virtual void buttonStopPressedCB(::GtkButton* pButton);
		virtual void comboBoxDriverChanged(::GtkComboBox* pComboBox);
		virtual void comboBoxSampleCountPerSentBlockChanged(::GtkComboBox* pComboBox);

		class PluginSettingReference
		{
			public:
				// name of the plugin containing the setting
				OpenViBE::CString plugin_name;

				// the name of the setting in the table
				OpenViBE::CString setting_name;

				// pointer to the setting structure inside the plugin
				PluginSetting* setting_ptr;

				// an unique name used for saving/loading settings, generated automatically
				OpenViBE::CString unique_name;

				// pointer to the widget containing the settings current value in the preferences dialog
				GtkWidget* gui_widget;

				// constructor of the structure, creates the unique name automatically
				PluginSettingReference(std::string pluginName, std::string settingName, PluginSetting* settingPtr)
					:
					  plugin_name(pluginName.c_str()),
					  setting_name(settingName.c_str()),
					  setting_ptr(settingPtr),
					  gui_widget(NULL)
				{
					// the unique name consists of a prefix, plugin name without spaces and setting name without spaces
					boost::erase_all(pluginName, " ");
					boost::erase_all(settingName, " ");

					unique_name = OpenViBE::CString(("AcquisitionServerPlugin_" + pluginName + "_"  + settingName).c_str());
				}
		};

		/// registers a new acquisition server plugin, the plugin is activated immediately
		void registerPlugin(IAcquisitionServerPlugin* plugin);

		/// scans all plugins for settings and puts them into a flat structure easier to handle
		void scanPluginSettings();

		/// holds references to the plugins' settings for faster access
		std::vector<PluginSettingReference> m_vPluginSettings;

	protected :

		const OpenViBE::Kernel::IKernelContext& m_rKernelContext;
		OpenViBEAcquisitionServer::IDriver* m_pDriver;
		OpenViBEAcquisitionServer::IDriverContext* m_pDriverContext;
		OpenViBEAcquisitionServer::CAcquisitionServer* m_pAcquisitionServer;
		OpenViBEAcquisitionServer::CAcquisitionServerThread* m_pAcquisitionServerThread;
		OpenViBEAcquisitionServer::CHeader m_oHeaderCopy;

		OpenViBE::uint32 m_ui32SampleCountPerBuffer;

		std::vector < OpenViBEAcquisitionServer::IDriver* > m_vDriver;

		::GtkBuilder* m_pBuilderInterface;

		::GtkWidget* m_pImpedanceWindow;
		std::vector < ::GtkWidget* > m_vLevelMesure;

		boost::thread* m_pThread;		
	};
};

#endif // __OpenViBE_AcquisitionServer_CAcquisitionServerGUI_H__
