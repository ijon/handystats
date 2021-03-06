PROJECT (handystats)
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

SET (LIB_MAJOR_VERSION "1")
SET (LIB_MINOR_VERSION "11")
SET (LIB_PATCH_VERSION "4")
SET (LIB_SOVERSION "1")

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -std=c++0x -Wreorder -Wreturn-type -Wunused-variable -pedantic -D_GLIBCXX_USE_NANOSLEEP -D_GLIBCXX_USE_CLOCK_MONOTONIC -D_GLIBCXX_USE_SCHED_YIELD")

SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -std=c99 -Wreturn-type -Wunused-variable -pedantic -D_GLIBCXX_USE_NANOSLEEP -D_GLIBCXX_USE_CLOCK_MONOTONIC -D_POSIX_C_SOURCE=200809L")


GET_PROPERTY (LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)

IF (LIB64)
	SET (LIBSUFFIX 64)
ELSE ()
	SET (LIBSUFFIX "")
ENDIF ()

SET (LIBDIR lib${LIBSUFFIX})


INCLUDE_DIRECTORIES (${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBDIR})

FILE (GLOB_RECURSE handy_src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")
ADD_LIBRARY (${PROJECT_NAME} SHARED ${handy_src})

TARGET_LINK_LIBRARIES (${PROJECT_NAME} rt pthread)

SET_TARGET_PROPERTIES (${PROJECT_NAME} PROPERTIES SOVERSION ${LIB_SOVERSION} VERSION ${LIB_MAJOR_VERSION}.${LIB_MINOR_VERSION})

INSTALL (TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${LIBDIR})
INSTALL (DIRECTORY ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

ADD_SUBDIRECTORY (benchmarks EXCLUDE_FROM_ALL)

ENABLE_TESTING ()
ADD_SUBDIRECTORY (tests)

