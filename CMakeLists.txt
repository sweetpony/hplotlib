cmake_minimum_required (VERSION 2.8)

project (hplotlib)

option (BUILD_EXAMPLES "Build examples" ON)

find_package (OpenGL REQUIRED)
find_package (Threads REQUIRED)
if (NOT CMAKE_USE_PTHREADS_INIT)
	error ("Could NOT find pthreads.")
endif ()

set (EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

set (CMAKE_BUILD_TYPE DEBUG)
#set (CMAKE_BUILD_TYPE RELEASE)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")

if (BUILD_EXAMPLES)
	add_subdirectory (examples)
endif ()
add_subdirectory (src)
