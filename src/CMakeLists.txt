file(GLOB SOURCES
    "*.cpp"
)

include_directories (
	${CMAKE_SOURCE_DIR}/ext/glld/
)

add_library (hplotlib ${SOURCES})
target_link_libraries (hplotlib glld)
