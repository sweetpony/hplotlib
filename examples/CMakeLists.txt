include_directories (
    ${CMAKE_SOURCE_DIR}/src
)

add_executable (CombinedPlots CombinedPlots.cpp)
target_link_libraries (CombinedPlots hplotlib)

add_executable (SimplePlot SimplePlot.cpp)
target_link_libraries (SimplePlot hplotlib)

add_executable (ContourPlot ContourPlot.cpp)
target_link_libraries (ContourPlot hplotlib)

add_executable (SimpleLayout SimpleLayout.cpp)
target_link_libraries (SimpleLayout hplotlib)

