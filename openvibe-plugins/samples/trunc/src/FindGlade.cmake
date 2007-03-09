# ---------------------------------
# Finds GLADE toolkit
#
# Sets GLADE_FOUND
# Sets GLADE_LIBRARIES
# Sets GLADE_LIBRARY_DIRS
# Sets GLADE_LDFLAGS
# Sets GLADE_LDFLAGS_OTHERS
# Sets GLADE_INCLUDE_DIRS
# Sets GLADE_CFLAGS
# Sets GLADE_CFLAGS_OTHERS
# ---------------------------------

INCLUDE("FindPkgConfig.cmake")
pkg_check_modules(GLADE libglade-2.0)
