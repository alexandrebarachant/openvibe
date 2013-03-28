INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})
ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)

ADD_EXECUTABLE(test_timearithmetic-dynamic test_timearithmetic.cpp)
TARGET_LINK_LIBRARIES(test_timearithmetic-dynamic ${PROJECT_NAME}-dynamic)

# We need to install because if the .exe is not in bin/, it won't find its .dll's and
# we'd need an extra launcher setting the paths etc
INSTALL(TARGETS test_timearithmetic-dynamic
	RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})
	

