# ---------------------------------
# Finds LUA
# Adds library to target
# Adds include path
# ---------------------------------
IF(WIN32)
	FIND_PATH(PATH_LUA lua.h PATHS ${OV_WIN32_DEP_DIR}/lua/include NO_DEFAULT_PATH)
ELSEIF(UNIX)
	FIND_PACKAGE(Lua51 REQUIRED)
	SET(PATH_LUA ${LUA_INCLUDE_DIR})
	MESSAGE(STATUS "${PATH_LUA}")
ENDIF(WIN32)

IF(PATH_LUA)
	MESSAGE(STATUS "  Found Lua...")
	INCLUDE_DIRECTORIES(${PATH_LUA})

	SET(name_liblua lua)
	IF(WIN32)
		SET(name_liblua lua51)
	ENDIF(WIN32)

	IF(WIN32)
		FIND_LIBRARY(LIB_LUA ${name_liblua} PATHS ${PATH_LUA}/../lib NO_DEFAULT_PATH)
		FIND_LIBRARY(LIB_LUA ${name_liblua} PATHS ${PATH_LUA}/../lib)
	ELSE(UNIX)
		SET(LIB_LUA ${LUA_LIBRARIES})
	ENDIF(WIN32)
	IF(LIB_LUA)
		MESSAGE(STATUS "    [  OK  ] lib ${LIB_LUA}")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_LUA})
	ELSE(LIB_LUA)
		MESSAGE(STATUS "    [FAILED] lib ${name_liblua}")
	ENDIF(LIB_LUA)

	ADD_DEFINITIONS(-DTARGET_HAS_ThirdPartyLua)
ELSE(PATH_LUA)
	MESSAGE(STATUS "  FAILED to find Lua")
ENDIF(PATH_LUA)
