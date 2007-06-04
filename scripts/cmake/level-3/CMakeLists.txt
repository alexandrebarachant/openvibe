# --------------------------#
# Include all trunc subdirs #
# --------------------------#

SUBDIRS($ENV{OpenViBE_plugin_acquisition})
SUBDIRS($ENV{OpenViBE_plugin_feature_extraction})
SUBDIRS($ENV{OpenViBE_plugin_file_io})
SUBDIRS($ENV{OpenViBE_plugin_samples})
SUBDIRS($ENV{OpenViBE_plugin_signal_processing})
SUBDIRS($ENV{OpenViBE_plugin_signal_processing_gpl})
SUBDIRS($ENV{OpenViBE_plugin_simple_visualisation})
SUBDIRS($ENV{OpenViBE_plugin_stimulation})
SUBDIRS($ENV{OpenViBE_plugin_tools})

SUBDIRS($ENV{OpenViBE_designer})
SUBDIRS($ENV{OpenViBE_acquisition_server})
SUBDIRS($ENV{OpenViBE_id_generator})
