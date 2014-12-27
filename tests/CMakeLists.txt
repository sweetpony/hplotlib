include_directories (
        ${CMAKE_INCLUDE_PATH}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/external/gmock-1.7.0/include
        ${CMAKE_SOURCE_DIR}/external/gtest-1.7.0/include
)

file(GLOB TEST_SOURCES
        "*.cpp"
)

add_executable (hplotlib-tests ${TEST_SOURCES})
target_link_libraries (hplotlib-tests gmock gtest hplotlib)
