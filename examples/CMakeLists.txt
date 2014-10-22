include_directories (
    ${CMAKE_SOURCE_DIR}/src
)

add_executable (example-lines example-lines.cpp)
target_link_libraries (example-lines hplotlib)
