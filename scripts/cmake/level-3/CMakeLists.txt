# --------------------------#
# Include all trunc subdirs #
# --------------------------#

SUBDIRS($ENV{OpenViBE_plugin_acquisition})
SUBDIRS($ENV{OpenViBE_plugin_samples})
SUBDIRS($ENV{OpenViBE_plugin_utility})

SUBDIRS($ENV{OpenViBE_designer})
SUBDIRS($ENV{OpenViBE_acquisition_server})
SUBDIRS($ENV{OpenViBE_id_generator})
