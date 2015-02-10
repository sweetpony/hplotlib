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

add_executable (BigGrid BigGrid.cpp)
target_link_libraries (BigGrid hplotlib)

add_executable (ManualSettings ManualSettings.cpp)
target_link_libraries (ManualSettings hplotlib)

add_executable (LogPlot LogPlot.cpp)
target_link_libraries (LogPlot hplotlib)

add_executable (MultiplePlots MultiplePlots.cpp)
target_link_libraries (MultiplePlots hplotlib)

