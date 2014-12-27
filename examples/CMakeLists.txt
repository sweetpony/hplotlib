include_directories (
    ${CMAKE_SOURCE_DIR}/src
)

add_executable (example-lines example-lines.cpp)
target_link_libraries (example-lines hplotlib)

add_executable (SimplePlot SimplePlot.cpp)
target_link_libraries (SimplePlot hplotlib)

add_executable (ContourPlot ContourPlot.cpp)
target_link_libraries (ContourPlot hplotlib)

