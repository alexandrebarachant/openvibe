# --------------------------#
# Include all trunc subdirs #
# --------------------------#
SUBDIRS($ENV{OpenViBE})

SUBDIRS($ENV{OpenViBE_kernel})

SUBDIRS($ENV{OpenViBE_module_ebml})
SUBDIRS($ENV{OpenViBE_module_fs})
SUBDIRS($ENV{OpenViBE_module_socket})
SUBDIRS($ENV{OpenViBE_module_system})
SUBDIRS($ENV{OpenViBE_module_xml})

SUBDIRS($ENV{OpenViBE_plugin_acquisition})
SUBDIRS($ENV{OpenViBE_plugin_samples})
SUBDIRS($ENV{OpenViBE_plugin_utility})

SUBDIRS($ENV{OpenViBE_app_authoring_tool})
