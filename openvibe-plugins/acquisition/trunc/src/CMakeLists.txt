FILE(GLOB_RECURSE source_files *.cpp)

ADD_LIBRARY(${PROJECT_NAME}-static ${source_files})
ADD_LIBRARY(${PROJECT_NAME}-dynamic SHARED ${source_files})

SET_TARGET_PROPERTIES(${PROJECT_NAME}-dynamic PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	COMPILE_FLAGS "-DOVP_Exports -DOVP_Shared")

ADD_DEPENDENCIES(${PROJECT_NAME}-dynamic OpenViBE-toolkit-dynamic)
ADD_DEPENDENCIES(${PROJECT_NAME}-dynamic OpenViBE-toolkit-static)

ADD_DEPENDENCIES(${PROJECT_NAME}-static OpenViBE-toolkit-dynamic)
ADD_DEPENDENCIES(${PROJECT_NAME}-static OpenViBE-toolkit-static)

INCLUDE("FindOpenViBE")
INCLUDE("FindOpenViBEToolkit")
INCLUDE("FindOpenViBEModuleEBML")
INCLUDE("FindOpenViBEModuleSocket")
INCLUDE("FindOpenViBEModuleSystem")
